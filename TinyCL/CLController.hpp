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
		inline const CLMachineInformation& Machine() const { return machineInfo; }

		

		inline cl_context GetContext() { return machineInfo.context; }
		__declspec(property(get = GetContext)) cl_context Context;
	};
}

#endif