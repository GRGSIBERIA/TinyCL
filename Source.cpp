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

	cl::CLReadWriteBuffer buf(exec, 32);

	std::array<int, 4> arr;
	buf.Write(arr);

	char a;
	std::cin >> a;
}