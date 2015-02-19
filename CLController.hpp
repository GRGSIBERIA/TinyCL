#pragma once

#include <string>
#include "CLException.hpp"
#include "CLInformation.hpp"
#include "CLSource.hpp"
#include "CLWorkGroupSettings.hpp"

namespace tcl
{
	/**
	* @brief デバイスの種類
	*/
	enum class DeviceType
	{
		GPU,
		CPU
	};

	/**
	* @brief TinyCLを制御するためのコントローラ
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
		* @brief TinyCLを制御するためのコントローラ
		* \param [in] sourcePath ソースコードのパス
		* \param [in] kernelFunction エントリーポイントになる関数名
		* \param [in] deviceType デバイスの種類
		* \param [in] sourceType ソースコードの種類
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
				throw CLDeviceNotFoundException("対象のデバイスが見つかりませんでした");

			source = new CLSource(sourcePath, kernelFunction, sourceType);
		}

		CLController& Setting(const cl_uint dimension = 0, const std::vector<size_t>& offset = {}, const std::vector<size_t>& workerRange = {}, const std::vector<size_t>& splitSize = {})
		{
			if (setting != nullptr)
				delete setting;	// 何度も呼び出される対策

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