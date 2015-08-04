#ifndef TCL_CONTROLLER_HPP
#define TCL_CONTROLLER_HPP

#include <string>
#include <memory>

#include "CLDeviceInformation.hpp"
#include "CLInformation.hpp"

#include "CLSource.hpp"
#include "CLSourceArray.hpp"

#include "CLWorkGroupSettings.hpp"
#include "CLExecuteProperty.hpp"
#include "CLExecute.hpp"

#include "CLBuffer.hpp"
#include "CLReadBuffer.hpp"
#include "CLReadWriteBuffer.hpp"
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
		std::shared_ptr<CLSource> source;
		const CLDeviceInformation* device;
		
		std::shared_ptr<CLWorkGroupSettings> setting;
		std::shared_ptr<CLExecute> exec;

		std::vector<CLBuffer*> argWrite;	//!< デバイスが書き込む
		std::vector<CLBuffer*> argRead;		//!< デバイスが読み込む

	private:
		void InitSetting(const cl_uint dimension, const std::vector<size_t>& offset, const std::vector<size_t>& workerRange, const std::vector<size_t>& splitSize)
		{
			setting = std::shared_ptr<CLWorkGroupSettings>(new CLWorkGroupSettings(dimension, offset, workerRange, splitSize));
			setting->Optimize(*device);
		}

		template <typename T>
		CLController& SetArgument(std::vector<CLBuffer*>& push, T& buffer)
		{
			argRead.push_back(&buffer);
			exec->SetArg(buffer);
			return *this;
		}

		CLController& SetArg(CLReadBuffer& buffer)
		{
			return SetArgument(argRead, buffer);
		}

		CLController& SetArg(CLWriteBuffer& buffer)
		{
			buffer.Write(exec->CommandQueue());
			return SetArgument(argWrite, buffer);
		}

		CLController& SetArg(CLReadWriteBuffer& buffer)
		{
			argWrite.push_back(&buffer);
			buffer.Write(exec->CommandQueue());
			return SetArgument(argRead, buffer);
		}

	public:
		/**
		* @brief 明示的にデバイスの種類を設定する
		* \param [in] deviceType デバイスの種類
		*/
		CLController& SetupDeviceType(const DeviceType& deviceType)
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

			return *this;
		}

		/**
		* @brief 明示的に実行するソースコードを指定する
		* \param [in] sourcePath ソースコードのパス
		* \param [in] kernelFunction エントリーポイントになる関数名
		* \param [in] sourceType ソースコードの種類
		*/
		CLController& SetupSource(const std::string& sourcePath, const std::string& kernelFunction, const SourceType& sourceType = SourceType::Text)
		{
			source = std::shared_ptr<CLSource>(new CLSource(sourcePath, kernelFunction, sourceType));
			exec = std::shared_ptr<CLExecute>(new CLExecute(*source, *device));
			return *this;
		}

		/**
		* @brief TinyCLを制御するためのコントローラ
		* \param [in] sourcePath ソースコードのパス
		* \param [in] kernelFunction エントリーポイントになる関数名
		* \param [in] deviceType デバイスの種類
		* \param [in] sourceType ソースコードの種類
		*/
		CLController(const std::string& sourcePath, const std::string& kernelFunction, const DeviceType& deviceType = DeviceType::GPU, const SourceType& sourceType = SourceType::Text)
		{
			SetupDeviceType(deviceType);

			SetupSource(sourcePath, kernelFunction, sourceType);
		}

		/**
		* @brief TinyCLを制御するためのコントローラ
		* @warning 明示的にSetupDeviceType，SetupSourceの順で呼び出すこと
		*/
		CLController()
		{

		}

		/**
		* @brief TinyCLを制御するためのコントローラ
		* \param [in] deviceType デバイスの種類
		* @warning 明示的にSetupSourceを呼び出すこと
		*/
		CLController(const DeviceType& deviceType)
		{
			SetupDeviceType(deviceType);
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
		* 引数にバッファを指定してカーネルを実行する
		*/
		template <typename T>
		CLController& Run(T& buffer)
		{
			SetArg(buffer);
			exec->Run(*setting);
			return *this;
		}

		/**
		* 引数にバッファを指定してカーネルを実行する
		*/
		template <typename BUFFER, typename... Args>
		CLController& Run(BUFFER& buffer, Args&... args)
		{
			SetArg(buffer);
			Run(args...);
			return *this;
		}

		/**
		* カーネルコードの終了を待ち，結果を書き込む
		* \attention この関数を呼び出さないと，結果が書き込まれません
		*/
		CLController& Result()
		{
			exec->Wait();
			
			for (auto& buf : argWrite)
				buf->Read(exec->CommandQueue());	// waitが呼び出された時点で結果を書き込む
			argRead.clear();
			argWrite.clear();
			return *this;
		}

		virtual ~CLController()
		{
			
		}

		cl_context& GetContext() { return information.context; }
		__declspec(property(get = GetContext)) cl_context& Context;
	};
}

#endif