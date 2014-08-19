#include <iostream>
#include "TinyCL.hpp"

int main() {
	auto device = tcl::information.GetGPU();

	tcl::CLSource source("test.cl", "test", tcl::SourceType::Text);

	tcl::CLExecute exec(source, device);

	const size_t N = 32;
	std::array<float, N> input, output;
	for (int i = 0; i < N; ++i)
	{
		input[i] = i;
		output[i] = 10;
	}

	tcl::CLReadWriteBuffer x(exec, sizeof(float) * N);
	tcl::CLReadWriteBuffer result(exec, sizeof(float) * N);

	x.Write(input);

	auto settings = tcl::CLWorkGroupSettings(1, { N }, { 0 }, { N }).Optimize(device);

	exec.SetArg(result());
	exec.Run(settings);
	
	result.Read(output);

	for (int i = 0; i < N; ++i)
		std::cout << output[i] << std::endl;

	char a;
	std::cin >> a;
}