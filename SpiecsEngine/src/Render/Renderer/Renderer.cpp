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
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Create renderpass.
		*/
		CreateRendererPass();

		/**
		* @brief Create specific renderer descriptorset.
		*/
		CreateDescriptorSet();

		/**
		* @brief Create specific renderer default material.
		*/
		CreateDefaultMaterial();
	}

	void Renderer::OnSlateResize()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Recreate RenderPass.
		*/
		CreateRendererPass();

		/**
		* @brief Create descriptorSet again.
		*/
		CreateDescriptorSet();
	}

	void Renderer::RegistyMaterial(const std::string& materialName, const std::string& subpassName)
	{
		SPIECS_PROFILE_ZONE;

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
		auto& subPass = *m_Pass->GetSubPasses().find_value(subpassName);
		VkPipelineLayout pipelinelayout = CreatePipelineLayout(rowSetLayouts, subPass);

		/**
		* @brief Create Pipeline.
		*/
		auto& pipeline = CreatePipeline(material, pipelinelayout, subPass);
		m_Pipelines[materialName] = pipeline;
	}

	void Renderer::CreateDefaultMaterial()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Iter all subpass.
		*/
		if (m_IsLoadDefaultMaterial)
		{
			m_Pass->GetSubPasses().for_each([&](const auto& K, const auto& V) {

				std::stringstream ss;
				ss << m_RendererName << "." << K << ".Default";

				auto material = ResourcePool<Material>::Load<Material>(ss.str());
				material->BuildMaterial();

				/**
				* @brief Not break loop.
				*/
				return false;
			});
		}
	}

	VkPipelineLayout Renderer::CreatePipelineLayout(
		std::vector<VkDescriptorSetLayout>& rowSetLayouts , 
		std::shared_ptr<RendererSubPass>    subPass
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Instance a VkPipelineLayoutCreateInfo.
		*/
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType                         = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount                = static_cast<uint32_t>(rowSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts                   = rowSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount        = 0;
		pipelineLayoutInfo.pPushConstantRanges           = nullptr;

		if (subPass->IsUsePushConstant())
		{
			pipelineLayoutInfo.pushConstantRangeCount    = 1;
			pipelineLayoutInfo.pPushConstantRanges       = &subPass->GetPushConstant();
		}

		/**
		* @brief Create a VkPipelineLayout.
		*/
		VkPipelineLayout pipelineLayout;
		VK_CHECK(vkCreatePipelineLayout(m_VulkanState.m_Device, &pipelineLayoutInfo, nullptr, &pipelineLayout));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_PIPELINE_LAYOUT, pipelineLayout, m_VulkanState.m_Device, "PipelineLayout");

		return pipelineLayout;
	}
	
	std::shared_ptr<VulkanPipeline> Renderer::CreatePipeline(
		std::shared_ptr<Material>        material , 
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get Dafault PipelineConfigInfo.
		*/
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);

		/**
		* @brief Fill in with configurable data.
		*/
		pipelineConfig.renderPass                     = m_Pass->Get();
		pipelineConfig.subpass                        = subPass->GetIndex();
		pipelineConfig.pipelineLayout                 = layout;
		pipelineConfig.colorBlendInfo.attachmentCount = (uint32_t)subPass->GetColorBlend().size();
		pipelineConfig.colorBlendInfo.pAttachments    = subPass->GetColorBlend().data();

		/**
		* @brief Create VulkanPipeline.
		*/
		return std::make_shared<VulkanPipeline>(
			m_VulkanState,
			material->GetName(),
			material->GetShaderPath(),
			pipelineConfig
		);
	}

	std::pair<glm::mat4, glm::mat4> Renderer::GetActiveCameraMatrix(FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

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
			int                   entityId  , 
			TransformComponent&   transComp , 
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

	void Renderer::GetDirectionalLight(FrameInfo& frameInfo, DirectionalLightComponent::DirectionalLight& directionalLight)
	{
		SPIECS_PROFILE_ZONE;

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
	}

	void Renderer::GetDirectionalLightMatrix(FrameInfo& frameInfo, std::array<glm::mat4, MAX_DIRECTIONALLIGHT_NUM>& directionalLight)
	{
		SPIECS_PROFILE_ZONE;

		for (int i = 0; i < MAX_DIRECTIONALLIGHT_NUM; i++)
		{
			directionalLight[i] = glm::mat4(1.0f);
		}

		int index = 0;
		IterWorldComp<DirectionalLightComponent>(
			frameInfo,
			[&](
			int                          entityId    ,
			TransformComponent&          transComp   ,
			DirectionalLightComponent&   dirlightComp
			) {
				//glm::mat4 view = glm::lookAt(dirlightComp.GetLight().direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				//glm::mat4 projection = glm::ortho(-2.0f, 2.0f, 2.0f, -2.0f, -100000.0f, 100000.0f);
				auto& [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);

				directionalLight[index] = projectionMatrix * glm::inverse(invViewMatrix);
				index++;
				return false;
		});
	}

	void Renderer::GetPointLight(FrameInfo& frameInfo, std::array<PointLightComponent::PointLight, 1000>& pLightArrat)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Iter PointLightComponent.
		*/
		int index = 0;
		IterWorldComp<PointLightComponent>(
			frameInfo, 
			[&](
			int                   entityId, 
			TransformComponent&   transComp, 
			PointLightComponent&  plightComp
			) {

			PointLightComponent::PointLight pointLight = plightComp.GetLight();
			pointLight.position = transComp.GetPosition();
			pLightArrat[index] = std::move(pointLight);
			index++;
			return false;
		});
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
		SPIECS_PROFILE_ZONE;

		m_Renderer->m_Pipelines[materialName]->Bind(m_CurrentFrame);
	}

	void Renderer::RenderBehaverBuilder::BeginNextSubPass(const std::string& subpassName)
	{
		SPIECS_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().find_value(subpassName);

		VulkanDebugUtils::EndLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
		VulkanDebugUtils::BeginLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], m_HandledSubPass->GetName());

		vkCmdNextSubpass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaverBuilder::BeginRenderPass()
	{
		SPIECS_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().first();

		/**
		* @brief Instance a VkRenderPassBeginInfo.
		*/
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass               = m_Renderer->m_Pass->Get();
		renderPassInfo.framebuffer              = m_Renderer->m_Pass->GetFramebuffer(m_CurrentImage);
		renderPassInfo.renderArea.offset        = { 0, 0 };

		/**
		* @brief In the first frame, we use window size rather than viewport size.
		*/
		if (m_Renderer->m_Pass->IsUseSwapChain() || !SlateSystem::GetRegister())
		{
			renderPassInfo.renderArea.extent    = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize;
		}
		else
		{
			ImVec2 size = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
			VkExtent2D extent = { static_cast<uint32_t>(size.x) , static_cast<uint32_t>(size.y) };
			renderPassInfo.renderArea.extent    = extent;
		}

		renderPassInfo.clearValueCount          = (uint32_t)m_Renderer->m_Pass->GetClearValues().size();
		renderPassInfo.pClearValues             = m_Renderer->m_Pass->GetClearValues().data();

		VulkanDebugUtils::BeginLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], m_Renderer->m_Pass->GetName());
		VulkanDebugUtils::BeginLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], m_HandledSubPass->GetName());

		vkCmdBeginRenderPass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaverBuilder::EndRenderPass()
	{
		SPIECS_PROFILE_ZONE;

		vkCmdEndRenderPass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);

		VulkanDebugUtils::EndLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
		VulkanDebugUtils::EndLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
	}

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(DescriptorSetInfo& infos)
	{
		SPIECS_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		BindDescriptorSet(infos, ss.str());
	}

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(DescriptorSetInfo& infos, const std::string& name)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Iter all desctiptorsets.
		*/
		for (auto pair : infos)
		{
			vkCmdBindDescriptorSets(
				m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				m_Renderer->m_Pipelines[name]->GetPipelineLayout(),
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
		SPIECS_PROFILE_ZONE;

		m_HandledSubPass  = *renderer->m_Pass->GetSubPasses().find_value(subPassName);
		m_DescriptorSetId = { m_Renderer->m_Pass->GetName(), m_HandledSubPass->GetName() };
	}

	Renderer:: DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddAttachmentTexture(
		uint32_t                         set,
		uint32_t                         binding,
		VkShaderStageFlags               stageFlags,
		const std::vector<std::string>& textureNames
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < textureNames.size(); i++)
		{
			RendererResourceCreateInfo Resinfo;
			Resinfo.name = textureNames[i];
			auto info = m_Renderer->m_RendererResourcePool->AccessResource(Resinfo);

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
		SPIECS_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < inputAttachmentNames.size(); i++)
		{
			RendererResourceCreateInfo Resinfo;
			Resinfo.name = inputAttachmentNames[i];
			auto info = m_Renderer->m_RendererResourcePool->AccessResource(Resinfo);

			m_ImageInfos[set][binding].push_back(*info);
		}

		auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, stageFlags, inputAttachmentNames.size());

		return *this;
	}

	void Renderer::DescriptorSetBuilder::Build()
	{
		SPIECS_PROFILE_ZONE;

		auto descriptorSets = DescriptorSetManager::GetByName(m_DescriptorSetId);

		for (auto& pair : descriptorSets)
		{
			/**
			* @brief AllocateDescriptorSet for Pool.
			*/
			pair.second->BuildDescriptorSet(m_HandledSubPass->GetName());
			
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
		SPIECS_PROFILE_ZONE;

		m_Renderer->m_Pass = std::make_shared<RendererPass>(rendererPassName, m_Renderer->m_Device);
	}

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddSubPass(const std::string& subPassName)
	{
		SPIECS_PROFILE_ZONE;

		uint32_t size = m_Renderer->m_Pass->GetSubPasses().size();
		m_HandledRendererSubPass = m_Renderer->m_Pass->AddSubPass(subPassName, size);
		return *this;
	}

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::EndSubPass()
	{
		SPIECS_PROFILE_ZONE;

		m_HandledRendererSubPass->BuildSubPassDescription();
		m_HandledRendererSubPass->BuildSubPassDependency(m_Renderer->m_Pass->GetSubPasses().size() - 1);
		return *this;
	}

	void Renderer::RendererPassBuilder::Build()
	{
		SPIECS_PROFILE_ZONE;

		m_Renderer->m_Pass->BuildRendererPass();
	}
}