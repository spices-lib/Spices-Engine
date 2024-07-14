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
	* @param[in] T Resource basic Class.
	* @todo Use UUID as resource identity instead.
	*/
	template<typename T>
	class ResourcePool
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ResourcePool() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~ResourcePool() {};

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
		* @param[in] T1 Resource specific Class.
		* @param[in] path Resource file path in disk.
		* @param[in] isCopy Whether this resource needs copy rather than reference.
		* @return Returns resource smart pointer.
		*/
		template<typename Ty>
		static std::shared_ptr<T> Load(const std::string& path, bool isCopy = false);

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
	template<typename Ty>
	inline std::shared_ptr<T> ResourcePool<T>::Load(const std::string& path, bool isCopy)
	{
		if (m_Resources.find(path) != m_Resources.end())
		{
			return m_Resources[path];
		}
		else
		{
			m_Resources[path] = std::make_shared<Ty>(path);

			return m_Resources[path];
		}
	}

	template<typename T>
	inline void ResourcePool<T>::Destroy()
	{
		m_Resources.clear();
	}
}