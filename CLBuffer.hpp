#ifndef CL_BUFFER_HPP
#define CL_BUFFER_HPP

#include <vector>
#include <array>
#include "CLInformation.hpp"
#include "CLExecute.hpp"

namespace tcl
{
	/**
	* デバイス側メモリ領域のラッパークラス
	*/
	class CLBuffer
	{
	private:
		CLExecute* exec;
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
			if (this->size < size * sizeof(T))
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

	protected:
		/**
		* 生で扱うのは危険なのでデフォルトコンストラクタは禁止
		*/
		CLBuffer()
			: exec(NULL), size(0) {}

		CLBuffer(CLExecute& exec, const cl_mem_flags flag, const size_t size, void* hostPtr)
			: exec(&exec), size(size)
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
		void Write(const std::vector<T>& enqueueData)
		{
			SizeTest<T>(enqueueData.size());

			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0], 
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* ホスト側からデバイス側に転送
		*/
		template <typename T, size_t NUM>
		void Write(const std::array<T, NUM>& enqueueData)
		{
			SizeTest<T>(enqueueData.size());

			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0],
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* ホスト側からデバイス側に転送
		*/
		template <typename T>
		void Write(const T& enqueueData, const unsigned dataCount)
		{
			SizeTest<T>(dataCount);

			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dataCount, &enqueueData,
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* デバイス側からホスト側に転送
		*/
		template <typename T>
		void Read(std::vector<T>& dequeueData)
		{
			SizeTest<T>(dequeueData.size());

			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dequeueData.size(), &dequeueData[0],
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* デバイス側からホスト側に転送
		*/
		template <typename T, size_t NUM>
		void Read(std::array<T, NUM>& dequeueData)
		{
			SizeTest<T>(dequeueData.size());

			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dequeueData.size(), &dequeueData[0],
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* デバイス側からホスト側に転送
		*/
		template <typename T>
		void Read(T& dequeueData, const unsigned dataCount)
		{
			SizeTest<T>(dataCount);

			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dataCount, &enqueueData,
				0, NULL, NULL);
			ResultTest(result);
		}
	};
}

#endif