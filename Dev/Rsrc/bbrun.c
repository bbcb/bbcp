/* C-startup and loader for BlackBox
* Implemented as the StdLoader.
*/

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/* #define BB_FILE "bb.boot" */
#define BB_FILE argv[0]
/* #define BB_FILE argv[1] */
/* the exact size (in bytes) of the executable part of the file. */
/* this constant needs to be updated everytime a change is made to this file */
#define exeSize  EXESIZE /* = size of exe.img */

/* fixup types */
#define absolute 100
#define relative 101
#define copy 102
#define table 103
#define tableend 104
#define deref 105
#define halfword 106

/* import types */
#define mConst 0x1
#define mTyp 0x2
#define mVar 0x3
#define mProc 0x4
#define mExported 4

#define any 1000000
#define init 0x10000

/* set to printf to debug and donothing to avoid debugging */

#define dprintf donothing

typedef void (*BodyProc)();

typedef char String[256];

typedef struct Type {
    int size;
    struct Module* mod;
    int id;
    int base[16]; /* should be ARRAY 16 OF TYPE */
    int fields;   /* should be Directory* */
    int ptroffs[any];
} Type;


typedef struct Object{
    int fprint;
    int offs;
    int id;
    Type* ostruct;
} Object;


typedef struct Directory{
    int num;
    Object obj[any];
} Directory;


typedef struct Module {  /* has to be exact copy of Kernel.Module */
    struct Module *next;
    int opts;
    int refcnt;
    short compTime[6], loadTime[6];
    int ext;
    int term; /* actually a pointer to type Command */
    int nofimps, nofptrs;
    int csize, dsize, rsize;
    int code, data, refs;
    int procBase, varBase; /* meta base addresses */
    char* names;  /* names[0] = 0X */
    int* ptrs;
    struct Module* imports;
    Directory* export;
    char name[256];
} Module;


typedef struct ImpList {
    struct ImpList* next;
    String name;
} ImpList;


typedef struct ModSpec {
    ImpList* imp;
    String name;
    int start, hs, ds, ms, cs, vs, dad, mad, cad, vad;
} ModSpec;

typedef struct BootInfo {
    Module* modList;
    int argc;
    char** argv;
} BootInfo;



FILE* f;
int nofMods;
String kernel, mainmod;
ModSpec mod;
Module *modlist;
BootInfo* bootInfo;
int newRecAdr, newArrAdr;
int newRecFP, newArrFP;

static void donothing(char* fmt, ...) {}

static void *AllocMem (size_t size) {
    size_t pagesize = getpagesize();

    size_t len = (size + pagesize - 1) & ~(pagesize - 1);

    void *mem = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    // NOTE:
    // Memory returned is page-aligned (guaranteed by mmap)
    // Memory is zero-filled (guaranteed for MAP_ANONYMOUS)
    // To free this memory, use: munmap(mem, len)

    return mem;
}

static bool FreeMem (void *mem, size_t size) {
    if (mem != NULL) {
        size_t pagesize = getpagesize();
        if (!munmap(mem, (size + pagesize - 1) & ~(pagesize - 1))) {
            perror("munmap");
            return false;
        }
    }
    return true;
}

static void DumpMod()
{
    dprintf("\n\n---- Mod info:\n");
    dprintf("        hs = %d\n", mod.hs);
    dprintf("        dad = %p, ds = %d\n", (void *)mod.dad, mod.ds);
    dprintf("        mad = %p, ms = %d\n", (void *)mod.mad, mod.ms);
    dprintf("        cad = %p, cs = %d\n", (void *)mod.cad, mod.cs);
    dprintf("        vad = %p, vs = %d\n\n", (void *)mod.vad, mod.vs);
}

