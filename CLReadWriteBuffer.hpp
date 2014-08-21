#ifndef TCL_READ_WRITE_BUFFER_HPP
#define TCL_READ_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
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
		CLReadWriteBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_READ_WRITE, size, NULL) { }

		/**
		* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T>
		CLReadWriteBuffer(CLExecute& exec, std::vector<T>& data)
			: CLBuffer(exec, CL_MEM_READ_WRITE, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		/**
		* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
		* \param[in] info OpenCL�̏��N���X
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T, size_t NUM>
		CLReadWriteBuffer(CLExecute& exec, std::array<T, NUM>& data)
			: CLBuffer(exec, CL_MEM_READ_WRITE, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif