#ifndef TCL_EXECUTE_HPP
#define TCL_EXECUTE_HPP

#include <typeinfo>

#include "CLException.hpp"
#include "CLInformation.hpp"
#include "CLSourceArray.hpp"
#include "CLExecuteProperty.hpp"
#include "CLDeviceInformation.hpp"
#include "CLWorkGroupSettings.hpp"
#include "CLBuffer.hpp"

namespace tcl
{
	class CLExecute
	{
	private:
		CLExecuteProperty executeProperty;
		cl_event event;

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

		void TestNDRange(const cl_int& result) const
		{
			if (result != CL_SUCCESS)
			{
				switch (result)
				{
				case CL_INVALID_PROGRAM_EXECUTABLE:
					throw CLException("�R�}���h�L���[�Ɋ֘A�t����ꂽ����Ƀr���h���ꂽ�v���O���������݂��܂���");
				case CL_INVALID_COMMAND_QUEUE:
					throw CLException("�R�}���h�L���[�������ł�");
				case CL_INVALID_KERNEL:
					throw CLException("�J�[�l���������ł�");
				case CL_INVALID_CONTEXT:
					throw CLException("�R���e�L�X�g�������ł�");
				case CL_INVALID_KERNEL_ARGS:
					throw CLException("�J�[�l���������w�肳��Ă��܂���");
				case CL_INVALID_WORK_DIMENSION:
					throw CLException("�J�[�l���̕��������K�؂ł͂���܂���");
				case CL_INVALID_GLOBAL_OFFSET:
					throw CLException("�I�t�Z�b�g�̒l�����[�N�T�C�Y�𒴂��Ă��܂�");
				case CL_INVALID_WORK_GROUP_SIZE:
					throw CLException("���[�N�O���[�v�̑傫�����s���ł�");
				case CL_INVALID_WORK_ITEM_SIZE:
					throw CLException("���[�N�A�C�e�������s���ł�");
				case CL_MISALIGNED_SUB_BUFFER_OFFSET:
					throw CLException("�T�u�o�b�t�@�I�t�Z�b�g�̃A���C�������g���s���ł�");
				case CL_INVALID_IMAGE_SIZE:
					throw CLException("�J�[�l���Ɏw�肵���C���[�W�I�u�W�F�N�g���f�o�C�X�ŃT�|�[�g����Ă��܂���");
				case CL_OUT_OF_RESOURCES:
					throw CLException("�J�[�l���̎��s�ɕK�v�ȃ��\�[�X���s�����Ă��܂�");
				case CL_MEM_OBJECT_ALLOCATION_FAILURE:
					throw CLException("�������I�u�W�F�N�g�̗̈���m�ۂ���̂Ɏ��s���܂���");
				case CL_INVALID_EVENT_WAIT_LIST:
					throw CLException("�C�x���g�҂����X�g���s���ł�");
				case CL_OUT_OF_HOST_MEMORY:
					throw CLException("�z�X�g���̃������̃��\�[�X�m�ۂɎ��s���܂���");
				}
			}
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
			: executeProperty(source, device.DeviceId), argCount(0)
		{

		}

		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] sourceArray �\�[�X�R�[�h�̔z��N���X
		* \param[in] device �f�o�C�X���̃C���X�^���X
		*/
		CLExecute(CLSourceArray& sourceArray, const CLDeviceInformation& device)
			: executeProperty(sourceArray, device.DeviceId), argCount(0)
		{

		}

		/**
		* �J�[�l���ɓn�����߂̈�����ݒ肷��
		* \param[in] buffer �J�[�l���ŗ��p���邽�߂̃o�b�t�@
		*/
		template <typename T>
		CLExecute& SetArg(T& buffer)
		{
#define TEST_BUFFER(X) typeid(buffer)==typeid(X)
			// CLBuffer���p�����Ă��邩�ǂ������؂���
			// �������@������ΒN�������āc�c
			if (
				TEST_BUFFER(CLReadWriteBuffer) || 
				TEST_BUFFER(CLReadBuffer) || 
				TEST_BUFFER(CLWriteBuffer) ||
				TEST_BUFFER(CLBuffer))
#undef TEST_BUFFER
			{
				SetBuffer(buffer);
			}
			else
			{
				cl_int resultArg;
				resultArg = clSetKernelArg(Kernel(), argCount++, sizeof(T), &buffer);
				TestKernelArg(resultArg);
			}
			return *this;
		}
		
		/**
		* �J�[�l���ɓn�����߂̈����������ւ�or�ݒ肷��
		* \param argIndex �����̃C���f�b�N�X
		* \param buffer �J�[�l���ŗ��p�������o�b�t�@
		*/
		template <typename T>
		CLExecute& SetArg(const cl_uint argIndex, T& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argIndex, sizeof(T), &buffer);
			TestKernelArg(resultArg);
			return *this;
		}

		/**
		* �J�[�l���ɓn�����߂̈�����ݒ肷��
		* \param[in] buffer �J�[�l���ŗ��p���邽�߂̃o�b�t�@
		* \param[in] otherBuffers �ϒ�����
		*/
		template <typename T, typename... Args>
		CLExecute& SetArg(T& buffer, Args&... otherBuffers)
		{
			if (typeid(buffer) == typeid(CLReadWriteBuffer))
				SetBuffer(buffer);
			else
				SetArg(buffer);
			argCount++;
			SetArg(otherBuffers...);
			argCount = 0;
			return *this;
		}

		/**
		* �J�[�l���ɓn�����߂̃o�b�t�@��ݒ肷��
		* \param[in] buffer CLBuffer�̃C���X�^���X
		*/
		CLExecute& SetBuffer(CLBuffer& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argCount++, sizeof(cl_mem), &buffer.Memory());
			TestKernelArg(resultArg);
			return *this;
		}

