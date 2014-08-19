__kernel void test(__global float* x, __global float* result){
	int gid = get_global_id(0);

	result[gid] = x[gid];
}