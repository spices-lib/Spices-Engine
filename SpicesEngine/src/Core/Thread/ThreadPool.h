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
	class ThreadPool
	{
	public:

		/**
		* @brief Thread Function lambda Object.
		*/
		using Task = std::function<void()>;

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
		void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency());

		/**
		* @brief Get Instance of ThreadPool.
		* @return Returns Instance of ThreadPool.
		*/
		static std::shared_ptr<ThreadPool> Get() { return  m_ThreadPool; };

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

	private:
		
		/**
		* @brief Thread Function.
		* @param[in] threadid Thread id from std
		*/
		void ThreadFunc(uint32_t threadid);

		/**
		* @brief Check whether this pool is still in running.
		*/
		bool CheckRunningState() const { return m_IsPoolRunning; }

	private:

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

		/**
		* @brief Instance of ThreadPool.
		*/
		static std::shared_ptr<ThreadPool> m_ThreadPool;
	};

	template<typename Func, typename ...Args>
	inline auto ThreadPool::SubmitTask(Func&& func, Args && ...args) -> std::future<decltype(func(args ...))>
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
		m_TaskQueue.emplace([task]() {(*task)(); });
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
}