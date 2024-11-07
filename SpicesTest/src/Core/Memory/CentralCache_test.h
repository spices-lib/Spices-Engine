/**
* @file CentralCache_test.h.
* @brief The CentralCache_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <..\..\..\..\SpicesEngine\src\Core\Memory\CentralCache.h>
#include "Instrumentor.h"

namespace SpicesTest {

	/**
	* @brief Testing Spices::CentralCache::FetchRange/ReleaseListToSpans.
	*/
	TEST(CentralCache_test, FetchRangeReleaseListToSpans) {

		SPICESTEST_PROFILE_FUNCTION();

		Spices::CentralCache cc;

		void* start = nullptr;
		void* end   = nullptr;

		/**
		* @brief 8B - 1KB.
		*/
		for (int i = 8; i < 1024; i+= 8)
		{
			const size_t alignBytes  = Spices::MemoryPool::AlignUp(i);
			const size_t actualSize = cc.FetchRange(start, end, UINT32_MAX, alignBytes);

			EXPECT_NE(start, nullptr);
			EXPECT_NE(end, nullptr);

			EXPECT_NE((char*)start + actualSize * alignBytes, (char*)end);

			cc.ReleaseListToSpans(start, alignBytes);
		}

		/**
		* @brief 1KB - 256KB.
		*/
		for (int i = 1024; i < 256 * 1024; i += 8 * 1024)
		{
			const size_t alignBytes = Spices::MemoryPool::AlignUp(i);
			const size_t acturlSize = cc.FetchRange(start, end, UINT32_MAX, alignBytes);

			EXPECT_NE(start, nullptr);
			EXPECT_NE(end, nullptr);

			EXPECT_NE((char*)start + acturlSize * alignBytes, (char*)end);

			cc.ReleaseListToSpans(start, alignBytes);
		}
	}
}