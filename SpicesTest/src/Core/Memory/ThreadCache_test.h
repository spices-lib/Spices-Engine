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
			: m_Tuple{ 1, 2.0f, nullptr }
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
		static constexpr size_t n = 1000000;
	};

	/**
	* @brief Testing Spices::ThrealCache::Allocate/Deallocate.
	*/
	TEST_F(ThrealCache_test, AllocateDeallocate) {

		SPICESTEST_PROFILE_FUNCTION();

		std::unique_ptr<std::array<ThrealCacheTest*, n>> objects = std::make_unique<std::array<ThrealCacheTest*, n>>();
		for (int i = 0; i < n; i++)
		{
			ThrealCacheTest* a = new(tc.Allocate(sizeof(ThrealCacheTest)))ThrealCacheTest;

			EXPECT_EQ(std::get<0>(a->m_Tuple), 1);
			EXPECT_EQ(std::get<1>(a->m_Tuple), 2.0f);
			EXPECT_EQ(std::get<2>(a->m_Tuple), nullptr);

			(*objects)[i] = std::move(a);
		}

		for (int i = 0; i < n; i++)
		{
			tc.Deallocate((*objects)[i], sizeof(ThrealCacheTest));
		}
	}

	/**
	* @brief Testing Spices::ThrealCache::Allocate/Deallocate in Thread.
	*/
	TEST_F(ThrealCache_test, ThreadAllocateDeallocate) {

		SPICESTEST_PROFILE_FUNCTION();

		std::vector<std::thread> threads;

		for(int i = 0; i < 2; i++)
		{
			std::thread t([&]() {
				std::unique_ptr<std::array<ThrealCacheTest*, n>> objects = std::make_unique<std::array<ThrealCacheTest*, n>>();
				for (int i = 0; i < n; i++)
				{
					ThrealCacheTest* a = new(tc.Allocate(sizeof(ThrealCacheTest)))ThrealCacheTest;

					EXPECT_EQ(std::get<0>(a->m_Tuple), 1);
					EXPECT_EQ(std::get<1>(a->m_Tuple), 2.0f);
					EXPECT_EQ(std::get<2>(a->m_Tuple), nullptr);

					(*objects)[i] = std::move(a);
				}

				for (int i = 0; i < n; i++)
				{
					tc.Deallocate((*objects)[i], sizeof(ThrealCacheTest));
				}
			});

			threads.push_back(std::move(t));
		}

		for (auto& t : threads)
		{
			t.join();
		}
	}
}