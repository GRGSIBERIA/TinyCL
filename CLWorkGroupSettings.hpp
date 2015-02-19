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
			// ワーカーの最大値以下になるように最適化
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
		/**
		* 次元数を返す
		*/
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

		inline size_t* WorkerRange()
		{
			return &globalWorker[0];
		}

		/**
		* ワーカーの仕事をする範囲を設定する
		*/
		inline CLWorkGroupSettings& WorkerRange(const std::vector<size_t>& workerRange)
		{
			globalWorker = std::vector<size_t>(workerRange);
			return *this;
		}

		inline size_t* Offset()
		{
			return &globalOffset[0];
		}

		/**
		* ワーカーの初期実行位置を設定する
		* \param[in] offset 初期実行位置の配列，長さは次元数に依存
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

		/**
		* 設定を最適化する
		*/
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
		CLWorkGroupSettings(const cl_uint dimension = 0, const std::vector<size_t>& offset = {}, const std::vector<size_t>& workerRange = {}, const std::vector<size_t>& splitSize = {})
			:
			workDimension(dimension),
			globalWorker(workerRange),
			globalOffset(offset),
			localWorker(splitSize)
		{
			
		}

		CLWorkGroupSettings(const cl_uint dimension = 0, const std::vector<size_t>& workerRange = {})
			:
			workDimension(dimension),
			globalWorker(workerRange),
			globalOffset({ 0 }),
			localWorker({ workerRange.size() })
	};
}

#endif