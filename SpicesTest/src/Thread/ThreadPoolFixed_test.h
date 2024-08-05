/**
* @file ThreadPoolFixed_test.h.
* @brief The ThreadPoolFixed_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Thread/ThreadPool.h>

namespace SpicesTest {

	class ThreadPoolFixed_test : public testing::Test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registy on Initialize.
		*/
		void SetUp() override {
			m_ThreadPool.SetMode(Spices::ThreadPool::PoolMode::MODE_FIXED);
			m_ThreadPool.Start();
		}

		// void TearDown() override {}

		Spices::ThreadPool m_ThreadPool;
	};

	TEST_F(ThreadPoolFixed_test, Initialize) {
		int nThreads = std::thread::hardware_concurrency();
		std::cout << "Hardware threads: " << nThreads << std::endl;
		EXPECT_EQ(m_ThreadPool.GetInitThreadSize(), nThreads);
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize(), nThreads);
	}
}