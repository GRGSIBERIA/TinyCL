#ifndef CL_WORK_GROUP_2D_SETTINGS_HPP
#define CL_WORK_GROUP_2D_SETTINGS_HPP

#include "WorkGroupSettings.hpp"

namespace tcl
{
	class WorkGroup2DSettings : public WorkGroupSettings<2>
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
		WorkGroup2DSettings()
			: WorkGroupSettings() { }


	};
}

#endif