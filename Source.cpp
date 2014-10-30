#include <iostream>
#include "TinyCL.hpp"

int main()
{
	// GPUのデバイスを取得する
	auto device = tcl::information.GetGPU();

	// ソースコードのコンパイル
	tcl::CLSource source("test.cl", "test", tcl::SourceType::Text);

	// カーネル実行クラスの生成
	tcl::CLExecute exec(source, device);

	// デバイスに渡すための配列を初期化
	const size_t N = 10;
	std::vector<float> input(N);
	for (int i = 0; i < N; ++i)
		input[i] = i;

	// デバイス側のメモリを確保
	tcl::CLReadWriteBuffer x(exec, input);
	
	// 並列実行したいカーネル（GPUクラスタ）の数を設定する
	// 1次元配列で，0からスタートし，N個の長さを持っていて，それをN個ごとに区切る
	auto settings = tcl::CLWorkGroupSettings(1, { 0 }, { N }, { N }).Optimize(device);
	
	// 引数を設定する
	exec.SetArg(x);

	// 設定を渡して実行
	exec.Run(settings);

	// デバイスのメモリから，配列へ読み出す
	x.Read(input);

	// かくにん
	for (int i = 0; i < N; ++i)
		std::cout << i << "," << input[i] << std::endl;

	// TODO: 速度を取ってみたい

	char a;
	std::cin >> a;
}
