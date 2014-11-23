#ifndef TCL_READ_BUFFER_HPP
#define TCL_READ_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* デバイス側で読み込みのみできるバッファ
	*/
	class CLReadBuffer : public CLBuffer
	{
	private:
		CLReadBuffer(const CLReadBuffer& buf) { }

	public:
		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size デバイス側に確保する領域
		*/
		CLReadBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_READ_ONLY, size, NULL) { }

		virtual ~CLReadBuffer() {}

		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] exec OpenCLの実行クラス
		* \param[in] data バッファに書き込みたい配列
		*/
		template <typename T>
		CLReadBuffer(CLExecute& exec, std::vector<T>& data)
			: CLBuffer(exec, CL_MEM_READ_ONLY, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] exec OpenCLの実行クラス
		* \param[in] data バッファに書き込みたい配列
		*/
		template <typename T, size_t NUM>
		CLReadBuffer(CLExecute& exec, std::array<T, NUM>& data)
			: CLBuffer(exec, CL_MEM_READ_ONLY, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		/**
		* デバイス側で読み込みのみできるバッファ
		* \param[in] exec OpenCLの実行クラス
		* \param[in] data バッファに書き込みたいデータ
		*/
		template <typename T>
		CLReadBuffer(CLExecute& exec, const T data)
			: CLBuffer(exec, CL_MEM_READ_ONLY, sizeof(T), NULL)
		{
			Write(data);
		}
	};
}

#endif