static void DumpModule (const Module *m) {

    dprintf("Module %s\n", m->name);
    dprintf("    opts = 0x%08x\n", m->opts);
    dprintf("    ext = %d\n", m->ext);
    dprintf("    term = %p\n", (void *)m->term);
    dprintf("    nofimps = %d, nofptrs = %d\n", m->nofimps, m->nofptrs);
    dprintf("    csize = %d, dsize = %d, rsize = %d\n", m->csize, m->dsize, m->rsize);
    dprintf("    code = %p, data = %p, refs = %p\n", (void *)m->code, (void *)m->data, (void *)m->refs);
    dprintf("    procBase = %p, varBase = %p\n", (void *)m->procBase, (void *)m->varBase);
}

static void RegisterModule()
{
    Module* m;
    m = (Module*) mod.dad;
    m->next = modlist;
    modlist = m;
    printf("Registred module %s\n", mod.name);
}

static void PrintMods()
{
    Module* ml;
    ml = modlist;
    printf("Loaded Modules\n");
    while (ml != NULL){
        printf("mod name: %s\n", ml->name);
        ml = ml->next;
    }
    printf("end of list\n");
}


static Module* ThisModule(char* name)
{
    Module* ml;
    ml = modlist;
    while ((ml != NULL) && (strcmp(ml->name, name) != 0)){ml = ml->next;}
    return ml;
}

static Object* ThisObject(Module* mod, char* name)
{
    int l, r, m;
    char* p;
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
    printf("loading: %s\n", name);
    if ((handle = dlopen(name, RTLD_LAZY + RTLD_GLOBAL)) == NULL) {
        printf("LoadDll: failed to load lib %s\n", name);
        printf(" - dlerror: %s\n", dlerror());
        exit(-1);
    }
    return handle != NULL;
}


static int ThisDllObj (int mode, int fprint, char* dll, char* name)
{
    void *handle;
    int ad = 0;
    if (strcmp(name, "dlopen") == 0) return (int)&dlopen;
    if (strcmp(name, "dlsym") == 0) return (int)&dlsym;
    if ((mode == mVar) || (mode == mProc)){
        if ((handle = dlopen(dll, RTLD_LAZY + RTLD_GLOBAL)) == NULL) {
            printf("ThisDllObj: lib %s not found\n", dll);
            printf(" - dlerror: %s\n", dlerror());
            exit(-1);
        } else {
            ad = (int)dlsym((void *) handle, name);
            if (ad == 0) {
                printf("ThisDllObj: symbol %s not found\n", name);
                /*        exit(-1); */
            }
        }
    }
    return ad;
}

static int Read4 ()
{
    unsigned char b;
    int   w;
    b = fgetc(f); w = b % 256;
    b = fgetc(f); w = w + 0x100 * (b % 256);
    b = fgetc(f); w = w + 0x10000 * (b % 256);
    b = fgetc(f); w = w + 0x1000000 * b;
    return w;
}

static int RNum()
{
    char b;
    int   s, y;
    s = 0; y = 0;
    b = fgetc(f);
    while (b < 0) {
        y = y + ((b + 128) << s);
        s = s + 7;
        b = fgetc(f);
    }
    return (((b + 64) % 128 - 64) << s) + y;
}

static void ReadName (char* str)
{
    unsigned char b;
    int i;

    i = 0; b = fgetc(f);
    while (b != 0)  {
        str[i] = b; i++; b = fgetc(f);
    }
    str[i] = 0;
}

