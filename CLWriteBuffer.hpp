#ifndef TCL_WRITE_BUFFER_HPP
#define TCL_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
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

		/**
		* デバイス側で書き込みのみできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size デバイス側に確保する領域
		*/
		template <typename T>
		CLWriteBuffer(CLExecute& exec, const size_t length)
			: CLBuffer(exec, CL_MEM_WRITE_ONLY, sizeof(T) * length, NULL) { }

		virtual ~CLWriteBuffer() {}

	};
}

#endif