#ifndef CL_WORK_GROUP_3D_SETTINGS_HPP
#define CL_WORK_GROUP_3D_SETTINGS_HPP

#include "WorkGroupSettings.hpp"

namespace tcl
{
	class WorkGroup3DSettings : public WorkGroupSettings<3>
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
		WorkGroup3DSettings()
			: WorkGroupSettings() { }


	};
}

#endif