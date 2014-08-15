#include <CL\cl.h>
#include <iostream>
#include <string>
#include "TinyCL.hpp"

void test(const std::string& hoge) {

}

int main() {
	cl::CLSource source("test.cl", "vec_add");

	cl::CLExecute exec(source, 0);

	cl::CLReadWriteBuffer buf(32);

	std::cout << source.Code() << std::endl;

	char a;
	std::cin >> a;
}