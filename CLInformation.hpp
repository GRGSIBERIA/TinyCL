#ifndef TCL_INFORMATION_HPP
#define TCL_INFORMATION_HPP

#include <vector>
#include <algorithm>

#include "CLException.hpp"
#include "CLDeviceInformation.hpp"

#define TCL_MAX_NUM_DEVICES 64

namespace tcl
{
	class CLInformation
	{
	public:
		cl_platform_id platformId;					//!< �R���s���[�^��ID
		cl_device_id deviceIds[TCL_MAX_NUM_DEVICES];	//!< PC�ɐڑ����ꂽ���Z���u�C4�܂�
		cl_context context;		//!< ���񉉎Z�Ǘ��N���X
		cl_uint numDevices;		//!< ���Z���u�̐�
		cl_uint numPlatforms;	//!< �R���s���[�^�̐�
		cl_int result;			//!< ����

		std::vector<CLDeviceInformation> deviceInfos;	//!< �f�o�C�X�̏��

	private:
		void GetPlatformIds()
		{
			// �R���s���[�^��ID���擾
			result = clGetPlatformIDs(1, &platformId, &numPlatforms);

			switch (result)
			{
			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("�Ȃ�����������������");

			case CL_OUT_OF_HOST_MEMORY:
				throw CLFailedAllacException("�z�X�g��Ń������̊m�ۂɎ��s");
			}
		}

		void GetDeviceIds()
		{
			// ���Z���u��ID���擾
			result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, TCL_MAX_NUM_DEVICES, deviceIds, &numDevices);

			switch (result)
			{
			case CL_INVALID_DEVICE_TYPE:
			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("num_entries��platforms���Ȃ񂩕�");

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
				throw CLInvalidArgumentException("�L���ȃv���b�g�t�H�[������Ȃ�");

			case CL_INVALID_DEVICE:
				throw CLInvalidArgumentException("�L���ȃf�o�C�X����Ȃ�");

			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("�����Ȓl���n���ꂽ");

			case CL_DEVICE_NOT_AVAILABLE:
				throw CLException("�n���ꂽdevices�Ɍ��ݗ��p�s�\�Ȃ��̂����݂���");

			case CL_OUT_OF_HOST_MEMORY:
				throw CLFailedAllacException("�z�X�g���Ń��\�[�X�̊m�ۂɎ��s");

			case CL_OUT_OF_RESOURCES:
				throw CLFailedAllacException("�f�o�C�X���Ń��\�[�X�̊m�ۂɎ��s");
			}
		}

		void GetDevicesInformations()
		{
			for (int i = 0; i < TCL_MAX_NUM_DEVICES; ++i)
			{
				if (deviceIds[i] != NULL)	// ���݂��Ȃ��f�o�C�X��NULL
					deviceInfos.emplace_back(deviceIds[i]);
			}
		}

		const CLDeviceInformation& GetType(const cl_device_type& type) const
		{
			for (const auto& device : deviceInfos)
			{
				if (device.DeviceType() == type)
					return device;
			}
			throw CLException("�Y������f�o�C�X�����݂��Ȃ�");
		}
		
	public:
		const CLDeviceInformation& GetGPU() const
		{
			return GetType(CL_DEVICE_TYPE_GPU);
		}

		const CLDeviceInformation& GetCPU() const
		{
			return GetType(CL_DEVICE_TYPE_CPU);
		}

	public:
		CLInformation()
			: 
			platformId(NULL), context(NULL), deviceInfos()
		{
			GetPlatformIds();	// �R���s���[�^��ID���擾

			GetDeviceIds();		// ���Z���u��ID���擾

			GetDevicesInformations();	// �f�o�C�X�����擾
			
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