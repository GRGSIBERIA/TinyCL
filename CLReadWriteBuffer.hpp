#ifndef TCL_READ_WRITE_BUFFER_HPP
#define TCL_READ_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* �f�o�C�X���œǂݏ����̂ł���o�b�t�@
	*/
	template <typename T>
	class CLReadWriteBuffer : public CLBuffer
	{
	private:
		CLReadWriteBuffer(const CLReadWriteBuffer& buf) { }

	public:
		/**
		* �f�o�C�X���œǂݏ����ł���o�b�t�@
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T>
		CLReadWriteBuffer(std::vector<T>& data)
			: CLBuffer(CL_MEM_READ_WRITE, data.size() * sizeof(T), NULL, &data[0]) { }

		/**
		* �f�o�C�X���œǂݏ����ł���o�b�t�@
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T, size_t NUM>
		CLReadWriteBuffer(std::array<T, NUM>& data)
			: CLBuffer(CL_MEM_READ_WRITE, NUM * sizeof(T), NULL, &data[0]) { }

		/**
		* �f�o�C�X���œǂݏ����ł���o�b�t�@
		* \param[in] data �o�b�t�@�ɏ������݂����f�[�^
		*/
		template <typename T>
		CLReadWriteBuffer(T& data)
			: CLBuffer(CL_MEM_READ_WRITE, sizeof(T), NULL, &data) { }

		/**
		* �f�o�C�X���œǂݏ����ł���o�b�t�@
		* \param[in] data �o�b�t�@�ɏ������݂����z��f�[�^�̃A�h���X
		* \param[in] num �������݂����f�[�^�̌�
		*/
		template <typename T>
		CLReadWriteBuffer(T* data, const size_t& num)
			: CLBuffer(CL_MEM_READ_WRITE, sizeof(T) * num, NULL, data) { }

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif