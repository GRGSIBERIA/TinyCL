#ifndef TCL_USE_HOST_BUFFER_HPP
#define TCL_USE_HOST_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* �z�X�g���̃������̈�𗘗p����o�b�t�@
	*/
	class CLUseHostBuffer : public CLBuffer<CLUseHostBuffer>
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
		CLUseHostBuffer(CLExecute& exec, const size_t size, void* hostPtr)
			: CLBuffer(exec, CL_MEM_USE_HOST_PTR, size, hostPtr) { }

		virtual ~CLUseHostBuffer() {}
	};
}

#endif