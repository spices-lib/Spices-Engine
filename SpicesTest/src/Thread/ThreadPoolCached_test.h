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
	* Registry on Initialize.
	*/
	class ThreadPoolCached_test : public testing::Test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registry on Initialize.
		*/
		void SetUp() override {
			m_ThreadPool.SetMode(Spices::PoolMode::MODE_CACHED);
			m_ThreadPool.SetThreadIdleTimeOut(5);
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
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit one task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitOneTask) {
		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
			return true;
		};

		std::future<bool> future = m_ThreadPool.SubmitTask(std::bind(func, 2));

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads - 1                 );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		future.get(); /* @brief Wait for sub thread finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit nThreads task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitnThreadsTask) {
		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
			return true;
		};

		std::vector<std::future<bool>> futures(nThreads);
		for (int i = 0; i < nThreads; i++)
		{
			futures[i] = m_ThreadPool.SubmitTask(std::bind(func, 2));
		}

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,0                            );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for sub thread finish. */
		for (int i = 0; i < nThreads; i++)
		{
			futures[i].get();
		}

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit 2 nThreads task successfully.
	*/
	TEST_F(ThreadPoolCached_test, Submit2nThreadsTask) {
		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
			return true;
		};

		std::vector<std::future<bool>> futures(2 * nThreads);
		for (int i = 0; i < 2 * nThreads; i++)
		{
			futures[i] = m_ThreadPool.SubmitTask(std::bind(func, 2));
		}

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,0                            );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for part tasks finish. */
		for (int i = 0; i < 2 * nThreads; i++)
		{
			futures[i].get();
		}

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for threads recovery finish. */
		func(15);

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit very much task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitVeryMuchTask) {
		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(sec));
			return true;
		};

		std::vector<std::future<bool>> futures(10000);
		for (int i = 0; i < 10000; i++)
		{
			futures[i] = m_ThreadPool.SubmitTask(std::bind(func, 1));
		}

		func(2000);  /* @brief Wait for part tasks finish. */

		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,Spices::THREAD_MAX_THRESHHOLD);

		/* @brief Wait for all tasks finish. */
		for (int i = 0; i < 10000; i++)
		{
			futures[i].get(); 
		}

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,Spices::THREAD_MAX_THRESHHOLD);
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,Spices::THREAD_MAX_THRESHHOLD);
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for all tasks recovery. */
		auto inTime = std::chrono::high_resolution_clock::now();
		while (m_ThreadPool.GetThreads().size() != nThreads) 
		{
			func(1000);
		}
		auto outTime = std::chrono::high_resolution_clock::now();

		std::cout << "Cache Mode recovery threads cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(outTime - inTime).count() << std::endl;

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTaskQueue().size() ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreads().size()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),5                            );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}
}