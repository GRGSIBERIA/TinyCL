#ifndef TCL_BUFFER_HPP
#define TCL_BUFFER_HPP

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#include <vector>
#include <array>
#include <mutex>
#include "CLInformation.hpp"

namespace tcl
{
	// CLExecuteで循環参照してしまった
	class CLExecute;

	/**
	* デバイス側メモリ領域のラッパークラス
	*/
	template <typename T>
	class CLBuffer
	{
	private:
		cl_mem memory;
		size_t size;
		T* hostDataPtr;

	public:
		/**
		* 確保した容量を取得
		* \return 確保した容量
		*/
		inline const size_t Size() const {
			return size;
		}

		/**
		* 確保したメモリオブジェクトを取得
		* \return 確保したメモリオブジェクト
		*/
		inline cl_mem& Memory() {
			return memory;
		}

	private:
		/**
		* コピー禁止
		*/
		CLBuffer(const CLBuffer& copy) { }

		template <typename T>
		void SizeTest(const size_t& size) const
		{
			auto argSize = size * sizeof(T);
			if (this->size < argSize)
			{
				throw CLFailedAllacException("バッファのサイズよりホスト側のサイズの方が大きい");
			}
		}

		void ResultTest(const cl_int result) const
		{
			if (result != CL_SUCCESS)
			{
				throw CLException("WriteかReadに失敗しました", result);
			}
		}

		void ReadTest(const cl_uint result) const
		{
			if (result != CL_SUCCESS)
			{
				switch (result)
				{
				case CL_INVALID_COMMAND_QUEUE:
					throw CLException("コマンドキューが無効です");
				case CL_INVALID_CONTEXT:
					throw CLException("コンテクストが無効です");
				case CL_INVALID_MEM_OBJECT:
					throw CLException("バッファオブジェクトが無効です");
				case CL_INVALID_VALUE:
					throw CLException("指定した読み込み領域が範囲外です");
				case CL_INVALID_EVENT_WAIT_LIST:
					throw CLException("イベント待ちリストが無効です");
				case CL_MISALIGNED_SUB_BUFFER_OFFSET:
					throw CLException("サブバッファのオフセットがアラインメントされていません");
				case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
					throw CLException("読み込みコマンドがブロッキングがどうの");
				case CL_MEM_OBJECT_ALLOCATION_FAILURE:
					throw CLException("バッファと関連付けられたデータ保存領域へのメモリ確保に失敗した");
				case CL_OUT_OF_RESOURCES:
					throw CLException("デバイス側のリソースの確保に失敗した");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("ホスト側のリソースの確保に失敗した");
				}
			}
		}

		void SafeEnqueueRead(CLExecute& exec)
		{
			auto result = clEnqueueReadBuffer(
				exec.CommandQueue(), memory, CL_TRUE,
				0, size, hostDataPtr,
				0, NULL, NULL);
			ReadTest(result);
		}

		void SafeEnqueueWrite(CLExecute& exec)
		{
			auto result = clEnqueueWriteBuffer(
				exec.CommandQueue(), memory, CL_TRUE,
				0, size, hostDataPtr,
				0, NULL, NULL);
			ResultTest(result);
		}

		cl_mem CreateBuffer(const cl_mem_flags& flag)
		{
			return clCreateBuffer(information.context, flag, size, NULL, &information.result)
		}

	protected:
		/**
		* 生で扱うのは危険なのでデフォルトコンストラクタは禁止
		*/
		CLBuffer()
			: size(0) {}

		//template <typename T>
		//CLBuffer(const cl_mem_flags flag, const size_t& size, void* hostPtr, T* hostDataPtr)
		//	: size(size), hostDataPtr(hostDataPtr)
		//{
		//	memory = clCreateBuffer(information.context, flag, size, hostPtr, &information.result);
		//}

		template <typename T>
		CLBuffer(const cl_mem_flags& flag, std::vector<T>& data)
			: size(data.size() * sizeof(T)), hostDataPtr(&data[0])
		{
			memory = CreateBuffer(flag);
		}

		template <typename T, size_t NUM>
		CLBuffer(const cl_mem_flags& flag, std::array<T, NUM>& data)
			: size(data.size() * sizeof(T)), hostDataPtr(&data[0])
		{
			memory = CreateBuffer(flag);
		}

		template <typename T>
		CLBuffer(const cl_mem_flags& flag, T& data)
			: size(sizeof(T)), hostDataPtr(&data)
		{
			memory = CreateBuffer(flag);
		}

		template <typename T>
		CLBuffer(const cl_mem_flags& flag, T* data, const size_t& num)
			: size(sizeof(T) * num), hostDataPtr(data)
		{
			memory = CreateBuffer(flag);
		}

	public:
		virtual ~CLBuffer()
		{
			clReleaseMemObject(memory);
		}

		/**
		* ホスト側からデバイス側に転送
		*/
		CLBuffer& Write(CLExecute& exec)
		{
			SafeEnqueueWrite(exec);
			return *this;
		}

		/**
		* デバイス側からホスト側に転送
		*/
		CLBuffer& Read(CLExecute& exec)
		{
			SafeEnqueueRead(exec);
			return *this;
		}
	};
}

#endif