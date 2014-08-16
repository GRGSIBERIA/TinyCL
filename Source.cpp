#include <iostream>
#include <array>

#include "TinyCL.hpp"

template <typename T>
void test(const T& param) {
	std::cout << param << std::endl;
}

template <typename T, typename... Args>
void test(const T& param, const Args&... args) {
	std::cout << param << std::endl;
	test(args...);
}

int main() {
	auto device = cl::information.GetGPU();

	cl::CLSource source("test.cl", "vec_add");

	cl::CLExecute exec(source, device);

	const size_t N = 100;

	std::array<float, N> hostX;
	std::array<float, N> hostY;
	std::array<float, N> hostZ;

	for (int i = 0; i < N; ++i)
	{
		hostX[i] = i;
		hostY[i] = i * i;
		hostZ[i] = i * i * i;
	}

	cl::CLReadWriteBuffer x(exec, sizeof(N) * N);
	cl::CLReadWriteBuffer y(exec, sizeof(N) * N);
	cl::CLReadWriteBuffer z(exec, sizeof(N) * N);

	x.Write(hostX);
	x.Write(hostY);
	x.Write(hostZ);

	exec.SendTask(N, x(), y(), z());

	x.Read(hostX);
	x.Read(hostY);
	x.Read(hostZ);

	char a;
	std::cin >> a;
}