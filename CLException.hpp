#ifndef CL_EXEPTION_HPP
#define CL_EXEPTION_HPP

#include <string>
#include <exception>

namespace cl
{
	/**
	* TinyCL�̊�{�I�ȗ�O�N���X
	*/
	class CLExeption : public std::exception
	{
		CLExeption(const std::string& cause)
			: std::exception(cause.c_str()) {}
	};
}

#endif