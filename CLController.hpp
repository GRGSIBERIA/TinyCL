#ifndef TCL_CONTROLLER_HPP
#define TCL_CONTROLLER_HPP

#include <string>
#include "CLException.hpp"
#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "CLWorkGroupSettings.hpp"
#include "CLExecute.hpp"

namespace tcl
{
	/**
	* @brief �f�o�C�X�̎��
	*/
	enum class DeviceType
	{
		GPU,
		CPU
	};

	/**
	* @brief TinyCL�𐧌䂷�邽�߂̃R���g���[��
	*/
	class CLController
	{
		CLSource* source;
		const CLDeviceInformation* device;
		CLWorkGroupSettings* setting;
		CLExecute* exec;

	private:
		void InitSetting(const cl_uint dimension, const std::vector<size_t>& offset, const std::vector<size_t>& workerRange, const std::vector<size_t>& splitSize)
		{
			if (setting != nullptr)
				delete setting;	// ���x���Ăяo�����΍�

			if (exec != nullptr)
				delete exec;

			exec = new CLExecute(*source, *device);
			setting = new CLWorkGroupSettings(dimension, offset, workerRange, splitSize);
			setting->Optimize(*device);
		}

	public:
		/**
		* @brief TinyCL�𐧌䂷�邽�߂̃R���g���[��
		* \param [in] sourcePath �\�[�X�R�[�h�̃p�X
		* \param [in] kernelFunction �G���g���[�|�C���g�ɂȂ�֐���
		* \param [in] deviceType �f�o�C�X�̎��
		* \param [in] sourceType �\�[�X�R�[�h�̎��
		*/
		CLController(const std::string& sourcePath, const std::string& kernelFunction, const DeviceType& deviceType = DeviceType::GPU, const SourceType& sourceType = SourceType::Text)
		{
			switch (deviceType)
			{
			case DeviceType::GPU:
				device = &information.GetGPU();
				break;

			case DeviceType::CPU:
				device = &information.GetCPU();
				break;
			}

			if (device == nullptr)
				throw CLDeviceNotFoundException("�Ώۂ̃f�o�C�X��������܂���ł���");

			source = new CLSource(sourcePath, kernelFunction, sourceType);
		}

		/**
		* @brief ���[�J�[�̐ݒ���ׂ����w�肷��
		* @param [in] dimension ���[�J�[�̎�����
		* @param [in] offset ���[�J�[�̏����ʒu
		* @param [in] workerRange ���������[�J�[�̐�
		* @param [in] splitSize ���[�J�[�̋�؂��
		*/
		CLController& Setting(const cl_uint dimension, const std::vector<size_t>& offset, const std::vector<size_t>& workerRange, const std::vector<size_t>& splitSize)
		{
			InitSetting(dimension, offset, workerRange, splitSize);
		}

		/**
		* @brief 1�����Ƃ��āCoffset����X�^�[�g���CworkerRange�̐��������[�J�[�𓮂���
		* @param [in] offset ���[�J�[�̏����ʒu
		* @param [in] workerRange ���������[�J�[�̐�
		*/
		CLController& Setting(const size_t& offset, const size_t& workerRange)
		{
			InitSetting(1, { offset }, { workerRange } , { workerRange });
		}

		/**
		* @brief 1�����Ƃ��āCwokerRange�̐��������[�J�[�𓮂���
		* @param [in] workerRange ���������[�J�[�̐�
		*/
		CLController& Setting(const size_t& workerRange)
		{
			InitSetting(1, { 0 }, { workerRange }, { workerRange });
		}

		/**
		* @brief �����ׂ����ݒ�����Ȃ�
		* @warning �G���[���o�邩������Ȃ��̂Ŕ񐄏�
		*/
		CLController& Setting()
		{
			InitSetting(0, {}, {}, {});
		}

		

		virtual ~CLController()
		{
			delete exec;
			delete setting;
			delete source;
		}
	};
}

#endif