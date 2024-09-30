/**
* @file FrameInfo.h.
* @brief The FrameInfo Class Definitions.
* @author Spices.
*/

#include "Pchheader.h"
#include "FrameInfo.h"

namespace Spices
{
	static FrameInfo m_FrameInfo;

	FrameInfo& FrameInfo::Get()
	{
		return m_FrameInfo;
	}

	uint32_t FrameInfo::PrevFrameIndex()
	{
		uint32_t f = m_FrameIndex;
		return f == 0 ? MaxFrameInFlight - 1 : (f--) % MaxFrameInFlight;
	}
}