#include <iostream>
#include "TinyCL.hpp"

int main() {
	auto device = tcl::information.GetGPU();

	tcl::CLSource source("__kernel void test(void) { }", "test");

	tcl::CLExecute exec(source, device);

	//auto settings = tcl::CLWorkGroupSettings(1, { 5 }, { 0 }, { 5 }).Optimize(device);

	//exec.Run(settings);

	char a;
	std::cin >> a;
}