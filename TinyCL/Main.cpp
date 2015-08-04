#include <CL\cl.h>

int main() {
	cl_platform_id test;
	cl_uint num;
	cl_uint ok = 1;
	clGetPlatformIDs(ok, &test, &num);
	return 0;
}