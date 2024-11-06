/**
* @file ObjectPool_test.h.
* @brief The ObjectPool_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Memory/ObjectPool.h>
#include <Core/Library/MemoryLibrary.h>
#include "Instrumentor.h"

namespace SpicesTest {

	class Object
	{
	public:

		Object() 
			: tuple{ 1.0f, 2, nullptr } 
		{}

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
		void SetUp() override {
			m_ObjectPool = std::make_unique<Spices::ObjectPool<Object>>(Spices::ObjectPoolSizeMode::FixedObjects, n);
		}

		/**
		* @brief The interface is inherited from testing::Test.
		* Call before Destructor.
		*/
		void TearDown() override {}

		/**
		* @brief ObjectPool.
		*/
		std::unique_ptr<Spices::ObjectPool<Object>> m_ObjectPool;

		/**
		* @brief ObjectPool objects number.
		*/
		static constexpr size_t n = 1000000;
	};

	/**
	* @brief Testing Spices::ObjectPool::Initialize.
	*/
	TEST_F(ObjectPool_test, Initialize) {

		SPICESTEST_PROFILE_FUNCTION();

		EXPECT_EQ(m_ObjectPool->GetPointer(), nullptr);
		EXPECT_EQ(m_ObjectPool->GetFreeList(), nullptr);
		EXPECT_EQ(m_ObjectPool->GetSpareBytes(), 0);
		EXPECT_EQ(m_ObjectPool->GetNMemoryBlocks(), 0);
	}

    /**
    * @brief Testing Spices::ObjectPool::New/Delete.
    */
	TEST_F(ObjectPool_test, NewDelete) {

        SPICESTEST_PROFILE_FUNCTION();

		Object& o = *m_ObjectPool->New();

		EXPECT_EQ(std::get<0>(o.tuple), 1.0f);
		EXPECT_EQ(std::get<1>(o.tuple), 2);
		EXPECT_EQ(std::get<2>(o.tuple), nullptr);

		size_t allBytes = Spices::MemoryLibrary::align_up<size_t>(n * sizeof(Object), 8 * 1024);
		size_t remainBytes = allBytes - sizeof(Object);
		EXPECT_EQ(m_ObjectPool->GetSpareBytes(), remainBytes);
		EXPECT_EQ(m_ObjectPool->GetNMemoryBlocks(), 1);

		m_ObjectPool->Delete(&o);

		EXPECT_EQ(std::get<0>(o.tuple), 0.0f);
		EXPECT_EQ(std::get<1>(o.tuple), 0);
		EXPECT_EQ(std::get<2>(o.tuple), nullptr);
		
		EXPECT_EQ(m_ObjectPool->GetSpareBytes(), allBytes);
		EXPECT_EQ(m_ObjectPool->GetNMemoryBlocks(), 1);
    }

	/**
	* @brief Testing Spices::ObjectPool::Compare.
	*/
	TEST_F(ObjectPool_test, CompareWithOrigin) {

		SPICESTEST_PROFILE_FUNCTION();

		{
			SPICESTEST_PROFILE_SCOPE("Malloc Cost.");

			for (size_t i = 0; i < n; i++)
			{
				Object* o = new Object;
				delete o;
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("ObjectPool Cost1.");

			std::vector<Object*> objects;
			objects.resize(n);
			for (size_t i = 0; i < n; i++)
			{
				Object* o = m_ObjectPool->New();
				objects[i] = o;
			}

			for (size_t i = 0; i < n; i++)
			{
				m_ObjectPool->Delete(objects[i]);
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("ObjectPool Cost2.");

			for (size_t i = 0; i < n; i++)
			{
				Object* o = m_ObjectPool->New();
				m_ObjectPool->Delete(o);
			}
		}
	}

	/**
	* @brief Testing Spices::ObjectPool::ThreadNew/ThreadDelete.
	*/
	TEST_F(ObjectPool_test, ThreadSafe) {

		SPICESTEST_PROFILE_FUNCTION();

		std::vector<Object*> objects1;
		std::vector<Object*> objects2;

		objects1.resize(n);
		objects2.resize(n);

		std::thread t1([&]() {
			for (int i = 0; i < n; i++)
			{
				objects1[i] = m_ObjectPool->ThreadNew();
			}
		});

		std::thread t2([&]() {
			for (int i = 0; i < n; i++)
			{
				objects2[i] = m_ObjectPool->ThreadNew();
			}
		});
		
		t1.join();
		t2.join();

		std::thread t3([&]() {
			for (int i = 0; i < n; i++)
			{
				m_ObjectPool->ThreadDelete(objects1[i]);
			}
		});

		std::thread t4([&]() {
			for (int i = 0; i < n; i++)
			{
				m_ObjectPool->ThreadDelete(objects2[i]);
			}
		});

		t3.join();
		t4.join();

		EXPECT_EQ(m_ObjectPool->GetNMemoryBlocks(), 2);

		size_t allBytes = Spices::MemoryLibrary::align_up<size_t>(n * sizeof(Object), 8 * 1024) * 2;
		EXPECT_EQ(m_ObjectPool->GetSpareBytes(), allBytes);
	}

	/**
	* @brief Testing Spices::ObjectPool::AllocAfterDeAlloc.
	*/
	TEST_F(ObjectPool_test, AllocAfterDeAlloc) {

		SPICESTEST_PROFILE_FUNCTION();

		std::vector<Object*> objects;
		objects.resize(n);

		for (int j = 0; j < 5; j++)
		{
			for (size_t i = 0; i < n; i++)
			{
				Object* o = m_ObjectPool->New();
				objects[i] = o;
			}

			for (size_t i = 0; i < n; i++)
			{
				m_ObjectPool->Delete(objects[i]);
			}
		}
	}

	/**
	* @brief Testing Spices::ObjectPool::ThreadAllocAfterDeAlloc.
	*/
	TEST_F(ObjectPool_test, ThreadAllocAfterDeAlloc) {

		SPICESTEST_PROFILE_FUNCTION();

		std::vector<Object*> objects1;
		std::vector<Object*> objects2;

		objects1.resize(n);
		objects2.resize(n);

		auto internalThreadNew = [&](std::vector<Object*>& objects) {
			for (int i = 0; i < n; i++)
			{
				objects[i] = m_ObjectPool->ThreadNew();
			}
		};

		auto internalThreadDelete = [&](std::vector<Object*>& objects) {
			for (int i = 0; i < n; i++)
			{
				m_ObjectPool->ThreadDelete(objects[i]);
			}
		};

		for (int k = 0; k < 3; k++)
		{
			std::thread t1([&]() { 
				for (int j = 0; j < 3; j++)
				{
					internalThreadNew(objects1);
					internalThreadDelete(objects1);
				}
			});

			std::thread t2([&]() {
				for (int j = 0; j < 3; j++)
				{
					internalThreadNew(objects2);
					internalThreadDelete(objects2);
				}
			});
		
			t1.join();
			t2.join();
		}
	}
}