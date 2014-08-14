#ifndef CLREADWRITEBUFFER_HPP
#define CLREADWRITEBUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* デバイス側で読み書きのできるバッファ
	*/
	class CLReadWriteBuffer : public CLBuffer
	{
	private:
		CLReadWriteBuffer(const CLReadWriteBuffer& buf) { }

	public:
		/**
		* デバイス側で読み書きのできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size デバイス側に確保する領域
		*/
		CLReadWriteBuffer(const size_t size)
			: CLBuffer(CL_MEM_READ_WRITE, size, NULL) { }

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif