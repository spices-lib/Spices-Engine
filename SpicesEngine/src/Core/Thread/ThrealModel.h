/**
* @file ThrealModel.h
* @brief The ThrealModel Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "ThreadPool.h"
#include "DelayThreadPool.h"
#include "Render/Vulkan/VulkanCmdThreadPool.h"

namespace Spices {

	/**
	* @brief ThreadPool Type Enum.
	*/
	enum class ThreadPoolEnum
	{
		Game    = 0,    /* @brief Game ThreadPool.   */
		RHI     = 1,    /* @brief RHI ThreadPool.    */
		Custom  = 2,    /* @brief Custom ThreadPool. */
	};

	/**
	* @brief Wrappers of all Thread Pool.
	*/
	class ThrealModel
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ThrealModel();

		/**
		* @brief Destructor Function.
		*/
		virtual ~ThrealModel() = default;

		/**
		* @brief Get ThrealModel single instance.
		* @return Returns this single instance.
		*/
		static std::shared_ptr<ThrealModel> Get();

		/**
		* @brief Init Custom ThreadPool.
		*/
		void InitCustomThreadPool();

		/**
		* @brief Init Game ThreadPool.
		*/
		void InitGameThreadPool();

		/**
		* @brief Init RHI ThreadPool.
		* @param[in] fn RHI ThreadPool Create function pointer.
		*/
		void InitRHIThreadPool(std::function<void(std::shared_ptr<VulkanCmdThreadPool>& ptr)> fn);

		/**
		* @brief Shutdown Custom ThreadPool.
		*/
		void ShutDownCustomThreadPool();

		/**
		* @brief Shutdown Game ThreadPool.
		*/
		void ShutDownGameThreadPool();

		/**
		* @brief Shutdown RHI ThreadPool.
		*/
		void ShutDownRHIThreadPool();

		/**
		* @brief Get Custom ThreadPool.
		* @return Returns Custom ThreadPool.
		*/
		std::shared_ptr<ThreadPool> GetCustomThreadPool() { return m_CustomThreadPool; }

		/**
		* @brief Get Game ThreadPool.
		* @return Returns Game ThreadPool.
		*/
		std::shared_ptr<DelayThreadPool> GetGameThreadPool() { return m_GameThreadPool; }

		/**
		* @brief Get RHI ThreadPool.
		* @return Returns RHI ThreadPool.
		*/
		std::shared_ptr<VulkanCmdThreadPool> GetRHIThreadPool() { return m_RHIThreadPool; }

	private:

		/**
		* @brief Get ThrealModel single instance.
		*/
		static std::shared_ptr<ThrealModel> m_ThrealModel;

		/**
		* @brief Custom ThreadPool.
		*/
		std::shared_ptr<ThreadPool> m_CustomThreadPool;

		/**
		* @brief Game ThreadPool.
		*/
		std::shared_ptr<DelayThreadPool> m_GameThreadPool;

		/**
		* @brief RHI ThreadPool.
		*/
		std::shared_ptr<VulkanCmdThreadPool> m_RHIThreadPool;
	};

	template<typename F, typename ...Args>
	static auto AnyscTask(ThreadPoolEnum pool, F&& func, Args&&... args) -> std::future<decltype(func(std::forward<Args>(args)...))>
	{
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

		return ThrealModel::Get()->GetRHIThreadPool()->SubmitPoolTask(func, std::forward<Args>(args)...);
	}
}