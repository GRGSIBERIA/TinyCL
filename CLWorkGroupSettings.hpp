#ifndef TCL_WORK_GROUP_SETTINGS_HPP
#define TCL_WORK_GROUP_SETTINGS_HPP

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#include <vector>

namespace tcl
{
	/**
	* CLExecute::Runでワークグループの数を設定するためのクラス
	* \tparam WORK_DIM ワークグループの次元数
	*/
	class CLWorkGroupSettings
	{
	protected:
		cl_uint workDimension;
		std::vector<size_t> globalWorker;
		std::vector<size_t> globalOffset;
		std::vector<size_t> localWorker;

	public:
		inline cl_uint Dimension() const
		{
			return workDimension;
		}

		inline size_t* WorkerSize()
		{
			return &globalWorker[0];
		}

		inline size_t* Offset()
		{
			return &globalOffset[0];
		}

		inline size_t* SplitSize()
		{
			return &localWorker[0];
		}

	public:
		/**
		* CLExecute::Runでワークグループの数を設定するためのクラス
		* \tparam WORK_DIM ワークグループの次元数
		*/
		CLWorkGroupSettings(const cl_uint dimension, const std::vector<size_t>& workerSize, const std::vector<size_t>& splitSize, const std::vector<size_t>& offset)
			: 
			workDimension(dimension), 
			globalWorker(workerSize.begin(), workerSize.end()), 
			globalOffset(offset.begin(), offset.end()), 
			localWorker(splitSize.begin(), splitSize.end())
		{
			if (workerSize.size() > dimension || splitSize.size() > dimension || offset.size() > dimension)
				throw CLException("与えられた引数の数が多すぎます");
		}

	};
}

#endif