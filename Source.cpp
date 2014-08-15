#include <iostream>
#include <array>

#include "TinyCL.hpp"

int main() {
	auto device = cl::information.GetGPU();

	cl::CLSource source("test.cl", "vec_add");

	cl::CLExecute exec(source, device);

	cl::CLReadWriteBuffer buf(exec, 32);

	std::array<int, 4> arr = {0, 1, 2, 3};

	buf.Write(arr);

	// タスクの実行をここでやる

	buf.Read(arr);

	char a;
	std::cin >> a;
}