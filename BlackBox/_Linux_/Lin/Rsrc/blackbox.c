
/*
 * C-startup and loader for BlackBox 
 * Implemented as the StdLoader. 
 */


#include <sys/mman.h>

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/* the exact size (in bytes) of the executable part of the file. */
/* this constant needs to be updated everytime a change is made to this file */
#define exeSize 17008


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
  int fields; /* should be Directory* */
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

typedef struct Module{ /* has to be exact copy of Kernel.Module */
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
  char* names;	/* names[0] = 0X */
  int* ptrs;
  struct Module* imports;
  Directory* export;
  char name[256];
} Module;

typedef struct ImpList
{
  struct ImpList* next;
  String name;
} ImpList;

typedef struct ModSpec
{
  ImpList* imp;
  String name;
  int start, hs, ms, ds, cs, vs, mad, dad;
} ModSpec;

typedef struct BootInfo
{
  Module* modList;
  int argc;
  char** argv;
} BootInfo;

const char bbfile[] = "./bb.boot";

FILE* f;
int nofMods;
String kernel, mainmod;
ModSpec mod;
Module *modlist;
BootInfo* bootInfo;
int newRecAdr, newArrAdr;
int newRecFP, newArrFP;

int zerofd;

int donothing(char* fmt, ...)
{
}

void DumpMod()
{	
  dprintf("\n\n---- Mod info:\n");
  dprintf("        hs, ms, ds, cs, vs = %d, %d, %d, %d, %d\n",
	 mod.hs, mod.ms, mod.ds, mod.cs, mod.vs);
  dprintf("        mad, dad = %d, %d\n\n", mod.mad, mod.dad);
}


void RegisterModule()
{
  Module* m;
  m = (Module*)mod.dad;
  m->next = modlist;
  modlist = m;
  /*
  if (modlist == NULL){
    modlist = m; 
  } else {
    last->next = m;
    last = m;
  }
  last->next = NULL;
  */
  dprintf("Registred module %s\n", mod.name);
}

void PrintMods()
{
  Module* ml;
  ml = modlist;
  dprintf("Loaded Modules\n");
  while (ml != NULL){
    dprintf("mod name: %s\n", ml->name);
    ml = ml->next;
  }
  dprintf("end of list\n");
}


Module* ThisModule(char* name)
{
  Module* ml;
  ml = modlist;
  while ((ml != NULL) && (strcmp(ml->name, name) != 0)){
    ml = ml->next;
  }
  return ml;
}
		
