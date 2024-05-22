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
		std::shared_ptr<RendererResourcePool>  rendererResourcePool  ,
		bool isLoadDefaultMaterial
	)
		: m_RendererName            (rendererName          )
		, m_VulkanState             (vulkanState           ) 
		, m_DesctiptorPool          (desctiptorPool        )
		, m_Device                  (device                )
		, m_RendererResourcePool    (rendererResourcePool  )
		, m_IsLoadDefaultMaterial   (isLoadDefaultMaterial )
	{}

	Renderer::~Renderer()
	{}

	void Renderer::OnSystemInitialize()
	{
		/**
		* @brief create renderpass.
		*/
		CreateRendererPass();

		/**
		* @brief create specific renderer descriptorset.
		*/
		CreateDescriptorSet();

		CreateDefaultMaterial();
	}

	void Renderer::RegistyMaterial(const std::string& materialName, const std::string& subpassName)
	{
		/**
		* @brief Instance a temp empty map for VkDescriptorSetLayout.
		* Before turn it to a continus container, sorted is required.
		*/
		std::map<uint32_t, VkDescriptorSetLayout> sortedRowSetLayouts;

		/**
		* @brief PreRenderer's DescriptorSetInfo.
		*/
		auto preRendererSetInfo = DescriptorSetManager::GetByName("PreRenderer");
		for (auto& pair : preRendererSetInfo)
		{
			sortedRowSetLayouts[pair.first] = pair.second->GetRowSetLayout();
		}

		/**
		* @brief SpecificRenderer's DescriptorSetInfo.
		*/
		auto specificRendererSetInfo = DescriptorSetManager::GetByName({ m_Pass->GetName(), subpassName});
		for (auto& pair : specificRendererSetInfo)
		{
			sortedRowSetLayouts[pair.first] = pair.second->GetRowSetLayout();
		}

		/**
		* @brief Material's DescriptorSetInfo.
		*/
		auto material = ResourcePool<Material>::Load<Material>(materialName);
		auto materialSetInfo = material->GetMaterialDescriptorSet();
		for (auto& pair : materialSetInfo)
		{
			sortedRowSetLayouts[pair.first] = pair.second->GetRowSetLayout();
		}

		/**
		* @brief Instance a temp empty vector for VkDescriptorSetLayout.
		*/
		std::vector<VkDescriptorSetLayout> rowSetLayouts;

		for (auto& pair : sortedRowSetLayouts)
		{
			rowSetLayouts.push_back(pair.second);
		}

		/**
		* @breif Create PipelineLayout.
		*/
		auto& subPass = m_Pass->GetSubPasses().find_value(subpassName);
		VkPipelineLayout pipelinelayout = CreatePipelineLayout(rowSetLayouts, subPass);

		/**
		* @brief Create Pipeline.
		*/
		auto& pipeline = CreatePipeline(material, m_Pass->Get(), pipelinelayout);
		subPass->AddPipeline(materialName, pipeline);
	}

	void Renderer::CreateDefaultMaterial()
	{
		if (m_IsLoadDefaultMaterial)
		{
			std::stringstream ss;
			ss << m_RendererName << ".Default";

			auto material = ResourcePool<Material>::Load<Material>(ss.str());
			material->BuildMaterial();
		}
	}

	VkPipelineLayout Renderer::CreatePipelineLayout(std::vector<VkDescriptorSetLayout>& rowSetLayouts, std::shared_ptr<RendererSubPass> subPass)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(rowSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = rowSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (subPass->IsUsePushConstant())
		{
			pipelineLayoutInfo.pushConstantRangeCount = 1;
			pipelineLayoutInfo.pPushConstantRanges = &subPass->GetPushConstant();
		}

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
		pipelineConfig.colorBlendInfo.attachmentCount = (uint32_t)m_Pass->GetColorBlend().size();
		pipelineConfig.colorBlendInfo.pAttachments    = m_Pass->GetColorBlend().data();
		return std::make_shared<VulkanPipeline>(
			m_VulkanState,
			GetSahderPath(material->GetShaderPath("vertShader"), "vert"),
			GetSahderPath(material->GetShaderPath("fragShader"), "frag"),
			pipelineConfig
		);
	}

	void Renderer::UnloadDescriptorSets()
	{
		m_Pass->GetSubPasses().for_each([&](const std::string& name, const std::shared_ptr<RendererSubPass>& subpass) {
			String2 s2(m_Pass->GetName(), name);
			DescriptorSetManager::UnLoad(s2);
		});
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

	Renderer::RenderBehaverBuilder::RenderBehaverBuilder(
		Renderer*         renderer       , 
		uint32_t          currentFrame   , 
		uint32_t          currentImage   )
		: m_Renderer    ( renderer       )
		, m_CurrentFrame( currentFrame   )
		, m_CurrentImage( currentImage   )
	{}

	void Renderer::RenderBehaverBuilder::BindPipeline(const std::string& materialName)
	{
		m_HandledSubPass->GetPipelines()[materialName]->Bind(m_CurrentFrame);
	}

	void Renderer::RenderBehaverBuilder::BeginNextSubPass(const std::string& subpassName)
	{
		m_HandledSubPass = m_Renderer->m_Pass->GetSubPasses().find_value(subpassName);

		vkCmdNextSubpass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaverBuilder::BeginRenderPass()
	{
		m_HandledSubPass = m_Renderer->m_Pass->GetSubPasses().first();

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass        = m_Renderer->m_Pass->Get();
		renderPassInfo.framebuffer       = m_Renderer->m_Pass->GetFramebuffer(m_CurrentImage);
		renderPassInfo.renderArea.offset = { 0, 0 };

		if (m_Renderer->m_Pass->IsUseSwapChain() || !SlateSystem::GetRegister())
		{
			renderPassInfo.renderArea.extent = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize;
		}
		else
		{
			ImVec2 size = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
			VkExtent2D extent = { static_cast<uint32_t>(size.x) , static_cast<uint32_t>(size.y) };
			renderPassInfo.renderArea.extent = extent;
		}

		renderPassInfo.clearValueCount = (uint32_t)m_Renderer->m_Pass->GetClearValues().size();
		renderPassInfo.pClearValues = m_Renderer->m_Pass->GetClearValues().data();

		vkCmdBeginRenderPass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaverBuilder::EndRenderPass()
	{
		vkCmdEndRenderPass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
	}

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(DescriptorSetInfo& infos)
	{
		std::stringstream ss;
		ss << m_Renderer->m_RendererName << ".Default";

		BindDescriptorSet(infos, ss.str());
	}

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(DescriptorSetInfo& infos, const std::string& name)
	{
		for (auto pair : infos)
		{
			vkCmdBindDescriptorSets(
				m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				m_HandledSubPass->GetPipelines()[name]->GetPipelineLayout(),
				pair.first,
				1,
				&pair.second->Get(),
				0,
				nullptr
			);
		}
	}

	Renderer::DescriptorSetBuilder::DescriptorSetBuilder(
		const std::string& subPassName      , 
		Renderer*          renderer
	)
		: m_Renderer(renderer)
	{
		m_HandledSubPass = renderer->m_Pass->GetSubPasses().find_value(subPassName);
		m_DescriptorSetId = { m_Renderer->m_Pass->GetName(), m_HandledSubPass->GetName() };
	}

	Renderer:: DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddAttachmentTexture(
		uint32_t                         set,
		uint32_t                         binding,
		VkShaderStageFlags               stageFlags,
		const std::vector<std::string>& textureNames
	)
	{
		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < textureNames.size(); i++)
		{
			auto info = m_Renderer->m_RendererResourcePool->AccessResource(textureNames[i]);

			m_ImageInfos[set][binding].push_back(*info);
		}

		auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags, textureNames.size());

		return *this;
	}

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddInput(
		uint32_t                         set                   ,
		uint32_t                         binding               ,
		VkShaderStageFlags               stageFlags            ,
		const std::vector<std::string>&  inputAttachmentNames  
	)
	{
		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < inputAttachmentNames.size(); i++)
		{
			auto info = m_Renderer->m_RendererResourcePool->AccessResource(inputAttachmentNames[i]);

			m_ImageInfos[set][binding].push_back(*info);
		}

		auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, stageFlags, inputAttachmentNames.size());

		return *this;
	}

	void Renderer::DescriptorSetBuilder::Build()
	{
		auto descriptorSets = DescriptorSetManager::GetByName(m_DescriptorSetId);

		for (auto& pair : descriptorSets)
		{
			/**
			* @brief AllocateDescriptorSet for Pool.
			*/
			pair.second->BuildDescriptorSet();

			/**
			* @brief UpdateDescriptorSet.
			*/
			pair.second->UpdateDescriptorSet(m_ImageInfos[pair.first], m_BufferInfos[pair.first]);
		}
	}

	Renderer::RendererPassBuilder::RendererPassBuilder(const std::string& rendererPassName, Renderer* renderer)
		: m_RendererPassName(rendererPassName)
		, m_Renderer(renderer)
	{
		m_Renderer->m_Pass = std::make_shared<RendererPass>(rendererPassName, m_Renderer->m_Device);
	}

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddSubPass(const std::string& subPassName)
	{
		m_HandledRendererSubPass = m_Renderer->m_Pass->AddSubPass(subPassName);
		return *this;
	}

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::EndSubPass()
	{
		m_HandledRendererSubPass->BuildSubPassDescription();
		m_HandledRendererSubPass->BuildSubPassDependency(m_Renderer->m_Pass->GetSubPasses().size() - 1);
		return *this;
	}

	void Renderer::RendererPassBuilder::Build()
	{
		m_Renderer->m_Pass->BuildRendererPass();
	}
}