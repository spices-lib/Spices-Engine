/**
* @file Renderer.cpp
* @brief The Renderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Renderer.h"
#include "Systems/SlateSystem.h"
#include "Resources/ResourcePool/ResourcePool.h"

namespace Spices {

	Renderer::Renderer
	(
		const std::string&                           rendererName          , 
		VulkanState&                                 vulkanState           ,
		const std::shared_ptr<VulkanDescriptorPool>& DescriptorPool        ,
		const std::shared_ptr<VulkanDevice>&         device                ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool  ,
		bool isLoadDefaultMaterial
	)
		: m_VulkanState             (vulkanState           )
		, m_DescriptorPool          (DescriptorPool        )
		, m_Device                  (device                )
		, m_RendererResourcePool    (rendererResourcePool  )
		, m_RendererName            (rendererName        )
	    , m_IsLoadDefaultMaterial   (isLoadDefaultMaterial )
	{}

	void Renderer::OnSystemInitialize()
	{
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

		/**
		* @brief Recreate RenderPass.
		*/
		CreateRendererPass();

		/**
		* @brief Create descriptorSet again.
		*/
		CreateDescriptorSet();
	}

	void Renderer::RegistryMaterial(const std::string& materialName, const std::string& subpassName)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a temp empty map for VkDescriptorSetLayout.
		* Before turn it to a continus container, sorted is required.
		*/
		std::map<uint32_t, VkDescriptorSetLayout> sortedRowSetLayouts;

		/**
		* @brief PreRenderer's DescriptorSetInfo.
		*/
		const auto preRendererSetInfo = DescriptorSetManager::GetByName("PreRenderer");
		for (auto& pair : preRendererSetInfo)
		{
			sortedRowSetLayouts[pair.first] = pair.second->GetRowSetLayout();
		}

		/**
		* @brief SpecificRenderer's DescriptorSetInfo.
		*/
		const auto specificRendererSetInfo = DescriptorSetManager::GetByName({ m_Pass->GetName(), subpassName});
		for (auto& pair : specificRendererSetInfo)
		{
			sortedRowSetLayouts[pair.first] = pair.second->GetRowSetLayout();
		}

		/**
		* @brief Material's DescriptorSetInfo.
		*/
		const auto material = ResourcePool<Material>::Load<Material>(materialName);
		const auto materialSetInfo = material->GetMaterialDescriptorSet();
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
		const auto& subPass = *m_Pass->GetSubPasses().find_value(subpassName);
		VkPipelineLayout pipelinelayout = CreatePipelineLayout(rowSetLayouts, subPass);

		/**
		* @brief Create Pipeline.
		*/
		const auto pipeline = CreatePipeline(material, pipelinelayout, subPass);
		m_Pipelines[materialName] = pipeline;
	}

	void Renderer::CreateDefaultMaterial() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter all subpass.
		*/
		if (m_IsLoadDefaultMaterial)
		{
			m_Pass->GetSubPasses().for_each([&](const auto& K, const auto& V) {

				std::stringstream ss;
				ss << m_RendererName << "." << K << ".Default";

				const auto material = ResourcePool<Material>::Load<Material>(ss.str());
				material->BuildMaterial();

				/**
				* @brief Not break loop.
				*/
				return false;
			});
		}
	}

	VkPipelineLayout Renderer::CreatePipelineLayout(
		const std::vector<VkDescriptorSetLayout>& rowSetLayouts , 
		std::shared_ptr<RendererSubPass>          subPass
	) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkPipelineLayoutCreateInfo.
		*/
		VkPipelineLayoutCreateInfo                         pipelineLayoutInfo{};
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
		VK_CHECK(vkCreatePipelineLayout(m_VulkanState.m_Device, &pipelineLayoutInfo, nullptr, &pipelineLayout))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_PIPELINE_LAYOUT, pipelineLayout, m_VulkanState.m_Device, "PipelineLayout");

		return pipelineLayout;
	}
	
	std::shared_ptr<VulkanPipeline> Renderer::CreatePipeline(
		std::shared_ptr<Material>        material , 
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

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
		pipelineConfig.colorBlendInfo.attachmentCount = static_cast<uint32_t>(subPass->GetColorBlend().size());
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

	std::tuple<glm::mat4, glm::mat4, unsigned int> Renderer::GetActiveCameraMatrix(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Init viewmatrix and projectionmatrix.
		*/
		glm::mat4 invViewMat       = glm::mat4(1.0f);
		glm::mat4 projectionMat    = glm::mat4(1.0f);
		unsigned int stableFrames  = 0;

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

			if (camComp.IsActive())
			{
				/**
				* @brief Viewmaterix is the inverse of camera's modelmatrix. 
				*/
				invViewMat    = transComp.GetModelMatrix();
				projectionMat = camComp.GetCamera()->GetPMatrix();
				stableFrames  = camComp.GetCamera()->GetStableFrames();

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

			SPICES_CORE_WARN(ss.str());
		}

		return std::make_tuple(invViewMat, projectionMat, stableFrames);
	}

	void Renderer::GetDirectionalLight(FrameInfo& frameInfo, std::array<SpicesShader::DirectionalLight, DIRECTIONALLIGHTBUFFERMAXNUM>& dLightBuffer)
	{
		SPICES_PROFILE_ZONE;

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

			SpicesShader::DirectionalLight directionalLight = dirlightComp.GetLight();
			directionalLight.rotationMatrix = transComp.GetRotateMatrix();
			dLightBuffer[index] = directionalLight;
			index++;
			return true;
		});

		/**
		* @brief End of DirectionalLightBuffer.
		*/
		dLightBuffer[index].intensity = -1000.0f;
	}

	void Renderer::GetDirectionalLightMatrix(FrameInfo& frameInfo, std::array<glm::mat4, MAX_DIRECTIONALLIGHT_NUM>& directionalLight)
	{
		SPICES_PROFILE_ZONE;

		for (int i = 0; i < MAX_DIRECTIONALLIGHT_NUM; i++)
		{
			directionalLight[i] = glm::mat4(1.0f);
		}

		TransformComponent camTranComp;
		float ratio;

		IterWorldComp<CameraComponent>(
			frameInfo,
			[&](
			int                  entityId,
			TransformComponent&  transComp,
			CameraComponent&     camComp
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
				tempComp.SetPosition(camTranComp.GetPosition());
				tempComp.SetRotation(camTranComp.GetRotation());

				const glm::mat4 view = tempComp.GetModelMatrix();
				const glm::mat4 projection = OtrhographicMatrix(-ratio * 30, ratio * 30, -1.0f * 30, 1.0f * 30, -100000.0f, 100000.0f);

				directionalLight[index] = projection * glm::inverse(view);
				index++;
				return false;
		});
	}

	void Renderer::GetPointLight(FrameInfo& frameInfo, std::array<SpicesShader::PointLight, POINTLIGHTBUFFERMAXNUM>& pLightBuffer)
	{
		SPICES_PROFILE_ZONE;

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

			SpicesShader::PointLight pointLight = plightComp.GetLight();
			pointLight.position = transComp.GetPosition();
			pLightBuffer[index] = pointLight;
			index++;
			return false;
		});

		/**
		* @brief End of PointLightBuffer.
		*/
		pLightBuffer[index].intensity = -1000.0f;
	}
	
	void Renderer::RenderBehaveBuilder::BindPipeline(const std::string& materialName, VkPipelineBindPoint  bindPoint)
	{
		SPICES_PROFILE_ZONE;

		m_Renderer->m_Pipelines[materialName]->Bind(m_CurrentFrame, bindPoint);
	}

	void Renderer::RenderBehaveBuilder::SetViewPort() const
	{
		/**
		* @brief Use Negative Viewport height filp here to handle axis difference.
		* Remember enable device extension (VK_KHR_MAINTENANCE1)
		*/
		VkViewport                   viewport {};
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
			const ImVec2 viewPortSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();

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
		VkRect2D                      scissor {};
		scissor.offset            = { 0, 0 };
		scissor.extent            = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize;

		/**
		* @brief Set VkRect2D.
		*/
		vkCmdSetScissor(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], 0, 1, &scissor);
	}

	void Renderer::RenderBehaveBuilder::BeginNextSubPass(const std::string& subpassName)
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().find_value(subpassName);

		VulkanDebugUtils::EndLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
		VulkanDebugUtils::BeginLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], m_HandledSubPass->GetName());

		vkCmdNextSubpass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaveBuilder::BeginRenderPass()
	{
		SPICES_PROFILE_ZONE;

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
			const ImVec2 size = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
			const VkExtent2D extent = { static_cast<uint32_t>(size.x) , static_cast<uint32_t>(size.y) };
			renderPassInfo.renderArea.extent    = extent;
		}

		renderPassInfo.clearValueCount          = static_cast<uint32_t>(m_Renderer->m_Pass->GetClearValues().size());
		renderPassInfo.pClearValues             = m_Renderer->m_Pass->GetClearValues().data();

		VulkanDebugUtils::BeginLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], m_Renderer->m_Pass->GetName());
		VulkanDebugUtils::BeginLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], m_HandledSubPass->GetName());

		vkCmdBeginRenderPass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaveBuilder::EndRenderPass() const
	{
		SPICES_PROFILE_ZONE;

		vkCmdEndRenderPass(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);

		VulkanDebugUtils::EndLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
		VulkanDebugUtils::EndLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
	}
	
	Renderer::RayTracingRenderBehaveBuilder::RayTracingRenderBehaveBuilder(
		Renderer* renderer     ,
		uint32_t  currentFrame ,
		uint32_t  currentImage
	)
		:RenderBehaveBuilder(renderer, currentFrame, currentImage)
	{
		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().first();
		vkCmdTraceRaysKHR = reinterpret_cast<PFN_vkCmdTraceRaysKHR>(vkGetInstanceProcAddr(renderer->m_VulkanState.m_Instance, "vkCmdTraceRaysKHR"));
	}

	void Renderer::RayTracingRenderBehaveBuilder::Recording(const std::string& caption)
	{
		VulkanDebugUtils::BeginLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame], caption);
	}

	void Renderer::RayTracingRenderBehaveBuilder::Endrecording()
	{
		VulkanDebugUtils::EndLabel(m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame]);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindPipeline(const std::string& materialName, VkPipelineBindPoint bindPoint)
	{
		RenderBehaveBuilder::BindPipeline(materialName, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, VkPipelineBindPoint bindPoint)
	{
		RenderBehaveBuilder::BindDescriptorSet(infos, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, const std::string& name, VkPipelineBindPoint bindPoint)
	{
		RenderBehaveBuilder::BindDescriptorSet(infos, name, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::TraceRays(
		const VkStridedDeviceAddressRegionKHR* rgenRegion ,
		const VkStridedDeviceAddressRegionKHR* missRegion ,
		const VkStridedDeviceAddressRegionKHR* hitRegion  ,
		const VkStridedDeviceAddressRegionKHR* callRegion
	)
	{
		const uint32_t width = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().x);
		const uint32_t height = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().y);

		/*
		* @attention Vulkan not allow dynamic state in mixing raytracing pipeline and custom graphic pipeline.
		* @see https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/8038.
		*/
		vkCmdTraceRaysKHR(
			m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
			rgenRegion,
			missRegion,
			hitRegion,
			callRegion,
			width,
			height,
			1
		);
	}

	void Renderer::RenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		BindDescriptorSet(infos, ss.str(), bindPoint);
	}

	void Renderer::RenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, const std::string& name, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter all desctiptorsets.
		*/
		for (const auto& pair : infos)
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
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (size_t i = 0; i < textureNames.size(); i++)
		{
			RendererResourceCreateInfo info;
			info.name                                = textureNames[i];
			info.type                                = type;
			info.width                               = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.width;
			info.height                              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height;
			info.description.samples                 = VK_SAMPLE_COUNT_1_BIT;
			info.description.format                  = format;
			info.usage                               = VK_IMAGE_USAGE_STORAGE_BIT;

			VkDescriptorImageInfo* imageInfo         = m_Renderer->m_RendererResourcePool->AccessResource(info);
			imageInfo->imageLayout                   = VK_IMAGE_LAYOUT_GENERAL;

			m_ImageInfos[set][binding].push_back(*imageInfo);
		}

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		const auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
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
		SPICES_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (size_t i = 0; i < textureNames.size(); i++)
		{
			RendererResourceCreateInfo resinfo;
			resinfo.name = textureNames[i];
			const auto info = m_Renderer->m_RendererResourcePool->AccessResource(resinfo);

			m_ImageInfos[set][binding].push_back(*info);
		}

		const auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
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
		SPICES_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (size_t i = 0; i < inputAttachmentNames.size(); i++)
		{
			RendererResourceCreateInfo resinfo;
			resinfo.name = inputAttachmentNames[i];
			const auto info = m_Renderer->m_RendererResourcePool->AccessResource(resinfo);

			m_ImageInfos[set][binding].push_back(*info);
		}

		const auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, stageFlags, static_cast<uint32_t>(inputAttachmentNames.size()));

		return *this;
	}

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddAccelerationStructure(
		uint32_t                         set                    , 
		uint32_t                         binding                , 
		VkShaderStageFlags               stageFlags
	)
	{
		SPICES_PROFILE_ZONE;

		const auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, stageFlags, 1);

		return *this;
	}

	void Renderer::DescriptorSetBuilder::Build(const VkAccelerationStructureKHR& accel)
	{
		SPICES_PROFILE_ZONE;

		const auto descriptorSets = DescriptorSetManager::GetByName(m_DescriptorSetId);

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
		SPICES_PROFILE_ZONE;

		m_Renderer->m_Pass = std::make_shared<RendererPass>(rendererPassName, m_Renderer->m_Device);
	}

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddSubPass(const std::string& subPassName)
	{
		SPICES_PROFILE_ZONE;

		const size_t size = m_Renderer->m_Pass->GetSubPasses().size();
		m_HandledRendererSubPass = m_Renderer->m_Pass->AddSubPass(subPassName, static_cast<uint32_t>(size));
		return *this;
	}

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::EndSubPass()
	{
		SPICES_PROFILE_ZONE;

		m_HandledRendererSubPass->BuildSubPassDescription();

		const size_t index = m_Renderer->m_Pass->GetSubPasses().size() - 1;
		m_HandledRendererSubPass->BuildSubPassDependency(static_cast<uint32_t>(index));
		return *this;
	}

	void Renderer::RendererPassBuilder::Build() const
	{
		SPICES_PROFILE_ZONE;

		m_Renderer->m_Pass->BuildRendererPass();
	}
}