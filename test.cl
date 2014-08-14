__kernel void vec_add(size_t n, __global float* z,
	__global float* x, __global float* y){
	const size_t para = 4;
	const size_t end = (n / para) * para;

	for (size_t i = 0; i<end; i += para){
		float4 vtmp = vload4(0, x + i) + vload4(0, y + i);
		vstore4(vtmp, 0, z + i);
	}

	for (size_t i = end; i<n; ++i){
		z[i] = x[i] + y[i];
	}
}