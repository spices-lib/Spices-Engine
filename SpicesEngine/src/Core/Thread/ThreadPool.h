/**
* @file ThreadPool.h
* @brief The ThreadPool Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

#include <future>
#include <queue>

namespace Spices {

	const uint32_t TASK_MAX_THRESHHOLD   = UINT32_MAX;
	const uint32_t THREAD_MAX_THRESHHOLD = std::thread::hardware_concurrency();
	const uint32_t THREAD_MAX_IDLE_TIME  = 60;
	
	/**
	* @brief Thread Function Object.
	*/
	class Thread
	{
	public:

		/**
		* @brief Thread Function.
		*/
		using ThreadFunc = std::function<void(int)>;

		/**
		* @brief Constructor Function.
		* @param[in] func Thread Function lambda.
		*/
		Thread(ThreadFunc func);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Thread() = default;

		/**
		* @brief Start a thread to execute Thread Function.
		*/
		void Start();

		/**
		* @brief Get Thread Id.
		*/
		uint32_t GetId() const { return m_ThreadId; }

	private:

		/**
		* @brief Thread Function.
		*/
		ThreadFunc m_Func;

		/**
		* @brief Thread Id Generator.
		*/
		static uint32_t m_GeneraterId;

		/**
		* @brief Thread Id.
		*/
		uint32_t m_ThreadId;
	};

	/**
	* @brief ThrealPool Run Mode
	*/
	enum class PoolMode
	{
		MODE_FIXED,  /* @brief Fixed Thread Number.   */
		MODE_CACHED, /* @brief Dynamic Thread Number. */
	};

	/**
	* @brief Thread Pool Class, 
	* Instance it and use multithreading feature.
	*/
	template<typename R, typename ...Params>
	class ThreadPool
	{
	public:

		/**
		* @brief Thread Function lambda Object.
		*/
		using Task = std::function<R(Params...)>;

	public:

		/**
		* @brief Constructor Function.
		*/
		ThreadPool();

		/**
		* @brief Destructor Function.
		*/
		virtual ~ThreadPool();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		ThreadPool(const ThreadPool&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		ThreadPool& operator=(const ThreadPool&) = delete;
		
		/**
		* @brief Set Pool Run Mode.
		* @param[in] mode Pool Run Mode.
		*/
		void SetMode(PoolMode mode);

		/**
		* @brief Set Pool Threads idle time out.
		* @param[in] idleTime .
		*/
		void SetThreadIdleTimeOut(int idleTime);

		/**
		* @brief Submit a task to task queue, and wait for a idle thread to execute it.
		* @tparam Func Task Function.
		* @tparam Args Task Funcion Parameter.
		* @return Returns task function return value as a future.
		*/
		template<typename Func, typename... Args>
		auto SubmitTask(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>;

		/**
		* @brief Start Run this thread pool.
		* @param[in] initThreadSize Thread Size.
		*/
		virtual void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency());

		/**
		* @brief Wait for all tasks executed finish in taskqueue.
		*/
		void Wait();

	public:

		/**
		* @brief GetThreads Container.
		* This function is just used for unit test, should not be used in engine.
		* @return Return Threads Container.
		*/
		const std::unordered_map<uint32_t, std::unique_ptr<Thread>>& GetThreads() const { return m_Threads; }

		/**
		* @brief GetInitThreadSize.
		* This function is just used for unit test, should not be used in engine.
		* @return Returns InitThreadSize.
		*/
		const int GetInitThreadSize() const { return m_InitThreadSize; }

		/**
		* @brief GetIdleThreadSize.
		* This function is just used for unit test, should not be used in engine.
		* @return Returns IdleThreadSize.
		*/
		const int GetIdleThreadSize() const { return m_IdleThreadSize.load(); }

		/**
		* @brief GetTaskQueue.
		* This function is just used for unit test, should not be used in engine.
		* @return Returns TaskQueue.
		*/
		const std::queue<Task>& GetTaskQueue() const { return m_TaskQueue; }

		/**
		* @brief GetPoolMode.
		* This function is just used for unit test, should not be used in engine.
		* @return Returns PoolMode.
		*/
		const PoolMode& GetPoolMode() const { return m_PoolMode; }

		/**
		* @brief ThreadIdleTimeOut.
		* This function is just used for unit test, should not be used in engine.
		* @return Returns ThreadIdleTimeOut.
		*/
		const int GetThreadIdleTimeOut() const { return m_ThreadIdleTimeOut; }

		/**
		* @brief GetIsPoolRunning.
		* This function is just used for unit test, should not be used in engine.
		* @return Returns true if pool is in use.
		*/
		const bool IsPoolRunning() const { return m_IsPoolRunning.load(); }

	protected:
		
		/**
		* @brief Thread Function.
		* @param[in] threadid Thread id from std
		*/
		virtual void ThreadFunc(uint32_t threadid);

		/**
		* @brief Check whether this pool is still in running.
		*/
		bool CheckRunningState() const { return m_IsPoolRunning; }

	protected:

		/**
		* @brief Threads Container.
		*/
		std::unordered_map<uint32_t, std::unique_ptr<Thread>> m_Threads;

		/**
		* @brief Initialized thread size.
		*/
		int m_InitThreadSize;

		/**
		* @brief thread idle time out.
		*/
		int m_ThreadIdleTimeOut;

		/**
		* @brief Idled thread size.
		*/
		std::atomic_int m_IdleThreadSize;

		/**
		* @brief Task Queue.
		*/
		std::queue<Task> m_TaskQueue;

		/**
		* @brief Mutex for thread safe.
		*/
		std::mutex m_Mutex;

		/**
		* @brief Task Queue not full.
		*/
		std::condition_variable m_NotFull;

		/**
		* @brief Task Queue not empty.
		*/
		std::condition_variable m_NotEmpty;

		/**
		* @brief Task is finished.
		*/
		std::condition_variable m_TaskFinish;

		/**
		* @brief Thread pool Exit Condition.
		*/
		std::condition_variable m_ExitCond;

		/**
		* @brief Thread Pool Run Mode.
		*/
		PoolMode m_PoolMode;

		/**
		* @brief True if this thread pool is in use.
		*/
		std::atomic_bool m_IsPoolRunning;
	};

	template<typename R, typename ...Params>
	template<typename Func, typename ...Args>
	inline auto ThreadPool<R, Params...>::SubmitTask(Func&& func, Args && ...args) -> std::future<decltype(func(args ...))>
	{
		SPICES_PROFILE_ZONE;

		using RType = decltype(func(args...));

		/**
		* @brief pack function pointer with packaged_task and get future.
		*/
		auto task = std::make_shared<std::packaged_task<RType()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
		std::future<RType> result = task->get_future();

		std::unique_lock<std::mutex> lock(m_Mutex);

		if (!m_NotFull.wait_for(lock, std::chrono::seconds(1), [&](){ return m_TaskQueue.size() < (size_t)TASK_MAX_THRESHHOLD; }))
		{
			SPICES_CORE_WARN("Task Submit failed");

			auto emptyTask = std::make_shared<std::packaged_task<RType()>>( [](){ return RType(); });
			(*emptyTask)();
			return emptyTask->get_future();
		}

		/**
		* @brief pack task as a lambda and submit it to queue.
		*/
		//m_TaskQueue.emplace([task]() {(*task)(); });
		m_TaskQueue.emplace(task);
		m_NotEmpty.notify_all();

		/**
		* @brief Expand threads container if in MODE_CACHED.
		*/
		if (m_PoolMode == PoolMode::MODE_CACHED && m_TaskQueue.size() > m_IdleThreadSize && m_Threads.size() < THREAD_MAX_THRESHHOLD)
		{
			auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::ThreadFunc, this, std::placeholders::_1));
			ptr->Start();
			uint32_t threadId = ptr->GetId();
			m_Threads.emplace(threadId, std::move(ptr));

			++m_IdleThreadSize;
		}

		return result;
	}

	template<typename R, typename ...Params>
	void ThreadPool<R, Params...>::SetMode(PoolMode mode)
	{
		SPICES_PROFILE_ZONE;

		if (CheckRunningState()) return;
		m_PoolMode = mode;
	}

	template<typename R, typename ...Params>
	ThreadPool<R, Params...>::ThreadPool()
		: m_InitThreadSize(0)
		, m_IdleThreadSize(0)
		, m_PoolMode(PoolMode::MODE_FIXED)
		, m_IsPoolRunning(false)
		, m_ThreadIdleTimeOut(THREAD_MAX_IDLE_TIME)
	{}

	template<typename R, typename ...Params>
	ThreadPool<R, Params...>::~ThreadPool()
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolRunning = false;

		/**
		* @brief Wait for all threads return.
		*/
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_NotEmpty.notify_all();
		m_ExitCond.wait(lock, [&]()->bool { return m_Threads.empty(); });
	}

	template<typename R, typename ...Params>
	void ThreadPool<R, Params...>::SetThreadIdleTimeOut(int idleTime)
	{
		SPICES_PROFILE_ZONE;

		if (CheckRunningState()) return;
		m_ThreadIdleTimeOut = idleTime;
	}

	template<typename R, typename ...Params>
	void ThreadPool<R, Params...>::Start(int initThreadSize)
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolRunning = true;
		m_InitThreadSize = initThreadSize;
		m_IdleThreadSize = initThreadSize;

		for (int i = 0; i < m_InitThreadSize; i++)
		{
			auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::ThreadFunc, this, std::placeholders::_1));
			int threadId = ptr->GetId();
			m_Threads.emplace(threadId, std::move(ptr));
			m_Threads[threadId]->Start();
		}
	}

	template<typename R, typename ...Params>
	void ThreadPool<R, Params...>::Wait()
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::mutex> lock(m_Mutex);
		m_TaskFinish.wait(lock, [&]() { return m_TaskQueue.empty(); });
	}

	template<typename R, typename ...Params>
	void ThreadPool<R, Params...>::ThreadFunc(uint32_t threadid)
	{
		SPICES_PROFILE_ZONE;

		auto lastTime = std::chrono::high_resolution_clock().now();

		while (1)
		{
			Task task;
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				while (m_TaskQueue.size() == 0)
				{
					/**
					* @brief Exit.
					*/
					if (!m_IsPoolRunning)
					{
						m_Threads.erase(threadid);
						m_ExitCond.notify_all();
						return;
					}

					if (m_PoolMode == PoolMode::MODE_CACHED)
					{
						if (m_NotEmpty.wait_for(lock, std::chrono::seconds(1)) == std::cv_status::timeout)
						{
							auto now = std::chrono::high_resolution_clock().now();
							auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);

							/**
							* @brief Try recovery unused threads.
							*/
							if (dur.count() >= m_ThreadIdleTimeOut && m_Threads.size() > m_InitThreadSize)
							{
								m_Threads.erase(threadid);
								m_IdleThreadSize--;

								return;
							}
						}
					}
					else
					{
						m_NotEmpty.wait(lock);
					}
				}

				/**
				* @brief Get a task from task queue.
				*/
				task = m_TaskQueue.front();
				m_TaskQueue.pop();

				--m_IdleThreadSize;
				m_NotFull.notify_all();
				if (m_TaskQueue.size() > 0) m_NotEmpty.notify_all();
			}

			/**
			* @brief execute task.
			*/
			if (task != nullptr)
			{
				task();
				m_TaskFinish.notify_all();
			}

			++m_IdleThreadSize;

			lastTime = std::chrono::high_resolution_clock().now();
		}
	}
}