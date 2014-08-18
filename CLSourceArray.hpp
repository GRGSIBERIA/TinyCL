#ifndef TCL_SOURCE_ARRAY_HPP
#define TCL_SOURCE_ARRAY_HPP

#include <vector>
#include "CLSource.hpp"

namespace tcl
{
	/**
	* 複数ソースコードを使いたい場合の配列
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
		* ソースコードのポインタ配列を返す
		* \return ソースコードのポインタ配列
		*/
		inline const char** Pointers() const {
			// これ安全かどうかわからないなぁ……
			return sourcePtrs.begin()._Ptr;
		}

		/**
		* ソースコードのサイズの配列を返す
		* \return ソースコードのサイズの配列
		*/
		inline const size_t* Sizes() const {
			return sourceSizes.begin()._Ptr;
		}

		/**
		* ソースコードの種類を返す
		* \return ソースコードの種類を返す
		*/
		inline const SourceType Type() const {
			return type;
		}

		/**
		* カーネルの名前を返す
		* \return カーネルの名前を返す
		*/
		inline const std::string KernelName() const {
			return kernelName;
		}

		/**
		* 利用したいソースコードを追加します
		* \param[in] filename ソースコードのファイル名
		*/
		inline void Append(const std::string& filename) {
			sources.emplace_back(CLSource(filename, kernelName, type));
			auto s = sources[sources.size() - 1];
			sourcePtrs.emplace_back(s.Code().c_str());
			sourceSizes.emplace_back(s.Size());
		}

	public:
		/**
		* 一つのカーネルに複数のソースコードを実行させたいときに使います
		* \param[in] kernelName エントリーポイントとなるカーネル名
		* \param[in] useSourceCodeType ソースコードの種類
		*/
		CLSourceArray(const std::string& kernelName, const SourceType useSourceCodeType)
			: type(useSourceCodeType), kernelName(kernelName)
		{

		}
	};
}

#endif