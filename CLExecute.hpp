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
		* カーネルの実行やバッファの転送を管理する
		* \param[in] info OpenCLの情報クラス
		* \param[in] source ソースコード
		* \param[in] 利用したいデバイスID
		*/
		CLExecute(CLSource& source, cl_device_id useDeviceId)
			: executeProperty(source, useDeviceId)
		{

		}

		/**
		* カーネルの実行やバッファの転送を管理する
		* \param[in] info OpenCLの情報クラス
		* \param[in] sourceArray ソースコードの配列クラス
		* \param[in] 利用したいデバイスID
		*/
		CLExecute(CLSourceArray& sourceArray, cl_device_id useDeviceId)
			: executeProperty(sourceArray, useDeviceId)
		{

		}
	};
}

#endif