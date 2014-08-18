#ifndef CL_WORK_GROUP_1D_SETTINGS_HPP
#define CL_WORK_GROUP_1D_SETTINGS_HPP

#include "WorkGroupSettings.hpp"

namespace tcl
{
	class WorkGroup1DSettings : public WorkGroupSettings<1>
	{
	protected:
		virtual void SetGlobalWorker()
		{

		}

		virtual void SetGlobalOffset()
		{

		}

		virtual void SetLocalWorker()
		{

		}

	public:
		WorkGroup1DSettings()
			: WorkGroupSettings() { }

		
	};
}

#endif