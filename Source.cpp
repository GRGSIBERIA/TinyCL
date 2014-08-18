#include <iostream>
#include "TinyCL.hpp"

int main() {
	auto device = tcl::information.GetGPU();

	tcl::CLSource source("__kernel void test(void) { }", "test");

	tcl::CLExecute exec(source, device);

	exec.Run();

	char a;
	std::cin >> a;
}