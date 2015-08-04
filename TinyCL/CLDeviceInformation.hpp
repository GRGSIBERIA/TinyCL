#ifndef TCL_DEVICE_HPP
#define TCL_DEVICE_HPP

#include <vector>
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
		std::vector<size_t> maxWorkItemSizes;

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
		inline const cl_device_id& GetDeviceId() const {
			return deviceId;
		}

		/**
		* デバイスの種類を返す
		*/
		inline const cl_device_type& GetDeviceType() const {
			return deviceType;
		}

		/**
		* 使用可能なコア数を返す
		*/
		inline const cl_uint& GetMaxComputeUnits() const {
			return maxComputeUnits;
		}

		/**
		* 使用可能なワークアイテムの次元数を返す
		*/
		inline const cl_uint& GetMaxWorkItemDimensions() const {
			return maxWorkItemDimensions;
		}

		/**
		* ワークグループの最大数を返す
		*/
		inline const size_t& GetMaxWorkGroupSize() const {
			return maxWorkGroupSize;
		}

		/**
		* デバイスの最大設計クロック周波数をMHzで返す
		*/
		inline const cl_uint& GetMaxClockFrequency() const {
			return maxClockFrequency;
		}

		/**
		* 次元ごとの最大アイテム数を返す
		*/
		inline const std::vector<size_t>& GetMaxWorkItemSizes() const {
			return maxWorkItemSizes;
		}

		/**
		* メモリの確保可能な領域をバイト数で返す
		*/
		inline const cl_ulong& GetMaxMemoryAllocSize() const {
			return maxMemoryAllocSize;
		}

		inline const cl_bool& GetImageSupport() const {
			return imageSupport;
		}

		inline const cl_uint& GetMaxReadImageArgs() const {
			return maxReadImageArgs;
		}

		inline const cl_uint& GetMaxWriteImageArgs() const {
			return maxWriteImageArgs;
		}

		inline const size_t& GetImage2dMaxWidth() const {
			return image2dMaxWidth;
		}

		inline const size_t& GetImage2dMaxHeight() const {
			return image2dMaxHeight;
		}

		inline const size_t& GetImage3dMaxWidth() const {
			return image3dMaxWidth;
		}

		inline const size_t& GetImage3dMaxHeight() const {
			return image3dMaxHeight;
		}

		inline const size_t& GetImage3dMaxDepth() const {
			return image3dMaxDepth;
		}

		inline const cl_uint& GetMaxSamplers() const {
			return maxSamplers;
		}

		inline const size_t& GetMaxParameterSize() const {
			return maxParameterSize;
		}

		inline const cl_uint& GetGlobalMemoryCachelineSize() const {
			return globalMemoryCachelineSize;
		}

		/**
		* グローバルキャッシュの容量
		*/
		inline const cl_ulong& GetGlobalMemoryCacheSize() const {
			return globalMemoryCacheSize;
		}

		/**
		* グローバルメモリの容量
		*/
		inline const cl_ulong& GetGlobalMemorySize() const {
			return globalMemorySize;
		}

		inline const cl_ulong& GetMaxConstantBufferSize() const {
			return maxConstantBufferSize;
		}

		inline const cl_ulong& GetMaxConstantArgs() const {
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

			maxWorkItemSizes = std::vector<size_t>(maxWorkItemDimensions);
			auto result = clGetDeviceInfo(deviceId, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * maxWorkItemDimensions, &maxWorkItemSizes[0], NULL);
		}

		/******************************************************
		* 面倒くさいのでRubyでコード自動生成しました
		*******************************************************/
		__declspec(property(get = GetDeviceId)) const cl_device_id& DeviceId;
		__declspec(property(get = GetDeviceType)) const cl_device_type& DeviceType;
		__declspec(property(get = GetMaxComputeUnits)) const cl_uint& MaxComputeUnits;
		__declspec(property(get = GetMaxWorkItemDimensions)) const cl_uint& MaxWorkItemDimensions;
		__declspec(property(get = GetMaxWorkGroupSize)) const size_t& MaxWorkGroupSize;
		__declspec(property(get = GetMaxClockFrequency)) const cl_uint& MaxClockFrequency;
		__declspec(property(get = GetMaxMemoryAllocSize)) const cl_ulong& MaxMemoryAllocSize;
		__declspec(property(get = GetMaxWorkItemSizes)) const std::vector<size_t>& MaxWorkItemSizes;
		__declspec(property(get = GetImageSupport)) const cl_bool& ImageSupport;
		__declspec(property(get = GetMaxReadImageArgs)) const cl_uint& MaxReadImageArgs;
		__declspec(property(get = GetMaxWriteImageArgs)) const cl_uint& MaxWriteImageArgs;
		__declspec(property(get = GetImage2dMaxWidth)) const size_t& Image2dMaxWidth;
		__declspec(property(get = GetImage2dMaxHeight)) const size_t& Image2dMaxHeight;
		__declspec(property(get = GetImage3dMaxWidth)) const size_t& Image3dMaxWidth;
		__declspec(property(get = GetImage3dMaxHeight)) const size_t& Image3dMaxHeight;
		__declspec(property(get = GetImage3dMaxDepth)) const size_t& Image3dMaxDepth;
		__declspec(property(get = GetMaxSamplers)) const cl_uint& MaxSamplers;
		__declspec(property(get = GetMaxParameterSize)) const size_t& MaxParameterSize;
		__declspec(property(get = GetGlobalMemoryCachelineSize)) const cl_uint& GlobalMemoryCachelineSize;
		__declspec(property(get = GetGlobalMemoryCacheSize)) const cl_ulong& GlobalMemoryCacheSize;
		__declspec(property(get = GetGlobalMemorySize)) const cl_ulong& GlobalMemorySize;
		__declspec(property(get = GetMaxConstantBufferSize)) const cl_ulong& MaxConstantBufferSize;
		__declspec(property(get = GetMaxConstantArgs)) const cl_ulong& MaxConstantArgs;
	};
}

#endif