#include <iostream>
#include "TinyCL.hpp"

int main()
{
	// デバイスに渡すための配列を0で初期化
	const size_t N = 10;
	std::vector<float> inputA(N, 0), inputB(N, 0);

	// ソースコードのコンパイル
	tcl::CLController controller("test.cl", "TestMain");

	// デバイス側のメモリを確保
	tcl::CLReadWriteBuffer bufA(inputA), bufB(inputB);

	// N個のワーカーを用意し，引数を設定して実行
	controller.Setting(N).Run(bufA, bufB).Wait();

	// デバイスのメモリから，配列へ読み出す
	bufA.Read(inputA);
	bufB.Read(inputB);

	// 中身が正しいかどうか確認する
	for (int i = 0; i < N; ++i)
		std::cout << i << "," << inputA[i] << "," << inputB[i] << std::endl;

	return 0;
}
