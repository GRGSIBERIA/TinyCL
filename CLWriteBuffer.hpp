#ifndef TCL_WRITE_BUFFER_HPP
#define TCL_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
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
		CLWriteBuffer(const size_t size)
			: CLBuffer(CL_MEM_WRITE_ONLY, size, NULL) { }

		/**
		* �f�o�C�X���ŏ������݂݂̂ł���o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �f�o�C�X���Ɋm�ۂ���̈�
		*/
		template <typename T>
		CLWriteBuffer(const size_t length)
			: CLBuffer(CL_MEM_WRITE_ONLY, sizeof(T) * length, NULL) { }

		virtual ~CLWriteBuffer() {}

	};
}

#endif