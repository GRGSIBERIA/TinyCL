#ifndef CL_EXECUTE_HPP
#define CL_EXECUTE_HPP

#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "CLExecuteProperty.hpp"

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
		* \param[in] ���p�������f�o�C�XID
		*/
		CLExecute(CLSource& source, cl_device_id useDeviceId)
			: executeProperty(source, useDeviceId)
		{

		}

		/**
		* �J�[�l���̎��s��o�b�t�@�̓]�����Ǘ�����
		* \param[in] info OpenCL�̏��N���X
		* \param[in] sourceArray �\�[�X�R�[�h�̔z��N���X
		* \param[in] ���p�������f�o�C�XID
		*/
		CLExecute(CLSourceArray& sourceArray, cl_device_id useDeviceId)
			: executeProperty(sourceArray, useDeviceId)
		{

		}
	};
}

#endif