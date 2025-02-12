/**
* @file ResourcePool.h.
* @brief The ResourcePool Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/UUID.h"
#include "Resource.h"

#include <any>
#include <unordered_map>
#include <memory>

namespace Spices {

	/**
	* @brief Template ResourcePool Class.
	* This class will assign Every Type of Resource per Pool.
	* When we ask for resource, we will get from here instead of load it from disk indirectly.
	* Now we use file path as resource identity.
	* @tparam T Resource basic Class.
	* @todo Use UUID as resource identity instead.
	*/
	template<typename T>
	class ResourcePool
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ResourcePool() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~ResourcePool() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behave.
		*/
		ResourcePool(const ResourcePool&) = delete;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behave.
		*/
		ResourcePool& operator=(const ResourcePool&) = delete;

		/**
		* @brief Load a resource by path.
		* When we need a resource, we call this API.
		* Load if resource is not found.
		* @tparam Ty Resource specific Class.
		* @tparam Args Resource Construct Parameters.
		* @param[in] path Resource file path in disk.
		* @return Returns resource smart pointer.
		*/
		template<typename Ty, typename ...Args>
		static std::shared_ptr<T> Load(const std::string& path, Args... args);

		/**
		* @brief Access a resource by path directly.
		* Do nothing if resource is not found.
		* @param[in] path Resource file path in disk.
		* @return Returns resource smart pointer.
		*/
		static std::shared_ptr<T> Access(const std::string& path);

		/**
		* @brief UnLoad a resource by path.
		* @param[in] path Resource file path in disk.
		*/
		static void UnLoad(const std::string& path);

		/**
		* @brief Determine if specific resource is exist.
		* @param[in] name Resource Name.
		* @return Returns true if exist.
		*/
		static bool Has(const std::string& name);

		/**
		* @brief Registry a resource to this Pool.
		* @param[in] name Resource Name.
		* @param[in] resource Resource.
		*/
		static void Registry(const std::string& name, std::shared_ptr<T> resource);

		/**
		* @brief Destroy this resource pool.
		* Release all Resource Pointer, which means resource can be destructed after called this API.
		*/
		static void Destroy();

	private:

		/**
		* @brief Static variable stores all specific resources in a basic type Pool.
		*/
		static std::unordered_map<std::string, std::unique_ptr<Resource>> m_Resources;

		/**
		* @brief Mutex for this pool.
		*/
		static std::shared_mutex m_Mutex;
	};

	template<typename T>
	std::unordered_map<std::string, std::unique_ptr<Resource>> ResourcePool<T>::m_Resources;

	template<typename T>
	std::shared_mutex ResourcePool<T>::m_Mutex;

	template<typename T>
	template<typename Ty, typename ...Args>
	inline std::shared_ptr<T> ResourcePool<T>::Load(const std::string& path, Args... args)
	{
		SPICES_PROFILE_ZONE;

		{
			std::unique_lock<std::shared_mutex> lock(m_Mutex);

			if (m_Resources.find(path) == m_Resources.end())
			{
				std::function<std::any()> fn = [&]() -> std::any {
					T* inst = new Ty(std::forward<Args>(args)...);
					return std::shared_ptr<T>(inst);
				};

				m_Resources[path] = std::make_unique<Resource>(fn);
			}

		}

		return m_Resources[path]->GetResource<T>();
	}

	template<typename T>
	inline std::shared_ptr<T> ResourcePool<T>::Access(const std::string& path)
	{
		SPICES_PROFILE_ZONE;

		assert(Has(path));
		assert(m_Resources[path]->GetState() == Resource::ResourceStateEnum::Loaded);

		return m_Resources[path]->GetResource<T>();
	}

	template<typename T>
	inline void ResourcePool<T>::UnLoad(const std::string& path)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		if (m_Resources.find(path) != m_Resources.end())
		{
			m_Resources.erase(path);
		}
	}

	template<typename T>
	inline bool ResourcePool<T>::Has(const std::string& name)
	{
		SPICES_PROFILE_ZONE;

		std::shared_lock<std::shared_mutex> lock(m_Mutex);

		if (m_Resources.find(name) != m_Resources.end()) return true;
		return false;
	}

	template<typename T>
	inline void ResourcePool<T>::Registry(const std::string& name, std::shared_ptr<T> resource)
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		if (m_Resources.find(name) != m_Resources.end())
		{
			return;
		}

		m_Resources[name] = std::make_unique<Resource>(resource);
	}

	template<typename T>
	inline void ResourcePool<T>::Destroy()
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::shared_mutex> lock(m_Mutex);

		m_Resources.clear();
	}
}