#pragma once
#include "Core/Core.h"
#include "InetAddress.h"

namespace Spices {

namespace Net {

	class Buffer
	{
	public:

		static const size_t header = 8;
		static const size_t initialSize = 1024;

	public:

		Buffer(size_t bytes = initialSize)
			: m_Buffer(header + initialSize)
			, m_ReaderIndex(header)
			, m_WriterIndex(header)
		{}

		size_t ReadableBytes() const
		{
			return m_WriterIndex - m_ReaderIndex;
		}

		size_t WriteableBytes() const
		{
			return m_Buffer.size() - m_WriterIndex;
		}

		size_t HeaderBytes() const
		{
			return m_ReaderIndex;
		}

		const char* Peek() const
		{
			return Begin() + m_ReaderIndex;
		}

		void Retrieve(size_t len)
		{
			if (len < ReadableBytes())
			{
				m_ReaderIndex += len;
			}
			else
			{
				RetrieveAll();
			}
		}

		void RetrieveAll()
		{
			m_ReaderIndex = m_WriterIndex = header;
		}

		std::string RetrieveAsString(size_t len)
		{
			std::string result(Peek(), len);
			Retrieve(len);
			return result;
		}

		void EnsureWriteableBytes(size_t len)
		{
			if (WriteableBytes() < len)
			{
				expend(len);
			}
		}

		void Append(const char* data, size_t len)
		{
			EnsureWriteableBytes(len);
			std::copy(data, data + len, BeginWrite());
			m_WriterIndex += len;
		}

		char* BeginWrite()
		{
			return Begin() + m_WriterIndex;
		}

		const char* BeginWrite() const
		{
			return Begin() + m_WriterIndex;
		}

		size_t ReadFd(SOCKET fd, int* saveErrno);
		size_t WriteFd(SOCKET fd, int* saveErrno);

	private:

		char* Begin()
		{
			return m_Buffer.data();
		}

		const char* Begin() const
		{
			return m_Buffer.data();
		}

		void expend(size_t len)
		{
			if (WriteableBytes() + HeaderBytes() < len + header)
			{
				m_Buffer.resize(m_WriterIndex + len);
			}
			else
			{
				size_t readableBytes = ReadableBytes();
				std::copy(
					Begin() + m_ReaderIndex,
					Begin() + m_WriterIndex,
					Begin() + header
				);

				m_ReaderIndex = header;
				m_WriterIndex = m_ReaderIndex + readableBytes;
			}
		}

	private:

		std::vector<char> m_Buffer;
		size_t m_ReaderIndex;
		size_t m_WriterIndex;
	};

}

}