static void Fixup (int adr)
{
    int link, offset, linkadr, n, x, t;

    dprintf("fixup: %X ", adr);
    link = RNum();
    while (link != 0) {
        offset = RNum();
        dprintf("+%d: ", offset);
        while (link != 0)
        {
            if (link > 0)
            {
                dprintf("c");
                linkadr = mod.cad + link;
            }
            else
            {
                dprintf("d");
                link = -link;
                if (link < mod.ms)
                    linkadr = mod.mad + link;
                else
                    linkadr = mod.dad + link - mod.ms;
            }
            dprintf("%X ", link);
            /*
            t = *(char*)(linkadr + 3);
            n = *(int*)linkadr;
            n = n << 8; n = n >> 8;
            */
            x = *(int*)linkadr;
            t = x / 0x1000000;
            n = (x + 0x800000) % 0x1000000 - 0x800000;

            switch (t) {
            case absolute: x = adr + offset; break;
            case relative: x = adr + offset - linkadr -4; break;
            case copy    : x = *(int*)(adr + offset); break;
            case table   : x = adr + n; n = link + 4; break;
            case tableend: x = adr + n; n = 0; break;
            case deref   : x = *(int*)(adr + 2); x = x + offset; break;
            case halfword: printf("fixup: halfword not implemented\n"); break;
            default:
                printf("fixup error(link=%d, offset=%d, linkadr=%d, t=%d, x=%d)\n", link, offset, linkadr, t, x);
                return;
            }
            *(int*)linkadr = x;
            link = n;
        }
        link = RNum();
    }
    dprintf("\n");
}

static int ReadBootHeader()
{
    int tag, version;
    fseek(f, exeSize, SEEK_SET);
    tag = Read4();
    version = Read4();
    if ((tag != 0x3A4B5C6D) || (version != 0))  { return 0; }
    nofMods = Read4();
    printf("Linked modules: %d\n", nofMods);
    ReadName(kernel);
    printf("kernel: %s\n", kernel);
    ReadName(mainmod);
    printf("main: %s\n", mainmod);
    newRecFP = Read4(); newRecAdr = 0;
    newArrFP = Read4(); newArrAdr = 0;
    mod.start = ftell(f);
    return 1;
}

static bool ReadHeader ()
{
    int ofTag, i, nofImps, processor;
    // char str[80];
    ImpList *imp, *last;
    char* n;

    ofTag = Read4();
    if (ofTag != 0x6F4F4346)
    {
        printf("wrong object file version\n");
        return false;
    }
    processor = Read4();
    mod.hs = Read4();
    mod.ms = Read4();
    mod.ds = Read4();
    mod.cs = Read4();
    mod.vs = Read4();
    dprintf("File tag: %d ", ofTag); dprintf("Processor: %d\n", processor);
    dprintf("Header size: %d ", mod.hs);
    dprintf("Meta size: %d ", mod.ms);
    dprintf("Desc size: %d ", mod.ds );
    dprintf("Code size: %d ", mod.cs);
    dprintf("Data size: %d\n", mod.vs);
    nofImps = RNum(); dprintf("Nof imports: %d\n", nofImps);
    ReadName(mod.name); dprintf("Module name: %s\n", mod.name);
    mod.imp = NULL;
    for (i = 0; i < nofImps; i++)
    {
        imp = (ImpList*)AllocMem(sizeof(ImpList)); assert(imp != NULL);
        ReadName(imp->name);
        if (mod.imp == NULL)
            mod.imp = imp;
        else
            last->next = imp;
        last = imp;
        last->next = NULL;
        dprintf("Import %d: %s\n", i, imp->name);
        if ((imp->name[0] == '$') && (imp->name[1] == '$'))
            strlcpy(imp->name, "Kernel", sizeof(imp->name));
        if (imp->name[0] == '$'){
            n = imp->name;
            n++;
            if (!LoadDll(n)){
                printf("Could not load lib: %s\n", imp->name);
                return false;
            }
        }
    }
    dprintf("Pos: %ld\n", ftell(f));
    return true;
}

