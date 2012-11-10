#include <dlfcn.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	void * h;
	int res;

	h = dlopen("libBB0.so", RTLD_LAZY | RTLD_GLOBAL);
	if (h != NULL) {
		res = 0;
	} else {
		perror(dlerror());
		res = 1;
	}

	return res;
}
