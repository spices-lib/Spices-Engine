#include "Pchheader.h"
#include "DescriptorSetManager.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

namespace Spices {

	DescriptorManagerContainer DescriptorSetManager::m_DescriptorSets;

	std::shared_ptr<VulkanDescriptorSet> DescriptorSetManager::Registy(const String2& name, uint32_t set)
	{
		if (m_DescriptorSets.find(name) != m_DescriptorSets.end())
		{
			if (m_DescriptorSets[name].find(set) != m_DescriptorSets[name].end())
			{
				return m_DescriptorSets[name][set];
			}
		}

		m_DescriptorSets[name][set] = std::make_shared<VulkanDescriptorSet>(VulkanRenderBackend::GetState(), VulkanRenderBackend::GetDescriptorPool());

		return m_DescriptorSets[name][set];
	}

	std::shared_ptr<VulkanDescriptorSet> DescriptorSetManager::Registy(const std::string& name, uint32_t set)
	{
		return Registy({ name , name }, set);
	}

	void DescriptorSetManager::UnLoad(const String2& name)
	{
		if (m_DescriptorSets.find(name) != m_DescriptorSets.end())
		{
			if (m_DescriptorSets[name].find(BINDLESSTEXTURESET) == m_DescriptorSets[name].end())
			{
				m_DescriptorSets.erase(name);
			}
			else
			{
				auto descriptorSet = m_DescriptorSets[name][BINDLESSTEXTURESET];
				m_DescriptorSets.erase(name);
				m_DescriptorSets[name][BINDLESSTEXTURESET] = descriptorSet;
			}
		}
	}

	void DescriptorSetManager::UnLoad(const std::string& name)
	{
		UnLoad({ name , name });
	}

	void DescriptorSetManager::UnLoadAll()
	{
		m_DescriptorSets.clear();
	}

	DescriptorSetInfo& DescriptorSetManager::GetByName(const String2& name)
	{
		return m_DescriptorSets[name];
	}

	DescriptorSetInfo& DescriptorSetManager::GetByName(const std::string& name)
	{
		const auto str = String2(name, name);
		return GetByName(str);
	}
}