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

		static bool Load(const std::string& path, std::shared_ptr<T> outResource, bool isAutoRelease = false, bool isCopy = false);

	protected:
		struct ResourcePack
		{
			UUID uuid;
			std::any resource;
		};

	protected:
		std::unordered_map<std::string, ResourcePack> m_Resources;
	};

	template<typename T>
	inline bool ResourcePool<T>::Load(const std::string& path, std::shared_ptr<T> outResource, bool isAutoRelease, bool isCopy)
	{
		if (m_Resources.find(path) != m_Resources.end())
		{
			
		}

		return false;
	}
}