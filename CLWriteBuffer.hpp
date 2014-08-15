#ifndef CL_WRITE_BUFFER_HPP
#define CL_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* デバイス側で書き込みのみできるバッファ
	*/
	class CLWriteBuffer : public CLBuffer
	{
	private:
		CLWriteBuffer(const CLWriteBuffer& buf) { }

	public:
		/**
		* デバイス側で書き込みのみできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size デバイス側に確保する領域
		*/
		CLWriteBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_WRITE_ONLY, size, NULL) { }

		virtual ~CLWriteBuffer() {}
	};
}

#endif