/**
* @file ObjectPool_test.h.
* @brief The ObjectPool_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Memory/ObjectPool.h>
#include <Core/Timer/ScopeTimer.h>
#include "Instrumentor.h"

namespace SpicesTest {

	class Object
	{
	public:
		Object() : tuple{ 1.0f, 2, nullptr } {}
		virtual ~Object() = default;
		void Hello() {}

		std::tuple<float, int, void*> tuple;
	};

	/**
	* @brief The interface is inherited from testing::Test.
	* Registry on Initialize.
	*/
	class ObjectPool_test : public testing::Test
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
		* @brief ObjectPool.
		*/
		Spices::ObjectPool<Object> m_ObjectPool;
	};

	/**
	* @brief Testing Spices::ObjectPool::Initialize.
	*/
	TEST_F(ObjectPool_test, Initialize) {

		SPICESTEST_PROFILE_FUNCTION();

		EXPECT_EQ(m_ObjectPool.GetPointer(), nullptr);
		EXPECT_EQ(m_ObjectPool.GetFreeList(), nullptr);
		EXPECT_EQ(m_ObjectPool.GetSpareBytes(), 0);
	}

    /**
    * @brief Testing Spices::ObjectPool::New/Delete.
    */
	TEST_F(ObjectPool_test, NewDelete) {

        SPICESTEST_PROFILE_FUNCTION();

		Object& o = *m_ObjectPool.New();

		EXPECT_EQ(std::get<0>(o.tuple), 1.0f);
		EXPECT_EQ(std::get<1>(o.tuple), 2);
		EXPECT_EQ(std::get<2>(o.tuple), nullptr);

		EXPECT_EQ(m_ObjectPool.GetSpareBytes(), 128 * 1024 - sizeof(Object));

		m_ObjectPool.Delete(&o);

		EXPECT_EQ(m_ObjectPool.GetSpareBytes(), 128 * 1024);
    }

	/**
	* @brief Testing Spices::ObjectPool::Compare.
	*/
	TEST_F(ObjectPool_test, CompareWithOrigin) {

		SPICESTEST_PROFILE_FUNCTION();

		size_t n = 1000000;

		{
			SCOPE_TIME_COUNTER("Malloc Cost.")

			for (size_t i = 0; i < n; i++)
			{
				Object* o = new Object;
				delete o;
			}
		}

		{
			SCOPE_TIME_COUNTER("ObjectPool Cost.")

			for (size_t i = 0; i < n; i++)
			{
				Object* o = m_ObjectPool.New();
			}
		}
	}
}