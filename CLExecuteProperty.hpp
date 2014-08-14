#ifndef CLEXECUTEPROPERTY_HPP
#define CLEXECUTEPROPERTY_HPP


#include "CLInformation.hpp"
#include "CLSource.hpp"

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
		void LoadSingleProgram(CLSource& source)
		{
			size_t sourceSize = source.Size();
			auto sourceCode = source.Code();

			switch (source.Type())
			{
			case SourceType::Text:
				clCreateProgramWithSource(
					information.context, 1,
					(const char**)&sourceCode, (size_t*)&sourceSize,
					&information.result);
				break;

			case SourceType::Binary:
				clCreateProgramWithBinary(
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
				clCreateProgramWithSource(
					information.context, 1,
					sourceArray.Pointers(), sourceArray.Sizes(),
					&information.result);
				break;

			case SourceType::Binary:
				clCreateProgramWithBinary(
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

			// �v���O�����̃r���h
			kernel = clCreateKernel(program, source.KernelName().c_str(), &information.result);
		}

		CLExecuteProperty(CLSourceArray& sourceArray, cl_device_id useDeviceId)
		{
			// �R�}���h�L���[�̐ݒ�
			commandQueue = clCreateCommandQueue(information.context, useDeviceId, 0, &information.result);

			// �v���O�����̓ǂݍ���
			LoadMultiProgram(sourceArray);

			// �v���O�����̃r���h
			kernel = clCreateKernel(program, sourceArray.KernelName().c_str(), &information.result);
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