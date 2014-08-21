#ifndef TCL_HOST_COPY_BUFFER_HPP
#define TCL_HOST_COPY_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* ホスト側のメモリ領域を，デバイス側にコピーするバッファ（非推奨）
	*/
	class CLHostCopyBuffer : public CLBuffer
	{
	private:
		CLHostCopyBuffer(const CLHostCopyBuffer& buf) { }

	public:
		/**
		* ホスト側のメモリ領域を，デバイス側にコピーするバッファ（非推奨）
		* \param[in] info OpenCLの情報クラス
		* \param[in] size ホスト側のメモリ領域の大きさ
		* \param[in] hostPtr ホスト側で確保されたメモリ領域
		*/
		CLHostCopyBuffer(CLExecute& exec, const size_t size, void* hostPtr)
			: CLBuffer(exec, CL_MEM_COPY_HOST_PTR, size, hostPtr) { }

		virtual ~CLHostCopyBuffer() {}
	};
}

#endif