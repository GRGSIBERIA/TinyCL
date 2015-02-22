#ifndef TCL_READ_BUFFER_HPP
#define TCL_READ_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* デバイス側で読み込みのみできるバッファ
	*/
	template <typename T>
	class CLReadBuffer : public CLBuffer
	{
	public:
		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] data バッファに書き込みたい配列
		*/
		template <typename T>
		CLReadBuffer(std::vector<T>& data)
			: CLBuffer(CL_MEM_READ_ONLY, data) { }

		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] data バッファに書き込みたい配列
		*/
		template <typename T, size_t NUM>
		CLReadBuffer(std::array<T, NUM>& data)
			: CLBuffer(CL_MEM_READ_ONLY, data) { }

		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] data バッファに書き込みたいデータ
		*/
		template <typename T>
		CLReadBuffer(T& data)
			: CLBuffer(CL_MEM_READ_ONLY, data) { }

		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] data バッファに書き込みたい配列データのアドレス
		* \param[in] num 書き込みたいデータの個数
		*/
		template <typename T>
		CLReadBuffer(T* data, const size_t& num)
			: CLBuffer(CL_MEM_READ_ONLY, data, num) { }

		virtual ~CLReadBuffer() {}
	};
}

#endif