#ifndef CL_READ_BUFFER_HPP
#define CL_READ_BUFFER_HPP

#include "CLBuffer.hpp"

namespace cl
{
	/**
	* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
	*/
	class CLReadBuffer : public CLBuffer
	{
	private:
		CLReadBuffer(const CLReadBuffer& buf) { }

	public:
		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] size �f�o�C�X���Ɋm�ۂ���̈�
		*/
		CLReadBuffer(const size_t size)
			: CLBuffer(CL_MEM_READ_ONLY, size, NULL) { }

		virtual ~CLReadBuffer() {}
	};
}

#endif