#ifndef TCL_READ_WRITE_BUFFER_HPP
#define TCL_READ_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* デバイス側で読み書きのできるバッファ
	*/
	class CLReadWriteBuffer : public CLBuffer<CLReadWriteBuffer>
	{
	private:
		CLReadWriteBuffer(const CLReadWriteBuffer& buf) { }

	public:
		/**
		* デバイス側で読み書きのできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size デバイス側に確保する領域
		*/
		CLReadWriteBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_READ_WRITE, size, NULL) { }

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif