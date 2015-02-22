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
	class CLBuffer
	{
	private:
		static CLExecute* exec;			// 循環参照してしまったのでポインタにしてる
		static std::mutex execMutex;	// マルチスレッド対策
		cl_mem memory;
		size_t size;

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

		cl_mem& operator() () {
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

		template <typename T>
		void SafeEnqueueRead(size_t size, T* data)
		{
			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, size, data,
				0, NULL, NULL);
			ReadTest(result);
		}

		template <typename T>
		void SafeEnqueueWrite(size_t size, T* data)
		{
			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, size, data,
				0, NULL, NULL);
			ResultTest(result);
		}

	protected:
		/**
		* 生で扱うのは危険なのでデフォルトコンストラクタは禁止
		*/
		CLBuffer()
			: size(0) {}

		CLBuffer(const cl_mem_flags flag, const size_t size, void* hostPtr)
			: size(size)
		{
			memory = clCreateBuffer(information.context, flag, size, hostPtr, &information.result);
		}

	public:
		virtual ~CLBuffer()
		{
			clReleaseMemObject(memory);
		}

		/**
		* ホスト側からデバイス側に転送
		*/
		template <typename T>
		CLBuffer& Write(const std::vector<T>& enqueueData)
		{
			SizeTest<T>(enqueueData.size());
			SafeEnqueueWrite(sizeof(T) * enqueueData.size(), &enqueueData[0]);
			return *this;
		}

		/**
		* ホスト側からデバイス側に転送
		*/
		template <typename T, size_t NUM>
		CLBuffer& Write(const std::array<T, NUM>& enqueueData)
		{
			SizeTest<T>(enqueueData.size());
			SafeEnqueueWrite(sizeof(T) * enqueueData.size(), &enqueueData[0]);
			return *this;
		}

		/**
		* ホスト側からデバイス側に転送
		* \attention 配列投げると落ちます
		*/
		template <typename T>
		CLBuffer& Write(const T& data)
		{
			SafeEnqueueWrite(sizeof(T), &data);
			return *this;
		}

		/**
		* ホスト側からデバイス側に転送
		* \attention 生ポインタ配列を扱う場合
		*/
		template <typename T>
		CLBuffer& Write(T* data, const size_t num)
		{
			SafeEnqueueWrite(sizeof(T) * num, data);
			return *this;
		}

		/**
		* デバイス側からホスト側に転送
		*/
		template <typename T>
		CLBuffer& Read(std::vector<T>& dequeueData)
		{
			SizeTest<T>(dequeueData.size());
			SafeEnqueueRead(sizeof(T) * dequeueData.size(), &dequeueData[0]);
			return *this;
		}

		/**
		* デバイス側からホスト側に転送
		*/
		template <typename T, size_t NUM>
		CLBuffer& Read(std::array<T, NUM>& dequeueData)
		{
			SizeTest<T>(dequeueData.size());
			SafeEnqueueRead(sizeof(T) * dequeueData.size(), &dequeueData[0]);
			return *this;
		}

		/**
		* デバイス側からホスト側に転送
		* \attention 配列投げると落ちます
		*/
		template <typename T>
		CLBuffer& Read(T& data)
		{
			SafeEnqueueRead(sizeof(T), &data);
			return *this;
		}

		/**
		* デバイス側からホスト側に転送
		* \attention 生ポインタ配列を扱う場合
		*/
		template <typename T>
		CLBuffer& Read(T* data, const size_t num)
		{
			SafeEnqueueRead(sizeof(T) * num, data);
			return *this;
		}
	};
}

#endif