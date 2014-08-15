#ifndef CL_DEVICE_HPP
#define CL_DEVICE_HPP

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif


namespace cl
{
	class CLDeviceInformation
	{
	private:
		cl_device_id deviceId;
		cl_device_info deviceInfo;
		cl_device_type deviceType;

	public:
		CLDeviceInformation(const cl_device_id& deviceId)
			: deviceId(deviceId)
		{
			clGetDeviceInfo(deviceId, CL_DEVICE_TYPE, sizeof(cl_device_type), &deviceType, NULL);
		}
	};
}

#endif