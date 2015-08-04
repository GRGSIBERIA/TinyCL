#ifndef CL_CONTROLLER_HPP
#define CL_CONTROLLER_HPP

#include "CLMachineInformation.hpp"

namespace tcl
{
	class CLController
	{
	private:
		CLMachineInformation machineInfo;

	public:
		inline const CLMachineInformation& Machine() { return machineInfo; }

		inline const cl_int GetNumberOfDevices() const { return machineInfo.numDevices; }
		inline const cl_int GetNumberOfPlatforms() const { return machineInfo.numPlatforms; }
		inline cl_context GetContext() { return machineInfo.context; }

		__declspec(property(get = GetNumberOfDevice)) cl_int NumberOfDevices;
		__declspec(property(get = GetNumberOfPlatforms)) cl_int NumberOfPlatforms;
		__declspec(property(get = GetContext)) cl_context Context;

	};
}

#endif