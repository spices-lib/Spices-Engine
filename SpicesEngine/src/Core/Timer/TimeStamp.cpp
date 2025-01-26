/**
* @file TimeStamp.cpp.
* @brief The TimeStamp Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "TimeStamp.h"

#include <time.h>

namespace Spices {

	TimeStamp::TimeStamp()
		: m_MicroSecondsSinceEpoch(0)
	{}

	TimeStamp::TimeStamp(int64_t microSecondsSinceEpoch)
		: m_MicroSecondsSinceEpoch(microSecondsSinceEpoch)
	{}

	TimeStamp TimeStamp::Now()
	{
		return TimeStamp(time(NULL));
	}

	std::string TimeStamp::ToString() const
	{
		SPICES_PROFILE_ZONE;

		char buf[128] = { 0 };
		tm* ti_time = nullptr;
		localtime_s(ti_time, &m_MicroSecondsSinceEpoch);

		snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d",
			ti_time->tm_year + 1900,
			ti_time->tm_mon + 1,
			ti_time->tm_mday,
			ti_time->tm_hour,
			ti_time->tm_min,
			ti_time->tm_sec
		);

		return buf;
	}
}
