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
			// ���[�J�[�̍ő�l�ȉ��ɂȂ�悤�ɍœK��
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
		* ��������Ԃ�
		*/
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

		inline size_t* WorkerRange()
		{
			return &globalWorker[0];
		}

		/**
		* ���[�J�[�̎d��������͈͂�ݒ肷��
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
		* ���[�J�[�̏������s�ʒu��ݒ肷��
		* \param[in] offset �������s�ʒu�̔z��C�����͎������Ɉˑ�
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

		/**
		* �ݒ���œK������
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
		* CLExecute::Run�Ń��[�N�O���[�v�̐���ݒ肷�邽�߂̃N���X
		* \param[in] dimension ���[�N�A�C�e���̎�����
		* \param[in] workerSize ���[�N�A�C�e���̑傫���C���������Ƃ̃A�C�e���̐�
		* \param[in] offset �������ɉ��������s�ʒu
		* \param[in] splitSize ���[�N�A�C�e���S�̂̋�؂��
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