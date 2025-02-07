/**
* @file Buffer.h.
* @brief The Buffer Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"
#include "InetAddress.h"

namespace Spices {

namespace Net {

	/**
	* @brief Wrapper of readwrite buffer.
	*/
	class Buffer
	{
	public:

		static constexpr size_t header = 8;
		static constexpr size_t initialSize = 1024;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] bytes Buffer read area Bytes.
		*/
		Buffer(size_t bytes = initialSize)
			: m_Buffer(header + initialSize)
			, m_ReaderIndex(header)
			, m_WriterIndex(header)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~Buffer() = default;

		/**
		* @brief Get readable area bytes.
		* @return Returns readable area bytes.
		*/
		size_t ReadableBytes() const
		{
			return m_WriterIndex - m_ReaderIndex;
		}

		/**
		* @brief Get writeable area bytes.
		* @return Returns writeable area bytes.
		*/
		size_t WriteableBytes() const
		{
			return m_Buffer.size() - m_WriterIndex;
		}

		/**
		* @brief Get header area bytes.
		* @return Returns header area bytes.
		*/
		size_t HeaderBytes() const
		{
			return m_ReaderIndex;
		}

		/**
		* @brief Get read area start pointer.
		* @return Returns read area start pointer.
		*/
		const char* Peek() const
		{
			return Begin() + m_ReaderIndex;
		}

		/**
		* @brief Retrieve some buffer data from data area.
		* @param[in] len Retrieve length.
		*/
		void Retrieve(size_t len);

		/**
		* @brief Retrieve all buffer data.
		*/
		void RetrieveAll()
		{
			m_ReaderIndex = m_WriterIndex = header;
		}

		/**
		* @brief Get readable area data as string.
		* @return Returns readable area data
		*/
		std::string RetrieveAllAsString()
		{
			return RetrieveAsString(ReadableBytes());
		}

		/**
		* @brief Get retrieve area data as string.
		* @param[in] len Retrieve length.
		* @return Returns retrieve area data
		*/
		std::string RetrieveAsString(size_t len);

		/**
		* @brief Ensure enough bytes to write.
		* @param[in] len bytes.
		*/
		void EnsureWriteableBytes(size_t len);

		/**
		* @brief Append Message to this buffer.
		* @param[in] msg Message.
		*/
		void Append(const std::string& msg);

		/**
		* @brief Append another buffer data.
		* @param[in] data Buffer start pointer.
		* @param[in] len Buffer bytes.
		*/
		void Append(const char* data, size_t len);

		/**
		* @brief Get writeable area pointer.
		* @return Returns writeable area pointer.
		*/
		char* BeginWrite()
		{
			return Begin() + m_WriterIndex;
		}

		/**
		* @brief Get writeable area pointer.
		* @return Returns writeable area pointer.
		*/
		const char* BeginWrite() const
		{
			return Begin() + m_WriterIndex;
		}

		/**
		* @brief Read from Socket.
		* @param[in] fd SOCKET.
		* @param[in,out] saveErrno Error value.
		* @return Returns read bytes.
		* @note read api can only be used in linux, on windows use recv instead,
		*/
		size_t ReadFd(SOCKET fd, int* saveErrno);

		/**
		* @brief Write to Socket.
		* @param[in] fd SOCKET.
		* @param[in,out] saveErrno Error value.
		* @return Returns write bytes.
		* @note write api can only be used in linux, on windows use send instead,
		*/
		size_t WriteFd(SOCKET fd, int* saveErrno);

	private:

		/**
		* @brief Get Buffer start pointer.
		* @return Returns Buffer start pointer.
		*/
		char* Begin()
		{
			return m_Buffer.data();
		}

		/**
		* @brief Get Buffer start pointer.
		* @return Returns Buffer start pointer.
		*/
		const char* Begin() const
		{
			return m_Buffer.data();
		}

		/**
		* @brief Expend buffer size.
		* @param[in] len buffer size.
		*/
		void expend(size_t len);

	private:

		/**
		* @brief Buffer data.
		*/
		std::vector<char> m_Buffer;

		/**
		* @brief Reader pointer.
		*/
		size_t m_ReaderIndex;

		/**
		* @brief Writer pointer.
		*/
		size_t m_WriterIndex;
	};

}

}