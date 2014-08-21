#ifndef TCL_READ_WRITE_BUFFER_HPP
#define TCL_READ_WRITE_BUFFER_HPP

#include "CLBuffer.hpp"

namespace tcl
{
	/**
	* デバイス側で読み書きのできるバッファ
	*/
	class CLReadWriteBuffer : public CLBuffer
	{
	private:
		CLReadWriteBuffer(const CLReadWriteBuffer& buf) { }

	public:
		/**
		* デバイス側で読み書きのできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] size デバイス側に確保する領域
		*/
		CLReadWriteBuffer(CLExecute& exec, const size_t size)
			: CLBuffer(exec, CL_MEM_READ_WRITE, size, NULL) { }

		/**
		* デバイス側で読み書きのできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] data バッファに書き込みたい配列
		*/
		template <typename T>
		CLReadWriteBuffer(CLExecute& exec, std::vector<T>& data)
			: CLBuffer(exec, CL_MEM_READ_WRITE, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		/**
		* デバイス側で読み書きのできるバッファ
		* \param[in] info OpenCLの情報クラス
		* \param[in] data バッファに書き込みたい配列
		*/
		template <typename T, size_t NUM>
		CLReadWriteBuffer(CLExecute& exec, std::array<T, NUM>& data)
			: CLBuffer(exec, CL_MEM_READ_WRITE, data.size() * sizeof(T), NULL)
		{
			Write(data);
		}

		virtual ~CLReadWriteBuffer() {}
	};
}

#endif