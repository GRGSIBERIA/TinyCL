#ifndef TCL_EXECUTE_HPP
#define TCL_EXECUTE_HPP

#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "CLExecuteProperty.hpp"
#include "CLDeviceInformation.hpp"
#include "CLWorkGroupSettings.hpp"

namespace tcl
{
	class CLExecute
	{
	private:
		CLExecuteProperty executeProperty;

		unsigned argCount;

	private:
		void TestKernelArg(const cl_int& result) const 
		{
			if (result != CL_SUCCESS)
			{
				switch (result)
				{
				case CL_INVALID_KERNEL:
					throw CLException("無効なカーネルが指定されています");
				case CL_INVALID_ARG_INDEX:
					throw CLException("引数のインデックスが不正です");
				case CL_INVALID_ARG_VALUE:
					throw CLException("引数の値が不正です");
				case CL_INVALID_MEM_OBJECT:
					throw CLException("メモリオブジェクトが不正です");
				case CL_INVALID_SAMPLER:
					throw CLException("サンプラーオブジェクトが不正です");
				case CL_INVALID_ARG_SIZE:
					throw CLException("引数で指定したサイズが不正です");
				case CL_OUT_OF_RESOURCES:
					throw CLException("デバイス側のリソースを確保できませんでした");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("ホスト側のリソースを確保できませんでした");
				}
			}
		}

		void TestEnqueueTask(const cl_int& result) const
		{
			if (result != CL_SUCCESS)
				throw CLException("何かエラーが起きてタスクを実行できませんでした", result);
		}

		

	public:
		inline cl_command_queue& CommandQueue() {
			return executeProperty.commandQueue;
		}

		inline cl_program& Program() {
			return executeProperty.program;
		}

		inline cl_kernel& Kernel() {
			return executeProperty.kernel;
		}

	public:
		/**
		* カーネルの実行やバッファの転送を管理する
		* \param[in] info OpenCLの情報クラス
		* \param[in] source ソースコード
		* \param[in] useDeviceId利用したいデバイスID
		*/
		CLExecute(CLSource& source, const cl_device_id& useDeviceId)
			: executeProperty(source, useDeviceId), argCount(0)
		{

		}

		/**
		* カーネルの実行やバッファの転送を管理する
		* \param[in] info OpenCLの情報クラス
		* \param[in] sourceArray ソースコードの配列クラス
		* \param[in] useDeviceId 利用したいデバイスID
		*/
		CLExecute(CLSourceArray& sourceArray, const cl_device_id& useDeviceId)
			: executeProperty(sourceArray, useDeviceId), argCount(0)
		{

		}

		/**
		* カーネルの実行やバッファの転送を管理する
		* \param[in] info OpenCLの情報クラス
		* \param[in] source ソースコード
		* \param[in] device デバイス情報のインスタンス
		*/
		CLExecute(CLSource& source, const CLDeviceInformation& device)
			: executeProperty(source, device.DeviceId()), argCount(0)
		{

		}

		/**
		* カーネルの実行やバッファの転送を管理する
		* \param[in] info OpenCLの情報クラス
		* \param[in] sourceArray ソースコードの配列クラス
		* \param[in] device デバイス情報のインスタンス
		*/
		CLExecute(CLSourceArray& sourceArray, const CLDeviceInformation& device)
			: executeProperty(sourceArray, device.DeviceId()), argCount(0)
		{

		}

		/**
		* カーネルに渡すための引数を設定する
		* \param[in] buffer カーネルで利用するためのバッファ
		*/
		template <typename T>
		void SetArg(T& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argCount, sizeof(T), &buffer);
			TestKernelArg(resultArg);
			argCount = 0;
		}

		/**
		* カーネルに渡すための引数を差し替えor設定する
		* \param argIndex 引数のインデックス
		* \param buffer カーネルで利用したいバッファ
		*/
		template <typename T>
		void SetArg(const cl_uint argIndex, T& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argIndex, sizeof(T), &buffer);
			TestKernelArg(resultArg);
		}

		/**
		* カーネルに渡すための引数を設定する
		* \param[in] buffer カーネルで利用するためのバッファ
		* \param[in] otherBuffers 可変長引数
		*/
		template <typename T, typename... Args>
		void SetArg(T& buffer, Args&... otherBuffers)
		{
			SetArg(buffer);
			argCount++;
			SetArg(otherBuffers...);
		}

		/**
		* 単一のカーネルを実行させる
		*/
		void Run()
		{
			// タスクの実行
			const auto resultTask = clEnqueueTask(CommandQueue(), Kernel(), 0, NULL, NULL);
			TestEnqueueTask(resultTask);
		}

		void Run(CLWorkGroupSettings& setting)
		{
			auto result = clEnqueueNDRangeKernel(
				CommandQueue(), Kernel(), setting.Dimension(), 
				setting.WorkerSize(), setting.Offset(), setting.SplitSize(), 
				0, NULL, NULL);
		}
	};
}

#endif