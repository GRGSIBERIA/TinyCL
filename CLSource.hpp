#ifndef CL_SOURCE_HPP
#define CL_SOURCE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace cl
{
	/**
	* �\�[�X�R�[�h�̎��
	*/
	enum SourceType
	{
		Text,	/// �I�����C��
		Binary	/// �I�t���C��
	};

	/**
	* �\�[�X�R�[�h���������߂̃N���X
	*/
	class CLSource
	{
	private:
		SourceType type;
		std::string fileName;
		std::string code;
		std::string kernelName;

	private:
		void OpenFile()
		{
			std::ifstream ifst(fileName, std::ifstream::binary);
			if (ifst.fail())
			{
				throw L"�t�@�C���̓ǂݍ��݂Ɏ��s���܂���";
			}
			code = std::string(
				std::istreambuf_iterator<char>(ifst),
				std::istreambuf_iterator<char>());
			ifst.close();
		}

	public:
		/**
		* �\�[�X�R�[�h�̕������Ԃ�
		* \return �\�[�X�R�[�h�̕�����
		*/
		inline const std::string& Code() const {
			return code;
		}

		/**
		* �\�[�X�R�[�h�̒�����Ԃ�
		* \return �\�[�X�R�[�h�̒���
		* \note Code().size()�Ɠ���
		*/
		inline const std::size_t Size() const {
			return code.size();
		}

		/**
		* �t�@�C������Ԃ�
		* \return �t�@�C����
		*/
		inline const std::string FileName() const {
			return fileName;
		}

		/**
		* �\�[�X�R�[�h�̎�ނ�Ԃ�
		* \return �\�[�X�R�[�h�̎��
		*/
		inline const SourceType Type() const {
			return type;
		}

		/**
		* �J�[�l���̖��O��Ԃ�
		* \return �J�[�l���̖��O
		*/
		inline const std::string& KernelName() const{
			return kernelName;
		}

	public:
		/**
		* �\�[�X�R�[�h���Ǘ����邽�߂̃N���X
		* \param[in] filename �t�@�C����
		* \param[in] kernelName �J�[�l����
		* \param[in] type �\�[�X�R�[�h�̎��
		*/
		CLSource(const std::string& filename, const std::string& kernelName, const SourceType type = SourceType::Text)
			: fileName(filename), kernelName(kernelName), type(type)
		{
			OpenFile();
		}
	};
}

#endif