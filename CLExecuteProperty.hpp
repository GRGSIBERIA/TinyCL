#ifndef TCL_EXECUTE_PROPERTY_HPP
#define TCL_EXECUTE_PROPERTY_HPP

#include <vector>

#include "CLInformation.hpp"
#include "CLSource.hpp"

namespace tcl
{
	/**
	* ソースコードの実行を扱うためのクラス
	*/
	class CLExecuteProperty
	{
	public:
		cl_command_queue commandQueue;
		cl_program program;
		cl_kernel kernel;

	private:
		void TestProgramResult()
		{
			if (information.result != CL_SUCCESS || program == NULL)
			{
				switch (information.result)
				{
				case CL_INVALID_CONTEXT:
					throw CLException("コンテクストが無効です");
				case CL_INVALID_VALUE:
					throw CLException("無効な値が指定されました");
				case CL_INVALID_DEVICE:
					throw CLException("デバイスが無効です");
				case CL_INVALID_BINARY:
					throw CLException("プログラムのバイナリが無効です");
				case CL_OUT_OF_RESOURCES:
					throw CLException("デバイス側のリソースを確保できませんでした");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("ホスト側のリソースを確保できませんでした");
				default:
					throw CLException("プログラムオブジェクトがNULLになっています");
				}
			}
		}

		void TestKernelResult()
		{
			if (information.result != CL_SUCCESS || kernel == NULL)
			{
				switch (information.result)
				{
				case CL_INVALID_PROGRAM:
					throw CLException("プログラムオブジェクトが無効です");
				case CL_INVALID_PROGRAM_EXECUTABLE:
					throw CLException("正常にビルドされた実行可能プログラムがありません");
				case CL_INVALID_KERNEL_NAME:
					throw CLException("指定されたカーネル名がプログラム内に見つかりません");
				case CL_INVALID_KERNEL_DEFINITION:
					throw CLException("カーネルの関数定義に対して，プログラムオブジェクトがビルドされたすべてのデバイスで同じではありません");
				case CL_INVALID_VALUE:
					throw CLException("無効な値が指定されました");
				case CL_OUT_OF_RESOURCES:
					throw CLException("デバイス側でリソースの確保に失敗しました");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("ホスト側でリソースの確保に失敗しました");
				default:
					throw CLException("プログラムオブジェクトがNULLになっています");
				}
			}
		}

		void TestBuild(const cl_int& result) const
		{
			if (result != CL_SUCCESS)
			{
				switch (result)
				{
				case CL_INVALID_PROGRAM:
					throw CLException("無効なプログラムオブジェクトです");
				case CL_INVALID_VALUE:
					throw CLException("無効な値です");
				case CL_INVALID_DEVICE:
					throw CLException("デバイスオブジェクトがプログラムオブジェクトに関連付けられていません");
				case CL_INVALID_BINARY:
					throw CLException("バイナリコードを実行できるデバイスがありません");
				case CL_INVALID_BUILD_OPTIONS:
					throw CLException("無効なビルドオプションが指定されました");
				case CL_INVALID_OPERATION:
					throw CLException("これ以前にビルドされた実行可能プログラムのビルドが完了していません");
				case CL_COMPILER_NOT_AVAILABLE:
					throw CLException("コンパイラが利用できません");
				case CL_BUILD_PROGRAM_FAILURE:
					throw CLException("実行可能プログラムのビルドに失敗しました");
				case CL_OUT_OF_RESOURCES:
					throw CLException("デバイス側のリソースの確保に失敗しました");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("ホスト側のリソースの確保に失敗しました");
				}
			}
		}

		void LoadSingleProgram(CLSource& source)
		{
			size_t sourceSize = source.Size();
			auto sourceCode = source.Code();

			switch (source.Type())
			{
			case SourceType::Text:
				program = clCreateProgramWithSource(
					information.context, 1,
					(const char**)&sourceCode, (size_t*)&sourceSize,
					&information.result);
				break;

			case SourceType::Binary:
				program = clCreateProgramWithBinary(
					information.context, information.numDevices, information.deviceIds,
					(size_t*)sourceSize, (const unsigned char**)&sourceCode,
					NULL, &information.result);
				break;

			default:
				throw L"なんかよくわからんソースコードの形式を投げられた";
			}
		}

		void LoadMultiProgram(CLSourceArray& sourceArray)
		{
			switch (sourceArray.Type())
			{
			case SourceType::Text:
				program = clCreateProgramWithSource(
					information.context, 1,
					sourceArray.Pointers(), sourceArray.Sizes(),
					&information.result);
				break;

			case SourceType::Binary:
				program = clCreateProgramWithBinary(
					information.context, information.numDevices, information.deviceIds,
					sourceArray.Sizes(), (const unsigned char**)sourceArray.Pointers(),
					NULL, &information.result);
				break;

			default:
				throw L"なんかよくわからんソースコードの形式を投げられた";
			}
		}

		void BuildProgram()
		{
			const auto result = clBuildProgram(program, information.numDevices, &information.deviceIds[0], NULL, NULL, NULL);
			TestBuild(result);
		}

	public:
		CLExecuteProperty(CLSource& source, cl_device_id useDeviceId)
		{
			// コマンドキューの設定
			commandQueue = clCreateCommandQueue(information.context, useDeviceId, 0, &information.result);

			// プログラムの読み込み
			LoadSingleProgram(source);
			TestProgramResult();

			// プログラムのビルド
			BuildProgram();

			// カーネルの作成
			kernel = clCreateKernel(program, source.KernelName().c_str(), &information.result);
			TestKernelResult();
		}

		CLExecuteProperty(CLSourceArray& sourceArray, cl_device_id useDeviceId)
		{
			// コマンドキューの設定
			commandQueue = clCreateCommandQueue(information.context, useDeviceId, 0, &information.result);

			// プログラムの読み込み
			LoadMultiProgram(sourceArray);
			TestProgramResult();

			// プログラムのビルド
			BuildProgram();

			// カーネルの作成
			kernel = clCreateKernel(program, sourceArray.KernelName().c_str(), &information.result);
			TestKernelResult();
		}

		virtual ~CLExecuteProperty()
		{
			clFlush(commandQueue);
			clFinish(commandQueue);
			clReleaseKernel(kernel);
			clReleaseProgram(program);
			clReleaseCommandQueue(commandQueue);
		}
	};

}

#endif