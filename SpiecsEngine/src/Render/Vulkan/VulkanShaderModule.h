#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	class VulkanShaderModule : public VulkanObject
	{
	public:
		VulkanShaderModule(VulkanState& vulkanState, const std::string& filePath);
		virtual ~VulkanShaderModule();

		VulkanShaderModule(const VulkanShaderModule&) = delete;
		VulkanShaderModule& operator=(const VulkanShaderModule&) = delete;

		VkShaderModule& Get() { return m_ShaderModule; };

	private:
		VkShaderModule m_ShaderModule;
	};
}