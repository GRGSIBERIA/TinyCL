#ifndef CLREADWRITEBUFFER_HPP
#define CLREADWRITEBUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
	*/
	class CLReadWriteBuffer : public CLBuffer
	{
	private:
		CLReadWriteBuffer(const CLReadWriteBuffer& buf) { }

	public:
		/**
		* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �f�o�C�X���Ɋm�ۂ���̈�
		*/
		CLReadWriteBuffer(const size_t size)
			: CLBuffer(CL_MEM_READ_WRITE, size, NULL) { }

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif