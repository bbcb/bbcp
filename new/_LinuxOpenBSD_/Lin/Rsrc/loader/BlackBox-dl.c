#include <dlfcn.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	void * h;
	/*
	void * h1;
	void (*SetKernelBaseStack) (int);
	void (*Init) (void);
	*/
	int res;

	h = dlopen("libBB.so", RTLD_LAZY | RTLD_GLOBAL);
	if (h != NULL) {
		/*
		h1 = dlsym(h, "SetKernelBaseStack");
		if (h1 != NULL) {
			*(void **) (&SetKernelBaseStack) = h1;
			h1 = dlsym(h, "Init");
			if (h1 != NULL) {
				*(void **) (&Init) = h1;

				asm ("movl %%esp, %[res]" : [res] "=m" (res) );
				SetKernelBaseStack(res - 8);

				printf("before Init()\n");
				Init();
				printf("after Init()\n");

				res = 0;
			} else {
				perror(dlerror());
				res = 3;
			}
		} else {
			perror(dlerror());
			res = 2;
		}
		*/
		res = 0;
	} else {
		perror(dlerror());
		res = 1;
	}

	return res;
}
