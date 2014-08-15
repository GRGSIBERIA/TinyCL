#ifndef CL_INFORMATION_HPP
#define CL_INFORMATION_HPP

#include <array>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#include "CLException.hpp"
#include "CLDeviceInformation.hpp"

#define CL_MAX_NUM_DEVICES 32

namespace cl
{
	class CLInformation
	{
	public:
		cl_platform_id platformId;						//!< コンピュータのID
		cl_device_id deviceIds[CL_MAX_NUM_DEVICES];	//!< PCに接続された演算装置，4つまで
		cl_context context;		//!< 並列演算管理クラス
		cl_uint numDevices;		//!< 演算装置の数
		cl_uint numPlatforms;	//!< コンピュータの数
		cl_int result;			//!< 結果

		std::array<CLDeviceInformation, CL_MAX_NUM_DEVICES> deviceInfos;	//!< デバイスの情報

	private:
		void GetPlatformIds()
		{
			// コンピュータのIDを取得
			result = clGetPlatformIDs(1, &platformId, &numPlatforms);

			switch (result)
			{
			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("なぜか引数がおかしい");

			case CL_OUT_OF_HOST_MEMORY:
				throw CLFailedAllacException("ホスト上でメモリの確保に失敗");
			}
		}

		void GetDeviceIds()
		{
			// 演算装置のIDを取得
			result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, CL_MAX_NUM_DEVICES, deviceIds, &numDevices);

			switch (result)
			{
			case CL_INVALID_DEVICE_TYPE:
			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("num_entriesかplatformsがなんか変");

			case CL_INVALID_PLATFORM:
				throw CLException("プラットフォームがOpenCLに対応してない");

			case CL_OUT_OF_HOST_MEMORY:
				throw CLFailedAllacException("ホスト側でリソースの確保に失敗");

			case CL_OUT_OF_RESOURCES:
				throw CLFailedAllacException("デバイス側でリソースの確保に失敗");
			}
		}

		void CreateContext()
		{
			// 並列演算管理クラスの生成，デバイスの数だけ生成する
			context = clCreateContext(NULL, numDevices, deviceIds, NULL, NULL, &result);

			switch (result)
			{
			case CL_INVALID_PLATFORM:
				throw CLInvalidArgumentException("有効なプラットフォームじゃない");

			case CL_INVALID_DEVICE:
				throw CLInvalidArgumentException("有効なデバイスじゃない");

			case CL_INVALID_VALUE:
				throw CLInvalidArgumentException("無効な値が渡された");

			case CL_DEVICE_NOT_AVAILABLE:
				throw CLException("渡されたdevicesに現在利用不可能なものが存在する");

			case CL_OUT_OF_HOST_MEMORY:
				throw CLFailedAllacException("ホスト側でリソースの確保に失敗");

			case CL_OUT_OF_RESOURCES:
				throw CLFailedAllacException("デバイス側でリソースの確保に失敗");
			}
		}

		void GetDevicesInformations()
		{
			for (int i = 0; i < CL_MAX_NUM_DEVICES; ++i)
			{
				deviceInfos[i] = CLDeviceInformation(deviceIds[i]);
			}
		}

	public:
		CLInformation()
			: 
			platformId(NULL), context(NULL)
		{
			GetPlatformIds();	// コンピュータのIDを取得

			GetDeviceIds();		// 演算装置のIDを取得


			
			CreateContext();	// 中央管理的なもの
		}

		virtual ~CLInformation()
		{
			clReleaseContext(context);
		}
	};

	CLInformation information;
}

#endif