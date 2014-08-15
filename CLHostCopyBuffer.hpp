#ifndef CL_HOST_COPY_BUFFER_HPP
#define CL_HOST_COPY_BUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* �z�X�g���̃������̈���C�f�o�C�X���ɃR�s�[�����o�b�t�@
	*/
	class CLHostCopyBuffer : public CLBuffer
	{
	private:
		CLHostCopyBuffer(const CLHostCopyBuffer& buf) { }

	public:
		/**
		* �z�X�g���̃������̈���C�f�o�C�X���ɃR�s�[�����o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �z�X�g���̃������̈�̑傫��
		* \param[in] hostPtr �z�X�g���Ŋm�ۂ��ꂽ�������̈�
		*/
		CLHostCopyBuffer(const size_t size, void* hostPtr)
			: CLBuffer(CL_MEM_COPY_HOST_PTR, size, hostPtr) { }

		virtual ~CLHostCopyBuffer() {}
	};
}

#endif