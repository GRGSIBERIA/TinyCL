#ifndef CLBUFFER_HPP
#define CLBUFFER_HPP

#include "CLInformation.hpp"

namespace cl
{
	/**
	* �f�o�C�X���������̈�̃��b�p�[�N���X
	*/
	class CLBuffer
	{
	private:
		cl_mem memory;
		size_t size;

	public:
		/**
		* �m�ۂ����e�ʂ��擾
		* \return �m�ۂ����e��
		*/
		inline const size_t Size() const {
			return size;
		}

		/**
		* �m�ۂ����������I�u�W�F�N�g���擾
		* \return �m�ۂ����������I�u�W�F�N�g
		*/
		inline cl_mem& Memory() {
			return memory;
		}

		cl_mem& operator() () {
			return memory;
		}

	private:
		/**
		* �R�s�[�֎~
		*/
		CLBuffer(const CLBuffer& copy) { }

	protected:
		CLBuffer(const cl_mem_flags flag, const size_t size, void* hostPtr) 
		{
			this->size = size;
			this->memory = clCreateBuffer(information.context, flag, size, hostPtr, &information.result);
		}

		/**
		* ���ň����̂͊댯�Ȃ̂Ńf�t�H���g�R���X�g���N�^�͋֎~
		*/
		CLBuffer() {}

		virtual ~CLBuffer()
		{
			clReleaseMemObject(memory);
		}
	};
}

#endif