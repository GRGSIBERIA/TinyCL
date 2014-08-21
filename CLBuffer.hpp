#ifndef TCL_BUFFER_HPP
#define TCL_BUFFER_HPP

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

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
			auto argSize = size * sizeof(T);
			if (this->size < argSize)
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

		void ReadTest(const cl_uint result) const
		{
			if (result != CL_SUCCESS)
			{
				switch (result)
				{
				case CL_INVALID_COMMAND_QUEUE:
					throw CLException("�R�}���h�L���[�������ł�");
				case CL_INVALID_CONTEXT:
					throw CLException("�R���e�N�X�g�������ł�");
				case CL_INVALID_MEM_OBJECT:
					throw CLException("�o�b�t�@�I�u�W�F�N�g�������ł�");
				case CL_INVALID_VALUE:
					throw CLException("�w�肵���ǂݍ��ݗ̈悪�͈͊O�ł�");
				case CL_INVALID_EVENT_WAIT_LIST:
					throw CLException("�C�x���g�҂����X�g�������ł�");
				case CL_MISALIGNED_SUB_BUFFER_OFFSET:
					throw CLException("�T�u�o�b�t�@�̃I�t�Z�b�g���A���C�������g����Ă��܂���");
				case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
					throw CLException("�ǂݍ��݃R�}���h���u���b�L���O���ǂ���");
				case CL_MEM_OBJECT_ALLOCATION_FAILURE:
					throw CLException("�o�b�t�@�Ɗ֘A�t����ꂽ�f�[�^�ۑ��̈�ւ̃������m�ۂɎ��s����");
				case CL_OUT_OF_RESOURCES:
					throw CLException("�f�o�C�X���̃��\�[�X�̊m�ۂɎ��s����");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("�z�X�g���̃��\�[�X�̊m�ۂɎ��s����");
				}
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
		CLBuffer& Write(const std::vector<T>& enqueueData)
		{
			SizeTest<T>(enqueueData.size());

			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0], 
				0, NULL, NULL);
			ResultTest(result);

			return *this;
		}

		/**
		* �z�X�g������f�o�C�X���ɓ]��
		*/
		template <typename T, size_t NUM>
		CLBuffer& Write(const std::array<T, NUM>& enqueueData)
		{
			SizeTest<T>(enqueueData.size());

			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * enqueueData.size(), &enqueueData[0],
				0, NULL, NULL);
			ResultTest(result);

			return *this;
		}

		/**
		* �z�X�g������f�o�C�X���ɓ]��
		* \attention �z�񓊂���Ɨ����܂�
		*/
		template <typename T>
		CLBuffer& Write(const T& data)
		{
			auto result = clEnqueueWriteBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T), &data,
				0, NULL, NULL);
			ResultTest(result);

			return *this;
		}

		/**
		* �f�o�C�X������z�X�g���ɓ]��
		*/
		template <typename T>
		CLBuffer& Read(std::vector<T>& dequeueData)
		{
			SizeTest<T>(dequeueData.size());

			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dequeueData.size(), &dequeueData[0],
				0, NULL, NULL);
			ReadTest(result);

			return *this;
		}

		/**
		* �f�o�C�X������z�X�g���ɓ]��
		*/
		template <typename T, size_t NUM>
		CLBuffer& Read(std::array<T, NUM>& dequeueData)
		{
			SizeTest<T>(dequeueData.size());
			auto size = sizeof(T) * dequeueData.size();

			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T) * dequeueData.size(), &dequeueData[0],
				0, NULL, NULL);
			ReadTest(result);

			return *this;
		}

		/**
		* �f�o�C�X������z�X�g���ɓ]��
		* \attention �z�񓊂���Ɨ����܂�
		*/
		template <typename T>
		CLBuffer& Read(T& data)
		{
			auto result = clEnqueueReadBuffer(
				exec->CommandQueue(), memory, CL_TRUE,
				0, sizeof(T), &data,
				0, NULL, NULL);
			ReadTest(result);

			return *this;
		}

		
	};
}

#endif