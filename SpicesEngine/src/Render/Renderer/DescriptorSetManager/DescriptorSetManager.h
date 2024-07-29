#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "Core/Math/Math.h"

namespace Spices {

	using DescriptorSetInfo = std::unordered_map<uint32_t, std::shared_ptr<VulkanDescriptorSet>>;
	using DescriptorManagerContainer = std::unordered_map<String2, DescriptorSetInfo>;

	class DescriptorSetManager
	{
	public:
		DescriptorSetManager() = default;
		virtual ~DescriptorSetManager() = default;

		static std::shared_ptr<VulkanDescriptorSet> Registy(const String2& name, uint32_t set);
		static std::shared_ptr<VulkanDescriptorSet> Registy(const std::string& name, uint32_t set);
		static void UnLoad(const String2& name);
		static void UnLoad(const std::string& name);
		static void UnLoadForce(const String2& name);
		static void UnLoadForce(const std::string& name);
		static void UnLoadAll();

		static DescriptorSetInfo& GetByName(const String2& name);
		static DescriptorSetInfo& GetByName(const std::string& name);

	private:
	
		static DescriptorManagerContainer m_DescriptorSets;
	};
}