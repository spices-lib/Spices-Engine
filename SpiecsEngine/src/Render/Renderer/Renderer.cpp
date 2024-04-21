/**
* @file Renderer.cpp
* @brief The Renderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Renderer.h"

namespace Spiecs {

	Renderer::Renderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool, std::shared_ptr<VulkanDevice> device)
		: m_RendererName(rendererName)
		, m_VulkanState(vulkanState) 
		, m_DesctiptorPool(desctiptorPool)
		, m_Device(device)
		, m_PipelineLayout{}
	{}

	Renderer::~Renderer()
	{
		vkDestroyPipelineLayout(m_VulkanState.m_Device, m_PipelineLayout, nullptr);
	}

	void Renderer::OnSystemInitialize()
	{
		/**
		* @brief create renderpass.
		*/
		CreateRenderPass();

		/**
		* @brief create pipeline layout and buffer type descriptor.
		*/
		CreatePipelineLayoutAndDescriptor();

		/**
		* @brief create pipeline.
		*/
		CreatePipeline(m_RenderPass->Get());
	}

	std::string Renderer::GetSahderPath(const std::string& shaderType)
	{
		return SPIECS_ENGINE_ASSETS_PATH + "Shaders/spv/Shader." + m_RendererName + "." + shaderType + ".spv";
	}

	std::pair<glm::mat4, glm::mat4> Renderer::GetActiveCameraMatrix(FrameInfo& frameInfo)
	{
		glm::mat4 viewMat = glm::mat4(1.0f);
		glm::mat4 projectionMat = glm::mat4(1.0f);

		IterWorldComp<CameraComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, CameraComponent& camComp) {
			if (camComp.IsActived())
			{
				viewMat = glm::inverse(transComp.GetModelMatrix());
				projectionMat = camComp.GetCamera()->GetPMatrix();

				/**
				* @brief Since we enable Negative viewport, we do not need reverse y axis here.
				*/
				//projectionMat[1][1] *= -1;

				return true;
			}
			return false;
		});

		return std::make_pair(viewMat, projectionMat);
	}

	DirectionalLightComponent::DirectionalLight Renderer::GetDirectionalLight(FrameInfo& frameInfo)
	{
		DirectionalLightComponent::DirectionalLight directionalLight;
		IterWorldComp<DirectionalLightComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, DirectionalLightComponent& dirlightComp) {
			directionalLight = dirlightComp.GetLight();
			return true;
		});

		return directionalLight;
	}

	std::array<PointLightComponent::PointLight, 10> Renderer::GetPointLight(FrameInfo& frameInfo)
	{
		std::vector<PointLightComponent::PointLight> pointLights;
		std::array<PointLightComponent::PointLight, 10> pointLightsArray;

		IterWorldComp<PointLightComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, PointLightComponent& plightComp) {
			PointLightComponent::PointLight pointLight = plightComp.GetLight();
			pointLight.position = transComp.GetPosition();
			pointLights.push_back(std::move(pointLight));
			return false;
		});
		
		for (int i = 0; i < pointLights.size(); i++)
		{
			if (i == 10) break;
			pointLightsArray[i] = pointLights[i];
		}

		return pointLightsArray;
	}

	void Renderer::PipelineLayoutBuilder::Build()
	{
		/**
		* @brief Create buffer type descriptor set
		*/
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			int setSize = (int)m_Renderer->m_VulkanLayoutWriters.size();
			ContainerLibrary::Resize<VkDescriptorSet>(m_Renderer->m_Resource[i].m_DescriptorSets, setSize);

			for (int j = 0; j < setSize; j++)
			{
				if (m_Renderer->m_VulkanLayoutWriters[j])
				{
					m_Renderer->m_VulkanLayoutWriters[j]->Build(m_Renderer->m_Resource[i].m_DescriptorSets[j]);
				}
			}
		}

		/**
		* @brief Create pipelinelayout
		*/
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_Renderer->m_DescriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = m_Renderer->m_DescriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (isUsePushConstant)
		{
			pipelineLayoutInfo.pushConstantRangeCount = 1;
			pipelineLayoutInfo.pPushConstantRanges = &m_PushConstantRange;
		}

		VK_CHECK(vkCreatePipelineLayout(m_Renderer->m_VulkanState.m_Device, &pipelineLayoutInfo, nullptr, &m_Renderer->m_PipelineLayout));
	}

	Renderer::RenderBehaverBuilder::RenderBehaverBuilder(Renderer* renderer, uint32_t currentFrame)
		: m_Renderer(renderer)
		, m_CurrentFrame(currentFrame)
	{
		BindPipeline();

		BindAllBufferTyepDescriptorSet();

		BeginRenderPass();
	}

	void Renderer::RenderBehaverBuilder::BindPipeline()
	{
		m_Renderer->m_VulkanPipeline->Bind(m_CurrentFrame);
	}

	void Renderer::RenderBehaverBuilder::BindAllBufferTyepDescriptorSet()
	{
		int setCount = (int)m_Renderer->m_VulkanLayoutWriters.size();
		for (int i = 0; i < setCount; i++)
		{
			bool IsPureBufferTypeSet = true;

			if (!m_Renderer->m_VulkanLayoutWriters[i]) continue;

			int bindingCount = (int)m_Renderer->m_VulkanLayoutWriters[i]->GetWritters().size();
			for (int j = 0; j < bindingCount; j++)
			{
				if (m_Renderer->m_VulkanLayoutWriters[i]->GetWritters()[j].descriptorType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
				{
					IsPureBufferTypeSet = false;
					break;
				}
			}

			if (IsPureBufferTypeSet)
			{
				BindDescriptorSet(i, m_Renderer->m_Resource[m_CurrentFrame].m_DescriptorSets[i]);
			}
		}
	}

	void Renderer::RenderBehaverBuilder::BeginRenderPass()
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Renderer->m_RenderPass->Get();
		renderPassInfo.framebuffer = m_Renderer->m_RenderPass->GetFramebuffer(m_CurrentFrame);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_Renderer->m_Device->GetSwapChainSupport().extent;

		renderPassInfo.clearValueCount = (uint32_t)m_Renderer->m_RenderPass->GetClearValues().size();
		renderPassInfo.pClearValues = m_Renderer->m_RenderPass->GetClearValues().data();

		vkCmdBeginRenderPass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaverBuilder::EndRenderPass()
	{
		vkCmdEndRenderPass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
	}

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(uint32_t set, VkDescriptorSet& descriptorset)
	{
		vkCmdBindDescriptorSets(
			m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_Renderer->m_PipelineLayout,
			set,
			1,
			&descriptorset,
			0,
			nullptr
		);
	}
}