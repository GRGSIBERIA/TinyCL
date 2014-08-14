#ifndef CLUSEHOSTBUFFER_HPP
#define CLUSEHOSTBUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* ホスト側のメモリ領域を利用するバッファ
	*/
	class CLUseHostBuffer : public CLBuffer
	{
	private:
		CLUseHostBuffer(const CLUseHostBuffer& buf) { }

	public:
		/**
		* ホスト側のメモリ領域を利用するバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size ホスト側のメモリ領域の大きさ
		* \param[in] hostPtr ホスト側で確保されたメモリ領域
		*/
		CLUseHostBuffer(const size_t size, void* hostPtr)
			: CLBuffer(CL_MEM_USE_HOST_PTR, size, hostPtr) { }

		virtual ~CLUseHostBuffer() {}
	};
}

#endif