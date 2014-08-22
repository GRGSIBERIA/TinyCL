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
	* CLExecute::Run�Ń��[�N�O���[�v�̐���ݒ肷�邽�߂̃N���X
	* \tparam WORK_DIM ���[�N�O���[�v�̎�����
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
			// ���������ۂ߂�
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
		* ���[�J�[�̎�������ݒ肷��
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
		* ���[�J�[�̐���ݒ肷��
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
		* ���[�J�[�̏������s�ʒu��ݒ肷��
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
		* ���[�J�[�����������鐔��ݒ肷��
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
		* CLExecute::Run�Ń��[�N�O���[�v�̐���ݒ肷�邽�߂̃N���X
		* \param[in] dimension ���[�N�A�C�e���̎�����
		* \param[in] workerSize ���[�N�A�C�e���̑傫���C���������Ƃ̃A�C�e���̐�
		* \param[in] offset �������ɉ��������s�ʒu
		* \param[in] splitSize ���[�N�A�C�e���S�̂̋�؂��
		*/
		CLWorkGroupSettings(const cl_uint dimension = 0, const std::vector<size_t>& offset = {}, const std::vector<size_t>& workerSize = {}, const std::vector<size_t>& splitSize = {})
			:
			workDimension(dimension),
			globalWorker(workerSize),
			globalOffset(offset),
			localWorker(splitSize)
		{
			if (workerSize.size() > dimension || splitSize.size() > dimension || offset.size() > dimension)
				throw CLException("�^����ꂽ�����ɑ΂��āC�^����ꂽ�����̒��������������܂�");
		}
	};
}

#endif