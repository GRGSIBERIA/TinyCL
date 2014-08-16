#ifndef CL_EXECUTE_HPP
#define CL_EXECUTE_HPP

#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "CLExecuteProperty.hpp"
#include "CLDeviceInformation.hpp"

namespace cl
{
	class CLExecute
	{
	private:
		CLExecuteProperty executeProperty;

		unsigned argCount;

	private:
		void TestKernelArg(const cl_int& result) const 
		{
			if (result != CL_SUCCESS)
			{
				switch (result)
				{
				case CL_INVALID_KERNEL:
					throw CLException("�L���ȃJ�[�l������Ȃ�");
				case CL_INVALID_ARG_INDEX:
					throw CLException("arg_index�Ŏw�肵�������̃C���f�b�N�X���K�؂���Ȃ�");
				case CL_INVALID_ARG_VALUE:
					throw CLException("");
				case CL_INVALID_MEM_OBJECT:
				case CL_INVALID_SAMPLER:
				case CL_INVALID_ARG_SIZE:
				case CL_OUT_OF_RESOURCES:
				case CL_OUT_OF_HOST_MEMORY:
				default:
					throw CLException("�����G���[���N���Ĉ������Z�b�g�ł��܂���ł���", result);
				}
			}
		}

		void TestEnqueueTask(const cl_int& result) const
		{
			if (result != CL_SUCCESS)
				throw CLException("�����G���[���N���ă^�X�N�����s�ł��܂���ł���", result);
		}

	public:
		inline cl_command_queue& CommandQueue() {
			return executeProperty.commandQueue;
		}

		inline cl_program& Program() {
			return executeProperty.program;
		}

		inline cl_kernel& Kernel() {
			return executeProperty.kernel;
		}

	public:
		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] source �\�[�X�R�[�h
		* \param[in] useDeviceId���p�������f�o�C�XID
		*/
		CLExecute(CLSource& source, const cl_device_id& useDeviceId)
			: executeProperty(source, useDeviceId), argCount(0)
		{

		}

		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] sourceArray �\�[�X�R�[�h�̔z��N���X
		* \param[in] useDeviceId ���p�������f�o�C�XID
		*/
		CLExecute(CLSourceArray& sourceArray, const cl_device_id& useDeviceId)
			: executeProperty(sourceArray, useDeviceId), argCount(0)
		{

		}

		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] source �\�[�X�R�[�h
		* \param[in] device �f�o�C�X���̃C���X�^���X
		*/
		CLExecute(CLSource& source, const CLDeviceInformation& device)
			: executeProperty(source, device.DeviceId()), argCount(0)
		{

		}

		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] sourceArray �\�[�X�R�[�h�̔z��N���X
		* \param[in] device �f�o�C�X���̃C���X�^���X
		*/
		CLExecute(CLSourceArray& sourceArray, const CLDeviceInformation& device)
			: executeProperty(sourceArray, device.DeviceId()), argCount(0)
		{

		}

		/**
		* �f�o�C�X���Ƀ\�[�X�̎��s�𑣂�
		* \param[in] buffer �\�[�X�R�[�h���ŗ��p���邽�߂̃o�b�t�@
		*/
		template <typename T>
		void SendTask(T& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argCount, sizeof(T), &buffer);
			TestKernelArg(resultArg);
			argCount = 0;

			// �^�X�N�̎��s
			const auto resultTask = clEnqueueTask(CommandQueue(), Kernel(), 0, NULL, NULL);
			TestEnqueueTask(resultTask);
		}

		/**
		* �f�o�C�X���Ƀ\�[�X�̎��s�𑣂�
		* \param[in] buffer �\�[�X�R�[�h���ŗ��p���邽�߂̃o�b�t�@
		* \param[in] otherBuffers �ϒ�����
		*/
		template <typename T, typename... Args>
		void SendTask(T& buffer, Args&... otherBuffers)
		{
			auto result = clSetKernelArg(Kernel(), argCount++, sizeof(T), &buffer);
			TestKernelArg(result);
			SendTask(otherBuffers...);
		}
	};
}

#endif