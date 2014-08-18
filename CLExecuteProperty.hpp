#ifndef TCL_EXECUTE_PROPERTY_HPP
#define TCL_EXECUTE_PROPERTY_HPP

#include <vector>

#include "CLInformation.hpp"
#include "CLSource.hpp"

namespace tcl
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
			if (information.result != CL_SUCCESS || program == NULL)
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
				default:
					throw CLException("�v���O�����I�u�W�F�N�g��NULL�ɂȂ��Ă��܂�");
				}
			}
		}

		void TestKernelResult()
		{
			if (information.result != CL_SUCCESS || kernel == NULL)
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
				default:
					throw CLException("�v���O�����I�u�W�F�N�g��NULL�ɂȂ��Ă��܂�");
				}
			}
		}

		void TestBuild(const cl_int& result) const
		{
			if (result != CL_SUCCESS)
			{
				switch (result)
				{
				case CL_INVALID_PROGRAM:
					throw CLException("�����ȃv���O�����I�u�W�F�N�g�ł�");
				case CL_INVALID_VALUE:
					throw CLException("�����Ȓl�ł�");
				case CL_INVALID_DEVICE:
					throw CLException("�f�o�C�X�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�Ɋ֘A�t�����Ă��܂���");
				case CL_INVALID_BINARY:
					throw CLException("�o�C�i���R�[�h�����s�ł���f�o�C�X������܂���");
				case CL_INVALID_BUILD_OPTIONS:
					throw CLException("�����ȃr���h�I�v�V�������w�肳��܂���");
				case CL_INVALID_OPERATION:
					throw CLException("����ȑO�Ƀr���h���ꂽ���s�\�v���O�����̃r���h���������Ă��܂���");
				case CL_COMPILER_NOT_AVAILABLE:
					throw CLException("�R���p�C�������p�ł��܂���");
				case CL_BUILD_PROGRAM_FAILURE:
					throw CLException("���s�\�v���O�����̃r���h�Ɏ��s���܂���");
				case CL_OUT_OF_RESOURCES:
					throw CLException("�f�o�C�X���̃��\�[�X�̊m�ۂɎ��s���܂���");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("�z�X�g���̃��\�[�X�̊m�ۂɎ��s���܂���");
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

		void BuildProgram()
		{
			const auto result = clBuildProgram(program, information.numDevices, &information.deviceIds[0], NULL, NULL, NULL);
			TestBuild(result);
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
			BuildProgram();

			// �J�[�l���̍쐬
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
			BuildProgram();

			// �J�[�l���̍쐬
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