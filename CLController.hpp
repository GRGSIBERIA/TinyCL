#pragma once

#include <string>
#include "CLInformation.hpp"
#include "CLSource.hpp"

namespace tcl
{
	enum class DeviceType
	{
		GPU,
		CPU
	};

	class CLController
	{
		CLSource* source;
		const CLDeviceInformation* device;

	public:
		CLController(const std::string& sourcePath, const std::string& kernelFunction, const DeviceType& deviceType = DeviceType::GPU, const SourceType& sourceType = SourceType::Text)
		{
			switch (deviceType)
			{
			case DeviceType::GPU:
				device = &information.GetGPU();
				break;

			case DeviceType::CPU:
				device = &information.GetCPU();
				break;
			}
			source = new CLSource(sourcePath, kernelFunction, sourceType);
			
		}

		virtual ~CLController()
		{
			delete source;
		}
	};
}