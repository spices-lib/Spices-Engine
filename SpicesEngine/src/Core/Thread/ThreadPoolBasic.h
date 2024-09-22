/**
* @file ThreadPool.h
* @brief The ThreadPool_Basic Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

#include <future>
#include <queue>

namespace Spices {

	const uint32_t THREAD_MAX_THRESHHOLD = std::thread::hardware_concurrency();
	const uint32_t THREAD_MAX_IDLE_TIME  = 60;

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
		* @note This Class not allowed copy behaver.
		*/
		Thread(const Thread&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
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
		* @brief Recevice a task must execute by this thread.
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
		void Wait();

		/**
		* @brief Set this Thread is in task or not.
		*/
		void SetThreadInTask(bool isInTask);

		/**
		* @brief Get this Thread is in task or not.
		* @return Returns true if in task.
		*/
		bool GetThreadInTask() { return m_IsInTask.load(); }

	public:

		/**
		* @brief Get Thread Tasks Count.
		* @reutrn Returns the Thread Tasks Count.
		*/
		int GetThreadTasksCount() { return m_ThreadTasks.load(); }

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
	* @brief ThrealPool Run Mode
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
		*/
		ThreadPool_Basic();

		/**
		* @brief Destructor Function.
		*/
		virtual ~ThreadPool_Basic();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		ThreadPool_Basic(const ThreadPool_Basic&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
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
		* @brief Submit a task to specific thread.
		*/
		void SubmitThreadTask_LightWeight(uint32_t threadId, std::function<void(Params...)> func);

		/**
		* @brief Submit a task to all thread.
		*/
		void SubmitThreadTask_LightWeight_ForEach(std::function<void(Params...)> func);

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
		* @brief Thread Pool Run Mode.
		*/
		PoolMode m_PoolMode;

		/**
		* @brief True if this thread pool is in use.
		*/
		std::atomic_bool m_IsPoolRunning;
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
	inline void Thread<Params...>::Wait()
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
	inline ThreadPool_Basic<Params...>::ThreadPool_Basic()
		: m_InitThreadSize(0)
		, m_IdleThreadSize(0)
		, m_PoolMode(PoolMode::MODE_FIXED)
		, m_IsPoolRunning(false)
		, m_ThreadIdleTimeOut(THREAD_MAX_IDLE_TIME)
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

		while (!(m_IdleThreadSize.load() == m_NThreads.load() && m_Tasks.load() == 0)) {}

		for (auto& pair : m_Threads)
		{
			pair.second->Wait();
		}
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
}