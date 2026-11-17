/* 64-bit C-startup and loader for BlackBox (OCF amd64 v2 format)
 * Principles inherited from 32-bit bbrun.c:
 *  - module descriptor from file IS the runtime Module (dad)
 *  - 6 fixup groups: newRec, newArr, meta, desc, code, data
 *  - import resolution via UseBlk chains, Fixup(target)
 *  - import table filled with Module pointers
 * Differences (native 64-bit):
 *  - pointer slots are 8 bytes (absolute/copy/table/tableend write 8 bytes)
 *  - ripBased fixups: disp32 = target - (ladr + 4 + immLen), immLen = typ - 106
 *  - all module blocks sub-allocated from one arena (keeps disp32 in range)
 */

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <signal.h>
#include <ucontext.h>

/* fixup types */
#define absolute 100
#define relative 101
#define copy 102
#define table 103
#define tableend 104
#define short_ 105
#define ripBased 106	/* + immLen: 106=0, 107=1, 108=2, 110=4, 114=8 bytes follow disp32 */

/* import types */
#define mConst 0x1
#define mTyp 0x2
#define mVar 0x3
#define mProc 0x4
#define mExported 4

#define any 1000000
#define init 0x10000

typedef void (*BodyProc)();

typedef char String[256];

typedef struct Type {
    int size;               /* @0 */
    int pad0;               /* @4 */
    struct Module* mod;     /* @8 */
    int id;                 /* @16 */
    int pad1;               /* @20 */
    intptr_t base[16];      /* @24 */
    intptr_t fields;        /* @152 */
    int ptroffs[any];       /* @160 */
} Type;

typedef struct __attribute__((packed)) Object {     /* 24 bytes */
    int fprint;             /* @0 */
    int offs;               /* @4 */
    int id;                 /* @8 */
    int pad;                /* @12 */
    intptr_t ostruct;       /* @16 */
} Object;

typedef struct __attribute__((packed)) Directory {
    int num;
    Object obj[any];
} Directory;

/* Must match Kernel64.Module exactly (OCF amd64 v2 ModDesc) */
typedef struct Module {
    struct Module *next;        /* @0 */
    int opts;                   /* @8 */
    int refcnt;                 /* @12 */
    char compTime[12];          /* @16 */
    char loadTime[12];          /* @28 */
    int ext;                    /* @40 */
    int pad0;                   /* @44 */
    intptr_t term;              /* @48 */
    int nofimps, nofptrs;       /* @56, @60 */
    int csize, dsize, rsize;    /* @64, @68, @72 */
    int pad1;                   /* @76 */
    intptr_t code, data, refs;  /* @80, @88, @96 */
    intptr_t procBase, varBase; /* @104, @112 */
    char* names;                /* @120 */
    intptr_t* ptrs;             /* @128 */
    struct Module** imports;    /* @136 */
    Directory* export;          /* @144 */
    char name[256];             /* @152 */
} Module;

/* ABI-инварианты OCF v2: сломаются — значит, раскладка разъехалась с
   компилированным Kernel.Module (compiler выравнивает поля max на 4,
   поэтому в Kernel.odc.txt стоят явные pad-поля). */
_Static_assert(offsetof(Module, refcnt) == 12, "Module.refcnt @12");
_Static_assert(offsetof(Module, term) == 48, "Module.term @48");
_Static_assert(offsetof(Module, code) == 80, "Module.code @80");
_Static_assert(offsetof(Module, names) == 120, "Module.names @120");
_Static_assert(offsetof(Module, export) == 144, "Module.export @144");
_Static_assert(offsetof(Module, name) == 152, "Module.name @152");

typedef struct ImpList {
    struct ImpList* next;
    String name;
} ImpList;

typedef struct ModSpec {
    ImpList* imp;
    String name;
    int start, hs, ds, ms, cs, vs;
    intptr_t dad, mad, cad, vad;
    bool loaded;
    char path[512];
} ModSpec;

FILE* f;
String kernel, mainmod;
ModSpec mod;
Module *modlist;
intptr_t newRecAdr, newArrAdr;

/* ---- arena: all module blocks in one region, keeps RIP-relative disp32 valid ---- */
#define ARENA_SIZE (512UL*1024*1024)
static char* arena;
static size_t arenaPos;

