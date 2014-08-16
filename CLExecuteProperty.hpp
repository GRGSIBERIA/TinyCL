#ifndef CL_EXECUTE_PROPERTY_HPP
#define CL_EXECUTE_PROPERTY_HPP


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
		void TestProgramResult()
		{
			if (information.result != CL_SUCCESS)
			{
				switch (information.result)
				{
				case CL_INVALID_VALUE:
				case CL_OUT_OF_RESOURCES:
				case CL_OUT_OF_HOST_MEMORY:
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