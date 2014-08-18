#include <iostream>
#include "TinyCL.hpp"

void test(const std::vector<size_t>& test)
{

}

int main() {
	auto device = tcl::information.GetGPU();

	tcl::CLSource source("__kernel void test(void) { }", "test");

	tcl::CLExecute exec(source, device);

	exec.Run();

	test({ 1, 2, 3 });

	char a;
	std::cin >> a;
}