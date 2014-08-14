#ifndef CLINFORMATION_HPP
#define CLINFORMATION_HPP

#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#define WCL_MAX_NUM_DEVICES 16

namespace cl
{
	class CLInformation
	{
	public:
		cl_platform_id platformId;						//!< コンピュータのID
		cl_device_id deviceIds[WCL_MAX_NUM_DEVICES];	//!< PCに接続された演算装置，4つまで
		cl_context context;		//!< 並列演算管理クラス
		cl_uint numDevices;		//!< 演算装置の数
		cl_uint numPlatforms;	//!< コンピュータの数
		cl_int result;			//!< 結果

	public:
		CLInformation()
			: 
			platformId(NULL), context(NULL)
		{
			// コンピュータのIDを取得
			result = clGetPlatformIDs(1, &platformId, &numPlatforms);

			// 演算装置のIDを取得
			result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, WCL_MAX_NUM_DEVICES, deviceIds, &numDevices);

			// 並列演算管理クラスの生成，デバイスの数だけ生成する
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