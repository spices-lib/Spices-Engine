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
	* @brief The interface is inherited from testing::Test.
	* Registry on Initialize.
	*/
	class PageCache_test : public testing::Test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registry on Initialize.
		*/
		void SetUp() override {}

		/**
		* @brief The interface is inherited from testing::Test.
		* Call before Destructor.
		*/
		void TearDown() override {}

		/**
		* @brief PageCache.
		*/
		Spices::PageCache pc;

		/**
		* @brief Test iter number.
		*/
		static constexpr size_t n = 256;
	};

	/**
	* @brief Testing Spices::PageCache::NewSpan.
	*/
	TEST_F(PageCache_test, NewSpan) {

		SPICESTEST_PROFILE_FUNCTION();

		std::array<scl::span*, n> spans;

		for (int i = 1; i < n; i++)
		{
			scl::span* s = pc.NewSpan(i);

			EXPECT_EQ(s->m_FreeList, nullptr);
			EXPECT_EQ(s->m_Next, nullptr);
			EXPECT_EQ(s->m_Prev, nullptr);
			EXPECT_EQ(s->m_BlockSize, i * (1 << Spices::MemoryPool::PAGE_SHIFT));
			EXPECT_EQ(s->m_IsUse, false);
			EXPECT_EQ(s->m_NPages, i);
			EXPECT_EQ(s->m_UseCount, 0);

			s->m_IsUse = true;

			spans[i] = std::move(s);
		}

		for (int i = 1; i < n; i++)
		{
			pc.ReleaseSpanToPageCache(spans[i]);
		}
	}
}