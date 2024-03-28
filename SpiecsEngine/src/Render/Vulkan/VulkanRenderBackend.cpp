#include "pchheader.h"
#include "VulkanRenderBackend.h"

namespace Spiecs {

	VulkanRenderBackend::VulkanRenderBackend()
	{
		m_Windows = std::make_shared<VulkanWindows>(800, 600, "hello");
		m_Instance = std::make_shared<VulkanInstance>("app", "engine", m_Windows->Get());
		m_Device = std::make_shared<VulkanDevice>();
	}

	VulkanRenderBackend::~VulkanRenderBackend()
	{

	}
}