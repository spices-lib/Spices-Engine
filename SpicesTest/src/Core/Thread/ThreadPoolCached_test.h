/**
* @file ThreadPoolCached_test.h.
* @brief The ThreadPoolCached_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Thread/ThreadPool.h>
#include "Instrumentor.h"

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
			m_ThreadPool.SetThreadIdleTimeOut(10);
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

		SPICESTEST_PROFILE_FUNCTION();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit one pool task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitOnePoolTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
			return true;
		};

		std::future<bool> future = m_ThreadPool.SubmitPoolTask(std::bind(func, 2));

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads - 1                 );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		future.get(); /* @brief Wait for sub thread finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit nThreads pool task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitnThreadsPoolTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
			return true;
		};

		std::vector<std::future<bool>> futures(nThreads);
		for (int i = 0; i < nThreads; i++)
		{
			futures[i] = m_ThreadPool.SubmitPoolTask(std::bind(func, 2));
		}

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,0                            );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for sub thread finish. */
		for (int i = 0; i < nThreads; i++)
		{
			futures[i].get();
		}

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit 2 nThreads pool task successfully.
	*/
	TEST_F(ThreadPoolCached_test, Submit2nThreadsPoolTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
			return true;
		};

		std::vector<std::future<bool>> futures(2 * nThreads);
		for (int i = 0; i < 2 * nThreads; i++)
		{
			futures[i] = m_ThreadPool.SubmitPoolTask(std::bind(func, 2));
		}

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,0                            );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for part tasks finish. */
		for (int i = 0; i < 2 * nThreads; i++)
		{
			futures[i].get();
		}

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,2 * nThreads                 );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for threads recovery finish. */
		func(25);

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit very much pool task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitVeryMuchPoolTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(sec));
			return true;
		};

		std::vector<std::future<bool>> futures(10000);
		for (int i = 0; i < 10000; i++)
		{
			futures[i] = m_ThreadPool.SubmitPoolTask(std::bind(func, 1));
		}

		func(2000);  /* @brief Wait for part tasks finish. */

		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,Spices::THREAD_MAX_THRESHHOLD);

		/* @brief Wait for all tasks finish. */
		for (int i = 0; i < 10000; i++)
		{
			futures[i].get(); 
		}

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,Spices::THREAD_MAX_THRESHHOLD);
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,Spices::THREAD_MAX_THRESHHOLD);
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for all threads recovery. */
		auto inTime = std::chrono::high_resolution_clock::now();
		while (m_ThreadPool.GetThreadsCount() != nThreads) 
		{
			func(1000);
		}
		auto outTime = std::chrono::high_resolution_clock::now();

		std::cout << "Cache Mode recovery threads cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(outTime - inTime).count() << std::endl;

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit one thread task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitOneThreadTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = []() -> void
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
		};

		m_ThreadPool.SubmitThreadTask_LightWeight(0, func);

		/* @brief Wait for all tasks finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(), 0);
		}
	}

	/**
	* @brief Testing if submit thread task for each successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitAllThreadTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = []() -> void
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
		};

		m_ThreadPool.SubmitThreadTask_LightWeight_ForEach(func);

		/* @brief Wait for all tasks finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(), 0);
		}
	}

	/**
	* @brief Testing if submit thread task for each twice successfully.
	*/
	TEST_F(ThreadPoolCached_test, Submit2AllThreadTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = []() -> void
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		};

		m_ThreadPool.SubmitThreadTask_LightWeight_ForEach(func);
		m_ThreadPool.SubmitThreadTask_LightWeight_ForEach(func);

		/* @brief Wait for all tasks finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(), 0);
		}
	}

	/**
	* @brief Testing if submit very much thread task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitVeryMuchThreadTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = []() -> void
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		};

		for (int i = 0; i < 2000; i++)
		{
			m_ThreadPool.SubmitThreadTask_LightWeight_ForEach(func);
		}

		/* @brief Wait for all tasks finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(), 0);
		}
	}

	/**
	* @brief Testing if submit very much mixed task successfully.
	*/
	TEST_F(ThreadPoolCached_test, SubmitVeryMuchAnyTask) {

		SPICESTEST_PROFILE_FUNCTION();

		auto func = []() -> void
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		};
		auto func1 = [](int sec) -> void
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(sec));
		};

		for (int i = 0; i < 5000; i++)
		{
			m_ThreadPool.SubmitPoolTask(func);
		}

		for (int i = 0; i < 1000; i++)
		{
			m_ThreadPool.SubmitThreadTask_LightWeight_ForEach(func);
		}

		/* @brief Wait for all tasks finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,Spices::THREAD_MAX_THRESHHOLD);
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,Spices::THREAD_MAX_THRESHHOLD);
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(), 0);
		}

		/* @brief Wait for all threads recovery. */
		while (m_ThreadPool.GetThreadsCount() != nThreads)
		{
			func1(1000);
		}

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_CACHED);
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),10                           );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}
}