#include <dlfcn.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	void * h;
	int res;

	if (argc == 2) {
		h = dlopen(argv[1], RTLD_LAZY | RTLD_GLOBAL);
		if (h != NULL) {
			res = 0;
		} else {
			perror(dlerror());
			res = 2;
		}
	} else {
		fprintf(stderr, "usage: %s file.so\n", argv[0]);
		res = 1;
	}

	return res;
}