/* strtoull не используем: на glibc >= 2.38 с -D_GNU_SOURCE хедеры
   перенаправляют его на __isoc23_strtoull@GLIBC_2.38, и бинарь перестаёт
   запускаться на старых системах. Свой разбор hex/decimal. */
static unsigned long long parse_ull (const char* s) {
    unsigned long long v = 0; int base = 10;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) { base = 16; s += 2; }
    for (; *s; s++) {
        int d;
        if (*s >= '0' && *s <= '9') d = *s - '0';
        else if (*s >= 'a' && *s <= 'f') d = *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'F') d = *s - 'A' + 10;
        else break;
        if (d >= base) break;
        v = v * base + (unsigned)d;
    }
    return v;
}

static void ArenaInit() {
    /* MAP_32BIT: refs-курсоры и ModSpec в CP-коде INTEGER-капнуты (<4 ГБ),
       GC Mark читает теги 32-битно — вся арена должна быть < 4 ГБ */
    void* hint = NULL;
    const char* ab = getenv("BB_ARENA_BASE");	/* hex — фикс. база для детерминизма под gdb */
    if (ab != NULL) hint = (void*)parse_ull(ab);
    arena = mmap(hint, ARENA_SIZE, PROT_READ|PROT_WRITE|PROT_EXEC,
                 MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT | (hint ? MAP_FIXED_NOREPLACE : 0), -1, 0);
    if (arena == MAP_FAILED) { perror("arena mmap"); exit(1); }
    arenaPos = 0;
}

static void* AllocMem (size_t size) {
    size_t pagesize = getpagesize();
    size_t len = (size + pagesize - 1) & ~(pagesize - 1);
    if (arenaPos + len > ARENA_SIZE) { fprintf(stderr, "arena overflow\n"); exit(1); }
    void* mem = arena + arenaPos;
    arenaPos += len;
    memset(mem, 0, len);
    return mem;
}

static Module* ThisModule(char* name)
{
    Module* ml = modlist;
    while ((ml != NULL) && (strcmp(ml->name, name) != 0)) ml = ml->next;
    if (ml) {
        /* sanity: name should be printable */
        if (ml->name[0] < 32 || ml->name[0] > 126) {
            printf("ThisModule(%s): corrupt entry at %p\n", name, (void*)ml);
        }
    }
    return ml;
}

/* ---- crash reporting: map rip/stack to modules, no gdb needed for triage ---- */
static Module* ModByCodeAdr(intptr_t a) {
    Module* m = modlist;
    while (m != NULL) {
        if ((a >= m->code) && (a < m->code + m->csize)) return m;
        m = m->next;
    }
    return NULL;
}

static int IsMapped(void* p) {
    unsigned char vec;
    return mincore((void*)((uintptr_t)p & ~(uintptr_t)(getpagesize()-1)), 1, &vec) == 0;
}

static intptr_t mainStackApprox;	/* адрес локала в main: потолок скана стека */

static void CrashHandler(int sig, siginfo_t* si, void* uc0) {
    ucontext_t* uc = (ucontext_t*)uc0;
    intptr_t rip = uc->uc_mcontext.gregs[REG_RIP];
    intptr_t rsp = uc->uc_mcontext.gregs[REG_RSP];
    intptr_t rbp = uc->uc_mcontext.gregs[REG_RBP];
    Module* m = ModByCodeAdr(rip);
    printf("\n*** CRASH %s: rip=%p fault=%p\n", strsignal(sig), (void*)rip, si->si_addr);
    if (m) printf("    in %s code+0x%lx\n", m->name, rip - m->code);
    else printf("    outside module code\n");
    printf("    rsp=%p rbp=%p\n", (void*)rsp, (void*)rbp);
    /* pseudo-backtrace: stack words pointing into module code */
    printf("    stack code refs:\n");
    intptr_t* sp = (intptr_t*) rsp;
    Module* last = NULL;
    for (int i = 0; i < 2048; i++) {
        if (((intptr_t)&sp[i] >= mainStackApprox - 8192) || !IsMapped(&sp[i])) break;
        Module* f = ModByCodeAdr(sp[i]);
        if ((f != NULL) && (f != last)) {
            printf("      %s+0x%lx\n", f->name, sp[i] - f->code);
            last = f;
        }
    }
    _exit(128 + sig);
}

