#include <dlfcn.h>

void * __dlopen (const char *path, int mode)
{
	return dlopen(path, mode);
}

int __dlclose (void *handle)
{
	return dlclose(handle);
}

void * __dlsym (void *handle, const char *symbol)
{
	return dlsym(handle, symbol);
}

const char * __dlerror (void)
{
	return dlerror();
}
