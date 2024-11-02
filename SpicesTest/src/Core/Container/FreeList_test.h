/**
* @file FreeList_test.h.
* @brief The FreeList_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Container/FreeList.h>
#include "Instrumentor.h"

namespace SpicesTest {

	/**
	* @brief The interface is inherited from testing::Test.
	* Registry on Initialize.
	*/
	class FreeList_test : public testing::Test
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

		scl::free_list m_FreeList;
	};

	/**
	* @brief Testing Spices::free_list::Push/Pop.
	*/
	TEST_F(FreeList_test, PushPop) {

		SPICESTEST_PROFILE_FUNCTION();

		std::array<int, 2> a;
		uint64_t b = 0;
		char* c = nullptr;
		std::tuple<int, void* (*)(int)> d { 1, nullptr };

		EXPECT_EQ(m_FreeList.Empty(), true);
		EXPECT_EQ(m_FreeList.ApplyforNBlocks(), 1);
		EXPECT_EQ(m_FreeList.Size(), 0);
		EXPECT_EQ(m_FreeList.Begin(), nullptr);
		EXPECT_EQ(m_FreeList.End(), nullptr);

		{
			m_FreeList.Push(&a);
			m_FreeList.Push(&b);
			m_FreeList.Push(&c);
			m_FreeList.Push(&d);

			EXPECT_EQ(m_FreeList.Empty(), false);
			EXPECT_EQ(m_FreeList.ApplyforNBlocks(), 1);
			EXPECT_EQ(m_FreeList.Size(), 4);
			EXPECT_EQ(m_FreeList.End(), &a);

			const void* p = m_FreeList.Begin();
			EXPECT_EQ(p, &d);

			const void* p1 = *(void**)p;
			EXPECT_EQ(p1, &c);

			const void* p2 = *(void**)p1;
			EXPECT_EQ(p2, &b);
			
			const void* p3 = *(void**)p2;
			EXPECT_EQ(p3, &a);
			
			const void* p4 = *(void**)p3;
			EXPECT_EQ(p4, nullptr);
		}

		{
			EXPECT_EQ(m_FreeList.Pop(), &d);
			EXPECT_EQ(m_FreeList.Pop(), &c);
			EXPECT_EQ(m_FreeList.Pop(), &b);
			EXPECT_EQ(m_FreeList.Pop(), &a);

			EXPECT_EQ(m_FreeList.Empty(), true);
			EXPECT_EQ(m_FreeList.ApplyforNBlocks(), 1);
			EXPECT_EQ(m_FreeList.Size(), 0);
			EXPECT_EQ(m_FreeList.Begin(), nullptr);
			EXPECT_EQ(m_FreeList.End(), nullptr);
		}
	}

	/**
	* @brief Testing Spices::free_list::PushRange/PopRange.
	*/
	TEST_F(FreeList_test, PushRangePopRange) {

		SPICESTEST_PROFILE_FUNCTION();

		scl::free_list list;

		std::array<int, 2> a;
		uint64_t b = 0;
		char* c = nullptr;
		std::tuple<int, void* (*)(int)> d { 1, nullptr };

		m_FreeList.Push(&a);
		m_FreeList.Push(&b);

		list.Push(&c);
		list.Push(&d);

		{
			m_FreeList.PushRange(list.Begin(), list.End(), list.Size());

			EXPECT_EQ(m_FreeList.Empty(), false);
			EXPECT_EQ(m_FreeList.ApplyforNBlocks(), 1);
			EXPECT_EQ(m_FreeList.Size(), 4);
			EXPECT_EQ(m_FreeList.End(), &a);

			const void* p = m_FreeList.Begin();
			EXPECT_EQ(p, &d);

			const void* p1 = *(void**)p;
			EXPECT_EQ(p1, &c);

			const void* p2 = *(void**)p1;
			EXPECT_EQ(p2, &b);

			const void* p3 = *(void**)p2;
			EXPECT_EQ(p3, &a);

			const void* p4 = *(void**)p3;
			EXPECT_EQ(p4, nullptr);
		}

		{
			scl::free_list list2;
			m_FreeList.PopRange(list2.Begin(), list2.End(), list.Size());

			{
				EXPECT_EQ(m_FreeList.Empty(), false);
				EXPECT_EQ(m_FreeList.ApplyforNBlocks(), 1);
				EXPECT_EQ(m_FreeList.Size(), 2);
				EXPECT_EQ(m_FreeList.End(), &a);

				const void* p = m_FreeList.Begin();
				EXPECT_EQ(p, &b);

				const void* p1 = *(void**)p;
				EXPECT_EQ(p1, &a);

				const void* p2 = *(void**)p1;
				EXPECT_EQ(p2, nullptr);
			}

			{
				EXPECT_EQ(list2.Empty(), false);
				EXPECT_EQ(list2.ApplyforNBlocks(), 1);
				EXPECT_EQ(list2.Size(), 0);
				EXPECT_EQ(list2.End(), &c);

				const void* p = list2.Begin();
				EXPECT_EQ(p, &d);

				const void* p1 = *(void**)p;
				EXPECT_EQ(p1, &c);

				const void* p2 = *(void**)p1;
				EXPECT_EQ(p2, nullptr);
			}
		}
	}
}