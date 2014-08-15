#ifndef CL_INFORMATION_HPP
#define CL_INFORMATION_HPP

#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

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

	public:
		CLInformation()
			: 
			platformId(NULL), context(NULL)
		{
			// �R���s���[�^��ID���擾
			result = clGetPlatformIDs(1, &platformId, &numPlatforms);

			// ���Z���u��ID���擾
			result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, CL_MAX_NUM_DEVICES, deviceIds, &numDevices);

			// ���񉉎Z�Ǘ��N���X�̐����C�f�o�C�X�̐�������������
			context = clCreateContext(NULL, numDevices, deviceIds, NULL, NULL, &result);
		}

		virtual ~CLInformation()
		{
			clReleaseContext(context);
		}
	};

	CLInformation information;
}

#endif