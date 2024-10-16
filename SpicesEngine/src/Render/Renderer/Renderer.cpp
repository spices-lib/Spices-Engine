/**
* @file Renderer.cpp
* @brief The Renderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Renderer.h"
#include "Systems/SlateSystem.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	Renderer::Renderer
	(
		const std::string&                           rendererName          , 
		VulkanState&                                 vulkanState           ,
		const std::shared_ptr<VulkanDescriptorPool>& DescriptorPool        ,
		const std::shared_ptr<VulkanDevice>&         device                ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool  ,
		const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool         ,
		bool                                         isLoadDefaultMaterial ,
		bool                                         isRegistryDGCPipeline
	)
		: m_VulkanState             (vulkanState           )
		, m_DescriptorPool          (DescriptorPool        )
		, m_Device                  (device                )
		, m_RendererResourcePool    (rendererResourcePool  )
		, m_CmdThreadPool           (cmdThreadPool         )
		, m_RendererName            (rendererName          )
	    , m_IsLoadDefaultMaterial   (isLoadDefaultMaterial )
		, m_IsRegistryDGCPipeline   (isRegistryDGCPipeline )
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

		/**
		* @brief Create Device Generated Commands Layout.
		*/
		if (m_IsRegistryDGCPipeline)
		{
			CreateDeviceGeneratedCommandsLayout();
		}
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
		* @note remove for material use bindless descriptorset.
		*/
		const auto material = ResourcePool<Material>::Load<Material>(materialName, materialName);

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

	void Renderer::RegistryDGCPipeline(const std::string& materialName, const std::string& subpassName)
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
		* @note remove for material use bindless descriptorset.
		*/
		const auto material = ResourcePool<Material>::Load<Material>(materialName, materialName);

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
		std::stringstream ss;
		ss << materialName << ".DGC";
		m_Pipelines[ss.str()] = CreateDGCPipeline(ss.str(), materialName, pipelinelayout, subPass);
	}

	std::shared_ptr<Material> Renderer::GetDefaultMaterial(const std::string& subpassName) const
	{
		SPICES_PROFILE_ZONE;
		
		std::stringstream ss;
		ss << m_RendererName << "." << subpassName << ".Default";

		/**
		* @brief Real Material.
		*/
		return ResourcePool<Material>::Load<Material>(ss.str(), ss.str());
	}

	void Renderer::CreateDefaultMaterial()
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

				/**
				* @brief Registry Real Material.
				*/
				auto material = GetDefaultMaterial(K);
				material->BuildMaterial();
				
				/**
				* @brief Abstract Indirect Material.
				*/
				if (m_IsRegistryDGCPipeline)
				{
					RegistryDGCPipeline(ss.str(), K);
				}

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
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_PIPELINE_LAYOUT, (uint64_t)pipelineLayout, m_VulkanState.m_Device, "PipelineLayout")

		return pipelineLayout;
	}

	std::shared_ptr<VulkanPipeline> Renderer::CreateDGCPipeline(
		const std::string&               pipelineName ,
		const std::string&               materialName ,
		VkPipelineLayout&                layout       ,
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
		return std::make_shared<VulkanIndirectPipelineNV>(
			m_VulkanState,
			pipelineName,
			materialName,
			m_PipelinesRef[subPass->GetName()],
			pipelineConfig
		);

		m_PipelinesRef.clear();
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

	std::tuple<glm::mat4, glm::mat4, unsigned int, float> Renderer::GetActiveCameraMatrix(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Init viewmatrix and projectionmatrix.
		*/
		glm::mat4 invViewMat       = glm::mat4(1.0f);
		glm::mat4 projectionMat    = glm::mat4(1.0f);
		unsigned int stableFrames  = 0;
		float fov                  = 0.0f;

		bool find = false;

		/**
		* @brief Iter CameraComponent, finding a active camera.
		*/
		IterWorldCompWithBreak<CameraComponent>(
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
				projectionMat = camComp.GetCamera()->GetPMatrixReverseZ();
				stableFrames  = camComp.GetCamera()->GetStableFrames();
				fov           = glm::radians(camComp.GetCamera()->GetPerspectiveParam().fov);

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

		return std::make_tuple(invViewMat, projectionMat, stableFrames, fov);
	}

	void Renderer::GetDirectionalLight(FrameInfo& frameInfo, std::array<SpicesShader::DirectionalLight, DIRECTIONALLIGHT_BUFFER_MAXNUM>& dLightBuffer)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif Iter DirectionalLightComponent, and just use the first one.
		*/
		int index = 0;
		IterWorldCompWithBreak<DirectionalLightComponent>(
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
			return false;
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

		IterWorldCompWithBreak<CameraComponent>(
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
		IterWorldCompWithBreak<DirectionalLightComponent>(
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

	void Renderer::GetPointLight(FrameInfo& frameInfo, std::array<SpicesShader::PointLight, POINTLIGHT_BUFFER_MAXNUM>& pLightBuffer)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter PointLightComponent.
		*/
		int index = 0;
		IterWorldCompWithBreak<PointLightComponent>(
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
	
	void Renderer::RenderBehaveBuilder::Recording(const std::string& caption)
	{
		SPICES_PROFILE_ZONE;

		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, caption)
		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, caption, 1, m_CurrentFrame)
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, caption)
		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter Pass:" + m_Renderer->m_Pass->GetName())
	}

	void Renderer::RenderBehaveBuilder::Endrecording()
	{
		SPICES_PROFILE_ZONE;

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 1)
		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Leave Pass:" + m_Renderer->m_Pass->GetName())
	}

	void Renderer::RenderBehaveBuilder::Async(std::function<void(VkCommandBuffer& cmdBuffer)> func)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Submit Cmds to Thread Pool.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			func(cmdBuffer);
		});
	}

	void Renderer::RenderBehaveBuilder::BindPipeline(const std::string& materialName, VkCommandBuffer cmdBuffer, VkPipelineBindPoint  bindPoint)
	{
		SPICES_PROFILE_ZONE;

		vkCmdBindPipeline(cmdBuffer ? cmdBuffer : m_CommandBuffer, bindPoint, m_Renderer->m_Pipelines[materialName]->GetPipeline());
	}

	void Renderer::RenderBehaveBuilder::BindPipelineAsync(const std::string& materialName, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			vkCmdBindPipeline(cmdBuffer, bindPoint, m_Renderer->m_Pipelines[materialName]->GetPipeline());
		});
	}

	void Renderer::RenderBehaveBuilder::SetViewPort(VkCommandBuffer cmdBuffer) const
	{
		SPICES_PROFILE_ZONE;
		
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
		* @brief Instance a VkRect2D
		*/
		VkRect2D                      scissor{};
		scissor.offset              = { 0, 0 };
		scissor.extent              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize;
			
		/**
		* @brief Set VkViewport with viewport slate.
		*/
		vkCmdSetViewport(cmdBuffer ? cmdBuffer : m_CommandBuffer, 0, 1, &viewport);

		/**
		* @brief Set VkRect2D.
		*/
		vkCmdSetScissor(cmdBuffer ? cmdBuffer : m_CommandBuffer, 0, 1, &scissor);
	}

	void Renderer::RenderBehaveBuilder::SetViewPortAsync() const
	{
		SPICES_PROFILE_ZONE;
		
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
		* @brief Instance a VkRect2D
		*/
		VkRect2D                      scissor{};
		scissor.offset              = { 0, 0 };
		scissor.extent              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize;

		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			
			/**
			* @brief Set VkViewport with viewport slate.
			*/
			vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

			/**
			* @brief Set VkRect2D.
			*/
			vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);
		});
	}

	void Renderer::RenderBehaveBuilder::BeginNextSubPass(const std::string& subpassName)
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().find_value(subpassName);
		++m_SubpassIndex;
		m_HandledIndirectData = m_Renderer->m_IndirectData[subpassName];

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		//NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter SubPass:" + m_HandledSubPass->GetName())

		vkCmdNextSubpass(m_CommandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaveBuilder::BeginNextSubPassAsync(const std::string& subpassName)
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().find_value(subpassName);
		++m_SubpassIndex;
		m_HandledIndirectData = m_Renderer->m_IndirectData[subpassName];

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		//NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter SubPass:" + m_HandledSubPass->GetName())

		vkCmdNextSubpass(m_CommandBuffer, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
	}

	void Renderer::RenderBehaveBuilder::BeginRenderPass()
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().first();
		m_SubpassIndex = 0;
		m_HandledIndirectData = m_Renderer->m_IndirectData[m_HandledSubPass->GetName()];

		/**
		* @brief Instance a VkRenderPassBeginInfo.
		*/
		VkRenderPassBeginInfo                     renderPassInfo{};
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

		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_Renderer->m_Pass->GetName())
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_Renderer->m_Pass->GetName(), 1, m_CurrentFrame)
		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_Renderer->m_Pass->GetName())
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())

		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter Pass:" + m_Renderer->m_Pass->GetName())

		vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::RenderBehaveBuilder::BeginRenderPassAsync()
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().first();
		m_SubpassIndex = 0;
		m_HandledIndirectData = m_Renderer->m_IndirectData[m_HandledSubPass->GetName()];

		/**
		* @brief Instance a VkRenderPassBeginInfo.
		*/
		VkRenderPassBeginInfo                     renderPassInfo{};
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

		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_Renderer->m_Pass->GetName())
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_Renderer->m_Pass->GetName(), 1, m_CurrentFrame)
		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_Renderer->m_Pass->GetName())
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())
			
		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter Pass:" + m_Renderer->m_Pass->GetName())

		/**
		* @brief This command not allow async.
		*/
		vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
	}

	void Renderer::RenderBehaveBuilder::EndRenderPass() const
	{
		SPICES_PROFILE_ZONE;

		vkCmdEndRenderPass(m_CommandBuffer);

		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Leave Pass:" + m_Renderer->m_Pass->GetName())

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_ENDLABEL(m_CommandBuffer)

		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 1)

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
	}

	Renderer::RayTracingRenderBehaveBuilder::RayTracingRenderBehaveBuilder(
		Renderer* renderer     ,
		uint32_t  currentFrame ,
		uint32_t  currentImage
	)
		: RenderBehaveBuilder(renderer, currentFrame, currentImage)
	{
		SPICES_PROFILE_ZONE;
		
		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().first();
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindPipeline(const std::string& materialName, VkCommandBuffer cmdBuffer, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindPipeline(materialName, cmdBuffer, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindPipelineAsync(const std::string& materialName, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::BindPipelineAsync(materialName, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, VkCommandBuffer cmdBuffer, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindDescriptorSet(infos, cmdBuffer, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSetAsync(const DescriptorSetInfo& infos, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::BindDescriptorSetAsync(infos, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, const std::string& name, VkCommandBuffer cmdBuffer, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindDescriptorSet(infos, name, cmdBuffer, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSetAsync(const DescriptorSetInfo& infos, const std::string& name, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::BindDescriptorSetAsync(infos, name, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::TraceRays(
		const VkStridedDeviceAddressRegionKHR* rgenRegion ,
		const VkStridedDeviceAddressRegionKHR* missRegion ,
		const VkStridedDeviceAddressRegionKHR* hitRegion  ,
		const VkStridedDeviceAddressRegionKHR* callRegion
	)
	{
		SPICES_PROFILE_ZONE;
		
		const uint32_t width = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().x);
		const uint32_t height = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().y);

		/*
		* @attention Vulkan not allow dynamic state in mixing raytracing pipeline and custom graphic pipeline.
		* @see https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/8038.
		*/
		m_Renderer->m_VulkanState.m_VkFunc.vkCmdTraceRaysKHR(
			m_CommandBuffer,
			rgenRegion,
			missRegion,
			hitRegion,
			callRegion,
			width,
			height,
			1
		);
	}

	void Renderer::RayTracingRenderBehaveBuilder::TraceRaysAsync(
		const VkStridedDeviceAddressRegionKHR* rgenRegion , 
		const VkStridedDeviceAddressRegionKHR* missRegion , 
		const VkStridedDeviceAddressRegionKHR* hitRegion  , 
		const VkStridedDeviceAddressRegionKHR* callRegion
	)
	{
		SPICES_PROFILE_ZONE;

		const uint32_t width = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().x);
		const uint32_t height = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().y);

		/*
		* @attention Vulkan not allow dynamic state in mixing raytracing pipeline and custom graphic pipeline.
		* @see https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/8038.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			m_Renderer->m_VulkanState.m_VkFunc.vkCmdTraceRaysKHR(
				cmdBuffer,
				rgenRegion,
				missRegion,
				hitRegion,
				callRegion,
				width,
				height,
				1
			);
		});
	}

	void Renderer::RenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, VkCommandBuffer cmdBuffer, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		BindDescriptorSet(infos, ss.str(), cmdBuffer, bindPoint);
	}

	void Renderer::RenderBehaveBuilder::BindDescriptorSet(
		const DescriptorSetInfo& infos          , 
		const std::string&       name           , 
		VkCommandBuffer          cmdBuffer      ,
		VkPipelineBindPoint      bindPoint
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter all desctiptorsets.
		*/
		for (const auto& pair : infos)
		{
			vkCmdBindDescriptorSets(
				cmdBuffer ? cmdBuffer : m_CommandBuffer,
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

	void Renderer::RenderBehaveBuilder::BindDescriptorSetAsync(const DescriptorSetInfo& infos, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		BindDescriptorSetAsync(infos, ss.str(), bindPoint);
	}

	void Renderer::RenderBehaveBuilder::BindDescriptorSetAsync(
		const DescriptorSetInfo& infos    , 
		const std::string&       name     , 
		VkPipelineBindPoint      bindPoint
	)
	{
		SPICES_PROFILE_ZONE;

		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& secCmdBuffer) {

			/**
			* @brief Iter all desctiptorsets.
			*/
			for (const auto& pair : infos)
			{
				vkCmdBindDescriptorSets(
					secCmdBuffer,
					bindPoint,
					m_Renderer->m_Pipelines[name]->GetPipelineLayout(),
					pair.first,
					1,
					&pair.second->Get(),
					0,
					nullptr
				);
			}
		});
	}

	void Renderer::RenderBehaveBuilder::RunDGC(VkCommandBuffer cmdBuffer)
	{
		SPICES_PROFILE_ZONE;

		PreprocessDGC_NV(cmdBuffer);

		PipelineMemoryBarrier(
			VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV   ,
			VK_ACCESS_INDIRECT_COMMAND_READ_BIT         ,
			VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV ,
			VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT         ,
			cmdBuffer
		);

		ExecuteDGC_NV(cmdBuffer);
	}

	void Renderer::RenderBehaveBuilder::RunDGCAsync()
	{
		SPICES_PROFILE_ZONE;

		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			PreprocessDGC_NV(cmdBuffer);

			PipelineMemoryBarrier(
				VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV   ,
				VK_ACCESS_INDIRECT_COMMAND_READ_BIT         ,
				VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV ,
				VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT         ,
				cmdBuffer
			);

			ExecuteDGC_NV(cmdBuffer);
		});
	}

	void Renderer::RenderBehaveBuilder::PreprocessDGC_NV(VkCommandBuffer cmdBuffer)
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default.DGC";

		/**
		* @brief Call vkCmdPreprocessGeneratedCommandsNV.
		*/
		m_HandledIndirectData->PreprocessDGC(cmdBuffer ? cmdBuffer : m_CommandBuffer, m_Renderer->m_Pipelines[ss.str()]->GetPipeline());
	}

	void Renderer::RenderBehaveBuilder::PreprocessDGCAsync_NV()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default.DGC";

		/**
		* @brief Call vkCmdPreprocessGeneratedCommandsNV.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			m_HandledIndirectData->PreprocessDGC(cmdBuffer, m_Renderer->m_Pipelines[ss.str()]->GetPipeline());
		});
	}

	void Renderer::RenderBehaveBuilder::ExecuteDGC_NV(VkCommandBuffer cmdBuffer)
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default.DGC";

		/**
		* @brief Call vkCmdExecuteGeneratedCommandsNV.
		*/
		m_HandledIndirectData->ExecuteDGC(cmdBuffer ? cmdBuffer : m_CommandBuffer, m_Renderer->m_Pipelines[ss.str()]->GetPipeline());
	}

	void Renderer::RenderBehaveBuilder::ExecuteDGCAsync_NV()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default.DGC";

		/**
		* @brief Call vkCmdExecuteGeneratedCommandsNV.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			m_HandledIndirectData->ExecuteDGC(cmdBuffer, m_Renderer->m_Pipelines[ss.str()]->GetPipeline());
		});
	}

	void Renderer::RenderBehaveBuilder::DrawFullScreenTriangle(VkCommandBuffer cmdBuffer)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call vkCmdDraw.
		*/
		vkCmdDraw(cmdBuffer ? cmdBuffer : m_CommandBuffer, 3, 1, 0, 0);
	}

	void Renderer::RenderBehaveBuilder::DrawFullScreenTriangleAsync()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call vkCmdDraw.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
		});
	}

	void Renderer::RenderBehaveBuilder::PipelineMemoryBarrier(
		VkAccessFlags          srcAccessMask , 
		VkAccessFlags          dstAccessMask , 
		VkPipelineStageFlags   srcStageMask  , 
		VkPipelineStageFlags   dstStageMask  ,
		VkCommandBuffer        cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		/**
		* @brief Instance a VkMemoryBarrier.
		*/
		VkMemoryBarrier                         barrier{};
		barrier.sType                         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		barrier.srcAccessMask                 = srcAccessMask;
		barrier.dstAccessMask                 = dstAccessMask;

		/**
		* @brief Call vkCmdPipelineBarrier.
		*/
		vkCmdPipelineBarrier(
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
			srcStageMask,
			dstStageMask,
			VK_DEPENDENCY_BY_REGION_BIT,
			1, &barrier,
			0, nullptr,
			0, nullptr
		);
	}

	void Renderer::RenderBehaveBuilder::PipelineMemoryBarrierAsync(
		VkAccessFlags          srcAccessMask  , 
		VkAccessFlags          dstAccessMask  , 
		VkPipelineStageFlags   srcStageMask   , 
		VkPipelineStageFlags   dstStageMask
	)
	{
		SPICES_PROFILE_ZONE;
		
		/**
		* @brief Instance a VkMemoryBarrier.
		*/
		VkMemoryBarrier                         barrier{};
		barrier.sType                         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		barrier.srcAccessMask                 = srcAccessMask;
		barrier.dstAccessMask                 = dstAccessMask;

		/**
		* @brief Call vkCmdPipelineBarrier.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			vkCmdPipelineBarrier(
				cmdBuffer,
				srcStageMask,
				dstStageMask,
				VK_DEPENDENCY_BY_REGION_BIT,
				1, &barrier,
				0, nullptr,
				0, nullptr
			);
		});
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

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddPushConstant(uint64_t size)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call RendererSubPass::SetPushConstant().
		*/
		m_HandledSubPass->SetPushConstant([&](auto& range) {
			range.stageFlags = VK_SHADER_STAGE_ALL;
			range.offset     = 0;
			range.size       = size;
		});

		return *this;
	}

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddUniformBuffer(
		uint32_t           set       , 
		uint32_t           binding   , 
		uint64_t           size      , 
		VkShaderStageFlags stageFlags
	)
	{
		SPICES_PROFILE_ZONE;

		const UInt2 id(set, binding);

		/**
		* @brief Creating VulkanBuffer.
		*/
		m_HandledSubPass->GetBuffers(id) = std::make_shared<VulkanBuffer>(
			m_Renderer->m_VulkanState,
			"UniformBuffer",
			size,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		/**
		* @brief fill in bufferInfos.
		*/
		m_BufferInfos[set][binding] = *m_HandledSubPass->GetBuffers(id)->GetBufferInfo();

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags, 1);

		return *this;
	}

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddStorageBuffer(
		uint32_t           set        , 
		uint32_t           binding    ,
		uint64_t           size       , 
		VkShaderStageFlags stageFlags
	)
	{
		SPICES_PROFILE_ZONE;

		const UInt2 id(set, binding);

		/**
		* @brief Creating VulkanBuffer.
		*/
		m_HandledSubPass->GetBuffers(id) = std::make_shared<VulkanBuffer>(
			m_Renderer->m_VulkanState,
			"StorageBuffer",
			size,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		/**
		* @brief fill in bufferInfos.
		*/
		m_BufferInfos[set][binding] = *m_HandledSubPass->GetBuffers(id)->GetBufferInfo();

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, stageFlags, 1);

		return *this;
	}

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddStorageBuffer(
		uint32_t                      set        , 
		uint32_t                      binding    , 
		VkShaderStageFlags            stageFlags , 
		std::shared_ptr<VulkanBuffer> buffer
	)
	{
		SPICES_PROFILE_ZONE;

		const UInt2 id(set, binding);

		/**
		* @brief Creating VulkanBuffer.
		*/
		m_HandledSubPass->GetBuffers(id) = buffer;

		/**
		* @brief fill in bufferInfos.
		*/
		m_BufferInfos[set][binding] = *m_HandledSubPass->GetBuffers(id)->GetBufferInfo();

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, stageFlags, 1);

		return *this;
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
		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
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

		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
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

		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
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

		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
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
			* @brief UpdateDescriptorSet, skip bindless texture set.
			*/
			if (pair.first == BINDLESS_TEXTURE_SET)
			{
				if (!pair.second->Get())
				{
					pair.second->BuildBindLessTextureDescriptorSet(m_HandledSubPass->GetName());
				}

				pair.second->UpdateBindLessTextureDescriptorSet(m_ImageInfos[pair.first]);
				continue;
			}

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

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddSelfDependency(
		VkAccessFlags        srcAccessMask , 
		VkAccessFlags        dstAccessMask , 
		VkPipelineStageFlags srcStageMask  , 
		VkPipelineStageFlags dstStageMask
	)
	{
		SPICES_PROFILE_ZONE;

		uint32_t index = m_HandledRendererSubPass->GetIndex();
		m_HandledRendererSubPass->AddSubPassDependency(index, index, srcAccessMask, dstAccessMask, srcStageMask, dstStageMask);

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

	Renderer::ComputeRenderBehaveBuilder::ComputeRenderBehaveBuilder(
		Renderer* renderer      , 
		uint32_t  currentFrame  , 
		uint32_t  currentImage
	)
		: RenderBehaveBuilder(renderer, currentFrame, currentImage)
	{
		SPICES_PROFILE_ZONE;
		
		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses().first();
		m_CommandBuffer = m_Renderer->m_VulkanState.m_ComputeCommandBuffer[currentFrame];
	}

	void Renderer::ComputeRenderBehaveBuilder::BindPipeline(const std::string& materialName, VkCommandBuffer cmdBuffer, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindPipeline(materialName, cmdBuffer, bindPoint);
	}

	void Renderer::ComputeRenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, VkCommandBuffer cmdBuffer, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindDescriptorSet(infos, cmdBuffer, bindPoint);
	}

	void Renderer::ComputeRenderBehaveBuilder::BindDescriptorSet(const DescriptorSetInfo& infos, const std::string& name, VkCommandBuffer cmdBuffer, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindDescriptorSet(infos, name, cmdBuffer, bindPoint);
	}

	void Renderer::ComputeRenderBehaveBuilder::Dispatch(uint32_t x, uint32_t y, uint32_t z)
	{
		SPICES_PROFILE_ZONE;
		
		vkCmdDispatch(m_CommandBuffer, x, y, z);
	}

	/*void Renderer::ComputeRenderBehaveBuilder::AddBarriers(
		VkBuffer              buffer        , 
		VkAccessFlags         srcAccessMask , 
		VkAccessFlags         dstAccessMask , 
		VkPipelineStageFlags  srcStageMask  , 
		VkPipelineStageFlags  dstStageMask
	)
	{
		SPICES_PROFILE_ZONE;
		
		VkBufferMemoryBarrier                   bufferBarrier {};
		bufferBarrier.sType                   = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		bufferBarrier.srcAccessMask           = srcAccessMask;
		bufferBarrier.dstAccessMask           = dstAccessMask;
		bufferBarrier.srcQueueFamilyIndex     = m_Renderer->m_VulkanState.m_GraphicQueueFamily;
		bufferBarrier.dstQueueFamilyIndex     = m_Renderer->m_VulkanState.m_ComputeQueueFamily;
		bufferBarrier.size                    = VK_WHOLE_SIZE;
		bufferBarrier.buffer                  = buffer;

		vkCmdPipelineBarrier(
			m_CommandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			1, &bufferBarrier,
			0, nullptr
		);
	}

	void Renderer::ComputeRenderBehaveBuilder::ReleaseBarriers(
		VkBuffer              buffer        , 
		VkAccessFlags         srcAccessMask , 
		VkAccessFlags         dstAccessMask , 
		VkPipelineStageFlags  srcStageMask  , 
		VkPipelineStageFlags  dstStageMask
	)
	{
		SPICES_PROFILE_ZONE;
		
		VkBufferMemoryBarrier                   bufferBarrier {};
		bufferBarrier.sType                   = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		bufferBarrier.srcAccessMask           = srcAccessMask;
		bufferBarrier.dstAccessMask           = dstAccessMask;
		bufferBarrier.srcQueueFamilyIndex     = m_Renderer->m_VulkanState.m_ComputeQueueFamily;
		bufferBarrier.dstQueueFamilyIndex     = m_Renderer->m_VulkanState.m_GraphicQueueFamily;
		bufferBarrier.size                    = VK_WHOLE_SIZE;
		bufferBarrier.buffer                  = buffer;

		vkCmdPipelineBarrier(
			m_CommandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			1, &bufferBarrier,
			0, nullptr
		);
	}

	void Renderer::ComputeRenderBehaveBuilder::InternalBarriers(
		VkBuffer              buffer        , 
		VkAccessFlags         srcAccessMask , 
		VkAccessFlags         dstAccessMask , 
		VkPipelineStageFlags  srcStageMask  , 
		VkPipelineStageFlags  dstStageMask
	)
	{
		SPICES_PROFILE_ZONE;
		
		VkBufferMemoryBarrier                   bufferBarrier {};
		bufferBarrier.sType                   = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		bufferBarrier.srcAccessMask           = srcAccessMask;
		bufferBarrier.dstAccessMask           = dstAccessMask;
		bufferBarrier.srcQueueFamilyIndex     = VK_QUEUE_FAMILY_IGNORED;
		bufferBarrier.dstQueueFamilyIndex     = VK_QUEUE_FAMILY_IGNORED;
		bufferBarrier.size                    = VK_WHOLE_SIZE;
		bufferBarrier.buffer                  = buffer;

		vkCmdPipelineBarrier(
			m_CommandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			1, &bufferBarrier,
			0, nullptr
		);
	}*/

	Renderer::DGCLayoutBuilder::DGCLayoutBuilder(
		const std::string& subPassName , 
		Renderer*          renderer
	)
		: m_Renderer(renderer)
		, m_SubpassName(subPassName)
	{
		SPICES_PROFILE_ZONE;

		m_InputInfos.clear();
		if (!m_Renderer->m_IndirectData[subPassName])
		{
			m_Renderer->m_IndirectData[subPassName] = std::make_shared<VulkanIndirectDrawNV>(m_Renderer->m_VulkanState);
		}

		m_Renderer->m_IndirectData[subPassName]->ResetCommandsLayout();
		m_HandledIndirectData = m_Renderer->m_IndirectData[subPassName];
	}

	Renderer::DGCLayoutBuilder& Renderer::DGCLayoutBuilder::AddShaderGroupInput()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkIndirectCommandsLayoutTokenNV.
		*/
		VkIndirectCommandsLayoutTokenNV      input{};
		input.sType                        = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
		input.tokenType                    = VK_INDIRECT_COMMANDS_TOKEN_TYPE_SHADER_GROUP_NV;
											   
		input.stream                       = static_cast<uint32_t>(m_InputInfos.size());
		input.offset                       = 0;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledIndirectData->AddInputStride(sizeof(VkBindShaderGroupIndirectCommandNV));

		return *this;
	}

	Renderer::DGCLayoutBuilder& Renderer::DGCLayoutBuilder::AddVertexBufferInput()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkIndirectCommandsLayoutTokenNV.
		*/
		VkIndirectCommandsLayoutTokenNV      input{};
		input.sType                        = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
		input.tokenType                    = VK_INDIRECT_COMMANDS_TOKEN_TYPE_VERTEX_BUFFER_NV;
		
		input.vertexBindingUnit            = 0;
		input.vertexDynamicStride          = VK_FALSE;

		input.stream                       = static_cast<uint32_t>(m_InputInfos.size());
		input.offset                       = 0;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledIndirectData->AddInputStride(sizeof(VkBindVertexBufferIndirectCommandNV));

		return *this;
	}

	Renderer::DGCLayoutBuilder& Renderer::DGCLayoutBuilder::AddIndexBufferInput()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkIndirectCommandsLayoutTokenNV.
		*/
		VkIndirectCommandsLayoutTokenNV      input{};
		input.sType                        = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
		input.tokenType                    = VK_INDIRECT_COMMANDS_TOKEN_TYPE_INDEX_BUFFER_NV;

		input.stream                       = static_cast<uint32_t>(m_InputInfos.size());
		input.offset                       = 0;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledIndirectData->AddInputStride(sizeof(VkBindIndexBufferIndirectCommandNV));

		return *this;
	}

	Renderer::DGCLayoutBuilder& Renderer::DGCLayoutBuilder::AddPushConstantInput()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_SubpassName << ".Default";

		/**
		* @brief Instance a VkIndirectCommandsLayoutTokenNV.
		*/
		VkIndirectCommandsLayoutTokenNV      input{}; 
		input.sType                        = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
		input.tokenType                    = VK_INDIRECT_COMMANDS_TOKEN_TYPE_PUSH_CONSTANT_NV;

		input.pushconstantPipelineLayout   = m_Renderer->m_Pipelines[ss.str()]->GetPipelineLayout();
		input.pushconstantShaderStageFlags = VK_SHADER_STAGE_ALL;
		input.pushconstantOffset           = 0;
		input.pushconstantSize             = sizeof(VkDeviceAddress);

		input.stream                       = static_cast<uint32_t>(m_InputInfos.size());
		input.offset                       = 0;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledIndirectData->AddInputStride(sizeof(VkDeviceAddress));

		return *this;
	}

	Renderer::DGCLayoutBuilder& Renderer::DGCLayoutBuilder::AddDrawIndexedInput()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkIndirectCommandsLayoutTokenNV.
		*/
		VkIndirectCommandsLayoutTokenNV      input{};
		input.sType                        = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
		input.tokenType                    = VK_INDIRECT_COMMANDS_TOKEN_TYPE_DRAW_INDEXED_NV;

		input.stream                       = static_cast<uint32_t>(m_InputInfos.size());
		input.offset                       = 0;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledIndirectData->AddInputStride(sizeof(VkDrawIndexedIndirectCommand));

		return *this;
	}

	Renderer::DGCLayoutBuilder& Renderer::DGCLayoutBuilder::AddDrawMeshTaskInput()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkIndirectCommandsLayoutTokenNV.
		*/
		VkIndirectCommandsLayoutTokenNV      input{};
		input.sType                        = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV; 
		input.tokenType                    = VK_INDIRECT_COMMANDS_TOKEN_TYPE_DRAW_TASKS_NV;
							                   
		input.stream                       = static_cast<uint32_t>(m_InputInfos.size());
		input.offset                       = 0;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledIndirectData->AddInputStride(sizeof(VkDrawMeshTasksIndirectCommandNV));

		return *this;
	}

	void Renderer::DGCLayoutBuilder::Build()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create IndirectCommandsLayout.
		*/
		m_HandledIndirectData->BuildCommandLayout(m_InputInfos);
	}
}