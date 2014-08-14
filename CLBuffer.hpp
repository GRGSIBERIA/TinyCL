#ifndef CLBUFFER_HPP
#define CLBUFFER_HPP

#include "CLInformation.hpp"

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

		virtual ~CLBuffer()
		{
			clReleaseMemObject(memory);
		}
	};
}

#endif