#ifndef TCL_EXECUTE_HPP
#define TCL_EXECUTE_HPP

#include <typeinfo>


#include "CLInformation.hpp"
#include "CLSourceArray.hpp"
#include "CLExecuteProperty.hpp"
#include "CLDeviceInformation.hpp"
#include "CLWorkGroupSettings.hpp"
#include "CLBuffer.hpp"

namespace tcl
{
	class CLExecute
	{
	private:
		CLExecuteProperty executeProperty;
		cl_event event;

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

		void TestNDRange(const cl_int& result) const
		{
			if (result != CL_SUCCESS)
			{
				switch (result)
				{
				case CL_INVALID_PROGRAM_EXECUTABLE:
					throw CLException("コマンドキューに関連付けられた正常にビルドされたプログラムが存在しません");
				case CL_INVALID_COMMAND_QUEUE:
					throw CLException("コマンドキューが無効です");
				case CL_INVALID_KERNEL:
					throw CLException("カーネルが無効です");
				case CL_INVALID_CONTEXT:
					throw CLException("コンテキストが無効です");
				case CL_INVALID_KERNEL_ARGS:
					throw CLException("カーネル引数が指定されていません");
				case CL_INVALID_WORK_DIMENSION:
					throw CLException("カーネルの分割数が適切ではありません");
				case CL_INVALID_GLOBAL_OFFSET:
					throw CLException("オフセットの値がワークサイズを超えています");
				case CL_INVALID_WORK_GROUP_SIZE:
					throw CLException("ワークグループの大きさが不正です");
				case CL_INVALID_WORK_ITEM_SIZE:
					throw CLException("ワークアイテム数が不正です");
				case CL_MISALIGNED_SUB_BUFFER_OFFSET:
					throw CLException("サブバッファオフセットのアラインメントが不正です");
				case CL_INVALID_IMAGE_SIZE:
					throw CLException("カーネルに指定したイメージオブジェクトがデバイスでサポートされていません");
				case CL_OUT_OF_RESOURCES:
					throw CLException("カーネルの実行に必要なリソースが不足しています");
				case CL_MEM_OBJECT_ALLOCATION_FAILURE:
					throw CLException("メモリオブジェクトの領域を確保するのに失敗しました");
				case CL_INVALID_EVENT_WAIT_LIST:
					throw CLException("イベント待ちリストが不正です");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("ホスト側のメモリのリソース確保に失敗しました");
				}
			}
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
		CLExecute& SetArg(T& buffer)
		{
#define TEST_BUFFER(X) typeid(buffer)==typeid(X)
			// CLBufferを継承しているかどうか検証する
			// いい方法があれば誰か教えて……
			if (
				TEST_BUFFER(CLReadWriteBuffer) || 
				TEST_BUFFER(CLReadBuffer) || 
				TEST_BUFFER(CLWriteBuffer) ||
				TEST_BUFFER(CLBuffer))
#undef TEST_BUFFER
			{
				SetBuffer(buffer);
			}
			else
			{
				cl_int resultArg;
				resultArg = clSetKernelArg(Kernel(), argCount++, sizeof(T), &buffer);
				TestKernelArg(resultArg);
			}
			return *this;
		}
		
		/**
		* カーネルに渡すための引数を差し替えor設定する
		* \param argIndex 引数のインデックス
		* \param buffer カーネルで利用したいバッファ
		*/
		template <typename T>
		CLExecute& SetArg(const cl_uint argIndex, T& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argIndex, sizeof(T), &buffer);
			TestKernelArg(resultArg);
			return *this;
		}

		/**
		* カーネルに渡すための引数を設定する
		* \param[in] buffer カーネルで利用するためのバッファ
		* \param[in] otherBuffers 可変長引数
		*/
		template <typename T, typename... Args>
		CLExecute& SetArg(T& buffer, Args&... otherBuffers)
		{
			if (typeid(buffer) == typeid(CLReadWriteBuffer))
				SetBuffer(buffer);
			else
				SetArg(buffer);
			argCount++;
			SetArg(otherBuffers...);
			argCount = 0;
			return *this;
		}

		/**
		* カーネルに渡すためのバッファを設定する
		* \param[in] buffer CLBufferのインスタンス
		*/
		CLExecute& SetBuffer(CLBuffer& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argCount++, sizeof(cl_mem), &buffer.Memory());
			TestKernelArg(resultArg);
			return *this;
		}

		/**
		* カーネルに渡すためのバッファを引数番号を指定して設定する
		* \param[in] argIndex 引数番号
		* \param[in] buffer CLBufferのインスタンス
		*/
		CLExecute& SetBuffer(const cl_uint argIndex, CLBuffer& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argIndex, sizeof(cl_mem), &buffer.Memory());
			TestKernelArg(resultArg);
			return *this;
		}

		/**
		* カーネルに渡すためのバッファを設定する
		* \param[in] buffer CLBufferのインスタンス
		* \param[in] otherBuffers 同じ，CLBufferのインスタンス
		*/
		template <typename... Args>
		CLExecute& SetBuffer(CLBuffer& buffer, Args&... otherBuffers)
		{
			SetBuffer(buffer);
			argCount++;
			SetBuffer(otherBuffers...);
			argCount = 0;
			return *this;
		}

		/**
		* 単一のカーネルを実行させる
		* \param[in] wait 実行終了まで待つかどうか
		*/
		CLExecute& Run()
		{
			auto resultTask = clEnqueueTask(CommandQueue(), Kernel(), 0, NULL, &event);

			TestEnqueueTask(resultTask);
			return *this;
		}

		/**
		* 範囲指定してカーネルを実行する
		* \param[in] setting 実行範囲を指定するクラスのインスタンス
		* \param[in] wait 実行終了まで待つかどうか
		*/
		CLExecute& Run(CLWorkGroupSettings& setting)
		{
			auto result = clEnqueueNDRangeKernel(
				CommandQueue(), Kernel(), setting.Dimension(), 
				setting.Offset(), setting.WorkerRange(), setting.SplitSize(),
				0, NULL, &event);

			TestNDRange(result);
			return *this;
		}

		CLExecute& Wait()
		{
			clWaitForEvents(1, &event);
			return *this;
		}
	};
}

#endif