static bool AllocModMem () {
    assert(mod.ds != 0);
    assert(mod.ms != 0);
    assert(mod.cs != 0);
    int ms = sizeof(int) + mod.ms;
    mod.dad = (int) AllocMem(mod.ds);
    mod.mad = (int) AllocMem(ms);
    mod.cad = (int) AllocMem(mod.cs);
    if (mod.vs != 0) {
        mod.vad = (int) AllocMem(mod.vs);
    } else {
        mod.vad = 0;
    }
    if ((mod.dad == 0) || (mod.mad == 0) || (mod.cad == 0) || ((mod.vad == 0) && (mod.vs != 0)))
    {
        bool ok;
        ok = FreeMem((void *)mod.dad, mod.ds); assert(ok); mod.dad = 0;
        ok = FreeMem((void *)mod.mad, ms); assert(ok); mod.mad = 0;
        ok = FreeMem((void *)mod.cad, mod.cs); assert(ok); mod.cad = 0;
        ok = FreeMem((void *)mod.vad, mod.vs); assert(ok); mod.vad = 0;
        return false;
    }
    *((int*)mod.mad) = ms;
    mod.mad += sizeof(int);
    return true;
}

static bool FixModMemPermissions () {
    assert(mod.ms != 0);
    assert(mod.cs != 0);
    if (mprotect((void *)(mod.mad - sizeof(int)), sizeof(int) + mod.ms, PROT_READ) != 0) {
        perror("mprotect");
        return false;
    }
    if (mprotect((void *)mod.cad, mod.cs, PROT_READ | PROT_EXEC) != 0) {
        perror("mprotect");
        return false;
    }
    return true;
}

