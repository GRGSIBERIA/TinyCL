#ifndef TCL_WORK_GROUP_SETTINGS_HPP
#define TCL_WORK_GROUP_SETTINGS_HPP

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#include <array>

namespace tcl
{
	/**
	* CLExecute::Runでワークグループの数を設定するためのクラス
	* \tparam WORK_DIM ワークグループの次元数
	*/
	template <cl_uint WORK_DIM>
	class WorkGroupSettings
	{
	private:
		cl_uint workDimension;
		std::array<size_t, WORK_DIM> globalWorker;
		std::array<size_t, WORK_DIM> globalOffset;
		std::array<size_t, WORK_DIM> localWorker;

	protected:
		virtual void SetGlobalWorker() = 0;
		virtual void SetGlobalOffset() = 0;
		virtual void SetLocalWorker() = 0;

	public:
		/**
		* CLExecute::Runでワークグループの数を設定するためのクラス
		* \tparam WORK_DIM ワークグループの次元数
		*/
		WorkGroupSettings()
			: workDimension(WORK_DIM)
		{

		}
	};
}

#endif