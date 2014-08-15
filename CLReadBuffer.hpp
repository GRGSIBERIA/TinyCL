#ifndef CL_READ_BUFFER_HPP
#define CL_READ_BUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* デバイス側で読み込みのみできるバッファ
	*/
	class CLReadBuffer : public CLBuffer
	{
	private:
		CLReadBuffer(const CLReadBuffer& buf) { }

	public:
		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size デバイス側に確保する領域
		*/
		CLReadBuffer(const size_t size)
			: CLBuffer(CL_MEM_READ_ONLY, size, NULL) { }

		virtual ~CLReadBuffer() {}
	};
}

#endif