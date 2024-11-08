/**
* @file MemoryPool_test.h.
* @brief The MemoryPool_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Memory/MemoryPool.h>
#include "Instrumentor.h"

namespace SpicesTest {

	class MemoryPoolTest
	{
	public:

		MemoryPoolTest()
			: m_Tuple{ 1, 2.0f, nullptr }
		{}

		MemoryPoolTest(const MemoryPoolTest&) = delete;
		MemoryPoolTest& operator=(const MemoryPoolTest&) = delete;

		std::tuple<int, float, void*> m_Tuple;
	};

	class MemoryPoolTest2
	{
	public:

		MemoryPoolTest2() = default;
		MemoryPoolTest2(const MemoryPoolTest2&) = delete;
		MemoryPoolTest2& operator=(const MemoryPoolTest2&) = delete;

		static constexpr size_t x = 1024;
		static constexpr size_t y = 1024;
		std::array<std::array<int, x>, y> datas;  // 4M
	};

	/**
	* @brief The interface is inherited from testing::Test.
	* Registry on Initialize.
	*/
	class MemoryPool_test : public testing::Test
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
		* @brief Iter counts.
		*/
		static constexpr size_t n = 10000;

		/**
		* @brief Iter counts 2.
		*/
		static constexpr size_t n2 = 256;
	};

	/**
	* @brief Testing Spices::MemoryPool::PointerSpace.
	*/
	TEST_F(MemoryPool_test, PointerSpace) {

		SPICESTEST_PROFILE_FUNCTION();

		uint64_t a = 1;
		uint64_t b = 10;
		uint64_t c = 100;

		Spices::MemoryPool::PointerSpace(&a) = &b;
		uint64_t* d0 = reinterpret_cast<uint64_t*>(a);
		EXPECT_EQ(d0, &b);
		EXPECT_EQ(*d0, b);

		Spices::MemoryPool::PointerSpace(&a) = &c;
		uint64_t* d1 = reinterpret_cast<uint64_t*>(a);
		EXPECT_EQ(d1, &c);
		EXPECT_EQ(*d1, c);

		Spices::MemoryPool::PointerSpace(&a) = nullptr;
		uint64_t* d2 = reinterpret_cast<uint64_t*>(a);
		EXPECT_EQ(d2, nullptr);
	}

	/**
	* @brief Testing Spices::MemoryPool::AlignUp.
	*/
	TEST_F(MemoryPool_test, AlignUp) {

		SPICESTEST_PROFILE_FUNCTION();

		EXPECT_EQ(Spices::MemoryPool::AlignUp(1), 8);
		EXPECT_EQ(Spices::MemoryPool::AlignUp(127), 128);

		EXPECT_EQ(Spices::MemoryPool::AlignUp(129), 144);
		EXPECT_EQ(Spices::MemoryPool::AlignUp(1023), 1024);

		EXPECT_EQ(Spices::MemoryPool::AlignUp(1025), 1152);
		EXPECT_EQ(Spices::MemoryPool::AlignUp(8 * 1024 - 1), 8 * 1024);

		EXPECT_EQ(Spices::MemoryPool::AlignUp(8 * 1024 + 1), 9 * 1024);
		EXPECT_EQ(Spices::MemoryPool::AlignUp(64 * 1024 - 1), 64 * 1024);

		EXPECT_EQ(Spices::MemoryPool::AlignUp(64 * 1024 + 1), 72 * 1024);
		EXPECT_EQ(Spices::MemoryPool::AlignUp(256 * 1024 - 1), 256 * 1024);

		EXPECT_EQ(Spices::MemoryPool::AlignUp(257 * 1024 + 1), 264 * 1024);

		EXPECT_EQ(Spices::MemoryPool::AlignUp(1024 * 1024 + 1), 1032 * 1024);
	}

	/**
	* @brief Testing Spices::MemoryPool::Index.
	*/
	TEST_F(MemoryPool_test, Index) {

		SPICESTEST_PROFILE_FUNCTION();

		EXPECT_EQ(Spices::MemoryPool::Index(1), 0);
		EXPECT_EQ(Spices::MemoryPool::Index(16), 1);
		EXPECT_EQ(Spices::MemoryPool::Index(128), 15);
		EXPECT_EQ(Spices::MemoryPool::Index(256 * 1024), 207);
	}

	/**
	* @brief Testing Spices::MemoryPool::Alloc/Free.
	*/
	TEST_F(MemoryPool_test, AllocFree) {

		SPICESTEST_PROFILE_FUNCTION();

		std::array<MemoryPoolTest*, n> objects;
		for (size_t i = 0; i < n; i++)
		{
			MemoryPoolTest* a = new(Spices::MemoryPool::Alloc(sizeof(MemoryPoolTest)))MemoryPoolTest;

			EXPECT_EQ(std::get<0>(a->m_Tuple), 1.0f);
			EXPECT_EQ(std::get<1>(a->m_Tuple), 2);
			EXPECT_EQ(std::get<2>(a->m_Tuple), nullptr);

			objects[i] = a;
		}

		std::array<MemoryPoolTest2*, n2> object2s;
		for (size_t i = 0; i < n2; i++)
		{
			MemoryPoolTest2* b = new(Spices::MemoryPool::Alloc(sizeof(MemoryPoolTest2)))MemoryPoolTest2;

			object2s[i] = b;
		}

		for (size_t i = 0; i < n; i++)
		{
			Spices::MemoryPool::Free(objects[i]);
		}

		for (size_t i = 0; i < n2; i++)
		{
			Spices::MemoryPool::Free(object2s[i]);
		}
	}

	/**
	* @brief Testing Spices::MemoryPool Performance.
	*/
	TEST_F(MemoryPool_test, Performance_FixedSize_OneThread) {

		SPICESTEST_PROFILE_FUNCTION();

		static constexpr int nThread = 1;
		static constexpr int nCount = 500000;

		{
			SPICESTEST_PROFILE_SCOPE("new / delete");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<MemoryPoolTest*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = new MemoryPoolTest;

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = objects[j];
						delete p;
					}
				});

				threads.push_back(std::move(t1));
			}

			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("malloc / free");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<MemoryPoolTest*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = static_cast<MemoryPoolTest*>(malloc(4 * sizeof(MemoryPoolTest)));
						new(p)MemoryPoolTest;

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = objects[j];

						p->~MemoryPoolTest();
						free(p);
					}
				});

				threads.push_back(std::move(t1));
			}

			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("MemoryPool::Alloc / MemoryPool::Free");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<MemoryPoolTest*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = static_cast<MemoryPoolTest*>(Spices::MemoryPool::Alloc(4 * sizeof(MemoryPoolTest)));
						new(p)MemoryPoolTest;

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = objects[j];
					
						p->~MemoryPoolTest();
						Spices::MemoryPool::Free(p);
					}
				});
			
				threads.push_back(std::move(t1));
			}
			
			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}
	}

	/**
	* @brief Testing Spices::MemoryPool Performance.
	*/
	TEST_F(MemoryPool_test, Performance_FixedSize_MultiThread) {

		SPICESTEST_PROFILE_FUNCTION();

		static constexpr int nThread = 10;
		static constexpr int nCount = 50000;

		{
			SPICESTEST_PROFILE_SCOPE("new / delete");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<MemoryPoolTest*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = new MemoryPoolTest;

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = objects[j];
						delete p;
					}
				});

				threads.push_back(std::move(t1));
			}

			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("malloc / free");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<MemoryPoolTest*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = static_cast<MemoryPoolTest*>(malloc(4 * sizeof(MemoryPoolTest)));
						new(p)MemoryPoolTest;

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = objects[j];

						p->~MemoryPoolTest();
						free(p);
					}
				});

				threads.push_back(std::move(t1));
			}

			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("MemoryPool::Alloc / MemoryPool::Free");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<MemoryPoolTest*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = static_cast<MemoryPoolTest*>(Spices::MemoryPool::Alloc(4 * sizeof(MemoryPoolTest)));
						new(p)MemoryPoolTest;

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						MemoryPoolTest* p = objects[j];
					
						p->~MemoryPoolTest();
						Spices::MemoryPool::Free(p);
					}
				});
			
				threads.push_back(std::move(t1));
			}
			
			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}
	}

	/**
	* @brief Testing Spices::MemoryPool Performance.
	*/
	TEST_F(MemoryPool_test, Performance_RandomSize_OneThread) {

		SPICESTEST_PROFILE_FUNCTION();

		static constexpr int nThread  = 1;
		static constexpr int nCount   = 500000;
		static constexpr int maxBytes = 1024;

		{
			SPICESTEST_PROFILE_SCOPE("malloc / free");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<void*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						const size_t bytes = maxBytes * std::rand() / float(RAND_MAX);
						void* p = malloc(bytes);

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						void* p = objects[j];

						free(p);
					}
				});

				threads.push_back(std::move(t1));
			}

			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("MemoryPool::Alloc / MemoryPool::Free");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<void*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						size_t bytes = maxBytes * std::rand() / float(RAND_MAX);
						bytes = std::max((size_t)8, bytes);
						void* p = Spices::MemoryPool::Alloc(bytes);

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						void* p = objects[j];

						Spices::MemoryPool::Free(p);
					}
				});
			
				threads.push_back(std::move(t1));
			}
			
			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}
	}

	/**
	* @brief Testing Spices::MemoryPool Performance.
	*/
	TEST_F(MemoryPool_test, Performance_RandomSize_MultiThread) {

		SPICESTEST_PROFILE_FUNCTION();

		static constexpr int nThread  = 10;
		static constexpr int nCount   = 50000;
		static constexpr int maxBytes = 1024;

		{
			SPICESTEST_PROFILE_SCOPE("malloc / free");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<void*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						const size_t bytes = maxBytes * std::rand() / float(RAND_MAX);
						void* p = malloc(bytes);

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						void* p = objects[j];

						free(p);
					}
				});

				threads.push_back(std::move(t1));
			}

			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}

		{
			SPICESTEST_PROFILE_SCOPE("MemoryPool::Alloc / MemoryPool::Free");

			std::vector<std::thread> threads;
			for (int i = 0; i < nThread; i++)
			{
				std::thread t1([&]() {
					std::vector<void*> objects;
					objects.resize(nCount);

					for (int j = 0; j < nCount; j++)
					{
						size_t bytes = maxBytes * std::rand() / float(RAND_MAX);
						bytes = std::max((size_t)8, bytes);
						void* p = Spices::MemoryPool::Alloc(bytes);

						objects[j] = std::move(p);
					}

					for (int j = 0; j < nCount; j++)
					{
						void* p = objects[j];
					
						Spices::MemoryPool::Free(p);
					}
				});
			
				threads.push_back(std::move(t1));
			}
			
			for (int i = 0; i < nThread; i++)
			{
				threads[i].join();
			}
		}
	}
}