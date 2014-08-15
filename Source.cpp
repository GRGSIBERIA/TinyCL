#include <CL\cl.h>
#include <iostream>
#include <string>
#include "TinyCL.hpp"

void test(const std::string& hoge) {

}

int main() {
	auto device = cl::information.GetGPU();

	cl::CLSource source("test.cl", "vec_add");

	cl::CLExecute exec(source, device);

	cl::CLReadWriteBuffer buf(32);

	std::array<int, 12> arr;
	buf.Write(exec, arr);

	char a;
	std::cin >> a;
}