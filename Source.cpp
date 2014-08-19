#include <iostream>
#include "TinyCL.hpp"

int main() {
	auto device = tcl::information.GetGPU();

	tcl::CLSource source("test.cl", "test", tcl::SourceType::Text);

	tcl::CLExecute exec(source, device);

	const size_t N = 32;

	tcl::CLReadWriteBuffer x(exec, N);
	tcl::CLReadWriteBuffer r(exec, N);

	auto settings = tcl::CLWorkGroupSettings(1, { N }, { 0 }, { 1 }).Optimize(device);

	exec.SetArg(x, r);
	exec.Run(settings);
	
	char a;
	std::cin >> a;
}