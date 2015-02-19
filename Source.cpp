#include <iostream>
#include "TinyCL.hpp"

int main()
{
	// デバイスに渡すための配列を初期化
	const size_t N = 10;
	std::vector<float> input(N);
	for (int i = 0; i < N; ++i) input[i] = i;

	// ソースコードのコンパイル
	tcl::CLController controller("test.cl", "test");

	// デバイス側のメモリを確保
	tcl::CLReadWriteBuffer buf(input);
		
	// N個のワーカーを用意し，引数を設定して実行
	controller.Setting(N).Run(buf);

	// デバイスのメモリから，配列へ読み出す
	buf.Read(input);

	// 中身が正しいかどうか確認する
	for (int i = 0; i < N; ++i)
		std::cout << i << "," << input[i] << std::endl;
	int a;
	std::cin >> a;

	return 0;
}
