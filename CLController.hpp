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
	* @brief �f�o�C�X�̎��
	*/
	enum class DeviceType
	{
		GPU,
		CPU
	};

	/**
	* @brief �o�b�t�@�̎��
	*/
	enum class BufferType
	{
		Read,		//!< �f�o�C�X���œǂݍ��݂̂�
		Write,		//!< �f�o�C�X���ŏ������݂̂�
		ReadWrite,	//!< �f�o�C�X���œǂݏ����ł���
		HostCopy,	//!< �z�X�g���̗̈���f�o�C�X���ɃR�s�[����
		UseHost,	//!< �z�X�g���̃�������Ԃ𗘗p����
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
			exec = new CLExecute(*source, *device);
			CLBuffer::SetCurrentExecute(exec);
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
			return *this;
		}

		/**
		* @brief 1�����Ƃ��āCoffset����X�^�[�g���CworkerRange�̐��������[�J�[�𓮂���
		* @param [in] offset ���[�J�[�̏����ʒu
		* @param [in] workerRange ���������[�J�[�̐�
		*/
		CLController& Setting(const size_t& offset, const size_t& workerRange)
		{
			InitSetting(1, { offset }, { workerRange } , { workerRange });
			return *this;
		}

		/**
		* @brief 1�����Ƃ��āCwokerRange�̐��������[�J�[�𓮂���
		* @param [in] workerRange ���������[�J�[�̐�
		*/
		CLController& Setting(const size_t& workerRange)
		{
			InitSetting(1, { 0 }, { workerRange }, { workerRange });
			return *this;
		}

		/**
		* @brief �����ׂ����ݒ�����Ȃ�
		* @warning �G���[���o�邩������Ȃ��̂Ŕ񐄏�
		*/
		CLController& Setting()
		{
			InitSetting(0, {}, {}, {});
			return *this;
		}

		/**
		* �R�[�h�����s����
		* @param [in] current �J�[�l���R�[�h�̈���
		*/
		void Run(CLBuffer& current)
		{
			exec->SetArg(current);
			exec->Run(*setting);
		}

		/**
		* �R�[�h�����s����
		* @param [in] current �J�[�l���R�[�h�̈���
		* @param [in] others �ϒ�����
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