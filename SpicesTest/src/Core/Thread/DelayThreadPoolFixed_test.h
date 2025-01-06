/**
* @file DelayThreadPoolFixed_test.h.
* @brief The DelayThreadPoolFixed_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Thread/DelayThreadPool.h>
#include "Instrumentor.h"

namespace SpicesTest {

	/**
	* @brief Testing Continue/Suspand API.
	*/
	TEST(DelayThreadPoolFixed_test, ContinueSuspand) {

		SPICESTEST_PROFILE_FUNCTION();

		Spices::DelayThreadPool threadPool;
		threadPool.SetMode(Spices::PoolMode::MODE_FIXED);
		threadPool.Start(4);
		threadPool.Suspend();

		std::atomic_int executeCount;

		{
			SPICESTEST_PROFILE_SCOPE("Quick Task");

			for (int i = 0; i < 10; i++)
			{
				threadPool.SubmitPoolTask([&]() {
					++executeCount;
				});
			}

			EXPECT_EQ(threadPool.GetIdleThreadSize(), 4);
			EXPECT_EQ(threadPool.GetTasks(), 10);
			
			threadPool.Continue();
			
			threadPool.Wait();
			threadPool.Suspend();

			EXPECT_EQ(threadPool.GetIdleThreadSize(), 4);
			EXPECT_EQ(threadPool.GetTasks(), 0);
			EXPECT_EQ(executeCount.load(), 10);
		}
		
		{
			SPICESTEST_PROFILE_SCOPE("Slow Task");

			for (int i = 0; i < 10; i++)
			{
				threadPool.SubmitPoolTask([&]() {
					std::this_thread::sleep_for(std::chrono::seconds(2));
					++executeCount;
				});
			}

			EXPECT_EQ(threadPool.GetIdleThreadSize(), 4);
			EXPECT_EQ(threadPool.GetTasks(), 10);

			threadPool.Continue();
			std::this_thread::sleep_for(std::chrono::seconds(1));
			threadPool.Suspend();
			std::this_thread::sleep_for(std::chrono::seconds(2));

			EXPECT_EQ(threadPool.GetIdleThreadSize(), 4);
			EXPECT_EQ(threadPool.GetTasks(), 6);
			EXPECT_EQ(executeCount.load(), 14);

			threadPool.Continue();
			threadPool.Wait();

			EXPECT_EQ(threadPool.GetIdleThreadSize(), 4);
			EXPECT_EQ(threadPool.GetTasks(), 0);
			EXPECT_EQ(executeCount.load(), 20);
		}
	}
}