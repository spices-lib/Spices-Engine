#pragma once
#include "Core/Core.h"
#include "ThreadPool.h"
#include "DelayThreadPool.h"
#include "Render/Vulkan/VulkanCmdThreadPool.h"

namespace Spices {

	enum class ThreadPoolEnum
	{
		Game,
		RHI,
		Custom,
	};

	class ThrealModel
	{
	public:
		ThrealModel();
		virtual ~ThrealModel() = default;

		static std::shared_ptr<ThrealModel> Get();

		void InitCustomThreadPool();
		void InitGameThreadPool();

		void InitRHIThreadPool(std::function<void(std::shared_ptr<VulkanCmdThreadPool>& ptr)> fn);

		void ShutDownCustomThreadPool();
		void ShutDownGameThreadPool();
		void ShutDownRHIThreadPool();

		std::shared_ptr<ThreadPool> GetCustomThreadPool() { return m_CustomThreadPool; }
		std::shared_ptr<DelayThreadPool> GetGameThreadPool() { return m_GameThreadPool; }
		std::shared_ptr<VulkanCmdThreadPool> GetRHIThreadPool() { return m_RHIThreadPool; }

	private:

		static std::shared_ptr<ThrealModel> m_ThrealModel;

		std::shared_ptr<ThreadPool> m_CustomThreadPool;
		std::shared_ptr<DelayThreadPool> m_GameThreadPool;
		std::shared_ptr<VulkanCmdThreadPool> m_RHIThreadPool;
	};

	template<typename F, typename ...Args>
	static auto AnyscTask(ThreadPoolEnum pool, F&& func, Args&&... args) -> std::future<decltype(func(std::forward<Args>(args)...))>
	{
		if (pool == ThreadPoolEnum::RHI)
		{
			SPICES_CORE_ERROR("RHI task shouble be submit by AnyscRHITask()")
			return std::future<decltype(func(std::forward<Args>(args)...))>();
		}

		switch (pool)
		{
		case Spices::ThreadPoolEnum::Custom:
			return ThrealModel::Get()->GetCustomThreadPool()->SubmitPoolTask(func, std::forward<Args>(args)...);
		case Spices::ThreadPoolEnum::Game:
			return ThrealModel::Get()->GetGameThreadPool()->SubmitPoolTask(func, std::forward<Args>(args)...);
		}
	}

	template<typename F, typename ...Args>
	static auto AnyscRHITask(ThreadPoolEnum pool, F&& func, Args&&... args) -> std::future<decltype(func(nullptr, std::forward<Args>(args)...))>
	{
		return ThrealModel::Get()->GetRHIThreadPool()->SubmitPoolTask(func, std::forward<Args>(args)...);
	}
}