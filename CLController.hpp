#pragma once

#include <string>
#include "CLException.hpp"
#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "CLWorkGroupSettings.hpp"

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

	private:
		void InitSetting(const cl_uint dimension, const std::vector<size_t>& offset, const std::vector<size_t>& workerRange, const std::vector<size_t>& splitSize)
		{

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

		CLController& Setting(const cl_uint dimension = 0, const std::vector<size_t>& offset = {}, const std::vector<size_t>& workerRange = {}, const std::vector<size_t>& splitSize = {})
		{
			if (setting != nullptr)
				delete setting;	// ���x���Ăяo�����΍�

			setting = new CLWorkGroupSettings(dimension, offset, workerRange, splitSize);
			setting->Optimize(*device);
		}

		CLController& Setting(const cl_uint dimension = 0, const std::vector<size_t>& workerRange = {})
		{

		}

		virtual ~CLController()
		{
			delete setting;
			delete source;
		}
	};
}