#ifndef CL_EXECUTE_HPP
#define CL_EXECUTE_HPP

#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "CLExecuteProperty.hpp"
#include "CLDeviceInformation.hpp"

namespace cl
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
					throw CLException("有効なカーネルじゃない");
				case CL_INVALID_ARG_INDEX:
					throw CLException("arg_indexで指定した引数のインデックスが適切じゃない");
				case CL_INVALID_ARG_VALUE:
					throw CLException("");
				case CL_INVALID_MEM_OBJECT:
				case CL_INVALID_SAMPLER:
				case CL_INVALID_ARG_SIZE:
				case CL_OUT_OF_RESOURCES:
				case CL_OUT_OF_HOST_MEMORY:
				default:
					throw CLException("何かエラーが起きて引数をセットできませんでした", result);
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
		* デバイス側にソースの実行を促す
		* \param[in] buffer ソースコード側で利用するためのバッファ
		*/
		template <typename T>
		void SendTask(T& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argCount, sizeof(T), &buffer);
			TestKernelArg(resultArg);
			argCount = 0;

			// タスクの実行
			const auto resultTask = clEnqueueTask(CommandQueue(), Kernel(), 0, NULL, NULL);
			TestEnqueueTask(resultTask);
		}

		/**
		* デバイス側にソースの実行を促す
		* \param[in] buffer ソースコード側で利用するためのバッファ
		* \param[in] otherBuffers 可変長引数
		*/
		template <typename T, typename... Args>
		void SendTask(T& buffer, Args&... otherBuffers)
		{
			auto result = clSetKernelArg(Kernel(), argCount++, sizeof(T), &buffer);
			TestKernelArg(result);
			SendTask(otherBuffers...);
		}
	};
}

#endif