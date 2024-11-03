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
	* @brief The interface is inherited from testing::Test.
	* Registry on Initialize.
	*/
	class ThrealCache_test : public testing::Test
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
		* @brief ThreadCache.
		*/
		Spices::ThreadCache tc;

		/**
		* @brief Iter counts.
		*/
		static constexpr size_t n = 10000;
	};

	/**
	* @brief Testing Spices::ThrealCache::Allocate/Deallocate.
	*/
	TEST_F(ThrealCache_test, AllocateDeallocate) {

		SPICESTEST_PROFILE_FUNCTION();

		std::array<ThrealCacheTest*, n> objects;
		for (int i = 0; i < n; i++)
		{
			ThrealCacheTest* a = new(tc.Allocate(sizeof(ThrealCacheTest)))ThrealCacheTest;

			EXPECT_EQ(std::get<0>(a->m_Tuple), 1.0f);
			EXPECT_EQ(std::get<1>(a->m_Tuple), 2);
			EXPECT_EQ(std::get<2>(a->m_Tuple), nullptr);

			objects[i] = a;
		}

		for (int i = 0; i < n; i++)
		{
			tc.Deallocate(objects[i], sizeof(ThrealCacheTest));
		}
	}
}