		/**
		* �J�[�l���ɓn�����߂̃o�b�t�@�������ԍ����w�肵�Đݒ肷��
		* \param[in] argIndex �����ԍ�
		* \param[in] buffer CLBuffer�̃C���X�^���X
		*/
		CLExecute& SetBuffer(const cl_uint argIndex, CLBuffer& buffer)
		{
			const auto resultArg = clSetKernelArg(Kernel(), argIndex, sizeof(cl_mem), &buffer.Memory());
			TestKernelArg(resultArg);
			return *this;
		}

		/**
		* �J�[�l���ɓn�����߂̃o�b�t�@��ݒ肷��
		* \param[in] buffer CLBuffer�̃C���X�^���X
		* \param[in] otherBuffers �����CCLBuffer�̃C���X�^���X
		*/
		template <typename... Args>
		CLExecute& SetBuffer(CLBuffer& buffer, Args&... otherBuffers)
		{
			SetBuffer(buffer);
			argCount++;
			SetBuffer(otherBuffers...);
			argCount = 0;
			return *this;
		}

		/**
		* �P��̃J�[�l�������s������
		* \param[in] wait ���s�I���܂ő҂��ǂ���
		*/
		CLExecute& Run()
		{
			auto resultTask = clEnqueueTask(CommandQueue(), Kernel(), 0, NULL, &event);

			TestEnqueueTask(resultTask);
			return *this;
		}

		/**
		* �͈͎w�肵�ăJ�[�l�������s����
		* \param[in] setting ���s�͈͂��w�肷��N���X�̃C���X�^���X
		* \param[in] wait ���s�I���܂ő҂��ǂ���
		*/
		CLExecute& Run(CLWorkGroupSettings& setting)
		{
			auto result = clEnqueueNDRangeKernel(
				CommandQueue(), Kernel(), setting.Dimension(), 
				setting.Offset(), setting.WorkerRange(), setting.SplitSize(),
				0, NULL, &event);

			TestNDRange(result);
			return *this;
		}

		CLExecute& Wait()
		{
			clWaitForEvents(1, &event);
			return *this;
		}
	};
}

#endif