static void InstallCrashHandler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = CrashHandler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
}

/* invariant check: no leftover fixup sentinel in module blocks */
static void CheckSentinels(char* name, intptr_t base, int size, int* total) {
    int n = 0;
    for (int off = 0; off + 4 <= size; off += 4)
        if (*(uint32_t*)(base + off) == 0x11223344) n++;
    if (n > 0) {
        printf("  ! %s: %d unpatched sentinel slots at %p\n", name, n, (void*)base);
        *total += n;
    }
}

static Object* ThisObject(Module* mod, char* name)
{
    int l, r, m;
    char* p;
    if (!mod || !mod->export) return NULL;
    l = 0; r = mod->export->num;
    while (l < r) {
        m = (l + r) / 2;
        p = (char*) &(mod->names[mod->export->obj[m].id / 256]);
        if (strcmp(p, name) == 0)
            return (Object*)&(mod->export->obj[m]);
        if (strcmp(p, name) < 0)
            l = m + 1;
        else
            r = m;
    }
    return NULL;
}

static Object* ThisDesc(Module* mod, int fprint)
{
    int i, n;
    if (!mod || !mod->export) return NULL;
    i = 0; n = mod->export->num;
    while ((i < n) && (mod->export->obj[i].id / 256 == 0))  {
        if (mod->export->obj[i].offs == fprint)
            return (Object*)&(mod->export->obj[i]);
        i++;
    }
    return NULL;
}

static int LoadDll (char* name)
{
    void *handle;
    if ((handle = dlopen(name, RTLD_LAZY + RTLD_GLOBAL)) == NULL) {
        printf("LoadDll: failed to load lib %s\n", name);
        printf(" - dlerror: %s\n", dlerror());
        return 0;
    }
    return 1;
}

/* trampolines: ripBased disp32 не дотягивается из арены (<4 ГБ) до libc (~140 ТБ).
   Создаём в арене стаб mov rax,imm64; jmp rax и отдаём его адрес — он в ±2 ГБ. */
static intptr_t StubFor (intptr_t target)
{
    char* p = AllocMem(16);
    p[0] = 0x48; p[1] = 0xB8;              /* mov rax, imm64 */
    memcpy(p + 2, &target, 8);
    p[10] = 0xFF; p[11] = 0xE0;            /* jmp rax */
    return (intptr_t)p;
}

static intptr_t ThisDllObj (int mode, int fprint, char* dll, char* name)
{
    void *handle;
    intptr_t ad = 0;
    if (strcmp(name, "dlopen") == 0) return (intptr_t)&dlopen;
    if (strcmp(name, "dlsym") == 0) return (intptr_t)&dlsym;
    if ((mode == mVar) || (mode == mProc)){
        if ((handle = dlopen(dll, RTLD_LAZY + RTLD_GLOBAL)) == NULL) {
            printf("ThisDllObj: lib %s not found\n", dll);
            printf(" - dlerror: %s\n", dlerror());
            exit(-1);
        } else {
            ad = (intptr_t)dlsym((void *) handle, name);
            if (ad == 0)
                printf("ThisDllObj: symbol %s not found\n", name);
        }
    }
    return ad;
}

static int Read4 ()
{
    unsigned char b;
    int w;
    b = fgetc(f); w = b % 256;
    b = fgetc(f); w = w + 0x100 * (b % 256);
    b = fgetc(f); w = w + 0x10000 * (b % 256);
    b = fgetc(f); w = w + 0x1000000 * b;
    return w;
}

static intptr_t RNum()
{
    int b;
    intptr_t s, y;
    s = 0; y = 0;
    b = fgetc(f);
    while (b >= 128) {
        y = y + ((intptr_t)(b - 128) << s);
        s = s + 7;
        b = fgetc(f);
    }
    return (((intptr_t)((b + 64) % 128 - 64)) << s) + y;
}

