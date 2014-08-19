__kernel void test(
	__global const float* x, 
	__global float* result)
{
	int gid = get_global_id(0);

	result[31] = 0.0;
}