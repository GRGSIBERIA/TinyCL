#ifndef TCL_EXECUTE_HPP
#define TCL_EXECUTE_HPP

#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "CLExecuteProperty.hpp"
#include "CLDeviceInformation.hpp"
#include "CLWorkGroupSettings.hpp"

namespace tcl
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
					throw CLException("�����ȃJ�[�l�����w�肳��Ă��܂�");
				case CL_INVALID_ARG_INDEX:
					throw CLException("�����̃C���f�b�N�X���s���ł�");
				case CL_INVALID_ARG_VALUE:
					throw CLException("�����̒l���s���ł�");
				case CL_INVALID_MEM_OBJECT:
					throw CLException("�������I�u�W�F�N�g���s���ł�");
				case CL_INVALID_SAMPLER:
					throw CLException("�T���v���[�I�u�W�F�N�g���s���ł�");
				case CL_INVALID_ARG_SIZE:
					throw CLException("�����Ŏw�肵���T�C�Y���s���ł�");
				case CL_OUT_OF_RESOURCES:
					throw CLException("�f�o�C�X���̃��\�[�X���m�ۂł��܂���ł���");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("�z�X�g���̃��\�[�X���m�ۂł��܂���ł���");
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
		* �J�[�l���ɓn�����߂̈�����ݒ肷��
		* \param[in] buffer �J�[�l���ŗ��p���邽�߂̃o�b�t�@
		*/
		template <typename T>
		void SetArg(T& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argCount, sizeof(T), &buffer);
			TestKernelArg(resultArg);
			argCount = 0;
		}

		/**
		* �J�[�l���ɓn�����߂̈����������ւ�or�ݒ肷��
		* \param argIndex �����̃C���f�b�N�X
		* \param buffer �J�[�l���ŗ��p�������o�b�t�@
		*/
		template <typename T>
		void SetArg(const cl_uint argIndex, T& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argIndex, sizeof(T), &buffer);
			TestKernelArg(resultArg);
		}

		/**
		* �J�[�l���ɓn�����߂̈�����ݒ肷��
		* \param[in] buffer �J�[�l���ŗ��p���邽�߂̃o�b�t�@
		* \param[in] otherBuffers �ϒ�����
		*/
		template <typename T, typename... Args>
		void SetArg(T& buffer, Args&... otherBuffers)
		{
			SetArg(buffer);
			argCount++;
			SetArg(otherBuffers...);
		}

		/**
		* �P��̃J�[�l�������s������
		*/
		void Run()
		{
			// �^�X�N�̎��s
			const auto resultTask = clEnqueueTask(CommandQueue(), Kernel(), 0, NULL, NULL);
			TestEnqueueTask(resultTask);
		}

		void Run(CLWorkGroupSettings& setting)
		{
			auto result = clEnqueueNDRangeKernel(
				CommandQueue(), Kernel(), setting.Dimension(), 
				setting.WorkerSize(), setting.Offset(), setting.SplitSize(), 
				0, NULL, NULL);
		}
	};
}

#endif