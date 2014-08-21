#ifndef TCL_READ_BUFFER_HPP
#define TCL_READ_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
	*/
	class CLReadBuffer : public CLBuffer<CLReadBuffer>
	{
	private:
		CLReadBuffer(const CLReadBuffer& buf) { }

	public:
		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �f�o�C�X���Ɋm�ۂ���̈�
		*/
		CLReadBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_READ_ONLY, size, NULL) { }

		virtual ~CLReadBuffer() {}
	};
}

#endif