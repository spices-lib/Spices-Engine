#include "Pchheader.h"
#include "Buffer.h"

namespace Spices {

namespace Net {



	size_t Buffer::ReadFd(SOCKET fd, int* saveErrno)
	{
		char buffer[65536] = { 0 };

		int n = ::read(fd, &buffer, sizeof(buffer));
		if (n < 0)
		{
			*saveErrno = errno;
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
			*saveErrno = errno;
		}
		return n;
	}

}

}