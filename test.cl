__kernel void test(
	__global float* result)
{
	int gid = get_local_id(0);
	result[gid] = 100.0;
}