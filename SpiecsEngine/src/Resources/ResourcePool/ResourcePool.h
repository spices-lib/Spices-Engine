#pragma once
#include "Core/Core.h"
#include "Core/UUID.h"

#include <any>
#include <unordered_map>
#include <memory>

namespace Spiecs {

	template<typename T>
	class ResourcePool
	{
	public:
		ResourcePool() {};
		virtual ~ResourcePool() {};

		ResourcePool(const ResourcePool&) = delete;
		ResourcePool& operator=(const ResourcePool&) = delete;

		template<typename T1>
		static std::shared_ptr<T> Load(const std::string& path, bool isCopy = false);

		static void Destroy();

	private:
		static std::unordered_map<std::string, std::shared_ptr<T>> m_Resources;
	};

	template<typename T>
	std::unordered_map<std::string, std::shared_ptr<T>> ResourcePool<T>::m_Resources;

	template<typename T>
	template<typename T1>
	inline std::shared_ptr<T> ResourcePool<T>::Load(const std::string& path, bool isCopy)
	{
		if (m_Resources.find(path) != m_Resources.end())
		{
			return m_Resources[path];
		}
		else
		{
			m_Resources[path] = std::make_shared<T1>(path);

			return m_Resources[path];
		}
	}

	template<typename T>
	inline void ResourcePool<T>::Destroy()
	{
		m_Resources.clear();
	}
}