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
		* \param[in] dimension ワークアイテムの次数
		* \param[in] workerSize 次数に応じたワークグループの全体の大きさ
		* \param[in] offset 次数に応じた実行位置
		* \param[in] splitSize ワークグループ1つあたりの大きさ
		*/
		CLWorkGroupSettings(const cl_uint dimension, const std::vector<size_t>& workerSize, const std::vector<size_t>& offset, const std::vector<size_t>& splitSize)
			: 
			workDimension(dimension), 
			globalWorker(workerSize), 
			globalOffset(offset), 
			localWorker(splitSize)
		{
			if (workerSize.size() != dimension || splitSize.size() != dimension || offset.size() != dimension)
				throw CLException("与えられた次数に対して，与えられた引数の長さが大きすぎるか小さすぎます");
		}
	};
}

#endif