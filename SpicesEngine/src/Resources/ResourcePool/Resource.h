/**
* @file Resource.h.
* @brief The Resource Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include <any>
#include <memory>

namespace Spices {

	/**
	* @brief Resource Wrapper of ResourceInstance.
	*/
	class Resource
	{
	public:

		/**
		* @brief Enum of ResourceState
		*/
		enum class ResourceStateEnum
		{
			UnLoad    = 0,   /* @brief UnLoad.         */
			OnLoading = 1,   /* @brief On Loading.     */
			Loaded    = 2,   /* @brief Already Loaded. */
			OnDestroy = 3,   /* @brief On Destroy.     */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] fn function pointer of create resource instance.
		*/
		explicit Resource(std::function<std::any()> fn)
			: m_CreateFunction(fn)
			, m_State(ResourceStateEnum::UnLoad)
		{};

		/**
		* @brief Constructor Function.
		* This mode cannot call CreateResource().
		* @param[in] resource Resource instance.
		*/
		explicit Resource(std::any resource)
			: m_CreateFunction(nullptr)
			, m_State(ResourceStateEnum::Loaded)
			, m_Resource(resource)
		{};

		/**
		* @brief Unload resource.
		*/
		virtual ~Resource()
		{
			SPICES_PROFILE_ZONE;

			UnLoad();
		}

		/**
		* @brief Get resource state.
		* @return Returns resource state.
		*/
		ResourceStateEnum GetState() const { return m_State; }

		/**
		* @brief Get resource instance.
		* @return Returns resource instance.
		*/
		template<typename T>
		std::shared_ptr<T> GetResource();
		
		/**
		* @brief UnLoad this resource.
		*/
		void UnLoad();

	private:

		/**
		* @brief Create resource instance.
		*/
		void CreateResource();

	private:

		/**
		* @brief Resource instance wrapper.
		*/
		std::any m_Resource;

		/**
		* @brief Resource state.
		*/
		ResourceStateEnum m_State;

		/**
		* @brief Resource create function.
		*/
		std::function<std::any()> m_CreateFunction;

		/**
		* @brief Mutex of this resource.
		*/
		std::mutex m_Mutex;
	};

	inline void Resource::UnLoad()
	{
		SPICES_PROFILE_ZONE;

		m_State = ResourceStateEnum::OnDestroy;

		m_Resource.reset();

		m_State = ResourceStateEnum::UnLoad;
	}

	inline void Resource::CreateResource()
	{
		SPICES_PROFILE_ZONE;

		assert(m_CreateFunction);

		m_State = ResourceStateEnum::OnLoading;

		m_Resource = m_CreateFunction();

		m_State = ResourceStateEnum::Loaded;
	}

	template<typename T>
	inline std::shared_ptr<T> Resource::GetResource()
	{
		SPICES_PROFILE_ZONE;

		if (m_State == ResourceStateEnum::Loaded)
		{
			return std::any_cast<std::shared_ptr<T>>(m_Resource);
		}

		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			if (m_State == ResourceStateEnum::Loaded)
			{
				return std::any_cast<std::shared_ptr<T>>(m_Resource);
			}

			CreateResource();

			return std::any_cast<std::shared_ptr<T>>(m_Resource);
		}
	}
}