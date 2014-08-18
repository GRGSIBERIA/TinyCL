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
		* CLExecute::Run�Ń��[�N�O���[�v�̐���ݒ肷�邽�߂̃N���X
		* \param[in] dimension ���[�N�A�C�e���̎���
		* \param[in] workerSize �����ɉ��������[�N�O���[�v�̑S�̂̑傫��
		* \param[in] offset �����ɉ��������s�ʒu
		* \param[in] splitSize ���[�N�O���[�v1������̑傫��
		*/
		CLWorkGroupSettings(const cl_uint dimension, const std::vector<size_t>& workerSize, const std::vector<size_t>& offset, const std::vector<size_t>& splitSize)
			: 
			workDimension(dimension), 
			globalWorker(workerSize), 
			globalOffset(offset), 
			localWorker(splitSize)
		{
			if (workerSize.size() != dimension || splitSize.size() != dimension || offset.size() != dimension)
				throw CLException("�^����ꂽ�����ɑ΂��āC�^����ꂽ�����̒������傫�����邩���������܂�");
		}
	};
}

#endif