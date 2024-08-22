/**
* @file ResourcePool.h.
* @brief The ResourcePool Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/UUID.h"

#include <any>
#include <unordered_map>
#include <memory>

namespace Spices {

	/**
	* @brief Template ResourcePool Class.
	* This class will assign Every Type of Resource per Pool.
	* When we ask for resource, we will get from here instand of load it from disk indirectlly.
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
		* @note This Class not allowed copy behaver.
		*/
		ResourcePool(const ResourcePool&) = delete;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		ResourcePool& operator=(const ResourcePool&) = delete;

		/**
		* @brief Load a resource by path.
		* When we need a resource, we call this API.
		* @tparam Ty Resource specific Class.
		* @tparam Args Resource Construct Parameters.
		* @param[in] path Resource file path in disk.
		* @return Returns resource smart pointer.
		*/
		template<typename Ty, typename ...Args>
		static std::shared_ptr<T> Load(const std::string& path, Args... args);

		/**
		* @brief Load a resource by path.
		* @param[in] path Resource file path in disk.
		* @return Returns resource smart pointer.
		*/
		static std::shared_ptr<T> Load(const std::string& path);

		/**
		* @brief Determain if specific resource is exist.
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
		static std::unordered_map<std::string, std::shared_ptr<T>> m_Resources;
	};

	template<typename T>
	std::unordered_map<std::string, std::shared_ptr<T>> ResourcePool<T>::m_Resources;

	template<typename T>
	template<typename Ty, typename ...Args>
	inline std::shared_ptr<T> ResourcePool<T>::Load(const std::string& path, Args... args)
	{
		SPICES_PROFILE_ZONE;

		if (m_Resources.find(path) != m_Resources.end())
		{
			return m_Resources[path];
		}
		else
		{
			m_Resources[path] = std::make_shared<Ty>(std::forward<Args>(args)...);

			return m_Resources[path];
		}
	}

	template<typename T>
	inline std::shared_ptr<T> ResourcePool<T>::Load(const std::string& path)
	{
		SPICES_PROFILE_ZONE;

		if (m_Resources.find(path) == m_Resources.end()) return nullptr;
		return m_Resources[path];
	}

	template<typename T>
	inline bool ResourcePool<T>::Has(const std::string& name)
	{
		SPICES_PROFILE_ZONE;

		if (m_Resources.find(name) != m_Resources.end()) return true;
		return false;
	}

	template<typename T>
	inline void ResourcePool<T>::Registry(const std::string& name, std::shared_ptr<T> resource)
	{
		SPICES_PROFILE_ZONE;

		if (m_Resources.find(name) != m_Resources.end()) return;
		m_Resources[name] = resource;
	}

	template<typename T>
	inline void ResourcePool<T>::Destroy()
	{
		SPICES_PROFILE_ZONE;

		m_Resources.clear();
	}
}