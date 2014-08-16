#ifndef CL_EXCEPTION_HPP
#define CL_EXCEPTION_HPP

#include <string>
#include <exception>

namespace tcl
{
	/**
	* TinyCL�̊�{�I�ȗ�O�N���X
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
	* �����Ȉ������n���ꂽ�Ƃ��̗�O
	*/
	class CLInvalidArgumentException : public CLException
	{
	public:
		/**
		* �����Ȉ������n���ꂽ�Ƃ��̗�O
		*/
		CLInvalidArgumentException(const std::string& cause)
			: CLException(cause) {}
	};

	/**
	* �������̊m�ۂɎ��s�����Ƃ��̗�O
	*/
	class CLFailedAllacException : public CLException
	{
	public:
		/**
		* �������̊m�ۂɎ��s�����Ƃ��̗�O
		*/
		CLFailedAllacException(const std::string& cause)
			: CLException(cause) {}
	};
}

#endif