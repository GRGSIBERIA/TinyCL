#ifndef CL_WRITE_BUFFER_HPP
#define CL_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* �f�o�C�X���ŏ������݂݂̂ł���o�b�t�@
	*/
	class CLWriteBuffer : public CLBuffer
	{
	private:
		CLWriteBuffer(const CLWriteBuffer& buf) { }

	public:
		/**
		* �f�o�C�X���ŏ������݂݂̂ł���o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �f�o�C�X���Ɋm�ۂ���̈�
		*/
		CLWriteBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_WRITE_ONLY, size, NULL) { }

		virtual ~CLWriteBuffer() {}
	};
}

#endif