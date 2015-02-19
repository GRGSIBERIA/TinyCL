#ifndef TCL_CONTROLLER_HPP
#define TCL_CONTROLLER_HPP

#include <string>

#include "CLDeviceInformation.hpp"
#include "CLInformation.hpp"

#include "CLSource.hpp"
#include "CLSourceArray.hpp"

#include "CLWorkGroupSettings.hpp"
#include "CLExecuteProperty.hpp"
#include "CLExecute.hpp"

#include "CLAllocHostBuffer.hpp"
#include "CLHostCopyBuffer.hpp"
#include "CLReadBuffer.hpp"
#include "CLReadWriteBuffer.hpp"
#include "CLUseHostBuffer.hpp"
#include "CLWriteBuffer.hpp"

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
		CLExecute* exec;

	private:
		void InitSetting(const cl_uint dimension, const std::vector<size_t>& offset, const std::vector<size_t>& workerRange, const std::vector<size_t>& splitSize)
		{
			if (setting != nullptr)
				delete setting;	// 何度も呼び出される対策

			setting = new CLWorkGroupSettings(dimension, offset, workerRange, splitSize);
			setting->Optimize(*device);
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
			exec = new CLExecute(*source, *device);
			CLBuffer::SetCurrentExecute(exec);
		}

		/**
		* @brief ワーカーの設定を細かく指定する
		* @param [in] offset ワーカーの初期位置
		* @param [in] workerRange 動かすワーカーの数
		* @param [in] splitSize ワーカーの区切り方
		*/
		CLController& Setting(const std::vector<size_t>& offset, const std::vector<size_t>& workerRange, const std::vector<size_t>& splitSize)
		{
			if (offset.size() != workerRange.size() || offset.size() != splitSize.size())
				throw CLException("ワーカーの次元数が各引数で一致していません");
			InitSetting(offset.size(), offset, workerRange, splitSize);
			return *this;
		}

		/**
		* @brief 次元数に応じてoffset, workerRangeを揃えてね
		* @param [in] offset ワーカーの初期位置
		* @param [in] workerRange 動かすワーカーの数
		*/
		CLController& Setting(const std::vector<size_t>& offset, const std::vector<size_t>& workerRange)
		{
			if (offset.size() != workerRange.size())
				throw CLException("ワーカーの次元数が各引数で一致していません");
			InitSetting(offset.size(), offset, workerRange, workerRange);
			return *this;
		}

		/**
		* @brief とりあえず，ワーカーを多次元で動かしてみたいときに使う
		* @param [in] workerRange 動かすワーカーの数
		*/
		CLController& Setting(const std::vector<size_t>& workerRange)
		{
			std::vector<size_t> offset(workerRange.size(), 0);
			std::vector<size_t> splitSize(workerRange.size());
			for (int i = 0; i < workerRange.size(); ++i)
				splitSize[i] = workerRange[i];

			InitSetting(offset.size(), offset, workerRange, workerRange);
			return *this;
		}

		/**
		* @brief 1次元として，offsetからスタートし，workerRangeの数だけワーカーを動かす
		* @param [in] offset ワーカーの初期位置
		* @param [in] workerRange 動かすワーカーの数
		*/
		CLController& Setting(const size_t& offset, const size_t& workerRange)
		{
			InitSetting(1, { offset }, { workerRange } , { workerRange });
			return *this;
		}

		/**
		* @brief 1次元として，wokerRangeの数だけワーカーを動かす
		* @param [in] workerRange 動かすワーカーの数
		*/
		CLController& Setting(const size_t& workerRange)
		{
			InitSetting(1, { 0 }, { workerRange }, { workerRange });
			return *this;
		}

		/**
		* @brief 何も細かい設定をしない
		* @warning エラーが出るかもしれないので非推奨
		*/
		CLController& Setting()
		{
			InitSetting(0, {}, {}, {});
			return *this;
		}

		/**
		* コードを実行する
		* @param [in] current カーネルコードの引数
		*/
		void Run(CLBuffer& current)
		{
			exec->SetArg(current);
			exec->Run(*setting);
		}

		/**
		* コードを実行する
		* @param [in] current カーネルコードの引数
		* @param [in] others 可変長引数
		*/
		template <typename T, typename... Args>
		void Run(CLBuffer& current, Args& ...others)
		{
			exec->SetArg(current);
			Run(others...);
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