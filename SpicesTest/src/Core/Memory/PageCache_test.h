/**
* @file PageCache_test.h.
* @brief The PageCache_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Memory/PageCache.h>
#include "Instrumentor.h"

namespace SpicesTest {

	/**
	* @brief Testing Spices::PageCache::NewSpan.
	*/
	TEST(PageCache_test, NewSpan) {

		SPICESTEST_PROFILE_FUNCTION();

		Spices::PageCache pc;

		static constexpr size_t iter = 256;

		for (int i = 1; i < iter; i++)
		{
			scl::span* s = pc.NewSpan(i);
			EXPECT_EQ(s->m_FreeList, nullptr);
			EXPECT_EQ(s->m_Next, nullptr);
			EXPECT_EQ(s->m_Prev, nullptr);
			EXPECT_EQ(s->m_BlockSize, 0);
			EXPECT_EQ(s->m_IsUse, false);
			EXPECT_EQ(s->m_NPages, i);
			EXPECT_EQ(s->m_UseCount, 0);
		}
	}
}