#ifndef TCL_READ_BUFFER_HPP
#define TCL_READ_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* デバイス側で読み込みのみできるバッファ
	*/
	class CLReadBuffer : public CLBuffer<CLReadBuffer>
	{
	private:
		CLReadBuffer(const CLReadBuffer& buf) { }

	public:
		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size デバイス側に確保する領域
		*/
		CLReadBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_READ_ONLY, size, NULL) { }

		virtual ~CLReadBuffer() {}
	};
}

#endif