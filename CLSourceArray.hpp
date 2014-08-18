#ifndef TCL_SOURCE_ARRAY_HPP
#define TCL_SOURCE_ARRAY_HPP

#include <vector>
#include "CLSource.hpp"

namespace tcl
{
	/**
	* �����\�[�X�R�[�h���g�������ꍇ�̔z��
	*/
	class CLSourceArray
	{
	private:
		std::vector<CLSource> sources;
		std::vector<size_t> sourceSizes;
		std::vector<const char*> sourcePtrs;
		std::string kernelName;
		SourceType type;

	public:
		/**
		* �\�[�X�R�[�h�̃|�C���^�z���Ԃ�
		* \return �\�[�X�R�[�h�̃|�C���^�z��
		*/
		inline const char** Pointers() const {
			// ������S���ǂ����킩��Ȃ��Ȃ��c�c
			return sourcePtrs.begin()._Ptr;
		}

		/**
		* �\�[�X�R�[�h�̃T�C�Y�̔z���Ԃ�
		* \return �\�[�X�R�[�h�̃T�C�Y�̔z��
		*/
		inline const size_t* Sizes() const {
			return sourceSizes.begin()._Ptr;
		}

		/**
		* �\�[�X�R�[�h�̎�ނ�Ԃ�
		* \return �\�[�X�R�[�h�̎�ނ�Ԃ�
		*/
		inline const SourceType Type() const {
			return type;
		}

		/**
		* �J�[�l���̖��O��Ԃ�
		* \return �J�[�l���̖��O��Ԃ�
		*/
		inline const std::string KernelName() const {
			return kernelName;
		}

		/**
		* ���p�������\�[�X�R�[�h��ǉ����܂�
		* \param[in] filename �\�[�X�R�[�h�̃t�@�C����
		*/
		inline void Append(const std::string& filename) {
			sources.emplace_back(CLSource(filename, kernelName, type));
			auto s = sources[sources.size() - 1];
			sourcePtrs.emplace_back(s.Code().c_str());
			sourceSizes.emplace_back(s.Size());
		}

	public:
		/**
		* ��̃J�[�l���ɕ����̃\�[�X�R�[�h�����s���������Ƃ��Ɏg���܂�
		* \param[in] kernelName �G���g���[�|�C���g�ƂȂ�J�[�l����
		* \param[in] useSourceCodeType �\�[�X�R�[�h�̎��
		*/
		CLSourceArray(const std::string& kernelName, const SourceType useSourceCodeType)
			: type(useSourceCodeType), kernelName(kernelName)
		{

		}
	};
}

#endif