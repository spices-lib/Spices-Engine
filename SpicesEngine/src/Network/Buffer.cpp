/**
* @file Buffer.cpp.
* @brief The Buffer Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "Buffer.h"

namespace Spices {

namespace Net {

	void Buffer::Retrieve(size_t len)
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

	std::string Buffer::RetrieveAsString(size_t len)
	{
		std::string result(Peek(), len);
		Retrieve(len);
		return result;
	}

	void Buffer::EnsureWriteableBytes(size_t len)
	{
		if (WriteableBytes() < len)
		{
			expend(len);
		}
	}

	void Buffer::Append(const char* data, size_t len)
	{
		EnsureWriteableBytes(len);
		std::copy(data, data + len, BeginWrite());
		m_WriterIndex += len;
	}

	size_t Buffer::ReadFd(SOCKET fd, int* saveErrno)
	{
		char buffer[65536] = {}; // 64k

		int n = ::read(fd, &buffer, sizeof(buffer));

		if (n < 0)
		{
			*saveErrno = WSAGetLastError();
		}
		else if (n <= WriteableBytes())
		{
			m_WriterIndex += n;
		}
		else
		{
			m_WriterIndex = m_Buffer.size();
			//Append();
		}

		return n;
	}

	size_t Buffer::WriteFd(SOCKET fd, int* saveErrno)
	{
		int n = ::write(fd, Peek(), ReadableBytes());

		if (n < 0)
		{
			*saveErrno = WSAGetLastError();
		}

		return n;
	}

	void Buffer::expend(size_t len)
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

}

}