static bool ReadModule ()
{
    char *dp, *mp, *cp;
    unsigned int cnt;
    ImpList* imp;
    int x, fp, opt, ofp, imptab, a;
    // int link;
    Module *desc, *k;
    String name;
    Object* obj;
    int isLib;
    char* im;

    if (!AllocModMem()) {
        printf("BootLoader: Couldn't initalize heap\n");
        return false;
    }
    dp = (char*) mod.dad;
    mp = (char*) mod.mad;
    cp = (char*) mod.cad;
    fseek(f, mod.start + mod.hs, SEEK_SET);
    dprintf("ReadModule after fseek pos: %ld\n", ftell(f));
    cnt = fread(mp, 1, mod.ms, f);
    dprintf("Read meta bulk (%d bytes. New pos: %ld)\n", cnt, ftell(f));
    cnt = fread(dp, 1, mod.ds, f);
    dprintf("Read desc bulk (%d bytes. New pos: %ld)\n", cnt, ftell(f));
    cnt = fread(cp, 1, mod.cs, f);
    dprintf("Read code bulk (%d bytes. New pos: %ld)\n", cnt, ftell(f));
    DumpMod();
    dprintf("before fixup: pos = %ld\n", ftell(f));

    if ((!newRecAdr) || (!newArrAdr)){
        k = ThisModule(kernel);
        if (k != NULL){
            /*      obj = ThisDesc(k, newRecFP);*/
            obj = ThisObject(k, "NewRec");
            if (obj != NULL)
                newRecAdr = k->procBase + obj->offs;
            /*      obj = ThisDesc(k, newArrFP);*/
            obj = ThisObject(k, "NewArr");
            if (obj != NULL)
                newArrAdr = k->procBase + obj->offs;
            dprintf("newRecFP: %X  newArrFP: %X\n", newRecFP, newArrFP);
            dprintf("newRecAdr: %X  newArrAdr: %X\n", newRecAdr, newArrAdr);
        } else {
            dprintf("no kernel before %s.\n", mod.name);
        }
    }
    Fixup(newRecAdr);
    Fixup(newArrAdr);
    Fixup(mod.mad);
    Fixup(mod.dad);
    Fixup(mod.cad);
    Fixup(mod.vad);
    dprintf("after fixup: pos = %ld\n", ftell(f));
    imp = mod.imp;
    imptab = (int)((Module*)(mod.dad))->imports;
    while (imp != NULL){
        x = RNum();
        if ((imp->name[0] == '$') && (imp->name[1] == '$'))        printf("should be Kernel\n");
        if (imp->name[0] == '$')        isLib = 1;
        else{
            isLib = 0;
            desc = ThisModule(imp->name);
            if (desc == NULL){
                printf("invalid import list\n");
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

                if ((obj != NULL) && (obj->id % 16 == x)){
                    ofp = obj->fprint;
                    switch (x){
                    case mTyp: opt = RNum();
                        if (opt % 2 == 1) ofp = obj->offs;
                        if ((opt > 1) && ((obj->id / 16) % 16 != mExported)){
                            printf("object not found (%s)\n", imp->name);
                            return false;
                        }
                        Fixup((int)obj->ostruct);
                        break;
                    case mVar:
                        Fixup(desc->varBase + obj->offs);
                        break;
                    case mProc:
                        Fixup(desc->procBase + obj->offs);
                    }

                    if (ofp != fp){
                        printf("illigal foot print (%s)\n", imp->name);
                        return false;
                    }
                } else {
                    if (obj == NULL) printf("obj == NULL\n");
                    printf("descriptor not found (%s, x: %d, id: %d)\n", name, x, obj->id);
                    return false;
                }
            }else{
                if ((x == mVar)  || (x == mProc)){
                    im = imp->name;
                    im++;
                    a = ThisDllObj(x, fp, im, name);
                    if (a != 0) Fixup(a);
                    else{
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
        *(int*)imptab = (int)desc; imptab += 4;
        imp = imp->next;
    }

    if (!FixModMemPermissions()) {
        return false;
    }

    mod.start = ftell(f);
    return true;
}

/*
    Reserve space for 0x80000000 cross border
    This space must not be accessed by GC

    ref.: https://forum.oberoncore.ru/viewtopic.php?f=134&t=6959#p118177
*/
static void ReserveCrossBorder () {
    size_t pagesize = (size_t)getpagesize();
    void *addr_0 = (void *)(0x80000000UL - pagesize);
    void *addr_1 = (void *)0x80000000UL;
    void *p;
    p = mmap(addr_0, pagesize, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if ((p != MAP_FAILED) && (p != addr_0)) {
        munmap(p, pagesize);
    }
    p = mmap(addr_1, pagesize, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if ((p != MAP_FAILED) && (p != addr_1)) {
        munmap(p, pagesize);
    }
}

int main (int argc, char *argv[])
{
    int i;
    bool ok;
    BodyProc body;
    // int callBackAdr;
    Module *k, *m;

    ReserveCrossBorder();

    modlist = NULL;
    dprintf("initializing BlackBox...\n");
    f = fopen(BB_FILE, "rb");
    if (f != NULL)
    {
        if (ReadBootHeader())
        {
            i = 0; ok = true;
            while ((i < nofMods) && (ok)){
                ok = ReadHeader();
                if (ok) {
                    ok = ReadModule();
                    if (ok)
                        RegisterModule();
                    else
                        printf("Incorrect module: %s\n", mod.name);
                } else
                    printf("Incorrect header: %s\n", mod.name);
                i++;
            }
            fclose(f);

            if (ok) {
                k = ThisModule(kernel);
                m = ThisModule(mainmod);
                if (k == NULL)
                    printf("no kernel\n");
                else
                {
                    if (m == NULL)
                        printf("no main module");
                    else
                    {
                        if ((void *)k->varBase != NULL) {
                            /* assign the boot info to first variable in Kernel */
                            bootInfo = AllocMem(sizeof(BootInfo)); assert(bootInfo != NULL);
                            bootInfo->modList = modlist;
                            bootInfo->argc = argc;
                            bootInfo->argv = argv;
                            *((int*)(k->varBase)) = (int)bootInfo;
                        } else {
                            printf("vars is empty => bootInfo not assigned\n");
                        }
                        body = (BodyProc)(m->code);
                        k->opts = k->opts | init; /* include init in opts */
                        DumpModule(k);
                        dprintf("before body\n");
                        body();
                        dprintf("after body\n");
                    }
                }
                PrintMods();
            }
        } else
            printf("Invalid BlackBox executable, make sure that the constant exeSize is correctly set\n");

    } else
        printf("Couldn't find file: %s\n", BB_FILE);
    return 0;
}
