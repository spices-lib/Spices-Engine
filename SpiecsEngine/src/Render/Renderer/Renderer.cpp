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
		: m_VulkanState             (vulkanState           )
		, m_DesctiptorPool          (desctiptorPool        )
		, m_Device                  (device                )
		, m_RendererResourcePool    (rendererResourcePool  )
		, m_IsLoadDefaultMaterial   (isLoadDefaultMaterial )
	    , m_RendererName            (rendererName          )
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
		auto pipeline = CreatePipeline(material, pipelinelayout, subPass);
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

	void Renderer::GetDirectionalLight(FrameInfo& frameInfo, std::array<SpiecsShader::DirectionalLight, DIRECTIONALLIGHTBUFFERMAXNUM>& dLightBuffer)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @breif Iter DirectionalLightComponent, and just use the first one.
		*/
		int index = 0;
		IterWorldComp<DirectionalLightComponent>(
			frameInfo, 
			[&](
			int                          entityId, 
			TransformComponent&          transComp, 
			DirectionalLightComponent&   dirlightComp
			) {

			SpiecsShader::DirectionalLight directionalLight = dirlightComp.GetLight();
			dLightBuffer[index] = std::move(directionalLight);
			index++;
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

		TransformComponent camTranComp;
		float ratio;

		IterWorldComp<CameraComponent>(
			frameInfo,
			[&](
			int                          entityId,
			TransformComponent& transComp,
			CameraComponent& camComp
			) {
				camTranComp = transComp;
				ratio = camComp.GetCamera()->GetAspectRatio();
				return true;
		});

		int index = 0;
		IterWorldComp<DirectionalLightComponent>(
			frameInfo,
			[&](
			int                          entityId    ,
			TransformComponent&          transComp   ,
			DirectionalLightComponent&   dirlightComp
			) {
				TransformComponent tempComp;
				tempComp.SetPostion(camTranComp.GetPosition());
				tempComp.SetRotation(camTranComp.GetRotation());

				glm::mat4 view = tempComp.GetModelMatrix();
				glm::mat4 projection = Otrhographic(-ratio * 30, ratio * 30, -1.0f * 30, 1.0f * 30, -100000.0f, 100000.0f);

				auto [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);

				directionalLight[index] = projection * glm::inverse(view);
				index++;
				return false;
		});
	}

	void Renderer::GetPointLight(FrameInfo& frameInfo, std::array<SpiecsShader::PointLight, POINTLIGHTBUFFERMAXNUM>& pLightBuffer)
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

			SpiecsShader::PointLight pointLight = plightComp.GetLight();
			pointLight.position = transComp.GetPosition();
			pLightBuffer[index] = std::move(pointLight);
			index++;
			return false;
		});
	}

	Renderer::RenderBehaverBuilder::RenderBehaverBuilder(
		Renderer*         renderer       , 
		uint32_t          currentFrame   , 
		uint32_t          currentImage   ,
		bool              isNonGraphicRender 
	)
		: m_Renderer    ( renderer       )
		, m_CurrentFrame( currentFrame   )
		, m_CurrentImage( currentImage   )
	{
		if (isNonGraphicRender)
		{
			m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().first();
		}
	}

	void Renderer::RenderBehaverBuilder::BindPipeline(const std::string& materialName, VkPipelineBindPoint  bindPoint)
	{
		SPIECS_PROFILE_ZONE;

		m_Renderer->m_Pipelines[materialName]->Bind(m_CurrentFrame, bindPoint);
	}

	void Renderer::RenderBehaverBuilder::SetViewPort()
	{
		/**
		* @brief Use Negative Viewport height filp here to handle axis difference.
		* Remember enable device extension (VK_KHR_MAINTENANCE1)
		*/
		VkViewport viewport {};
		viewport.x                =  0.0f;
		viewport.y                =  static_cast<float>(m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height);
		viewport.width            =  static_cast<float>(m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.width);
		viewport.height           = -static_cast<float>(m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height);
		viewport.minDepth         =  0.0f;
		viewport.maxDepth         =  1.0f;

		/**
		* @brief Though we draw world to viewport but not surface,
		* Set Correct viewport here is necessary.
		*/
		if (SlateSystem::GetRegister())
		{
			ImVec2 viewPortSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();

			viewport.y            =  viewPortSize.y;
			viewport.width        =  viewPortSize.x;
			viewport.height       = -viewPortSize.y;
		}

		/**
		* @brief Set VkViewport with viewport slate.
		*/
		vkCmdSetViewport(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], 0, 1, &viewport);

		/**
		* @brief Instance a VkRect2D
		*/
		VkRect2D scissor {};
		scissor.offset            = { 0, 0 };
		scissor.extent            = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize;

		/**
		* @brief Set VkRect2D.
		*/
		vkCmdSetScissor(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], 0, 1, &scissor);
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

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(DescriptorSetInfo& infos, VkPipelineBindPoint bindPoint)
	{
		SPIECS_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		BindDescriptorSet(infos, ss.str(), bindPoint);
	}

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(DescriptorSetInfo& infos, const std::string& name, VkPipelineBindPoint bindPoint)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Iter all desctiptorsets.
		*/
		for (auto pair : infos)
		{
			vkCmdBindDescriptorSets(
				m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
				bindPoint,
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

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddStorageTexture(
		uint32_t                         set           , 
		uint32_t                         binding       , 
		VkShaderStageFlags               stageFlags    , 
		const std::vector<std::string>&  textureNames  , 
		VkFormat                         format        ,
		TextureType                      type
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < textureNames.size(); i++)
		{
			RendererResourceCreateInfo Info;
			Info.name                                = textureNames[i];
			Info.type                                = type;
			Info.width                               = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.width;
			Info.height                              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height;
			Info.description.samples                 = VK_SAMPLE_COUNT_1_BIT;
			Info.description.format                  = format;
			Info.usage                               = VK_IMAGE_USAGE_STORAGE_BIT;

			VkDescriptorImageInfo* imageInfo         = m_Renderer->m_RendererResourcePool->AccessResource(Info);
			imageInfo->imageLayout                   = VK_IMAGE_LAYOUT_GENERAL;

			m_ImageInfos[set][binding].push_back(*imageInfo);
		}

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, stageFlags, static_cast<uint32_t>(textureNames.size()));

		return *this;
	}

	Renderer:: DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddAttachmentTexture(
		uint32_t                         set,
		uint32_t                         binding,
		VkShaderStageFlags               stageFlags,
		const std::vector<std::string>&  textureNames
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
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags,  static_cast<uint32_t>(textureNames.size()));

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
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, stageFlags, static_cast<uint32_t>(inputAttachmentNames.size()));

		return *this;
	}

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddAccelerationStructure(
		uint32_t                         set                    , 
		uint32_t                         binding                , 
		VkShaderStageFlags               stageFlags
	)
	{
		SPIECS_PROFILE_ZONE;

		auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, stageFlags, 1);

		return *this;
	}

	void Renderer::DescriptorSetBuilder::Build(const VkAccelerationStructureKHR& accel)
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
			pair.second->UpdateDescriptorSet(m_ImageInfos[pair.first], m_BufferInfos[pair.first], accel);
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

		size_t size = m_Renderer->m_Pass->GetSubPasses().size();
		m_HandledRendererSubPass = m_Renderer->m_Pass->AddSubPass(subPassName, static_cast<uint32_t>(size));
		return *this;
	}

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::EndSubPass()
	{
		SPIECS_PROFILE_ZONE;

		m_HandledRendererSubPass->BuildSubPassDescription();

		size_t index = m_Renderer->m_Pass->GetSubPasses().size() - 1;
		m_HandledRendererSubPass->BuildSubPassDependency(static_cast<uint32_t>(index));
		return *this;
	}

	void Renderer::RendererPassBuilder::Build()
	{
		SPIECS_PROFILE_ZONE;

		m_Renderer->m_Pass->BuildRendererPass();
	}
}