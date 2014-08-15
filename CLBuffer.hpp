#ifndef CL_BUFFER_HPP
#define CL_BUFFER_HPP

#include <vector>
#include <array>
#include "CLInformation.hpp"
#include "CLExecute.hpp"

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

	public:
		virtual ~CLBuffer()
		{
			clReleaseMemObject(memory);
		}

		template <typename T>
		void Write(CLExecute& exec, const std::vector<T>& enqueueData)
		{
			clEnqueueWriteBuffer(
				exec.CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0], 
				0, NULL, NULL);
		}

		template <typename T, int NUM>
		void Write(CLExecute& exec, const std::array<T, NUM>& enqueueData)
		{
			clEnqueueWriteBuffer(
				exec.CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0],
				0, NULL, NULL);
		}

		void Dequeue()
		{

		}
	};
}

#endif