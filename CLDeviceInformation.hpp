#ifndef TCL_DEVICE_HPP
#define TCL_DEVICE_HPP

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#include "CLException.hpp"

namespace tcl
{
	class CLDeviceInformation
	{
	private:
		cl_device_id deviceId;
		cl_device_type deviceType;

		cl_uint maxComputeUnits;
		cl_uint maxWorkItemDimensions;
		size_t maxWorkGroupSize;

		cl_uint maxClockFrequency;
		cl_ulong maxMemoryAllocSize;

		cl_bool imageSupport;
		cl_uint maxReadImageArgs;
		cl_uint maxWriteImageArgs;
		
		size_t image2dMaxWidth;
		size_t image2dMaxHeight;
		size_t image3dMaxWidth;
		size_t image3dMaxHeight;
		size_t image3dMaxDepth;
		cl_uint maxSamplers;
		size_t maxParameterSize;

		cl_uint globalMemoryCachelineSize;
		cl_ulong globalMemoryCacheSize;
		cl_ulong globalMemorySize;
		cl_ulong maxConstantBufferSize;
		cl_ulong maxConstantArgs;

	private:
		template <typename T>
		void GetDeviceInfo(T& var, const cl_device_info& type)
		{
			auto result = clGetDeviceInfo(deviceId, type, sizeof(T), &var, NULL);
			if (result != CL_SUCCESS)
				throw CLException("デバイスの情報が取得できない");
		}

	public:
		/**
		* デバイスIDを返す
		*/
		inline const cl_device_id& DeviceId() const {
			return deviceId;
		}

		/**
		* デバイスの種類を返す
		*/
		inline const cl_device_type& DeviceType() const {
			return deviceType;
		}

		/**
		* 使用可能なコア数を返す
		*/
		inline const cl_uint& MaxComputeUnits() const {
			return maxComputeUnits;
		}

		/**
		* 使用可能なワークアイテムの次元数を返す
		*/
		inline const cl_uint& MaxWorkItemDimensions() const {
			return maxWorkItemDimensions;
		}

		/**
		* ワークグループの最大数を返す
		*/
		inline const size_t& MaxWorkGroupSize() const {
			return maxWorkGroupSize;
		}

		/**
		* コアの最大周波数を返す
		*/
		inline const cl_uint& MaxClockFrequency() const {
			return maxClockFrequency;
		}

		/**
		* メモリの確保可能な領域をバイト数で返す
		*/
		inline const cl_ulong& MaxMemoryAllocSize() const {
			return maxMemoryAllocSize;
		}

		inline const cl_bool& ImageSupport() const {
			return imageSupport;
		}

		inline const cl_uint& MaxReadImageArgs() const {
			return maxReadImageArgs;
		}

		inline const cl_uint& MaxWriteImageArgs() const {
			return maxWriteImageArgs;
		}

		inline const size_t& Image2dMaxWidth() const {
			return image2dMaxWidth;
		}

		inline const size_t& Image2dMaxHeight() const {
			return image2dMaxHeight;
		}

		inline const size_t& Image3dMaxWidth() const {
			return image3dMaxWidth;
		}

		inline const size_t& Image3dMaxHeight() const {
			return image3dMaxHeight;
		}

		inline const size_t& Image3dMaxDepth() const {
			return image3dMaxDepth;
		}

		inline const cl_uint& MaxSamplers() const {
			return maxSamplers;
		}

		inline const size_t& MaxParameterSize() const {
			return maxParameterSize;
		}

		inline const cl_uint& GlobalMemoryCachelineSize() const {
			return globalMemoryCachelineSize;
		}

		inline const cl_ulong& GlobalMemoryCacheSize() const {
			return globalMemoryCacheSize;
		}

		inline const cl_ulong& GlobalMemorySize() const {
			return globalMemorySize;
		}

		inline const cl_ulong& MaxConstantBufferSize() const {
			return maxConstantBufferSize;
		}

		inline const cl_ulong& MaxConstantArgs() const {
			return maxConstantArgs;
		}

	public:
		CLDeviceInformation(const cl_device_id& deviceId)
			: deviceId(deviceId)
		{
			GetDeviceInfo<cl_device_type>(deviceType, CL_DEVICE_TYPE);
			GetDeviceInfo<cl_uint>(maxComputeUnits, CL_DEVICE_MAX_COMPUTE_UNITS);
			GetDeviceInfo<cl_uint>(maxWorkItemDimensions, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
			GetDeviceInfo<size_t>(maxWorkGroupSize, CL_DEVICE_MAX_WORK_GROUP_SIZE);
			GetDeviceInfo<cl_uint>(maxClockFrequency, CL_DEVICE_MAX_CLOCK_FREQUENCY);
			GetDeviceInfo<cl_ulong>(maxMemoryAllocSize, CL_DEVICE_MAX_MEM_ALLOC_SIZE);
			GetDeviceInfo<cl_uint>(maxReadImageArgs, CL_DEVICE_MAX_READ_IMAGE_ARGS);
			GetDeviceInfo<cl_uint>(maxWriteImageArgs, CL_DEVICE_MAX_WRITE_IMAGE_ARGS);
			GetDeviceInfo<cl_bool>(imageSupport, CL_DEVICE_IMAGE_SUPPORT);
			
			GetDeviceInfo<size_t>(image2dMaxWidth, CL_DEVICE_IMAGE2D_MAX_WIDTH);
			GetDeviceInfo<size_t>(image2dMaxHeight, CL_DEVICE_IMAGE2D_MAX_HEIGHT);
			GetDeviceInfo<size_t>(image3dMaxWidth, CL_DEVICE_IMAGE3D_MAX_WIDTH);
			GetDeviceInfo<size_t>(image3dMaxHeight, CL_DEVICE_IMAGE3D_MAX_HEIGHT);
			GetDeviceInfo<size_t>(image3dMaxDepth, CL_DEVICE_IMAGE3D_MAX_DEPTH);

			GetDeviceInfo<cl_uint>(maxSamplers, CL_DEVICE_MAX_SAMPLERS);
			GetDeviceInfo<size_t>(maxParameterSize, CL_DEVICE_MAX_PARAMETER_SIZE);
			GetDeviceInfo<cl_uint>(globalMemoryCachelineSize, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE);
			GetDeviceInfo<cl_ulong>(globalMemoryCacheSize, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE);
			GetDeviceInfo<cl_ulong>(globalMemorySize, CL_DEVICE_GLOBAL_MEM_SIZE);
			GetDeviceInfo<cl_ulong>(maxConstantBufferSize, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE);
			GetDeviceInfo<cl_ulong>(maxConstantArgs, CL_DEVICE_MAX_CONSTANT_ARGS);
		}
	};
}

#endif