#include "Pchheader.h"
#include "ThrealModel.h"

namespace Spices {

	std::shared_ptr<ThrealModel> ThrealModel::m_ThrealModel = nullptr;
		
	ThrealModel::ThrealModel()
		: m_CustomThreadPool(nullptr)
		, m_GameThreadPool(nullptr)
		, m_RHIThreadPool(nullptr)
	{}

	std::shared_ptr<ThrealModel> ThrealModel::Get()
	{
		if (!m_ThrealModel)
		{
			m_ThrealModel = std::make_shared<ThrealModel>();
		}

		return m_ThrealModel;
	}

	void ThrealModel::InitCustomThreadPool()
	{
		SPICES_PROFILE_ZONE;

		if (!m_CustomThreadPool)
		{
			m_CustomThreadPool = std::make_shared<ThreadPool>();
			m_CustomThreadPool->SetMode(PoolMode::MODE_FIXED);
			m_CustomThreadPool->Start(4);
		}
	}

	void ThrealModel::InitGameThreadPool()
	{
		SPICES_PROFILE_ZONE;

		if (!m_GameThreadPool)
		{
			m_GameThreadPool = std::make_shared<DelayThreadPool>();
			m_GameThreadPool->SetMode(PoolMode::MODE_FIXED);
			m_GameThreadPool->Start(4);
		}
	}

	void ThrealModel::InitRHIThreadPool(std::function<void(std::shared_ptr<VulkanCmdThreadPool>& ptr)> fn)
	{
		SPICES_PROFILE_ZONE;

		if (!m_RHIThreadPool)
		{
			fn(m_RHIThreadPool);
		}
	}

	void ThrealModel::ShutDownCustomThreadPool()
	{
		SPICES_PROFILE_ZONE;

		m_CustomThreadPool = nullptr;
	}

	void ThrealModel::ShutDownGameThreadPool()
	{
		SPICES_PROFILE_ZONE;

		m_GameThreadPool = nullptr;
	}

	void ThrealModel::ShutDownRHIThreadPool()
	{
		SPICES_PROFILE_ZONE;

		m_RHIThreadPool = nullptr;
	}

}