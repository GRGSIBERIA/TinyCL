#ifndef CLUSEHOSTBUFFER_HPP
#define CLUSEHOSTBUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* �z�X�g���̃������̈�𗘗p����o�b�t�@
	*/
	class CLUseHostBuffer : public CLBuffer
	{
	private:
		CLUseHostBuffer(const CLUseHostBuffer& buf) { }

	public:
		/**
		* �z�X�g���̃������̈�𗘗p����o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �z�X�g���̃������̈�̑傫��
		* \param[in] hostPtr �z�X�g���Ŋm�ۂ��ꂽ�������̈�
		*/
		CLUseHostBuffer(const size_t size, void* hostPtr)
			: CLBuffer(CL_MEM_USE_HOST_PTR, size, hostPtr) { }

		virtual ~CLUseHostBuffer() {}
	};
}

#endif