static void ReadName (char* str)
{
    unsigned char b;
    int i;
    i = 0; b = fgetc(f);
    while (b != 0) { str[i] = b; i++; b = fgetc(f); }
    str[i] = 0;
}

/* Process one fixup group from the file.
   adr = base address (wrt) of the referenced block; 0 for newRec/newArr skip
   when kernel procs are not known yet. */
static void Fixup (intptr_t adr)
{
    intptr_t link, offset, linkadr, n;
    int t, immLen, guard;

    link = RNum();
    while (link != 0) {
        offset = RNum();
        guard = 0;
        while (link != 0) {
            if (++guard > 100000) { printf("fixup: chain loop\n"); exit(1); }
            int is_code = (link > 0);
            intptr_t abslink = is_code ? link : -link;
            int off, max_sz;
            if (is_code) {
                off = (int)link; max_sz = mod.cs; linkadr = mod.cad + link;
            } else if (abslink < mod.ms) {
                off = (int)abslink; max_sz = mod.ms; linkadr = mod.mad + abslink;
            } else {
                off = (int)(abslink - mod.ms); max_sz = mod.ds; linkadr = mod.dad + abslink - mod.ms;
            }
            if ((off < 0) || (off > max_sz - 4)) {
                printf("fixup: link out of range (off=%d, max=%d)\n", off, max_sz);
                exit(1);
            }
            int x = *(int*)linkadr;
            t = (x >> 24) & 0xFF;
            n = x & 0xFFFFFF;
            if (n & 0x800000) n -= 0x1000000;

            switch (t) {
            case absolute:
                *(intptr_t*)linkadr = adr + offset;
                break;
            case relative:
                *(int*)linkadr = (int)(adr + offset - linkadr - 4);
                break;
            case copy:
                *(intptr_t*)linkadr = *(intptr_t*)(adr + offset);
                break;
            case table:
                *(intptr_t*)linkadr = adr + n;
                n = link + 8;
                break;
            case tableend:
                *(intptr_t*)linkadr = adr + n;
                n = 0;
                break;
            default:
                if ((t >= ripBased) && (t <= ripBased + 8)) {
                    immLen = t - ripBased;
                                        *(int*)linkadr = (int)(adr + offset - (linkadr + 4 + immLen));
                } else {
                    printf("fixup: unknown type %d at link=%ld\n", t, link);
                    exit(1);
                }
            }
            link = n;
        }
        link = RNum();
    }
}

static bool ReadHeader ()
{
    int ofTag, i, nofImps, processor;
    ImpList *imp, *last;
    char* n;

    ofTag = Read4();
    if (ofTag != 0x6F4F4346) {
        printf("wrong object file version\n");
        return false;
    }
    processor = Read4();
    mod.hs = Read4();
    mod.ms = Read4();
    mod.ds = Read4();
    mod.cs = Read4();
    mod.vs = Read4();
    if (processor != 12) {
        printf("wrong processor %d (expected 12)\n", processor);
        return false;
    }
    nofImps = RNum();
    ReadName(mod.name);
    mod.imp = NULL;
    for (i = 0; i < nofImps; i++) {
        imp = (ImpList*)AllocMem(sizeof(ImpList));
        ReadName(imp->name);
        if (mod.imp == NULL)
            mod.imp = imp;
        else
            last->next = imp;
        last = imp;
        last->next = NULL;
        if ((imp->name[0] == '$') && (imp->name[1] == '$'))
            strncpy(imp->name, kernel, sizeof(imp->name) - 1); imp->name[sizeof(imp->name)-1] = 0;
        if (imp->name[0] == '$') {
            n = imp->name;
            n++;
            if (!LoadDll(n)) {
                printf("Could not load lib: %s\n", imp->name);
                return false;
            }
        }
    }
    return true;
}

static bool AllocModMem () {
    int ms = sizeof(int) + mod.ms;
    mod.dad = (intptr_t) AllocMem(mod.ds);
    mod.mad = (intptr_t) AllocMem(ms);
    mod.cad = (intptr_t) AllocMem(mod.cs);
    if (mod.vs != 0)
        mod.vad = (intptr_t) AllocMem(mod.vs);
    else
        mod.vad = 0;
    *((int*)mod.mad) = ms;
    mod.mad += sizeof(int);
    return true;
}

