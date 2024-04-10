#include "Pchheader.h"
#include "FrameInfo.h"

namespace Spiecs
{
	static FrameInfo m_FrameInfo;

	FrameInfo& FrameInfo::Get()
	{
		return m_FrameInfo;
	}

}