/**
* @file ThreadPoolCached_test.h.
* @brief The ThreadPoolCached_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Thread/ThreadPool.h>

namespace SpicesTest {

	/**
	* @brief The interface is inherited from testing::Test.
	* Registy on Initialize.
	*/
	class ThreadPoolCached_test : public testing::Test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registy on Initialize.
		*/
		void SetUp() override {
			m_ThreadPool.SetMode(Spices::PoolMode::MODE_CACHED);
			m_ThreadPool.Start(nThreads);
		}

		// void TearDown() override {}

		/**
		* @brief ThreadPool.
		*/
		Spices::ThreadPool m_ThreadPool;

		/**
		* @brief Number of Threads in ThreadPool.
		*/
		const int nThreads = 4;
	};

	/**
	* @brief Testing if initialize successfully.
	*/
	TEST_F(ThreadPoolCached_test, Initialize) {
		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit one task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitOneTask) {
		auto func = [](int sec)
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
		};

		m_ThreadPool.SubmitTask(std::bind(func, 2));

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads - 1                 );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		func(2); /* @brief Wait for subthread finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit nThreads task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitnThreadsTask) {
		auto func = [](int sec)
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
		};

		for (int i = 0; i < nThreads; i++)
		{
			m_ThreadPool.SubmitTask(std::bind(func, 2));
		}

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,0                            );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		func(2);  /* @brief Wait for subthread finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit 2 nThreads task successfully.
	*/
	TEST_F(ThreadPoolCached_test, Submit2nThreadsTask) {
		auto func = [](int sec)
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
		};

		for (int i = 0; i < 2 * nThreads; i++)
		{
			m_ThreadPool.SubmitTask(std::bind(func, 2));
		}

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,0                            );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		func(2);  /* @brief Wait for part tasks finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		func(120);  /* @brief Wait for threads recovery finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit very much task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitVeryMuchTask) {
		auto func = [](int sec)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(sec));
		};

		for (int i = 0; i < 10000; i++)
		{
			m_ThreadPool.SubmitTask(std::bind(func, 1));
		}

		func(2000);  /* @brief Wait for part tasks finish. */

		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,Spices::THREAD_MAX_THRESHHOLD);

		func(120000);  /* @brief Wait for threads recovery finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}
}