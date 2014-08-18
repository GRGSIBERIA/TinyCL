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
	* CLExecute::Run�Ń��[�N�O���[�v�̐���ݒ肷�邽�߂̃N���X
	* \tparam WORK_DIM ���[�N�O���[�v�̎�����
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
		* CLExecute::Run�Ń��[�N�O���[�v�̐���ݒ肷�邽�߂̃N���X
		* \tparam WORK_DIM ���[�N�O���[�v�̎�����
		*/
		WorkGroupSettings()
			: workDimension(WORK_DIM)
		{

		}
	};
}

#endif