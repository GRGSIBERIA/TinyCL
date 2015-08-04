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
		std::shared_ptr<CLSource> source;
		const CLDeviceInformation* device;
		
		std::shared_ptr<CLWorkGroupSettings> setting;
		std::shared_ptr<CLExecute> exec;

		std::vector<CLBuffer*> argWrite;	//!< �f�o�C�X����������
		std::vector<CLBuffer*> argRead;		//!< �f�o�C�X���ǂݍ���

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
		* @brief �����I�Ƀf�o�C�X�̎�ނ�ݒ肷��
		* \param [in] deviceType �f�o�C�X�̎��
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
				throw CLDeviceNotFoundException("�Ώۂ̃f�o�C�X��������܂���ł���");

			return *this;
		}

		/**
		* @brief �����I�Ɏ��s����\�[�X�R�[�h���w�肷��
		* \param [in] sourcePath �\�[�X�R�[�h�̃p�X
		* \param [in] kernelFunction �G���g���[�|�C���g�ɂȂ�֐���
		* \param [in] sourceType �\�[�X�R�[�h�̎��
		*/
		CLController& SetupSource(const std::string& sourcePath, const std::string& kernelFunction, const SourceType& sourceType = SourceType::Text)
		{
			source = std::shared_ptr<CLSource>(new CLSource(sourcePath, kernelFunction, sourceType));
			exec = std::shared_ptr<CLExecute>(new CLExecute(*source, *device));
			return *this;
		}

		/**
		* @brief TinyCL�𐧌䂷�邽�߂̃R���g���[��
		* \param [in] sourcePath �\�[�X�R�[�h�̃p�X
		* \param [in] kernelFunction �G���g���[�|�C���g�ɂȂ�֐���
		* \param [in] deviceType �f�o�C�X�̎��
		* \param [in] sourceType �\�[�X�R�[�h�̎��
		*/
		CLController(const std::string& sourcePath, const std::string& kernelFunction, const DeviceType& deviceType = DeviceType::GPU, const SourceType& sourceType = SourceType::Text)
		{
			SetupDeviceType(deviceType);

			SetupSource(sourcePath, kernelFunction, sourceType);
		}

		/**
		* @brief TinyCL�𐧌䂷�邽�߂̃R���g���[��
		* @warning �����I��SetupDeviceType�CSetupSource�̏��ŌĂяo������
		*/
		CLController()
		{

		}

		/**
		* @brief TinyCL�𐧌䂷�邽�߂̃R���g���[��
		* \param [in] deviceType �f�o�C�X�̎��
		* @warning �����I��SetupSource���Ăяo������
		*/
		CLController(const DeviceType& deviceType)
		{
			SetupDeviceType(deviceType);
		}

		/**
		* @brief ���[�J�[�̐ݒ���ׂ����w�肷��
		* @param [in] offset ���[�J�[�̏����ʒu
		* @param [in] workerRange ���������[�J�[�̐�
		* @param [in] splitSize ���[�J�[�̋�؂��
		*/
		CLController& Setting(const std::vector<size_t>& offset, const std::vector<size_t>& workerRange, const std::vector<size_t>& splitSize)
		{
			if (offset.size() != workerRange.size() || offset.size() != splitSize.size())
				throw CLException("���[�J�[�̎��������e�����ň�v���Ă��܂���");
			InitSetting(offset.size(), offset, workerRange, splitSize);
			return *this;
		}

		/**
		* @brief �������ɉ�����offset, workerRange�𑵂��Ă�
		* @param [in] offset ���[�J�[�̏����ʒu
		* @param [in] workerRange ���������[�J�[�̐�
		*/
		CLController& Setting(const std::vector<size_t>& offset, const std::vector<size_t>& workerRange)
		{
			if (offset.size() != workerRange.size())
				throw CLException("���[�J�[�̎��������e�����ň�v���Ă��܂���");
			InitSetting(offset.size(), offset, workerRange, workerRange);
			return *this;
		}

		/**
		* @brief �Ƃ肠�����C���[�J�[�𑽎����œ������Ă݂����Ƃ��Ɏg��
		* @param [in] workerRange ���������[�J�[�̐�
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
		* �����Ƀo�b�t�@���w�肵�ăJ�[�l�������s����
		*/
		template <typename T>
		CLController& Run(T& buffer)
		{
			SetArg(buffer);
			exec->Run(*setting);
			return *this;
		}

		/**
		* �����Ƀo�b�t�@���w�肵�ăJ�[�l�������s����
		*/
		template <typename BUFFER, typename... Args>
		CLController& Run(BUFFER& buffer, Args&... args)
		{
			SetArg(buffer);
			Run(args...);
			return *this;
		}

		/**
		* �J�[�l���R�[�h�̏I����҂��C���ʂ���������
		* \attention ���̊֐����Ăяo���Ȃ��ƁC���ʂ��������܂�܂���
		*/
		CLController& Result()
		{
			exec->Wait();
			
			for (auto& buf : argWrite)
				buf->Read(exec->CommandQueue());	// wait���Ăяo���ꂽ���_�Ō��ʂ���������
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