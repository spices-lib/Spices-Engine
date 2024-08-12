/**
* @file ThreadPoolFixed_test.h.
* @brief The ThreadPoolFixed_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Core/Thread/ThreadPool.h>

namespace SpicesTest {

	/**
	* @brief Test Function Class.
	*/
	class ThreadPoolFuncTest
	{
	public:

		/**
		* @brief Basic Override Class Function.
		* @return Returns true.
		*/
		bool Test() { return true; }

		/**
		* @brief Basic Override Class Function.
		* @param[in] str In String.
		* @return Returns In String.
		*/
		std::string Test(std::string str) { return str; }

		/**
		* @brief Basic Class Function.
		* @param[in] a In a.
		* @param[in] b In b.
		* @return Returns a + b.
		*/
		int Test0(int a, int b) { return a + b; }

		/**
		* @brief Static Class Function.
		* @return Returns false.
		*/
		static bool Test1() { return false; }
	};

	/**
	* @brief Template Function.
	* @tparam RType Return type.
	* @tparam Args any type.
	*/
	template<typename RType, typename ...Args>
	RType TestT(Args&& ...args)
	{
		return RType();
	}

	/**
	* @brief The interface is inherited from testing::Test.
	* Registy on Initialize.
	*/
	class ThreadPoolFixed_test : public testing::Test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registry on Initialize.
		*/
		void SetUp() override {
			m_ThreadPool.SetMode(Spices::PoolMode::MODE_FIXED);
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
		int nThreads = std::thread::hardware_concurrency();
	};

	/**
	* @brief Testing if initialize successfully.
	*/
	TEST_F(ThreadPoolFixed_test, Initialize) {
		std::cout << "Hardware threads: " << nThreads << std::endl;

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit different type of function successfully.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitTask_Range_FunctionType) {
		ThreadPoolFuncTest funcTestClass;
		
		std::future<bool>         future0  = m_ThreadPool.SubmitPoolTask(std::bind((bool(ThreadPoolFuncTest::*)())&ThreadPoolFuncTest::Test, &funcTestClass));                            /* @brief Override Class Function. */
		std::future<std::string>  future1  = m_ThreadPool.SubmitPoolTask(std::bind((std::string(ThreadPoolFuncTest::*)(std::string))&ThreadPoolFuncTest::Test, &funcTestClass, "Hello")); /* @brief Override Class Function. */
		std::future<int>          future2  = m_ThreadPool.SubmitPoolTask(std::bind(&ThreadPoolFuncTest::Test0, &funcTestClass, 1, 2));                                                    /* @brief Class Function.          */
		std::future<bool>         future3  = m_ThreadPool.SubmitPoolTask(std::bind(&ThreadPoolFuncTest::Test1));                                                                          /* @brief Static Class Function.   */
		std::future<float>        future4  = m_ThreadPool.SubmitPoolTask(std::bind(&TestT<float, int&, std::string&&>, 2, ""));                                                           /* @brief Template Function.       */

		std::future<bool>         future5  = m_ThreadPool.SubmitPoolTask([&]() { return funcTestClass.Test(); });                                                                         /* @brief Lambda Override Class Function. */
		std::future<std::string>  future6  = m_ThreadPool.SubmitPoolTask([&]() { return funcTestClass.Test("Hello"); });																  /* @brief Lambda Override Class Function. */
		std::future<int>          future7  = m_ThreadPool.SubmitPoolTask([&]() { return funcTestClass.Test0(1, 2); });																	  /* @brief Lambda Class Function.          */
		std::future<bool>         future8  = m_ThreadPool.SubmitPoolTask([&]() { return ThreadPoolFuncTest::Test1(); });																  /* @brief Lambda Static Class Function.   */
		std::future<float>        future9  = m_ThreadPool.SubmitPoolTask([&]() { return TestT<float, int, std::string>(2, ""); });														  /* @brief Lambda Template Function.       */

		std::future<bool>         future10 = m_ThreadPool.SubmitPoolTask([](bool val) { return val; }, true);                                                                             /* @brief Lambda Function.         */


		EXPECT_EQ(future0.get(), true    );
		EXPECT_EQ(future1.get(), "Hello" );
		EXPECT_EQ(future2.get(), 3       );
		EXPECT_EQ(future3.get(), false   );
		EXPECT_EQ(future4.get(), 0.0f    );

		EXPECT_EQ(future5.get(), true    );
		EXPECT_EQ(future6.get(), "Hello" );
		EXPECT_EQ(future7.get(), 3       );
		EXPECT_EQ(future8.get(), false   );
		EXPECT_EQ(future9.get(), 0.0f    );

		EXPECT_EQ(future10.get(), true   );
	}

	/**
	* @brief Testing if submit task get correct blocked result successfully.
	* With profermance test.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitTask_ReturnVal) {
		auto func = [](uint32_t a, uint32_t b) -> uint32_t
		{
			uint32_t val = 0;
			for (uint32_t i = a; i < b; i++)
			{
				val += i;
			}
			return val;
		};

		auto inTime = std::chrono::high_resolution_clock::now();

		std::future<uint32_t> future0 = m_ThreadPool.SubmitPoolTask(std::bind(func, 0        , 100000000));
		std::future<uint32_t> future1 = m_ThreadPool.SubmitPoolTask(std::bind(func, 100000000, 200000000));
		std::future<uint32_t> future2 = m_ThreadPool.SubmitPoolTask(std::bind(func, 200000000, 300000000));
		std::future<uint32_t> future3 = m_ThreadPool.SubmitPoolTask(std::bind(func, 300000000, 400000000));
		std::future<uint32_t> future4 = m_ThreadPool.SubmitPoolTask(std::bind(func, 400000000, 500000000));

		uint32_t subThreadCalVal = future0.get() + future1.get() + future2.get() + future3.get() + future4.get();

		auto multiThreadTime = std::chrono::high_resolution_clock::now();

		std::cout << "Calculate with multiple thread cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(multiThreadTime - inTime).count() << std::endl;

		inTime = std::chrono::high_resolution_clock::now();

		uint32_t mainThreadCalVal = func(0, 500000000);

		multiThreadTime = std::chrono::high_resolution_clock::now();

		std::cout << "Calculate with main thread cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(multiThreadTime - inTime).count() << std::endl;

		EXPECT_EQ(mainThreadCalVal, subThreadCalVal);
	}

	/**
	* @brief Testing if submit one pool task successfully.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitOnePoolTask) {
		auto func = [](int sec) -> bool
		{
			std::this_thread::sleep_for(std::chrono::seconds(sec));
			return true;
		};

		std::future<bool> future = m_ThreadPool.SubmitPoolTask(std::bind(func, 2));

		func(1); /* @brief Wait for task execute. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads - 1                 );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		m_ThreadPool.Wait(); /* @brief Wait for sub thread finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit nThreads pool task successfully.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitnThreadsPoolTask) {
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
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for sub thread finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit 2 nThreads pool task successfully.
	*/
	TEST_F(ThreadPoolFixed_test, Submit2nThreadsPoolTask) {
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
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		func(2);  /* @brief Wait for part tasks finish. */

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,0                            );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		/* @brief Wait for subthread finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit very much pool task successfully.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitVeryMuchPoolTask) {
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

		/* @brief Wait for subthread finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );
	}

	/**
	* @brief Testing if submit one thread task successfully.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitOneThreadTask) {
		auto func = []() -> void
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
		};

		m_ThreadPool.SubmitThreadTask_LightWeight(0, func);

		/* @brief Wait for all tasks finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(),0);
		}
	}

	/**
	* @brief Testing if submit thread task for each successfully.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitAllThreadTask) {
		auto func = []() -> void
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
		};

		m_ThreadPool.SubmitThreadTask_LightWeight_ForEach(func);

		/* @brief Wait for all tasks finish. */
		m_ThreadPool.Wait();

		EXPECT_EQ(m_ThreadPool.GetInitThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(),0);
		}
	}

	/**
	* @brief Testing if submit thread task for each twice successfully.
	*/
	TEST_F(ThreadPoolFixed_test, Submit2AllThreadTask) {
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
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(),0);
		}
	}

	/**
	* @brief Testing if submit very much thread task successfully.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitVeryMuchThreadTask) {
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
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(),0);
		}
	}

	/**
	* @brief Testing if submit very much mixed task successfully.
	*/
	TEST_F(ThreadPoolFixed_test, SubmitVeryMuchAnyTask) {
		auto func = []() -> void
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
		EXPECT_EQ(m_ThreadPool.GetIdleThreadSize()   ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetPoolMode()         ,Spices::PoolMode::MODE_FIXED );
		EXPECT_EQ(m_ThreadPool.GetTasks()            ,0                            );
		EXPECT_EQ(m_ThreadPool.GetThreadsCount()     ,nThreads                     );
		EXPECT_EQ(m_ThreadPool.GetThreadIdleTimeOut(),Spices::THREAD_MAX_IDLE_TIME );
		EXPECT_EQ(m_ThreadPool.IsPoolRunning()       ,true                         );

		for (auto& pair : m_ThreadPool.GetThreads())
		{
			EXPECT_EQ(pair.second->GetThreadTasksCount(),0);
		}
	}
}