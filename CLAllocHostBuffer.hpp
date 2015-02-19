#ifndef TCL_ALLOC_HOST_BUFFER_HPP
#define TCL_ALLOC_HOST_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* ホスト側にメモリ領域を確保して，それを利用するバッファ（非推奨）
	*/
	class CLAllocHostBuffer : public CLBuffer
	{
	private:
		void* hostPtr;

	private:
		CLAllocHostBuffer(const CLAllocHostBuffer& buf) { }

	public:
		/**
		* 確保されたホスト側のメモリ領域を返す
		* \return ホスト側のメモリ領域
		*/
		inline void* Ptr() {
			return hostPtr;
		}

	public:
		/**
		* ホスト側にメモリ領域を確保して，それを利用するバッファ（非推奨）
		* \attention 危険すぎるので非推奨
		* \param[in] info OpenCLの情報クラス
		* \param[in] size ホスト側とデバイス側で確保するメモリ領域の大きさ
		*/
		CLAllocHostBuffer(const size_t size)
			: CLBuffer(CL_MEM_ALLOC_HOST_PTR, size, hostPtr) { }

		virtual ~CLAllocHostBuffer()
		{
			delete hostPtr;		// 正しいかどうかは知りません
		}
	};
}

#endif