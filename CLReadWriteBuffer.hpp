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
		* \param[in] exec OpenCL�̎��s�N���X
		* \param[in] size �f�o�C�X���Ɋm�ۂ���̈�
		*/
		CLReadWriteBuffer(const size_t size)
			: CLBuffer(CL_MEM_READ_WRITE, size, NULL) { }

		/**
		* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
		* \param[in] exec OpenCL�̎��s�N���X
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T>
		CLReadWriteBuffer(std::vector<T>& data)
			: CLBuffer(CL_MEM_READ_WRITE, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		/**
		* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
		* \param[in] exec OpenCL�̎��s�N���X
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T, size_t NUM>
		CLReadWriteBuffer(std::array<T, NUM>& data)
			: CLBuffer(CL_MEM_READ_WRITE, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		/**
		* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
		* \param[in] exec OpenCL�̎��s�N���X
		* \param[in] data �o�b�t�@�ɏ������݂����f�[�^
		*/
		template <typename T>
		CLReadWriteBuffer(const T data)
			: CLBuffer(CL_MEM_READ_WRITE, sizeof(T), NULL)
		{
			Write(data);
		}

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif