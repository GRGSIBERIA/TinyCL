#ifndef CL_ALLOC_HOST_BUFFER_HPP
#define CL_ALLOC_HOST_BUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* �z�X�g���Ƀ������̈���m�ۂ��āC����𗘗p����o�b�t�@
	*/
	class CLAllocHostBuffer : public CLBuffer
	{
	private:
		void* hostPtr;

	private:
		CLAllocHostBuffer(const CLAllocHostBuffer& buf) { }

	public:
		/**
		* �m�ۂ��ꂽ�z�X�g���̃������̈��Ԃ�
		* \return �z�X�g���̃������̈�
		*/
		inline void* Ptr() {
			return hostPtr;
		}

	public:
		/**
		* �z�X�g���Ƀ������̈���m�ۂ��āC����𗘗p����o�b�t�@
		* �댯������̂Ŕ񐄏�
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �z�X�g���ƃf�o�C�X���Ŋm�ۂ��郁�����̈�̑傫��
		*/
		CLAllocHostBuffer(const size_t size)
			: CLBuffer(CL_MEM_ALLOC_HOST_PTR, size, hostPtr) { }

		virtual ~CLAllocHostBuffer()
		{
			delete hostPtr;		// ���������ǂ����͒m��܂���
		}
	};
}

#endif