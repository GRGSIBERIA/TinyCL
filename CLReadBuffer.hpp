#ifndef TCL_READ_BUFFER_HPP
#define TCL_READ_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
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
		CLReadBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_READ_ONLY, size, NULL) { }

		virtual ~CLReadBuffer() {}

		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] exec OpenCL�̎��s�N���X
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T>
		CLReadBuffer(CLExecute& exec, std::vector<T>& data)
			: CLBuffer(exec, CL_MEM_READ_ONLY, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] exec OpenCL�̎��s�N���X
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T, size_t NUM>
		CLReadBuffer(CLExecute& exec, std::array<T, NUM>& data)
			: CLBuffer(exec, CL_MEM_READ_ONLY, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] exec OpenCL�̎��s�N���X
		* \param[in] data �o�b�t�@�ɏ������݂����f�[�^
		*/
		template <typename T>
		CLReadBuffer(CLExecute& exec, const T data)
			: CLBuffer(exec, CL_MEM_READ_ONLY, sizeof(T), NULL)
		{
			Write(data);
		}
	};
}

#endif