/**
* @file ThreadCache_test.h.
* @brief The ThreadCache_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Memory/ThrealCache.h>
#include "Instrumentor.h"

namespace SpicesTest {

	class ThrealCacheTest
	{
	public:

		ThrealCacheTest()
			: m_Tuple{ 1.0f, 2, nullptr }
		{}

		ThrealCacheTest(const ThrealCacheTest&) = delete;
		ThrealCacheTest& operator=(const ThrealCacheTest&) = delete;

		std::tuple<int, float, void*> m_Tuple;
	};

	/**
	* @brief Testing Spices::ThrealCache::Allocate.
	*/
	TEST(ThrealCache_test, Allocate) {

		SPICESTEST_PROFILE_FUNCTION();

		Spices::ThreadCache tc;

		for (int i = 0; i < 10000; i++)
		{
			int* a = new(tc.Allocate(sizeof(int)))int;

			EXPECT_EQ(*a, 0);
		}

		for (int i = 0; i < 10000; i++)
		{
			ThrealCacheTest* b = new(tc.Allocate(sizeof(ThrealCacheTest)))ThrealCacheTest;

			EXPECT_EQ(std::get<0>(b->m_Tuple), 1.0f);
			EXPECT_EQ(std::get<1>(b->m_Tuple), 2);
			EXPECT_EQ(std::get<2>(b->m_Tuple), nullptr);
		}
	}
}