__kernel void TestMain(
	__global float* inputA, __global float* inputB)
{
	int gid = get_local_id(0);
	inputA[gid] = gid;
	inputB[gid] = gid * gid;
}