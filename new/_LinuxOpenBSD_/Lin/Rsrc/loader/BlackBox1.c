// #include <stdio.h>

// extern void SetKernelBaseStack (int);
extern void Init (void);

int main (int argc, char *argv[])
{
	// int res;

	// printf("START\n");

	/* 2012.09.02: This is from oberoncore.ru
		2012.09.05: not required in case of static linking of shared library (-lBB) */
	// asm ("movl %%esp, %[res]" : [res] "=m" (res) );
	// SetKernelBaseStack(res - 8);
	// printf("SetKernelBaseStack(0x%02x): done\n", res - 8);
	// <return adr> <caller ebp> -> base stack = locals of main proc

	Init();

	// printf("END\n");

	return 0;
}
