#ifndef CL_EXECUTE_PROPERTY_HPP
#define CL_EXECUTE_PROPERTY_HPP

#include <vector>

#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "ExceptionTest.hpp"

namespace cl
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
			if (information.result != CL_SUCCESS)
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
				}
			}
		}

		void TestKernelResult()
		{
			if (information.result != CL_SUCCESS)
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

	public:
		CLExecuteProperty(CLSource& source, cl_device_id useDeviceId)
		{
			// コマンドキューの設定
			commandQueue = clCreateCommandQueue(information.context, useDeviceId, 0, &information.result);

			// プログラムの読み込み
			LoadSingleProgram(source);
			TestProgramResult();

			// プログラムのビルド
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