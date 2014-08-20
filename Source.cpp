#include <iostream>
#include "TinyCL.hpp"

int main()
{
	auto device = tcl::information.GetGPU();

	tcl::CLSource source("test.cl", "test", tcl::SourceType::Text);

	tcl::CLExecute exec(source, device);

	const size_t N = 10;
	std::vector<float> input(N), output(N);
	for (int i = 0; i < N; ++i)
	{
		input[i] = i + 1;
		output[i] = 0;
	}

	tcl::CLReadWriteBuffer x(exec, sizeof(float) * N);

	x.Write(input);

	auto settings = tcl::CLWorkGroupSettings(1, { 0 }, { N }, { N }).Optimize(device);
	exec.SetArg(x());
	clSetKernelArg(exec.Kernel(), 0, sizeof(cl_mem), &x());
	exec.Run(settings);

	//clSetKernelArg(exec.Kernel(), 0, sizeof(cl_mem), &x.Memory());
	//size_t of[3] = { 0 };
	//size_t gw[3] = { N };
	//size_t lw[3] = { N };
	//clEnqueueNDRangeKernel(exec.CommandQueue(), exec.Kernel(), 1, of, gw, lw, 0, NULL, NULL);

	x.Read(output);

	for (int i = 0; i < N; ++i)
		std::cout << input[i] << "," << output[i] << std::endl;

	char a;
	std::cin >> a;
}