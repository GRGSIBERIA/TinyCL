#ifndef CL_EXECUTE_PROPERTY_HPP
#define CL_EXECUTE_PROPERTY_HPP

#include <vector>

#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "ExceptionTest.hpp"

namespace cl
{
	/**
	* �\�[�X�R�[�h�̎��s���������߂̃N���X
	*/
	class CLExecuteProperty
	{
	public:
		cl_command_queue commandQueue;
		cl_program program;
		cl_kernel kernel;

	private:
		void TestProgramResult()
		{
			if (information.result != CL_SUCCESS)
			{
				switch (information.result)
				{
				case CL_INVALID_CONTEXT:
					throw CLException("�R���e�N�X�g�������ł�");
				case CL_INVALID_VALUE:
					throw CLException("�����Ȓl���w�肳��܂���");
				case CL_INVALID_DEVICE:
					throw CLException("�f�o�C�X�������ł�");
				case CL_INVALID_BINARY:
					throw CLException("�v���O�����̃o�C�i���������ł�");
				case CL_OUT_OF_RESOURCES:
					throw CLException("�f�o�C�X���̃��\�[�X���m�ۂł��܂���ł���");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("�z�X�g���̃��\�[�X���m�ۂł��܂���ł���");
				}
			}
		}

		void TestKernelResult()
		{
			if (information.result != CL_SUCCESS)
			{
				switch (information.result)
				{
				case CL_INVALID_PROGRAM:
					throw CLException("�v���O�����I�u�W�F�N�g�������ł�");
				case CL_INVALID_PROGRAM_EXECUTABLE:
					throw CLException("����Ƀr���h���ꂽ���s�\�v���O����������܂���");
				case CL_INVALID_KERNEL_NAME:
					throw CLException("�w�肳�ꂽ�J�[�l�������v���O�������Ɍ�����܂���");
				case CL_INVALID_KERNEL_DEFINITION:
					throw CLException("�J�[�l���̊֐���`�ɑ΂��āC�v���O�����I�u�W�F�N�g���r���h���ꂽ���ׂẴf�o�C�X�œ����ł͂���܂���");
				case CL_INVALID_VALUE:
					throw CLException("�����Ȓl���w�肳��܂���");
				case CL_OUT_OF_RESOURCES:
					throw CLException("�f�o�C�X���Ń��\�[�X�̊m�ۂɎ��s���܂���");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("�z�X�g���Ń��\�[�X�̊m�ۂɎ��s���܂���");
				}
			}
		}

		void LoadSingleProgram(CLSource& source)
		{
			size_t sourceSize = source.Size();
			auto sourceCode = source.Code();

			switch (source.Type())
			{
			case SourceType::Text:
				program = clCreateProgramWithSource(
					information.context, 1,
					(const char**)&sourceCode, (size_t*)&sourceSize,
					&information.result);
				break;

			case SourceType::Binary:
				program = clCreateProgramWithBinary(
					information.context, information.numDevices, information.deviceIds,
					(size_t*)sourceSize, (const unsigned char**)&sourceCode,
					NULL, &information.result);
				break;

			default:
				throw L"�Ȃ񂩂悭�킩���\�[�X�R�[�h�̌`���𓊂���ꂽ";
			}
		}

		void LoadMultiProgram(CLSourceArray& sourceArray)
		{
			switch (sourceArray.Type())
			{
			case SourceType::Text:
				program = clCreateProgramWithSource(
					information.context, 1,
					sourceArray.Pointers(), sourceArray.Sizes(),
					&information.result);
				break;

			case SourceType::Binary:
				program = clCreateProgramWithBinary(
					information.context, information.numDevices, information.deviceIds,
					sourceArray.Sizes(), (const unsigned char**)sourceArray.Pointers(),
					NULL, &information.result);
				break;

			default:
				throw L"�Ȃ񂩂悭�킩���\�[�X�R�[�h�̌`���𓊂���ꂽ";
			}
		}

	public:
		CLExecuteProperty(CLSource& source, cl_device_id useDeviceId)
		{
			// �R�}���h�L���[�̐ݒ�
			commandQueue = clCreateCommandQueue(information.context, useDeviceId, 0, &information.result);

			// �v���O�����̓ǂݍ���
			LoadSingleProgram(source);
			TestProgramResult();

			// �v���O�����̃r���h
			kernel = clCreateKernel(program, source.KernelName().c_str(), &information.result);
			TestKernelResult();
		}

		CLExecuteProperty(CLSourceArray& sourceArray, cl_device_id useDeviceId)
		{
			// �R�}���h�L���[�̐ݒ�
			commandQueue = clCreateCommandQueue(information.context, useDeviceId, 0, &information.result);

			// �v���O�����̓ǂݍ���
			LoadMultiProgram(sourceArray);
			TestProgramResult();

			// �v���O�����̃r���h
			kernel = clCreateKernel(program, sourceArray.KernelName().c_str(), &information.result);
			TestKernelResult();
		}

		virtual ~CLExecuteProperty()
		{
			clFlush(commandQueue);
			clFinish(commandQueue);
			clReleaseKernel(kernel);
			clReleaseProgram(program);
			clReleaseCommandQueue(commandQueue);
		}
	};

}

#endif