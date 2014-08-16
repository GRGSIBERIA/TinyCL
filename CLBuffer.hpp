#ifndef CL_BUFFER_HPP
#define CL_BUFFER_HPP

#include <vector>
#include <array>
#include "CLInformation.hpp"
#include "CLExecute.hpp"

namespace tcl
{
	/**
	* �f�o�C�X���������̈�̃��b�p�[�N���X
	*/
	class CLBuffer
	{
	private:
		CLExecute* exec;
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

		template <typename T>
		void SizeTest(const size_t& size) const
		{
			if (this->size < size * sizeof(T))
			{
				throw CLFailedAllacException("�o�b�t�@�̃T�C�Y���z�X�g���̃T�C�Y�̕����傫��");
			}
		}

		void ResultTest(const cl_int result) const
		{
			if (result != CL_SUCCESS)
			{
				throw CLException("Write��Read�Ɏ��s���܂���", result);
			}
		}

	protected:
		/**
		* ���ň����̂͊댯�Ȃ̂Ńf�t�H���g�R���X�g���N�^�͋֎~
		*/
		CLBuffer()
			: exec(NULL), size(0) {}

		CLBuffer(CLExecute& exec, const cl_mem_flags flag, const size_t size, void* hostPtr)
			: exec(&exec), size(size)
		{
			memory = clCreateBuffer(information.context, flag, size, hostPtr, &information.result);
		}

	public:
		virtual ~CLBuffer()
		{
			clReleaseMemObject(memory);
		}

		/**
		* �z�X�g������f�o�C�X���ɓ]��
		*/
		template <typename T>
		void Write(const std::vector<T>& enqueueData)
		{
			SizeTest<T>(enqueueData.size());

			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0], 
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* �z�X�g������f�o�C�X���ɓ]��
		*/
		template <typename T, size_t NUM>
		void Write(const std::array<T, NUM>& enqueueData)
		{
			SizeTest<T>(enqueueData.size());

			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0],
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* �z�X�g������f�o�C�X���ɓ]��
		*/
		template <typename T>
		void Write(const T& enqueueData, const unsigned dataCount)
		{
			SizeTest<T>(dataCount);

			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dataCount, &enqueueData,
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* �f�o�C�X������z�X�g���ɓ]��
		*/
		template <typename T>
		void Read(std::vector<T>& dequeueData)
		{
			SizeTest<T>(dequeueData.size());

			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dequeueData.size(), &dequeueData[0],
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* �f�o�C�X������z�X�g���ɓ]��
		*/
		template <typename T, size_t NUM>
		void Read(std::array<T, NUM>& dequeueData)
		{
			SizeTest<T>(dequeueData.size());

			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dequeueData.size(), &dequeueData[0],
				0, NULL, NULL);
			ResultTest(result);
		}

		/**
		* �f�o�C�X������z�X�g���ɓ]��
		*/
		template <typename T>
		void Read(T& dequeueData, const unsigned dataCount)
		{
			SizeTest<T>(dataCount);

			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dataCount, &enqueueData,
				0, NULL, NULL);
			ResultTest(result);
		}
	};
}

#endif