#ifndef TCL_WORK_GROUP_SETTINGS_HPP
#define TCL_WORK_GROUP_SETTINGS_HPP

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL\cl.h>
#endif

#include <vector>

#include "CLDeviceInformation.hpp"

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

	private:
		inline void OptimizeDimension(const CLDeviceInformation& device)
		{
			// 次元数を丸める
			if (device.MaxWorkItemDimensions() < workDimension)
				workDimension = device.MaxWorkItemDimensions();
		}

		inline void OptimizeVectors(const CLDeviceInformation& device, std::vector<size_t>& target)
		{
			const auto& itemSizes = device.MaxWorkItemSizes();
			size_t dimension = device.MaxWorkItemDimensions();
			if (target.size() < dimension)
				dimension = target.size();
			for (size_t i = 0; i < dimension; ++i)
			{
				if (itemSizes[i] < target[i])
					target[i] = itemSizes[i];
			}
		}


	public:
		inline cl_uint Dimension() const
		{
			return workDimension;
		}

		/**
		* ワーカーの次元数を設定する
		*/
		inline CLWorkGroupSettings& Dimension(const cl_uint dimention)
		{
			workDimension = dimention;
			return *this;
		}

		inline size_t* WorkerSize()
		{
			return &globalWorker[0];
		}

		/**
		* ワーカーの数を設定する
		*/
		inline CLWorkGroupSettings& WorkerSize(const std::vector<size_t>& workerSize)
		{
			globalWorker = std::vector<size_t>(workerSize);
			return *this;
		}

		inline size_t* Offset()
		{
			return &globalOffset[0];
		}

		/**
		* ワーカーの初期実行位置を設定する
		*/
		inline CLWorkGroupSettings& Offset(const std::vector<size_t>& offset)
		{
			globalOffset = std::vector<size_t>(offset);
			return *this;
		}

		inline size_t* SplitSize()
		{
			return &localWorker[0];
		}

		/**
		* ワーカーを小分けする数を設定する
		*/
		inline CLWorkGroupSettings& SplitSize(const std::vector<size_t>& splitSize)
		{
			localWorker = std::vector<size_t>(splitSize);
			return *this;
		}

		CLWorkGroupSettings& Optimize(const CLDeviceInformation& device)
		{
			OptimizeDimension(device);
			OptimizeVectors(device, globalWorker);
			OptimizeVectors(device, globalOffset);
			OptimizeVectors(device, localWorker);
			return *this;
		}

	public:
		/**
		* CLExecute::Runでワークグループの数を設定するためのクラス
		* \param[in] dimension ワークアイテムの次元数
		* \param[in] workerSize ワークアイテムの大きさ，次元数ごとのアイテムの数
		* \param[in] offset 次元数に応じた実行位置
		* \param[in] splitSize ワークアイテム全体の区切り方
		*/
		CLWorkGroupSettings(const cl_uint dimension = 0, const std::vector<size_t>& offset = {}, const std::vector<size_t>& workerSize = {}, const std::vector<size_t>& splitSize = {})
			:
			workDimension(dimension),
			globalWorker(workerSize),
			globalOffset(offset),
			localWorker(splitSize)
		{
			if (workerSize.size() > dimension || splitSize.size() > dimension || offset.size() > dimension)
				throw CLException("与えられた次数に対して，与えられた引数の長さが小さすぎます");
		}
	};
}

#endif