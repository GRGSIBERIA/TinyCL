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
	* ソースコードの種類
	*/
	enum SourceType
	{
		Text,	/// オンライン
		Binary	/// オフライン
	};

	/**
	* ソースコードを扱うためのクラス
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
				throw L"ファイルの読み込みに失敗しました";
			}
			code = std::string(
				std::istreambuf_iterator<char>(ifst),
				std::istreambuf_iterator<char>());
			ifst.close();
		}

	public:
		/**
		* ソースコードの文字列を返す
		* \return ソースコードの文字列
		*/
		inline const std::string& Code() const {
			return code;
		}

		/**
		* ソースコードの長さを返す
		* \return ソースコードの長さ
		* \note Code().size()と同じ
		*/
		inline const std::size_t Size() const {
			return code.size();
		}

		/**
		* ファイル名を返す
		* \return ファイル名
		*/
		inline const std::string FileName() const {
			return fileName;
		}

		/**
		* ソースコードの種類を返す
		* \return ソースコードの種類
		*/
		inline const SourceType Type() const {
			return type;
		}

		/**
		* カーネルの名前を返す
		* \return カーネルの名前
		*/
		inline const std::string& KernelName() const{
			return kernelName;
		}

	public:
		/**
		* ソースコードを管理するためのクラス
		* \param[in] filename ファイル名
		* \param[in] kernelName カーネル名
		* \param[in] type ソースコードの種類
		*/
		CLSource(const std::string& filename, const std::string& kernelName, const SourceType type = SourceType::Text)
			: fileName(filename), kernelName(kernelName), type(type)
		{
			OpenFile();
		}
	};
}

#endif