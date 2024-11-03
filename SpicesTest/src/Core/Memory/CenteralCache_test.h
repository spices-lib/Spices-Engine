/**
* @file CenteralCache_test.h.
* @brief The CenteralCache_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Memory/CenteralCache.h>
#include "Instrumentor.h"

namespace SpicesTest {

	/**
	* @brief Testing Spices::CenteralCache::FetchRange/ReleaseListToSpans.
	*/
	TEST(CenteralCache_test, FetchRangeReleaseListToSpans) {

		SPICESTEST_PROFILE_FUNCTION();

		Spices::CenteralCache cc;

		void* start = nullptr;
		void* end   = nullptr;

		/**
		* @brief 8B - 1KB.
		*/
		for (int i = 8; i < 1024; i+= 8)
		{
			size_t alignBytes  = Spices::MemoryPool::AlignUp(i);
			size_t acturalSize = cc.FetchRange(start, end, UINT32_MAX, alignBytes);

			EXPECT_NE(start, nullptr);
			EXPECT_NE(end, nullptr);

			EXPECT_NE((char*)start + acturalSize * alignBytes, (char*)end);

			cc.ReleaseListToSpans(start, alignBytes);
		}

		/**
		* @brief 1KB - 256KB.
		*/
		for (int i = 1024; i < 256 * 1024; i += 8 * 1024)
		{
			size_t alignBytes = Spices::MemoryPool::AlignUp(i);
			size_t acturalSize = cc.FetchRange(start, end, UINT32_MAX, alignBytes);

			EXPECT_NE(start, nullptr);
			EXPECT_NE(end, nullptr);

			EXPECT_NE((char*)start + acturalSize * alignBytes, (char*)end);

			cc.ReleaseListToSpans(start, alignBytes);
		}
	}
}