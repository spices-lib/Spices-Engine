/**
* @file Renderer.cpp
* @brief The Renderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Renderer.h"
#include "Systems/SlateSystem.h"
#include "Resources/ResourcePool/ResourcePool.h"

namespace Spiecs {

	Renderer::Renderer
	(
		const std::string&                     rendererName          , 
		VulkanState&                           vulkanState           , 
		std::shared_ptr<VulkanDescriptorPool>  desctiptorPool        , 
		std::shared_ptr<VulkanDevice>          device                , 
		std::shared_ptr<RendererResourcePool>  rendererResourcePool
	)
		: m_RendererName            (rendererName          )
		, m_VulkanState             (vulkanState           ) 
		, m_DesctiptorPool          (desctiptorPool        )
		, m_Device                  (device                )
		, m_RendererResourcePool    (rendererResourcePool  )
	{}

	Renderer::~Renderer()
	{}

	void Renderer::OnSystemInitialize()
	{
		/**
		* @brief create renderpass.
		*/
		CreateRenderPass();

		/**
		* @brief create specific renderer descriptorset.
		*/
		CreateDescriptorSet();
	}
	
	void Renderer::RegistyMaterial(const std::string& materialName)
	{
		/**
		* @brief PreRenderer's DescriptorSetInfo.
		*/
		auto preRendererSetInfo = DescriptorSetManager::GetByName("PreRenderer");

		/**
		* @brief SpecificRenderer's DescriptorSetInfo.
		*/
		auto specificRendererSetInfo = DescriptorSetManager::GetByName(m_RendererName);

		/**
		* @brief Material's DescriptorSetInfo.
		*/
		auto material = ResourcePool<Material>::Load<Material>(materialName);
		auto materialSetInfo = material->GetMaterialDescriptorSet();

		/**
		* @breif Create PipelineLayout.
		*/
		VkPipelineLayout layout = CreatePipelineLayout(preRendererSetInfo, specificRendererSetInfo, materialSetInfo);

		/**
		* @brief Create Pipeline.
		*/
		m_Pipelines[materialName] = CreatePipeline(material, m_RenderPass->Get(), layout);

		/**
		* @breif Destroy PipelineLayout.
		*/
		vkDestroyPipelineLayout(m_VulkanState.m_Device, layout, nullptr);
	}

	VkPipelineLayout Renderer::CreatePipelineLayout(
		const DescriptorSetInfo& preRendererSetInfo, 
		const DescriptorSetInfo& specificRendererSetInfo, 
		const DescriptorSetInfo& materialSetInfo
	)
	{
		/**
		* @brief Instance a empty vector.
		*/
		std::vector<VkDescriptorSetLayout> rowSetLayouts;

		/**
		* @brief Add PreRenderer's DescriptorSetLayout..
		*/
		for (auto& pair : preRendererSetInfo)
		{
			rowSetLayouts.push_back(pair.second->GetRowSetLayout());
		}

		/**
		* @brief Add SpecificRenderer's DescriptorSetLayout..
		*/
		for (auto& pair : specificRendererSetInfo)
		{
			rowSetLayouts.push_back(pair.second->GetRowSetLayout());
		}

		/**
		* @brief Add Material's DescriptorSetLayout..
		*/
		for (auto& pair : materialSetInfo)
		{
			rowSetLayouts.push_back(pair.second->GetRowSetLayout());
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(rowSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = rowSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &m_PushConstantRange;

		VkPipelineLayout pipelineLayout;
		VK_CHECK(vkCreatePipelineLayout(m_VulkanState.m_Device, &pipelineLayoutInfo, nullptr, &pipelineLayout));

		return pipelineLayout;
	}
	
	std::shared_ptr<VulkanPipeline> Renderer::CreatePipeline(
		std::shared_ptr<Material> material, 
		VkRenderPass&             renderPass, 
		VkPipelineLayout&         layout
	)
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass                     = renderPass;
		pipelineConfig.pipelineLayout                 = layout;
		pipelineConfig.colorBlendInfo.attachmentCount = (uint32_t)m_RenderPass->GetColorBlend().size();
		pipelineConfig.colorBlendInfo.pAttachments    = m_RenderPass->GetColorBlend().data();
		return std::make_shared<VulkanPipeline>(
			m_VulkanState,
			GetSahderPath(material->GetShaderPath("vert"), "vert"),
			GetSahderPath(material->GetShaderPath("frag"), "vert"),
			pipelineConfig
		);
	}

	std::string Renderer::GetSahderPath(const std::string& name, const std::string& shaderType)
	{
		/**
		* @brief Get full path of shader file.
		*/
		std::stringstream ss;
		ss << SPIECS_ENGINE_ASSETS_PATH << "Shaders/spv/Shader." << name << "." << shaderType << ".spv";

		return ss.str();
	}

	std::pair<glm::mat4, glm::mat4> Renderer::GetActiveCameraMatrix(FrameInfo& frameInfo)
	{
		/**
		* @brief Init viewmatrix and projectionmatrix.
		*/
		glm::mat4 invViewMat       = glm::mat4(1.0f);
		glm::mat4 projectionMat = glm::mat4(1.0f);

		bool find = false;

		/**
		* @brief Iter CameraComponent, finding a active camera.
		*/
		IterWorldComp<CameraComponent>(
			frameInfo, 
			[&](
			int                   entityId, 
			TransformComponent&   transComp, 
			CameraComponent&      camComp
			){

			if (camComp.IsActived())
			{
				/**
				* @brief Viewmaterix is the inverse of camera's modelmatrix. 
				*/
				invViewMat = transComp.GetModelMatrix();
				projectionMat = camComp.GetCamera()->GetPMatrix();

				/**
				* @brief Since we enable Negative viewport, we do not need reverse y axis here.
				*/
				//projectionMat[1][1] *= -1;

				/**
				* @brief Break if finded.
				*/
				find = true;
				return find;
			}

			/**
			* @brief Continue if not finded.
			*/
			return false;
		});

		/**
		* @brief Throwout a warning if not a active camera.
		*/
		if (!find)
		{
			std::stringstream ss;
			ss << m_RendererName << ": " << "not find a active camera in world, please check again";

			SPIECS_CORE_WARN(ss.str());
		}

		return std::make_pair(invViewMat, projectionMat);
	}

	DirectionalLightComponent::DirectionalLight Renderer::GetDirectionalLight(FrameInfo& frameInfo)
	{
		/**
		* @breif Init a DirectionalLight.
		*/
		DirectionalLightComponent::DirectionalLight directionalLight;

		/**
		* @breif Iter DirectionalLightComponent, and just use the first one.
		*/
		IterWorldComp<DirectionalLightComponent>(
			frameInfo, 
			[&](
			int                          entityId, 
			TransformComponent&          transComp, 
			DirectionalLightComponent&   dirlightComp
			) {

			directionalLight = dirlightComp.GetLight();

			/**
			* @breif Break.
			*/
			return true;
		});

		return directionalLight;
	}

	std::array<PointLightComponent::PointLight, 10> Renderer::GetPointLight(FrameInfo& frameInfo)
	{
		/**
		* @brief Init a PointLight contioner.
		*/
		std::vector<PointLightComponent::PointLight> pointLights;
		std::array<PointLightComponent::PointLight, 10> pointLightsArray;

		/**
		* @brief Iter PointLightComponent.
		*/
		IterWorldComp<PointLightComponent>(
			frameInfo, 
			[&](
			int                   entityId, 
			TransformComponent&   transComp, 
			PointLightComponent&  plightComp
			) {

			PointLightComponent::PointLight pointLight = plightComp.GetLight();
			pointLight.position = transComp.GetPosition();
			pointLights.push_back(std::move(pointLight));
			return false;
		});
		
		/**
		* @breif Select ten pointlight.
		*/
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

	Renderer::RenderBehaverBuilder::RenderBehaverBuilder(Renderer* renderer, uint32_t currentFrame, uint32_t currentImage)
		: m_Renderer(renderer)
		, m_CurrentFrame(currentFrame)
		, m_CurrentImage(currentImage)
	{
		//BindPipeline();

		//BindAllBufferTyepDescriptorSet();

		//BindInputDescriptorSet();

		//BeginRenderPass();
	}

	void Renderer::RenderBehaverBuilder::BindPipeline()
	{
		if (!m_Renderer->m_VulkanPipeline) return;

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

	void Renderer::RenderBehaverBuilder::BindInputDescriptorSet()
	{
		int setCount = (int)m_Renderer->m_VulkanLayoutWriters.size();
		for (int i = 0; i < setCount; i++)
		{
			bool IsPureBufferTypeSet = true;

			if (!m_Renderer->m_VulkanLayoutWriters[i]) continue;

			int bindingCount = (int)m_Renderer->m_VulkanLayoutWriters[i]->GetWritters().size();
			for (int j = 0; j < bindingCount; j++)
			{
				if (m_Renderer->m_VulkanLayoutWriters[i]->GetWritters()[j].descriptorType != VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
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
		renderPassInfo.framebuffer = m_Renderer->m_RenderPass->GetFramebuffer(m_CurrentImage);
		renderPassInfo.renderArea.offset = { 0, 0 };

		if (m_Renderer->m_RenderPass->IsUseSwapChain() || !SlateSystem::GetRegister())
		{
			renderPassInfo.renderArea.extent = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize;
		}
		else
		{
			ImVec2 size = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
			VkExtent2D extent = { static_cast<uint32_t>(size.x) , static_cast<uint32_t>(size.y) };
			renderPassInfo.renderArea.extent = extent;
		}

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

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(DescriptorSetInfo& infos)
	{
		for (auto pair : infos)
		{
			vkCmdBindDescriptorSets(
				m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				m_Renderer->m_PipelineLayout,
				pair.first,
				1,
				&pair.second->Get(),
				0,
				nullptr
			);
		}
	}


	inline DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddInput(uint32_t set, uint32_t binding, uint32_t arrayNum, VkShaderStageFlags stageFlags, const std::vector<std::string>& inputAttachmentNames)
	{
		// TODO: 在此处插入 return 语句
	}
	void Renderer::DescriptorSetBuilder::Build()
	{
	}
}