#include <iostream>
#include "TinyCL.hpp"

int main() {
	auto device = tcl::information.GetGPU();

	tcl::CLSource source("test.cl", "test", tcl::SourceType::Text);

	tcl::CLExecute exec(source, device);

	const size_t N = 32;
	std::array<float, N> input, output;
	for (int i = 0; i < N; ++i)
		input[i] = i;

	tcl::CLReadWriteBuffer x(exec, sizeof(float) * N);
	tcl::CLReadWriteBuffer r(exec, sizeof(float) * N);

	x.Write(input);

	auto settings = tcl::CLWorkGroupSettings(1, { N }, { 0 }, { 1 }).Optimize(device);

	exec.SetArg(x(), r());
	exec.Run(settings);
	
	r.Read(output);

	for (int i = 0; i < N; ++i)
		std::cout << i << std::endl;

	char a;
	std::cin >> a;
}