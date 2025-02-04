/**
* @file ThreadModel.cpp
* @brief The ThreadModel Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ThreadModel.h"

namespace Spices {

	std::shared_ptr<ThreadModel> ThreadModel::m_ThreadModel = nullptr;
		
	ThreadModel::ThreadModel()
		: m_CustomThreadPool(nullptr)
		, m_GameThreadPool(nullptr)
		, m_RHIThreadPool(nullptr)
	{}

	std::shared_ptr<ThreadModel> ThreadModel::Get()
	{
		SPICES_PROFILE_ZONE;

		if (!m_ThreadModel)
		{
			m_ThreadModel = std::make_shared<ThreadModel>();
		}

		return m_ThreadModel;
	}

	void ThreadModel::InitCustomThreadPool()
	{
		SPICES_PROFILE_ZONE;

		if (!m_CustomThreadPool)
		{
			m_CustomThreadPool = std::make_shared<ThreadPool>("CusT");
			m_CustomThreadPool->SetMode(PoolMode::MODE_CACHED);
			m_CustomThreadPool->Start(8);
		}
	}

	void ThreadModel::InitGameThreadPool()
	{
		SPICES_PROFILE_ZONE;

		if (!m_GameThreadPool)
		{
			m_GameThreadPool = std::make_shared<ThreadPool>("GameT");
			m_GameThreadPool->SetMode(PoolMode::MODE_FIXED);
			m_GameThreadPool->Start(4);
			m_GameThreadPool->Suspend();
		}
	}

	void ThreadModel::InitRHIThreadPool(std::function<void(std::shared_ptr<VulkanCmdThreadPool>& ptr)> fn)
	{
		SPICES_PROFILE_ZONE;

		if (!m_RHIThreadPool)
		{
			fn(m_RHIThreadPool);
		}
	}

	void ThreadModel::ShutDownCustomThreadPool()
	{
		SPICES_PROFILE_ZONE;

		m_CustomThreadPool = nullptr;
	}

	void ThreadModel::ShutDownGameThreadPool()
	{
		SPICES_PROFILE_ZONE;

		m_GameThreadPool = nullptr;
	}

	void ThreadModel::ShutDownRHIThreadPool()
	{
		SPICES_PROFILE_ZONE;

		m_RHIThreadPool = nullptr;
	}

	void ThreadModel::ClearMainThreadTaskQueue()
	{
		SPICES_PROFILE_ZONE;

		m_MainThreadTasks.Clear();
	}

	void ThreadModel::ShutDownThreadModel()
	{
		SPICES_PROFILE_ZONE;

		ShutDownRHIThreadPool();
		ShutDownGameThreadPool();
		ShutDownCustomThreadPool();
		ClearMainThreadTaskQueue();
	}
}