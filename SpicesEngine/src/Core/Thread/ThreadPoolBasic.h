/**
* @file ThreadPoolBasic.h
* @brief The ThreadPool_Basic Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/Library/ThreadLibrary.h"

#include <future>
#include <queue>

namespace Spices {

	const     uint32_t THREAD_MAX_THRESHHOLD = std::thread::hardware_concurrency();
	constexpr uint32_t THREAD_MAX_IDLE_TIME  = 60;

	/**
	* @brief Thread Function Object.
	*/
	template<typename ...Params>
	class Thread
	{
	public:

		/**
		* @brief Thread Task.
		*/
		using ThreadTask = std::function<void(Params...)>;

	public:

		/**
		* @brief Thread Function.
		*/
		using ThreadFunc = std::function<void(Thread*)>;

		/**
		* @brief Constructor Function.
		* @param[in] func Thread Function lambda.
		* @param[in] threadId Thread ID.
		*/
		Thread(ThreadFunc func, uint32_t threadId)
			: m_Func(func)
			, m_ThreadId(threadId)
			, m_ThreadTasks(0)
			, m_IsInTask(false)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~Thread() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		Thread(const Thread&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		Thread& operator=(const Thread&) = delete;

		/**
		* @brief Start a thread to execute Thread Function.
		*/
		void Start();

		/**
		* @brief Get Thread Id.
		*/
		uint32_t GetId() const { return m_ThreadId; }

		/**
		* @brief Receive a task must execute by this thread.
		* @param[in] func Function Pointer.
		*/
		void ReceiveThreadTask(std::function<void(Params...)> func);

		/**
		* @brief Get this mutex.
		*/
		auto RequireTask() -> ThreadTask;

		/**
		* @brief Wait for all thread tasks finished.
		*/
		void Wait() const;

		/**
		* @brief Set this Thread is in task or not.
		*/
		void SetThreadInTask(bool isInTask);

		/**
		* @brief Get this Thread is in task or not.
		* @return Returns true if in task.
		*/
		bool GetThreadInTask() const { return m_IsInTask.load(); }

	public:

		/**
		* @brief Get Thread Tasks Count.
		* @reutrn Returns the Thread Tasks Count.
		*/
		int GetThreadTasksCount() const { return m_ThreadTasks.load(); }

	private:

		/**
		* @brief A Mutex for Thread.
		*/
		std::mutex m_Mutex;

		/**
		* @brief Thread Function.
		*/
		ThreadFunc m_Func;

		/**
		* @brief Thread Id.
		*/
		uint32_t m_ThreadId;

		/**
		* @brief Thread Tasks Queue.
		*/
		std::queue<ThreadTask> m_ThreadTasksQueue;

		/**
		* @brief Thread Tasks Count.
		*/
		std::atomic_int m_ThreadTasks;

		/**
		* @brief True if this thread is executing a task.
		*/
		std::atomic_bool m_IsInTask;
	};

	/**
	* @brief ThreadPool Run Mode
	*/
	enum class PoolMode
	{
		MODE_FIXED,  /* @brief Fixed Thread Number.   */
		MODE_CACHED, /* @brief Dynamic Thread Number. */
	};

	/**
	* @brief Thread Pool Basic Class.
	* Instance inherited from it and use multithreading feature.
	*/
	template<typename... Params>
	class ThreadPool_Basic
	{
	public:

		/**
		* @brief Thread Function lambda Object.
		*/
		using Task = std::function<void(Params...)>;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] name ThreadPool Name.
		*/
		ThreadPool_Basic(const std::string& name = "NonNameT");

		/**
		* @brief Destructor Function.
		*/
		virtual ~ThreadPool_Basic();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		ThreadPool_Basic(const ThreadPool_Basic&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		ThreadPool_Basic& operator=(const ThreadPool_Basic&) = delete;
		
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
		* @brief Wait for all tasks executed finish in taskqueue.
		*/
		void Wait();

		/**
		* @brief Continue ThreadPool.
		*/
		void Continue();

		/**
		* @brief Suspend ThreadPool.
		*/
		void Suspend();

		/**
		* @brief Submit a task to specific thread.
		*/
		void SubmitThreadTask_LightWeight(uint32_t threadId, std::function<void(Params...)> func);

		/**
		* @brief Submit a task to all thread.
		*/
		void SubmitThreadTask_LightWeight_ForEach(std::function<void(Params...)> func);

		/***************************Must Implementation if Agent with Multiple Parameters*****************************/

		/**
		* @brief Thread Function.
		* @param[in] thread Thread Entity.
		*/
		void ThreadFunc(Thread<>* thread);

		/**
		* @brief Submit a task to task queue, and wait for a idle thread to execute it.
		* @tparam Func Task Function.
		* @tparam Args Task Function Parameter.
		* @return Returns task function return value as a future.
		*/
		template<typename Func, typename... Args>
		auto SubmitPoolTask(Func&& func, Args&&... args) -> std::future<decltype(func(std::forward<Args>(args)...))>;

		/**
		* @brief Start Run this thread pool.
		* @param[in] initThreadSize Thread Size.
		*/
		void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency());

		/*************************************************************************************************************/

	public:

		/**
		* @brief Get Threads Count.
		* This function is just used for unit test, should not be used in engine.
		* @return Return Threads Count.
		*/
		const int GetThreadsCount() const { return m_NThreads.load(); }

		/**
		* @brief Get Threads.
		* This function is just used for unit test, should not be used in engine.
		* @return Return Threads.
		*/
		const std::unordered_map<uint32_t, std::unique_ptr<Thread<Params...>>>& GetThreads() const { return m_Threads; }

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
		* @brief Get TaskQueue Count.
		* This function is just used for unit test, should not be used in engine.
		* @return Returns Task Queue Count.
		*/
		const int GetTasks() const { return m_Tasks.load(); }

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
		* @brief Check whether this pool is still in running.
		*/
		bool CheckRunningState() const { return m_IsPoolRunning; }

	protected:

		/**
		* @brief This ThreadPool Name.
		*/
		std::string m_PoolName;

		/**
		* @brief Threads Container.
		*/
		std::unordered_map<uint32_t, std::unique_ptr<Thread<Params...>>> m_Threads;

		/**
		* @brief Threads Count.
		*/
		std::atomic_int m_NThreads;

		/**
		* @brief Initialized thread size.
		*/
		uint32_t m_InitThreadSize;

		/**
		* @brief thread idle time out.
		*/
		uint32_t m_ThreadIdleTimeOut;

		/**
		* @brief Idled thread size.
		*/
		std::atomic_int m_IdleThreadSize;

		/**
		* @brief Task Queue.
		*/
		std::queue<Task> m_TaskQueue;

		/**
		* @brief Number of tasks;
		*/
		std::atomic_int m_Tasks;

		/**
		* @brief Mutex for thread safe.
		*/
		std::mutex m_Mutex;

		/**
		* @brief Task Queue not empty.
		*/
		std::condition_variable m_NotEmpty;

		/**
		* @brief Thread pool Exit Condition.
		*/
		std::condition_variable m_ExitCond;

		/**
		* @brief Thread pool thread idle Condition.
		*/
		std::condition_variable m_IdleCond;

		/**
		* @brief Thread Pool Run Mode.
		*/
		PoolMode m_PoolMode;

		/**
		* @brief True if this thread pool is in use.
		*/
		std::atomic_bool m_IsPoolRunning;

		/**
		* @brief True if needs suspend on executing the task.
		*/
		bool m_IsSuspend;
	};

	template<typename ...Params>
	inline void Thread<Params...>::Start()
	{
		SPICES_PROFILE_ZONE;

		std::thread t(m_Func, this);
		t.detach();
	}

	template<typename ...Params>
	inline void Thread<Params...>::ReceiveThreadTask(std::function<void(Params...)> func)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::mutex> lock(m_Mutex);

		m_ThreadTasksQueue.push(func);
		++m_ThreadTasks;
	}

	template<typename ...Params>
	inline auto Thread<Params...>::RequireTask() -> Thread<Params...>::ThreadTask
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::mutex> lock(m_Mutex);

		if (m_ThreadTasksQueue.empty())
		{
			m_IsInTask = false;
			return nullptr;
		}

		auto ptr = m_ThreadTasksQueue.front();
		m_ThreadTasksQueue.pop();
		m_IsInTask = true;
		--m_ThreadTasks;
		return ptr;
	}

	template<typename ...Params>
	inline void Thread<Params...>::Wait() const
	{
		SPICES_PROFILE_ZONE;

		while (m_ThreadTasks.load() != 0 || m_IsInTask.load()) {};
	}

	template<typename ...Params>
	inline void Thread<Params...>::SetThreadInTask(bool isInTask)
	{
		SPICES_PROFILE_ZONE;

		m_IsInTask = isInTask;
	}

	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::SetMode(PoolMode mode)
	{
		SPICES_PROFILE_ZONE;

		if (CheckRunningState()) return;
		m_PoolMode = mode;
	}

	template<typename ...Params>
	inline ThreadPool_Basic<Params...>::ThreadPool_Basic(const std::string& name)
		: m_PoolName(name)
		, m_InitThreadSize(0)
		, m_ThreadIdleTimeOut(THREAD_MAX_IDLE_TIME)
		, m_IdleThreadSize(0)
		, m_PoolMode(PoolMode::MODE_FIXED)
		, m_IsPoolRunning(false)
		, m_IsSuspend(false)
	{}

	template<typename ...Params>
	inline ThreadPool_Basic<Params...>::~ThreadPool_Basic()
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

	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::SetThreadIdleTimeOut(int idleTime)
	{
		SPICES_PROFILE_ZONE;

		if (CheckRunningState()) return;
		m_ThreadIdleTimeOut = idleTime;
	}
	
	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::Wait()
	{
		SPICES_PROFILE_ZONE;

		auto idleCond = [&]() {
			return m_IdleThreadSize.load() == m_NThreads.load() && m_Tasks.load() == 0;
		};

		if (!idleCond())
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_IdleCond.wait(lock, idleCond);
		}

		for (auto& pair : m_Threads)
		{
			pair.second->Wait();
		}
	}

	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::Continue()
	{
		SPICES_PROFILE_ZONE;

		m_IsSuspend = false;

		if (m_Tasks.load() > 0)
		{
			m_NotEmpty.notify_all();
		}
	}

	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::Suspend()
	{
		SPICES_PROFILE_ZONE;

		m_IsSuspend = true;

		std::unique_lock<std::mutex> lock(m_Mutex);

		m_IdleCond.wait(lock, [&]() { return m_IdleThreadSize.load() == m_NThreads.load(); });
	}

	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::SubmitThreadTask_LightWeight(uint32_t threadId, std::function<void(Params...)> func)
	{
		SPICES_PROFILE_ZONE;

		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			m_Threads[threadId]->ReceiveThreadTask(func);
		}

		m_NotEmpty.notify_all();
	}

	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::SubmitThreadTask_LightWeight_ForEach(std::function<void(Params...)> func)
	{
		SPICES_PROFILE_ZONE;

		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			for (auto& pair : m_Threads)
			{
				pair.second->ReceiveThreadTask(func);
			}
		}

		m_NotEmpty.notify_all();
	}

	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::ThreadFunc(Thread<>* thread)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Name thread.
		*/
		std::stringstream ss;
		ss << m_PoolName << thread->GetId();

		ThreadLibrary::SetThreadName(ss.str());

		auto lastTime = std::chrono::high_resolution_clock::now();

		for (;;)
		{
			Task task;
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				while ((m_Tasks.load() == 0 && thread->GetThreadTasksCount() == 0) || m_IsSuspend)
				{
					/**
					* @brief Exit.
					*/
					if (!m_IsPoolRunning)
					{
						m_Threads.erase(thread->GetId());
						--m_IdleThreadSize;
						m_ExitCond.notify_all();
						return;
					}

					if (m_PoolMode == PoolMode::MODE_CACHED)
					{
						if (m_NotEmpty.wait_for(lock, std::chrono::seconds(1)) == std::cv_status::timeout)
						{
							auto now = std::chrono::high_resolution_clock::now();
							auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);

							/**
							* @brief Try recovery unused threads.
							*/
							if (dur.count() >= m_ThreadIdleTimeOut && m_Threads.size() > m_InitThreadSize)
							{
								m_Threads.erase(thread->GetId());
								--m_IdleThreadSize;
								--m_NThreads;

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
				* @brief Rink First, Thread Tasks.
				*/
				task = thread->RequireTask();

				/**
				* @brief Rink Second, Pool Tasks.
				*/
				if (!task)
				{
					task = m_TaskQueue.front();
					m_TaskQueue.pop();
					thread->SetThreadInTask(true);
					--m_Tasks;
				}

				--m_IdleThreadSize;
			}

			if (m_Tasks > 0)
			{
				m_NotEmpty.notify_all();
			}

			/**
			* @brief execute task.
			*/
			if (task != nullptr)
			{
				task();
				thread->SetThreadInTask(false);
				++m_IdleThreadSize;
				m_IdleCond.notify_all();
			}
			else
			{
				thread->SetThreadInTask(false);
				++m_IdleThreadSize;
				m_IdleCond.notify_all();
			}

			lastTime = std::chrono::high_resolution_clock::now();
		}
	}

	template<typename ...Params>
	inline void ThreadPool_Basic<Params...>::Start(int initThreadSize)
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolRunning = true;
		m_InitThreadSize = initThreadSize;
		m_IdleThreadSize = initThreadSize;
		m_NThreads = initThreadSize;

		for (uint32_t i = 0; i < m_InitThreadSize; i++)
		{
			auto ptr = std::make_unique<Thread<>>(std::bind(&ThreadPool_Basic<>::ThreadFunc, this, std::placeholders::_1), i);
			int threadId = ptr->GetId();

			m_Threads.emplace(threadId, std::move(ptr));
			m_Threads[threadId]->Start();
		}
	}

	template<typename ...Params>
	template<typename Func, typename ...Args>
	inline auto ThreadPool_Basic<Params...>::SubmitPoolTask(Func&& func, Args && ...args) -> std::future<decltype(func(std::forward<Args>(args)...))>
	{
		SPICES_PROFILE_ZONE;

		using RType = decltype(func(args...));

		/**
		* @brief pack function pointer with packaged_task and get future.
		*/
		auto task = std::make_shared<std::packaged_task<RType()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
		std::future<RType> result = task->get_future();

		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			/**
			* @brief pack task as a lambda and submit it to queue.
			*/
			m_TaskQueue.emplace([task]() {(*task)(); });
			++m_Tasks;

			/**
			* @brief Expand threads container if in MODE_CACHED.
			*/
			if (m_PoolMode == PoolMode::MODE_CACHED && m_TaskQueue.size() > m_IdleThreadSize && m_Threads.size() < THREAD_MAX_THRESHHOLD)
			{
				for (uint32_t i = 0; i < THREAD_MAX_THRESHHOLD; i++)
				{
					if (m_Threads.find(i) == m_Threads.end())
					{
						auto ptr = std::make_unique<Thread<>>(std::bind(&ThreadPool_Basic::ThreadFunc, this, std::placeholders::_1), i);
						uint32_t threadId = ptr->GetId();

						ptr->Start();
						m_Threads.emplace(threadId, std::move(ptr));

						++m_IdleThreadSize;
						++m_NThreads;

						break;
					}
				}
			}
		}

		m_NotEmpty.notify_all();

		return result;
	}

	/**
	* @brief Using ThreadPool as a simple format of ThreadPool_Basic<>
	*/
	using ThreadPool = ThreadPool_Basic<>;
}