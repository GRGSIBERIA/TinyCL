#ifndef CL_HOST_COPY_BUFFER_HPP
#define CL_HOST_COPY_BUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* ホスト側のメモリ領域を，デバイス側にコピーしたバッファ
	*/
	class CLHostCopyBuffer : public CLBuffer
	{
	private:
		CLHostCopyBuffer(const CLHostCopyBuffer& buf) { }

	public:
		/**
		* ホスト側のメモリ領域を，デバイス側にコピーしたバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size ホスト側のメモリ領域の大きさ
		* \param[in] hostPtr ホスト側で確保されたメモリ領域
		*/
		CLHostCopyBuffer(const size_t size, void* hostPtr)
			: CLBuffer(CL_MEM_COPY_HOST_PTR, size, hostPtr) { }

		virtual ~CLHostCopyBuffer() {}
	};
}

#endif