Object* ThisObject(Module* mod, char* name)
{
  int l, r, m;
  char* p;
  l = 0; r = mod->export->num;
  while (l < r){
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

Object* ThisDesc(Module* mod, int fprint)
{
  int i, n;
  i = 0; n = mod->export->num;
  while ((i < n) && (mod->export->obj[i].id / 256 == 0))
    {
      if (mod->export->obj[i].offs == fprint)
	return (Object*)&(mod->export->obj[i]);
      i++;
    }
  return NULL;
}

int LoadDll (char* name)
{
  
  void *handle;
  dprintf("loading: %s\n", name);
  if ((handle = dlopen(name, RTLD_LAZY + RTLD_GLOBAL)) == NULL) {
    printf("LoadDll: failed to load lib %s\n", name); 
    printf(" - dlerror: %s\n", dlerror());
    exit(-1);
  }
  return handle != NULL;
}

	
int ThisDllObj (int mode, int fprint, char* dll, char* name)
{
  void *handle; 
  int ad = 0;
  if ((mode == mVar) || (mode == mProc)){
    if ((handle = dlopen(dll, RTLD_LAZY + RTLD_GLOBAL)) == NULL) {
      printf("ThisDllObj: lib %s not found\n", dll); 
      printf(" - dlerror: %s\n", dlerror());
      exit(-1);
    } else {
      ad = (int)dlsym((void *) handle, name);
      if (ad == 0) {
	printf("ThisDllObj: symbol %s not found\n", name); exit(-1);
      }
    }
  }
  return ad;
 }

int Read4 ()
{
  unsigned char b;
  int	w;
  b = fgetc(f); w = b % 256;
  b = fgetc(f); w = w + 0x100 * (b % 256);
  b = fgetc(f); w = w + 0x10000 * (b % 256);
  b = fgetc(f); w = w + 0x1000000 * b;
  return w;
}

int RNum()
{
  char b;
  int	s, y;
  s = 0; y = 0;
  b = fgetc(f);
  while (b < 0) 
    {
      y = y + ((b + 128) << s);
      s = s + 7;
      b = fgetc(f); 
    }
  return (((b + 64) % 128 - 64) << s) + y;
}

void ReadName (char* str)
{
  unsigned char b;
  int i;
  i = 0; b = fgetc(f);
  while (b != 0)
    {
      str[i] = b; i++; b = fgetc(f);
    }
  str[i] = 0;
}

void Fixup (int adr)
{
  int link, offset, linkadr, n, x, t;

  dprintf("fixup: %X ", adr);
  link = RNum();
  while (link != 0)
    {
      offset = RNum();
      dprintf("+%d: ", offset);
      while (link != 0)
	{
	  if (link > 0) 
	    {
	      dprintf("c");
	      linkadr = mod.mad + mod.ms + link;
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
	  case copy: x = *(int*)(adr + offset); break;
	  case table: x = adr + n; n = link + 4; break;
	  case tableend: x = adr + n; n = 0; break;
	  case deref: x = *(int*)(adr + 2); x = x + offset; break;
	  case halfword:
	    printf("fixup: halfword not implemented\n");
	    break;
	  default:
	    printf("fixup error(link=%d, offset=%d, linkadr=%d, t=%d, x=%d)\n",
		   link, offset, linkadr, t, x);
	    return;
	  }			
	  *(int*)linkadr = x;
	  link = n;
	}
      link = RNum();
    }
  dprintf("\n");
}

int ReadBootHeader()
{
  int tag, version;

  fseek(f, exeSize, SEEK_SET);
  tag = Read4();
  version = Read4();
  if ((tag != 0x3A4B5C6D) || (version != 0))
    {
      return 0;
    }
  nofMods = Read4();
  dprintf("Linked modules: %d\n", nofMods);
  ReadName(kernel);
  dprintf("kernel: %s\n", kernel);
  ReadName(mainmod);
  dprintf("main: %s\n", mainmod);
  newRecFP = Read4(); newRecAdr = 0;
  newArrFP = Read4(); newArrAdr = 0;
  mod.start = ftell(f);
  return 1;
}

void * MemAlloc (size_t len)
{
	void * res;

	res = mmap(0, len, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANON, zerofd, 0);
	if (res == MAP_FAILED) {
		res = NULL;
	} else {
		bzero(res, len);
	}
	return res;
}

int ReadHeader ()
{
  int ofTag, i, nofImps, processor;
  char str[80];
  ImpList *imp, *last;
  char* n;

  ofTag = Read4();
  if (ofTag != 0x6F4F4346) 
    {
      printf("wrong object file version\n");
      return 0;
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
      imp = (ImpList*)MemAlloc(sizeof(ImpList));
      ReadName(imp->name);
      if (mod.imp == NULL)
	mod.imp = imp;
      else
	last->next = imp;
      last = imp;
      last->next = NULL;
      dprintf("Import %d: %s\n", i, imp->name);
      if ((imp->name[0] == '$') && (imp->name[1] == '$'))
	strcpy(imp->name, "Kernel");
      if (imp->name[0] == '$'){
	n = imp->name;
	n++;
	if (!LoadDll(n)){
	  printf("Could not load lib: %s\n", (char *)(&(imp->name[1])));
	  return 0;
	}
      }
    }
  dprintf("Pos: %d\n", ftell(f));
  return 1;
}

int ReadModule ()
{
  char *dp, *mp; 
  unsigned int cnt;
  ImpList* imp;
  int x, fp, opt, link, ofp, imptab, a;
  Module *desc, *k;
  String name;
  Object* obj;
  int isLib;
  char* im;

  mod.dad = (int) MemAlloc(mod.ds);
  mod.mad = (int) MemAlloc(mod.ms + mod.cs + mod.vs);
  if ((mod.dad == 0) || (mod.mad == 0)) 
    {
      printf("BootLoader: Couldn't initalize heap\n");
      free((void*)mod.dad);
      free((void*)mod.mad);
      return 0;
    }
  dp = (char*) mod.dad;
  mp = (char*) mod.mad;
  fseek(f, mod.start + mod.hs, SEEK_SET);
  dprintf("ReadModule after fseek pos: %d\n", ftell(f));
  cnt = fread(mp, 1, mod.ms, f);
  dprintf("Read meta bulk (%d bytes. New pos: %d)\n", cnt, ftell(f));
  cnt = fread(dp, 1, mod.ds, f);
  dprintf("Read desc bulk (%d bytes. New pos: %d)\n", cnt, ftell(f));
  mp = (char*)(mod.mad + mod.ms);
  cnt = fread(mp, 1, mod.cs, f); 
  dprintf("Read code bulk (%d bytes. New pos: %d)\n", cnt, ftell(f));

  DumpMod();

  dprintf("before fixup: pos = %d\n", ftell(f));
  
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
  Fixup(mod.mad + mod.ms);
  Fixup(mod.mad + mod.ms + mod.cs);
  dprintf("after fixup: pos = %d\n", ftell(f));
  imp = mod.imp; imptab = (int)((Module*)(mod.dad))->imports;
  while (imp != NULL){
    x = RNum();
    if ((imp->name[0] == '$') && (imp->name[1] == '$'))
      printf("should be Kerneln");
    if (imp->name[0] == '$')
      isLib = 1;
    else{
      isLib = 0;
      desc = ThisModule(imp->name);
      if (desc == NULL){
	printf("invalid import list\n");
	return 0;
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
	  case mTyp:
	    opt = RNum();
	    if (opt % 2 == 1) ofp = obj->offs;
	    if ((opt > 1) && ((obj->id / 16) % 16 != mExported)){
	      printf("object not found (%s)\n", imp->name);
	      return 0;
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
	    return 0;
	  }
	} else {
	  if (obj == NULL) printf("obj == NULL\n");
	  printf("descriptor not found (%s, x: %d, id: %d)\n", name, x, obj->id);
	  return 0;
	}
      }else{
	if ((x == mVar)  || (x == mProc)){	
	  im = imp->name;
	  im++;
	  a = ThisDllObj(x, fp, im, name);
	  if (a != 0)
	    Fixup(a);
	  else{
	    printf("ReadModule: Object not found: %s\n", name);
	    return 0;
	  }
	} else {
	  if (x == mTyp) {
	    opt = RNum();
	    x = RNum();
	    if (x != 0) {
	      printf("ReadModule: Object not found: %s\n", name);
	      return 0;
	    }
	  }
	}
      }
      x = RNum();
    }
    *(int*)imptab = (int)desc; imptab += 4;
    imp = imp->next;
  }

  mod.start = ftell(f);
  return 1;
}

int main (int argc, char *argv[])
{
  int i, ok;
  BodyProc body;
  int callBackAdr;
  Module *k, *m;

	zerofd = open("/dev/zero", O_RDWR);
	if (zerofd == -1) {
		printf("open /dev/zero failed: %s\n", strerror(errno));
		return 101;
	}

  modlist = NULL;
  dprintf("initializing BlackBox for Linux...\n");
  /*f = fopen(bbfile, "rb");*/
  f = fopen(argv[0], "r");
  if (f != NULL) 
    {
      if (ReadBootHeader())
	{
	  i = 0; ok = 1;
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
		    /* assign the boot info to first variable in Kernel */
		    bootInfo = MemAlloc(sizeof(BootInfo));
		    bootInfo->modList = modlist;
		    bootInfo->argc = argc;
		    bootInfo->argv = argv;
		    *((int*)(k->varBase)) = (int)bootInfo;
		    dprintf("before body\n");
		    body = (BodyProc)(m->code);
		    k->opts = k->opts | init; /* include init in opts */
		    body();
		    dprintf("after body\n");
		  }
	      }
	    PrintMods();
	  }
	} else
	  printf("Invalid BlackBox executable, make sure that the constant exeSize is correctly set\n");

    } else 
      printf("Couldn't find file: %s\n", bbfile);
}
