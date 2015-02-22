#ifndef TCL_READ_WRITE_BUFFER_HPP
#define TCL_READ_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* デバイス側で読み書きのできるバッファ
	*/
	template <typename T>
	class CLReadWriteBuffer : public CLBuffer
	{
	private:
		CLReadWriteBuffer(const CLReadWriteBuffer& buf) { }

	public:
		/**
		* デバイス側で読み書きできるバッファ
		* \param[in] data バッファに書き込みたい配列
		*/
		template <typename T>
		CLReadWriteBuffer(std::vector<T>& data)
			: CLBuffer(CL_MEM_READ_WRITE, data.size() * sizeof(T), NULL, &data[0]) { }

		/**
		* デバイス側で読み書きできるバッファ
		* \param[in] data バッファに書き込みたい配列
		*/
		template <typename T, size_t NUM>
		CLReadWriteBuffer(std::array<T, NUM>& data)
			: CLBuffer(CL_MEM_READ_WRITE, NUM * sizeof(T), NULL, &data[0]) { }

		/**
		* デバイス側で読み書きできるバッファ
		* \param[in] data バッファに書き込みたいデータ
		*/
		template <typename T>
		CLReadWriteBuffer(T& data)
			: CLBuffer(CL_MEM_READ_WRITE, sizeof(T), NULL, &data) { }

		/**
		* デバイス側で読み書きできるバッファ
		* \param[in] data バッファに書き込みたい配列データのアドレス
		* \param[in] num 書き込みたいデータの個数
		*/
		template <typename T>
		CLReadWriteBuffer(T* data, const size_t& num)
			: CLBuffer(CL_MEM_READ_WRITE, sizeof(T) * num, NULL, data) { }

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif