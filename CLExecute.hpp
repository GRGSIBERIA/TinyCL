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

	public:
		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] source �\�[�X�R�[�h
		* \param[in] useDeviceId���p�������f�o�C�XID
		*/
		CLExecute(CLSource& source, const cl_device_id& useDeviceId)
			: executeProperty(source, useDeviceId)
		{

		}

		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] sourceArray �\�[�X�R�[�h�̔z��N���X
		* \param[in] useDeviceId ���p�������f�o�C�XID
		*/
		CLExecute(CLSourceArray& sourceArray, const cl_device_id& useDeviceId)
			: executeProperty(sourceArray, useDeviceId)
		{

		}

		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] source �\�[�X�R�[�h
		* \param[in] useDeviceId���p�������f�o�C�XID
		*/
		CLExecute(CLSource& source, const CLDeviceInformation& device)
			: executeProperty(source, device.DeviceId())
		{

		}

		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] sourceArray �\�[�X�R�[�h�̔z��N���X
		* \param[in] useDeviceId ���p�������f�o�C�XID
		*/
		CLExecute(CLSourceArray& sourceArray, const CLDeviceInformation& device)
			: executeProperty(sourceArray, device.DeviceId())
		{

		}
	};
}

#endif