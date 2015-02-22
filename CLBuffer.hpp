#ifndef TCL_BUFFER_HPP
#define TCL_BUFFER_HPP

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#include <vector>
#include <array>
#include <mutex>
#include "CLInformation.hpp"

namespace tcl
{
	// CLExecute�ŏz�Q�Ƃ��Ă��܂���
	class CLExecute;

	/**
	* �f�o�C�X���������̈�̃��b�p�[�N���X
	*/
	template <typename T>
	class CLBuffer
	{
	private:
		cl_mem memory;
		size_t size;
		T* hostDataPtr;

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

		void SafeEnqueueRead(CLExecute& exec)
		{
			auto result = clEnqueueReadBuffer(
				exec.CommandQueue(), memory, CL_TRUE,
				0, size, hostDataPtr,
				0, NULL, NULL);
			ReadTest(result);
		}

		void SafeEnqueueWrite(CLExecute& exec)
		{
			auto result = clEnqueueWriteBuffer(
				exec.CommandQueue(), memory, CL_TRUE,
				0, size, hostDataPtr,
				0, NULL, NULL);
			ResultTest(result);
		}

		cl_mem CreateBuffer(const cl_mem_flags& flag)
		{
			return clCreateBuffer(information.context, flag, size, NULL, &information.result)
		}

	protected:
		/**
		* ���ň����̂͊댯�Ȃ̂Ńf�t�H���g�R���X�g���N�^�͋֎~
		*/
		CLBuffer()
			: size(0) {}

		//template <typename T>
		//CLBuffer(const cl_mem_flags flag, const size_t& size, void* hostPtr, T* hostDataPtr)
		//	: size(size), hostDataPtr(hostDataPtr)
		//{
		//	memory = clCreateBuffer(information.context, flag, size, hostPtr, &information.result);
		//}

		template <typename T>
		CLBuffer(const cl_mem_flags& flag, std::vector<T>& data)
			: size(data.size() * sizeof(T)), hostDataPtr(&data[0])
		{
			memory = CreateBuffer(flag);
		}

		template <typename T, size_t NUM>
		CLBuffer(const cl_mem_flags& flag, std::array<T, NUM>& data)
			: size(data.size() * sizeof(T)), hostDataPtr(&data[0])
		{
			memory = CreateBuffer(flag);
		}

		template <typename T>
		CLBuffer(const cl_mem_flags& flag, T& data)
			: size(sizeof(T)), hostDataPtr(&data)
		{
			memory = CreateBuffer(flag);
		}

		template <typename T>
		CLBuffer(const cl_mem_flags& flag, T* data, const size_t& num)
			: size(sizeof(T) * num), hostDataPtr(data)
		{
			memory = CreateBuffer(flag);
		}

	public:
		virtual ~CLBuffer()
		{
			clReleaseMemObject(memory);
		}

		/**
		* �z�X�g������f�o�C�X���ɓ]��
		*/
		CLBuffer& Write(CLExecute& exec)
		{
			SafeEnqueueWrite(exec);
			return *this;
		}

		/**
		* �f�o�C�X������z�X�g���ɓ]��
		*/
		CLBuffer& Read(CLExecute& exec)
		{
			SafeEnqueueRead(exec);
			return *this;
		}
	};
}

#endif