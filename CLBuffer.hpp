#ifndef CL_BUFFER_HPP
#define CL_BUFFER_HPP

#include <vector>
#include <array>
#include "CLInformation.hpp"
#include "CLExecute.hpp"

namespace cl
{
	/**
	* デバイス側メモリ領域のラッパークラス
	*/
	class CLBuffer
	{
	private:
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

	protected:
		CLBuffer(const cl_mem_flags flag, const size_t size, void* hostPtr) 
		{
			this->size = size;
			this->memory = clCreateBuffer(information.context, flag, size, hostPtr, &information.result);
		}

		/**
		* 生で扱うのは危険なのでデフォルトコンストラクタは禁止
		*/
		CLBuffer() {}

	public:
		virtual ~CLBuffer()
		{
			clReleaseMemObject(memory);
		}

		template <typename T>
		void Write(CLExecute& exec, const std::vector<T>& enqueueData)
		{
			clEnqueueWriteBuffer(
				exec.CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0], 
				0, NULL, NULL);
		}

		template <typename T, int NUM>
		void Write(CLExecute& exec, const std::array<T, NUM>& enqueueData)
		{
			clEnqueueWriteBuffer(
				exec.CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0],
				0, NULL, NULL);
		}

		void Dequeue()
		{

		}
	};
}

#endif