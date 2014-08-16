#ifndef CL_EXCEPTION_HPP
#define CL_EXCEPTION_HPP

#include <string>
#include <exception>

namespace tcl
{
	/**
	* TinyCLの基本的な例外クラス
	*/
	class CLException : public std::exception
	{
	public:
		CLException(const std::string& cause)
			: std::exception(cause.c_str()) {}

		CLException(const std::string& cause, const cl_int& errorNo)
			: std::exception((cause + ":" + std::to_string(errorNo)).c_str()) {}
	};

	/**
	* 無効な引数が渡されたときの例外
	*/
	class CLInvalidArgumentException : public CLException
	{
	public:
		/**
		* 無効な引数が渡されたときの例外
		*/
		CLInvalidArgumentException(const std::string& cause)
			: CLException(cause) {}
	};

	/**
	* メモリの確保に失敗したときの例外
	*/
	class CLFailedAllacException : public CLException
	{
	public:
		/**
		* メモリの確保に失敗したときの例外
		*/
		CLFailedAllacException(const std::string& cause)
			: CLException(cause) {}
	};
}

#endif