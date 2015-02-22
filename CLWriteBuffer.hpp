#ifndef TCL_WRITE_BUFFER_HPP
#define TCL_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* デバイス側で書き込みのみできるバッファ
	*/
	class CLWriteBuffer : public CLBuffer
	{
	public:
		/**
		* デバイス側で書き込みのみできるバッファ
		* \param[in] data 実行後に転送対象になる配列
		*/
		template <typename T>
		CLWriteBuffer(std::vector<T>& data)
			: CLBuffer(CL_MEM_WRITE_ONLY, data) { }

		/**
		* デバイス側で書き込みのみできるバッファ
		* \param[in] data 実行後に転送対象になる配列
		*/
		template <typename T, size_t NUM>
		CLWriteBuffer(std::array<T, NUM>& data)
			: CLBuffer(CL_MEM_WRITE_ONLY, data) { }

		/**
		* デバイス側で書き込みのみできるバッファ
		* \param[in] data 実行後に転送対象になるデータ
		*/
		template <typename T>
		CLWriteBuffer(T& data)
			: CLBuffer(CL_MEM_WRITE_ONLY, data) { }

		/**
		* デバイス側で書き込みのみできるバッファ
		* \param[in] data 実行後に転送対象になる配列データのアドレス
		* \param[in] num 書き込みたいデータの個数
		*/
		template <typename T>
		CLWriteBuffer(T* data, const size_t& num)
			: CLBuffer(CL_MEM_WRITE_ONLY, data, num) { }

		virtual ~CLWriteBuffer() {}
	};
}

#endif