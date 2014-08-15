#ifndef CL_INFORMATION_HPP
#define CL_INFORMATION_HPP

#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#include "CLException.hpp"

#define CL_MAX_NUM_DEVICES 32

namespace cl
{
	class CLInformation
	{
	public:
		cl_platform_id platformId;						//!< �R���s���[�^��ID
		cl_device_id deviceIds[CL_MAX_NUM_DEVICES];	//!< PC�ɐڑ����ꂽ���Z���u�C4�܂�
		cl_context context;		//!< ���񉉎Z�Ǘ��N���X
		cl_uint numDevices;		//!< ���Z���u�̐�
		cl_uint numPlatforms;	//!< �R���s���[�^�̐�
		cl_int result;			//!< ����

	private:
		void GetPlatformIds()
		{
			// �R���s���[�^��ID���擾
			result = clGetPlatformIDs(1, &platformId, &numPlatforms);

			switch (result)
			{
			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("clGetPlatformIDs", "�Ȃ�����������������");

			case CL_OUT_OF_HOST_MEMORY:
				throw CLFailedAllacException("�z�X�g��Ń������̊m�ۂɎ��s");
			}
		}

		void GetDeviceIds()
		{
			// ���Z���u��ID���擾
			result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, CL_MAX_NUM_DEVICES, deviceIds, &numDevices);

			switch (result)
			{
			case CL_INVALID_DEVICE_TYPE:
			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("clGetDeviceIDs", "num_entries��platforms���Ȃ񂩕�");

			case CL_INVALID_PLATFORM:
				throw CLException("�v���b�g�t�H�[����OpenCL�ɑΉ����ĂȂ�");

			case CL_OUT_OF_HOST_MEMORY:
				throw CLFailedAllacException("�z�X�g���Ń��\�[�X�̊m�ۂɎ��s");

			case CL_OUT_OF_RESOURCES:
				throw CLFailedAllacException("�f�o�C�X���Ń��\�[�X�̊m�ۂɎ��s");
			}
		}

		void CreateContext()
		{
			// ���񉉎Z�Ǘ��N���X�̐����C�f�o�C�X�̐�������������
			context = clCreateContext(NULL, numDevices, deviceIds, NULL, NULL, &result);

			switch (result)
			{
			case CL_INVALID_PLATFORM:
				throw CLInvalidArgumentException("clCreateContext", "�L���ȃv���b�g�t�H�[������Ȃ�");

			case CL_INVALID_DEVICE:
				throw CLInvalidArgumentException("clCreateContext", "�L���ȃf�o�C�X����Ȃ�");

			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("clCreateContext", "�����Ȓl���n���ꂽ");

			case CL_DEVICE_NOT_AVAILABLE:
				throw CLException("�n���ꂽdevices�Ɍ��ݗ��p�s�\�Ȃ��̂����݂���");

			case CL_OUT_OF_HOST_MEMORY:
				throw CLFailedAllacException("�z�X�g���Ń��\�[�X�̊m�ۂɎ��s");

			case CL_OUT_OF_RESOURCES:
				throw CLFailedAllacException("�f�o�C�X���Ń��\�[�X�̊m�ۂɎ��s");
			}
		}

	public:
		CLInformation()
			: 
			platformId(NULL), context(NULL)
		{
			GetPlatformIds();	// �R���s���[�^��ID���擾

			GetDeviceIds();		// ���Z���u��ID���擾
			
			CreateContext();	// �����Ǘ��I�Ȃ���
		}

		virtual ~CLInformation()
		{
			clReleaseContext(context);
		}
	};

	CLInformation information;
}

#endif