/* can all non-dll imports be resolved now? */
static bool ImportsReady () {
    ImpList* imp = mod.imp;
    while (imp != NULL) {
        if (imp->name[0] != '$') {
            if (ThisModule(imp->name) == NULL) return false;
        }
        imp = imp->next;
    }
    return true;
}

static bool ReadModule ()
{
    char *dp, *mp, *cp;
    unsigned int cnt;
    ImpList* imp;
    intptr_t x, fp, imptab, a;
    int opt, ofp;
    Module *desc, *k;
    String name;
    Object* obj;
    int isLib;
    char* im;

    if (!AllocModMem()) return false;
    dp = (char*) mod.dad;
    mp = (char*) mod.mad;
    cp = (char*) mod.cad;
    fseek(f, mod.start + mod.hs, SEEK_SET);
    cnt = fread(mp, 1, mod.ms, f);   /* MetaBlk */
    cnt = fread(dp, 1, mod.ds, f);   /* DescBlk */
    cnt = fread(cp, 1, mod.cs, f);   /* CodeBlk */
    /* FixBlk follows immediately; VarBlk is not stored */

    /* Перерешиваем NewRec/NewArr для КАЖДОГО модуля: пока CP "Kernel" не загружен,
       NEW идёт в Kernel64 (bump-heap арены); после — в CP Kernel (кластерная куча с GC).
       Иначе bump-объекты ссылаются на кластерные, а Mark (InHeap-охрана) их не
       помечает → GC забирает живой объект → reuse → порча attr-слотов (краш
       DevMarkers.SizePref через WriteSChar piece.attr). */
    {
        Module* ck = ThisModule("Kernel");
        if (ck == NULL) ck = ThisModule(kernel);
        if (ck != NULL) {
            obj = ThisObject(ck, "NewRec");
            if (obj != NULL) newRecAdr = ck->procBase + obj->offs;
            obj = ThisObject(ck, "NewArr");
            if (obj != NULL) newArrAdr = ck->procBase + obj->offs;
        }
    }
    Fixup(newRecAdr);
    Fixup(newArrAdr);
    Fixup(mod.mad);
    Fixup(mod.dad);
    Fixup(mod.cad);
    Fixup(mod.vad);

    /* UseBlk: import resolution */
    imp = mod.imp;
    imptab = (intptr_t)((Module*)(mod.dad))->imports;
    while (imp != NULL) {
        x = RNum();
        if (imp->name[0] == '$') isLib = 1;
        else {
            isLib = 0;
            desc = ThisModule(imp->name);
            if (desc == NULL) {
                printf("%s: invalid import list (%s not loaded)\n", mod.name, imp->name);
                return false;
            }
        }
        while (x != 0) {
            ReadName(name); fp = RNum(); opt = 0;
            if (!isLib) {
                if (name[0] == 0)
                    obj = ThisDesc(desc, fp);
                else
                    obj = ThisObject(desc, name);

                if ((obj != NULL) && (obj->id % 16 == x)) {
                    ofp = obj->fprint;
                    switch (x) {
                    case mTyp: opt = RNum();
                        if (opt % 2 == 1) ofp = obj->offs;
                        if ((opt > 1) && ((obj->id / 16) % 16 != mExported)) {
                            printf("%s: object not found (%s)\n", mod.name, imp->name);
                            return false;
                        }
                        Fixup(obj->ostruct);
                        break;
                    case mVar:
                        Fixup(desc->varBase + obj->offs);
                        break;
                    case mProc:
                        Fixup(desc->procBase + obj->offs);
                    }

                    if (ofp != fp) {
                        printf("%s: illegal footprint (%s.%s)\n", mod.name, imp->name, name);
                        return false;
                    }
                } else {
                    printf("%s: descriptor not found (%s.%s, x=%d) obj=%p desc=%s export=%p num=%d names=%p\n",
                        mod.name, imp->name, name, (int)x, (void*)obj,
                        desc ? desc->name : "?", desc ? (void*)desc->export : 0,
                        desc && desc->export ? desc->export->num : -1,
                        desc ? (void*)desc->names : 0);
                    return false;
                }
            } else {
                if ((x == mVar) || (x == mProc)) {
                    im = imp->name;
                    im++;
                    a = ThisDllObj(x, fp, im, name);
                    if (a != 0) Fixup(StubFor(a));
                    else {
                        printf("ReadModule: Object not found: %s\n", name);
                        return false;
                    }
                } else {
                    if (x == mTyp) {
                        opt = RNum();
                        x = RNum();
                        if (x != 0) {
                            printf("ReadModule: Object not found: %s\n", name);
                            return false;
                        }
                    }
                }
            }
            x = RNum();
        }
        *(intptr_t*)imptab = (intptr_t)desc; imptab += 8;
        imp = imp->next;
    }

    return true;
}

