#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanDescriptor.h"

namespace Spiecs {

	using DescriptorSetInfo = std::unordered_map<uint32_t, std::shared_ptr<VulkanDescriptorSet>>;
	using DescriptorManagerContainer = std::unordered_map<std::string, DescriptorSetInfo>;

	class DescriptorSetManager
	{
	public:
		DescriptorSetManager() {};
		virtual ~DescriptorSetManager() {};

		static std::shared_ptr<VulkanDescriptorSet> Registy(const std::string& name, uint32_t set);

		static DescriptorSetInfo& GetByName(const std::string& name);

	private:
	
		static DescriptorManagerContainer m_DescriptorSets;
	};
}