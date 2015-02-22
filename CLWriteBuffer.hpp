#ifndef TCL_WRITE_BUFFER_HPP
#define TCL_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* �f�o�C�X���ŏ������݂݂̂ł���o�b�t�@
	*/
	template <typename T>
	class CLWriteBuffer : public CLBuffer
	{
	public:
		/**
		* �f�o�C�X���ŏ������݂݂̂ł���o�b�t�@
		* \param[in] data ���s��ɓ]���ΏۂɂȂ�z��
		*/
		template <typename T>
		CLWriteBuffer(std::vector<T>& data)
			: CLBuffer(CL_MEM_WRITE_ONLY, data.size() * sizeof(T), NULL, &data[0]) { }

		/**
		* �f�o�C�X���ŏ������݂݂̂ł���o�b�t�@
		* \param[in] data ���s��ɓ]���ΏۂɂȂ�z��
		*/
		template <typename T, size_t NUM>
		CLWriteBuffer(std::array<T, NUM>& data)
			: CLBuffer(CL_MEM_WRITE_ONLY, NUM * sizeof(T), NULL, &data[0]) { }

		/**
		* �f�o�C�X���ŏ������݂݂̂ł���o�b�t�@
		* \param[in] data ���s��ɓ]���ΏۂɂȂ�f�[�^
		*/
		template <typename T>
		CLWriteBuffer(T& data)
			: CLBuffer(CL_MEM_WRITE_ONLY, sizeof(T), NULL, &data) { }

		/**
		* �f�o�C�X���ŏ������݂݂̂ł���o�b�t�@
		* \param[in] data ���s��ɓ]���ΏۂɂȂ�z��f�[�^�̃A�h���X
		* \param[in] num �������݂����f�[�^�̌�
		*/
		template <typename T>
		CLWriteBuffer(T* data, const size_t& num)
			: CLBuffer(CL_MEM_WRITE_ONLY, sizeof(T) * num, NULL, data) { }

		virtual ~CLWriteBuffer() {}
	};
}

#endif