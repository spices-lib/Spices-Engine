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

	class ThreadCacheTest
	{
	public:

		ThreadCacheTest()
			: m_Tuple{ 1, 2.0f, nullptr }
		{}

		ThreadCacheTest(const ThreadCacheTest&) = delete;
		ThreadCacheTest& operator=(const ThreadCacheTest&) = delete;

		std::tuple<int, float, void*> m_Tuple;
	};

	/**
	* @brief The interface is inherited from testing::Test.
	* Registry on Initialize.
	*/
	class ThreadCache_test : public testing::Test
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
		static constexpr size_t n = 500000;
	};

	/**
	* @brief Testing Spices::ThreadCache::Allocate/Deallocate.
	*/
	TEST_F(ThreadCache_test, AllocateDeallocate) {

		SPICESTEST_PROFILE_FUNCTION();

		const std::unique_ptr<std::array<ThreadCacheTest*, n>> objects = std::make_unique<std::array<ThreadCacheTest*, n>>();
		for (size_t i = 0; i < n; i++)
		{
			auto a = new(tc.Allocate(sizeof(ThreadCacheTest)))ThreadCacheTest;

			EXPECT_EQ(std::get<0>(a->m_Tuple), 1);
			EXPECT_EQ(std::get<1>(a->m_Tuple), 2.0f);
			EXPECT_EQ(std::get<2>(a->m_Tuple), nullptr);

			(*objects)[i] = std::move(a);
		}

		for (size_t i = 0; i < n; i++)
		{
			tc.Deallocate((*objects)[i], sizeof(ThreadCacheTest));
		}
	}

	/**
	* @brief Testing Spices::ThreadCache::Allocate/Deallocate in Thread.
	*/
	TEST_F(ThreadCache_test, ThreadAllocateDeallocate) {

		SPICESTEST_PROFILE_FUNCTION();

		std::vector<std::thread> threads;

		for(int i = 0; i < 5; i++)
		{
			std::thread t([&]() {
				Spices::ThreadCache internalTc;

				const std::unique_ptr<std::array<ThreadCacheTest*, n>> objects = std::make_unique<std::array<ThreadCacheTest*, n>>();
				for (size_t j = 0; j < n; j++)
				{
					auto a = new(internalTc.Allocate(sizeof(ThreadCacheTest)))ThreadCacheTest;

					EXPECT_EQ(std::get<0>(a->m_Tuple), 1);
					EXPECT_EQ(std::get<1>(a->m_Tuple), 2.0f);
					EXPECT_EQ(std::get<2>(a->m_Tuple), nullptr);

					(*objects)[j] = std::move(a);
				}

				for (size_t j = 0; j < n; j++)
				{
					internalTc.Deallocate((*objects)[j], sizeof(ThreadCacheTest));
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