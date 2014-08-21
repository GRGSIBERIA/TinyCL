#ifndef TCL_READ_WRITE_BUFFER_HPP
#define TCL_READ_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
	*/
	class CLReadWriteBuffer : public CLBuffer<CLReadWriteBuffer>
	{
	private:
		CLReadWriteBuffer(const CLReadWriteBuffer& buf) { }

	public:
		/**
		* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �f�o�C�X���Ɋm�ۂ���̈�
		*/
		CLReadWriteBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_READ_WRITE, size, NULL) { }

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif