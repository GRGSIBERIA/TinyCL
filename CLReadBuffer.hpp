#ifndef TCL_READ_BUFFER_HPP
#define TCL_READ_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
	*/
	template <typename T>
	class CLReadBuffer : public CLBuffer
	{
	public:
		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T>
		CLReadBuffer(std::vector<T>& data)
			: CLBuffer(CL_MEM_READ_ONLY, data) { }

		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] data �o�b�t�@�ɏ������݂����z��
		*/
		template <typename T, size_t NUM>
		CLReadBuffer(std::array<T, NUM>& data)
			: CLBuffer(CL_MEM_READ_ONLY, data) { }

		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] data �o�b�t�@�ɏ������݂����f�[�^
		*/
		template <typename T>
		CLReadBuffer(T& data)
			: CLBuffer(CL_MEM_READ_ONLY, data) { }

		/**
		* �f�o�C�X���œǂݍ��݂݂̂ł���o�b�t�@
		* \param[in] data �o�b�t�@�ɏ������݂����z��f�[�^�̃A�h���X
		* \param[in] num �������݂����f�[�^�̌�
		*/
		template <typename T>
		CLReadBuffer(T* data, const size_t& num)
			: CLBuffer(CL_MEM_READ_ONLY, data, num) { }

		virtual ~CLReadBuffer() {}
	};
}

#endif