static void RegisterModule()
{
    Module* m = (Module*) mod.dad;
    static int badSlots = 0;
    printf("  + %-16s dad=%p ms=%d ds=%d cs=%d cad=%p\n", mod.name, (void*)mod.dad, mod.ms, mod.ds, mod.cs, (void*)mod.cad);
    CheckSentinels(mod.name, mod.mad, mod.ms, &badSlots);
    CheckSentinels(mod.name, mod.dad, mod.ds, &badSlots);
    CheckSentinels(mod.name, mod.cad, mod.cs, &badSlots);
    m->next = modlist;
    modlist = m;
}

/* scan subsystem Code dirs for .ocf files; Kernel64 goes first */
#include <dirent.h>

static const char *subsystems[] = {"System", "Std", "Text", "Form", "Lin", "Cons", "Dev", NULL};

#define MAXMODS 256
static ModSpec specs[MAXMODS];
static int nSpecs;
static Module* loadOrder[MAXMODS];
static int nLoaded;

int main (int argc, char *argv[])
{
    int i, pass, loaded;
    Module *k;

    setvbuf(stdout, NULL, _IONBF, 0);
    printf("64-bit BlackBox boot loader (OCF v2)\n");
    mainStackApprox = (intptr_t)&i;
    InstallCrashHandler();
    ArenaInit();
    modlist = NULL;
    strcpy(kernel, "Kernel64");
    strcpy(mainmod, "LinInit");

    /* read headers of all .ocf in subsystem Code dirs; kernel first */
    nSpecs = 0;
    for (int k64 = 1; k64 >= 0; k64--) {
        for (int si = 0; subsystems[si] != NULL; si++) {
            char dirpath[128];
            snprintf(dirpath, sizeof(dirpath), "%s/Code", subsystems[si]);
            DIR* dir = opendir(dirpath);
            if (dir == NULL) continue;
            struct dirent* de;
            while ((de = readdir(dir)) != NULL && nSpecs < MAXMODS) {
                if (nSpecs >= MAXMODS - 1) { printf("FATAL: MAXMODS reached, increase it\n"); }
                int len = strlen(de->d_name);
                if (len < 5 || strcmp(de->d_name + len - 4, ".ocf") != 0) continue;
                if (k64 && strcmp(de->d_name, "Kernel64.ocf") != 0) continue;
                if (!k64 && strcmp(de->d_name, "Kernel64.ocf") == 0) continue;
                char path[512];
                snprintf(path, sizeof(path), "%s/%s", dirpath, de->d_name);
                f = fopen(path, "rb");
                if (f == NULL) continue;
                mod.start = 0;
                if (!ReadHeader()) {
                    printf("  ! %s (bad header, skipped)\n", path);
                    fclose(f);
                    continue;
                }
                /* главные модули НЕ грузим статически: LinLoader/LinIntLoader
                   загружают их динамически через StdLoader (иначе Load()
                   падает в FatalError "already loaded") */
                if (strcmp(mod.name, "LinInit") == 0 || strcmp(mod.name, "LinIntInit") == 0) {
                    fclose(f);
                    continue;
                }
                specs[nSpecs] = mod;
                strcpy(specs[nSpecs].path, path);
                specs[nSpecs].loaded = false;
                nSpecs++;
                fclose(f);
            }
            closedir(dir);
        }
    }
    printf("%d modules found\n", nSpecs);

    /* multi-pass loading: load modules whose imports are ready */
    for (pass = 0; pass < 16; pass++) {
        loaded = 0;
        for (i = 0; i < nSpecs; i++) {
            if (specs[i].loaded) continue;
            mod = specs[i];
            if (!ImportsReady()) continue;
            f = fopen(specs[i].path, "rb");
            if (f == NULL) continue;
            if (!ReadModule()) {
                printf("FAILED: %s\n", specs[i].path);
                fclose(f);
                goto out;
            }
            fclose(f);
            RegisterModule();
            specs[i] = mod;
            specs[i].loaded = true;
            loadOrder[nLoaded++] = (Module*) mod.dad;
            loaded++;
        }
        if (loaded == 0) break;
    }
    {
        int remaining = 0;
        for (i = 0; i < nSpecs; i++)
            if (!specs[i].loaded) {
                printf("  not loaded: %-28s needs:", specs[i].path);
                for (ImpList* t = specs[i].imp; t; t = t->next)
                    if (t->name[0] != '$' && ThisModule(t->name) == NULL)
                        printf(" %s", t->name);
                printf("\n");
                remaining++;
            }
        printf("%d modules loaded, %d pending\n", nSpecs - remaining, remaining);
    }

out:
    k = ThisModule(kernel);
    if (k == NULL) {
        printf("no kernel\n");
        return 1;
    }

    /* modlist sanity: раскладка Module сломана — лучше упасть здесь, чем в scasb */
    {
        Module* p = modlist; int n = 0;
        while (p != NULL && n < 4096) {
            if (p->name[0] < 32 || p->name[0] > 126) {
                printf("FATAL: corrupt Module in modlist at %p (name[0]=%d) — layout mismatch?\n",
                       (void*)p, p->name[0]);
                return 1;
            }
            p = p->next; n++;
        }
        if (p != NULL) { printf("FATAL: modlist cycle\n"); return 1; }
    }

    printf("kernel %s: code=%p varBase=%p\n", k->name, (void*)k->code, (void*)k->varBase);

    /* call kernel body */
    {
        BodyProc body = (BodyProc) k->code;
        k->opts = k->opts | init;
        body();
        printf("KERNEL OK\n");
    }

    /* inject module list: bump-ядро Kernel64 (varBase+0 = modList по дизайну)
       и System Kernel (его CP-код ходит по своему modList: ThisLoadedMod, GC) */
    if (k->varBase != 0)
        *(intptr_t*)k->varBase = (intptr_t)modlist;
    {
        Module* sk = ThisModule("Kernel");
        if (sk != NULL) {
            Object* ml = ThisObject(sk, "modList");
            if (ml != NULL) *(intptr_t*)(sk->varBase + ml->offs) = (intptr_t)modlist;
            /* bootInfo для LinKernel.argc/argv (gtk_init_check, GetCmdLineArg).
               Порядок полей в Kernel.BootInfo подобран так, чтобы раскладка
               не зависела от правила выравнивания компилятора:
               modList@0, argv@8, argc@16 (и при 4-, и при 8-выравнивании). */
            Object* bi = ThisObject(sk, "bootInfo");
            if (bi != NULL) {
                static char bootInfoBuf[24];
                *(intptr_t*)(bootInfoBuf + 0) = (intptr_t)modlist;
                *(intptr_t*)(bootInfoBuf + 8) = (intptr_t)argv;
                *(int*)(bootInfoBuf + 16) = argc;
                *(intptr_t*)(sk->varBase + bi->offs) = (intptr_t)bootInfoBuf;
            }
        }
    }

    int consoleMode = 0;
    for (int a = 1; a < argc; a++) if (strcmp(argv[a], "--console") == 0) consoleMode = 1;
    if (getenv("BB_CONSOLE") != NULL) consoleMode = 1;

    /* лоадеры помечаем init ДО выполнения любых тел: ленивый
       Kernel.InitModule (ThisLoadedMod <- Meta.Lookup <- LinRegistry.SearchVar
       идёт по ВСЕМУ modList) иначе исполняет тело лоадера ПРЕЖДЕВРЕМЕННО,
       в середине чужого тела — LinInit грузится до готовности остальных
       модулей и падает с fileNotFound. Пометки внутри цикла тел недостаточно:
       LinLoader в loadOrder идёт ПОСЛЕ LinRegistry. Тело выбранного
       лоадера вызывается явно в хвосте main. */
    {
        Module* ml;
        ml = ThisModule("LinLoader");    if (ml != NULL) ml->opts = ml->opts | init;
        ml = ThisModule("LinIntLoader"); if (ml != NULL) ml->opts = ml->opts | init;
    }

    /* инфраструктура загрузчика — первой, в порядке 32-битного dev0 link:
       тела этих модулей устанавливают хуки (Files.dir, SetLoader...),
       без которых тела остальных модулей падают (Librarian нужен Files.dir).
       Режим: bbrun64 --console или BB_CONSOLE=1 — консоль (LinIntLoader);
       по умолчанию GUI (LinLoader). */
    {
        static const char* infra[] = {"Utf", "LinKernel", "Files", "LinEnv",
            "LinFiles", "LinPackedFiles", "StdLoader", "StdInterpreter", NULL};
        for (int j = 0; infra[j] != NULL; j++) {
            Module* m = ThisModule((char*)infra[j]);
            if (m == NULL) { printf("infra %s: NOT FOUND\n", infra[j]); continue; }
            if (m->opts & init) { printf("infra %s: already init\n", infra[j]); continue; }
            m->opts = m->opts | init;
            BodyProc body = (BodyProc) m->code;
            printf("init %s (infra)...\n", m->name);
            body();
        }
    }

    /* run all module bodies in load order (like Kernel.InitModule);
       лоадеры (LinLoader/LinIntLoader) уже помечены init выше и здесь
       пропускаются — их тела грузят главный модуль через StdLoader, и к
       этому моменту все остальные тела уже отработали (иначе InitModule-
       рекурсия бежит по неинициализированным модулям: Dialog требует
       Librarian.lib из тела Librarian) */
    for (i = 0; i < nLoaded; i++) {
        Module* m = loadOrder[i];
        if (m == k) continue;
        if (m->opts & init) continue;
        m->opts = m->opts | init;
        BodyProc body = (BodyProc) m->code;
        printf("init %s...\n", m->name);
        body();
        /* FPU stack leak check: after a well-behaved body x87 stack is empty */
        if (getenv("BB_FPUCHECK") != NULL) {
            static unsigned char fx[512] __attribute__((aligned(16)));
            unsigned short cw = *(unsigned short*)(fx + 0);
            unsigned short sw = *(unsigned short*)(fx + 2);
            unsigned char tw = *(unsigned char*)(fx + 4);
            __asm__ volatile("fxsave %0" : "=m"(fx));
            tw = *(unsigned char*)(fx + 4);
            if ((tw != 0) || (*(unsigned short*)(fx + 2) & 1))
                printf("  ! FPU dirty after %s: ftw=%02x fsw=%04x fcw=%04x\n",
                       m->name, tw, *(unsigned short*)(fx + 2), *(unsigned short*)(fx + 0));
            (void)cw; (void)sw;
        }
        fprintf(stderr, "done %s\n", m->name);
    }
    fprintf(stderr, "body loop finished\n");

    /* главный лоадер последним: загружает LinInit/LinIntInit и запускает среду */
    if (getenv("BB_TRAP") != NULL) __builtin_trap();	/* gdb hook: модули загружены, можно ставить bp в коде модулей */
    {
        Module* m = ThisModule(consoleMode ? "LinIntLoader" : "LinLoader");
        /* оба лоадера уже помечены init в цикле тел выше (защита от
           преждевременного ленивого InitModule) — тело выбранного
           вызываем явно, независимо от флага */
        if (m != NULL) {
            fprintf(stderr, "init %s (main loader, %s mode)...\n", m->name, consoleMode ? "console" : "gui");
            ((BodyProc) m->code)();
        }
    }
    fprintf(stderr, "MAIN OK (all module bodies done)\n");

    /* call main module body if present (usually already run above) */
    {
        Module *m = ThisModule(mainmod);
        if (m == NULL) printf("no main module %s\n", mainmod);
    }

    printf("module list:\n");
    {
        Module *p = modlist;
        while (p != NULL) {
            printf("  %s\n", p->name);
            p = p->next;
        }
    }
    return 0;
}
