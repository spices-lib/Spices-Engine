#include "Pchheader.h"
#include "DescriptorSetManager.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	DescriptorManagerContainer DescriptorSetManager::m_DescriptorSets;

	std::shared_ptr<VulkanDescriptorSet> DescriptorSetManager::Registy(const std::string& name, uint32_t set)
	{
		m_DescriptorSets[name][set] = std::make_shared<VulkanDescriptorSet>(VulkanRenderBackend::GetState(), VulkanRenderBackend::GetDescriptorPool());
		return m_DescriptorSets[name][set];
	}

	DescriptorSetInfo& DescriptorSetManager::GetByName(const std::string& name)
	{
		return m_DescriptorSets[name];
	}
}