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
		bool                                         isLoadDefaultMaterial
	)
		: m_VulkanState             (vulkanState           )
		, m_DescriptorPool          (DescriptorPool        )
		, m_Device                  (device                )
		, m_RendererResourcePool    (rendererResourcePool  )
		, m_RendererName            (rendererName          )
	    , m_IsLoadDefaultMaterial   (isLoadDefaultMaterial )
		, m_IsActive                (false)
		, m_DescriptorSetCombine    (DescriptorSetManager::ALL)
	{
		SPICES_PROFILE_ZONE;

		m_RenderCache = std::make_shared<RendererCache>();
	}

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
		if (m_IsLoadDefaultMaterial)
		{
			CreateDefaultMaterial();
		}
		
		/**
		* @brief Create Device Generated Commands Layout.
		*/
		CreateDeviceGeneratedCommandsLayout();
	}
	
	void Renderer::OnSlateResize()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Clear unused caches.
		*/
		m_RenderCache->ClearCaches();

		/**
		* @brief Recreate RenderPass.
		*/
		CreateRendererPass();

		/**
		* @brief Create descriptorSet again.
		*/
		CreateDescriptorSet();
	}

	void Renderer::OnMeshAddedWorld()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Clear unused caches.
		*/
		m_RenderCache->ClearCaches();
	}

	void Renderer::RegistryMaterial(const std::string& materialName, const std::string& subPassName)
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
		if (m_DescriptorSetCombine & DescriptorSetManager::PreRenderer)
		{
			const auto preRendererSetInfo = DescriptorSetManager::GetByName("PreRenderer");
			for (auto& pair : preRendererSetInfo)
			{
				sortedRowSetLayouts[pair.first] = pair.second->GetRowSetLayout();
			}
		}

		/**
		* @brief SpecificRenderer's DescriptorSetInfo.
		*/
		if (m_DescriptorSetCombine & DescriptorSetManager::ThisRenderer)
		{
			const auto specificRendererSetInfo = DescriptorSetManager::GetByName({ m_Pass->GetName(), subPassName });
			for (auto& pair : specificRendererSetInfo)
			{
				sortedRowSetLayouts[pair.first] = pair.second->GetRowSetLayout();
			}
		}

		/**
		* @brief Material's DescriptorSetInfo.
		* @note remove for material use bindless descriptorset.
		*/
		const auto material = ResourcePool<Material>::Load<Material>(materialName, materialName);
		if (m_DescriptorSetCombine & DescriptorSetManager::Material)
		{
			// Not Implementated.
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
		const auto subPass = *m_Pass->GetSubPasses()->find_value(subPassName);
		VkPipelineLayout pipelineLayout = CreatePipelineLayout(rowSetLayouts, subPass);

		/**
		* @brief Create Pipeline.
		*/
		CreatePipeline(material, pipelineLayout, subPass);
	}

	void Renderer::RegistryDGCPipeline(const std::string& materialName, const std::string& subPassName, VulkanDeviceGeneratedCommandsNV* indirectPtr)
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
		const auto specificRendererSetInfo = DescriptorSetManager::GetByName({ m_Pass->GetName(), subPassName});
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
		const auto& subPass = *m_Pass->GetSubPasses()->find_value(subPassName);
		VkPipelineLayout pipelineLayout = CreatePipelineLayout(rowSetLayouts, subPass);

		/**
		* @brief Create Pipeline.
		*/
		std::stringstream ss;
		ss << materialName << ".DGC";
		CreateDeviceGeneratedCommandsPipeline(ss.str(), materialName, pipelineLayout, subPass, indirectPtr);
	}

	std::shared_ptr<Material> Renderer::GetDefaultMaterial(const std::string& subPassName) const
	{
		SPICES_PROFILE_ZONE;
		
		std::stringstream ss;
		ss << m_RendererName << "." << subPassName << ".Default";

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
		m_Pass->GetSubPasses()->for_each([&](const auto& K, const auto& V) {

			std::stringstream ss;
			ss << m_RendererName << "." << K << ".Default";

			/**
			* @brief Registry Real Material.
			*/
			auto material = GetDefaultMaterial(K);
			material->BuildMaterial();

			/**
			* @brief Not break loop.
			*/
			return false;
		});
	}

	void Renderer::CreateDGCMaterial(const std::string& subPass, VulkanDeviceGeneratedCommandsNV* indirectPtr)
	{
		SPICES_PROFILE_ZONE;
		
		std::stringstream ss;
		ss << m_RendererName << "." << subPass << ".Default";

		/**
		* @brief Registry DGC Pipeline.
		*/
		RegistryDGCPipeline(ss.str(), subPass, indirectPtr);
	}

	VkPipelineLayout Renderer::CreatePipelineLayout(
		const std::vector<VkDescriptorSetLayout>& rowSetLayouts ,
		const std::shared_ptr<RendererSubPass>&   subPass
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
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_PIPELINE_LAYOUT, reinterpret_cast<uint64_t>(pipelineLayout), m_VulkanState.m_Device, "PipelineLayout")

		return pipelineLayout;
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

			SPICES_CORE_WARN(ss.str())
		}

		return std::make_tuple(invViewMat, projectionMat, stableFrames, fov);
	}

	void Renderer::GetDirectionalLight(FrameInfo& frameInfo, std::array<SpicesShader::DirectionalLight, SpicesShader::DIRECTIONALLIGHT_BUFFER_MAXNUM>& dLightBuffer)
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
			const TransformComponent&    tranComp, 
			DirectionalLightComponent&   dirlightComp
			) {

			SpicesShader::DirectionalLight directionalLight = dirlightComp.GetLight();
			directionalLight.rotationMatrix = tranComp.GetRotateMatrix();
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

		for (uint32_t i = 0; i < MAX_DIRECTIONALLIGHT_NUM; i++)
		{
			directionalLight[i] = glm::mat4(1.0f);
		}

		TransformComponent camTranComp;
		float ratio;

		IterWorldCompWithBreak<CameraComponent>(
			frameInfo,
			[&](
			int                  entityId,
			TransformComponent&  tranComp,
			CameraComponent&     camComp
			) {
				camTranComp = tranComp;
				ratio = camComp.GetCamera()->GetAspectRatio();
				return true;
		});

		int index = 0;
		IterWorldCompWithBreak<DirectionalLightComponent>(
			frameInfo,
			[&](
			int                          entityId    ,
			TransformComponent&          tranComp   ,
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

	void Renderer::GetPointLight(FrameInfo& frameInfo, std::array<SpicesShader::PointLight, SpicesShader::POINTLIGHT_BUFFER_MAXNUM>& pLightBuffer)
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
			TransformComponent&   tranComp, 
			PointLightComponent&  plightComp
			) {

			SpicesShader::PointLight pointLight = plightComp.GetLight();
			pointLight.position = tranComp.GetPosition();
			pLightBuffer[index] = pointLight;
			index++;
			return false;
		});

		/**
		* @brief End of PointLightBuffer.
		*/
		pLightBuffer[index].intensity = -1000.0f;
	}
	
	std::shared_ptr<scl::behave_state_list<void, Renderer::RenderBehaveBuilder*, VkCommandBuffer>> Renderer::m_StatisticsStateList;

	Renderer::RenderBehaveBuilder::RenderBehaveBuilder(Renderer* renderer, uint32_t currentFrame, uint32_t currentImage)
		: m_Renderer(renderer)
		, m_CurrentFrame(currentFrame)
		, m_CurrentImage(currentImage)
	{
		SPICES_PROFILE_ZONE;

		m_Renderer->m_IsActive = true;
		m_CommandBuffer = m_Renderer->m_VulkanState.m_GraphicCommandBuffer[currentFrame];

		/**
		* @brief Create Statistics state list.
		*/
		if (!m_Renderer->m_StatisticsStateList)
		{
			m_Renderer->m_StatisticsStateList = std::make_shared<scl::behave_state_list<void, RenderBehaveBuilder*, VkCommandBuffer>>();

			// Do nothing.
			{
				const auto state = m_Renderer->m_StatisticsStateList->AddNode();

				state->PushBehave("EndRenderer", nullptr);
				state->PushBehave("BeginStatistics", nullptr);
				state->PushBehave("EndStatistics", nullptr);
			}

			// Query Statistics item.
			{
				for(int i = 0; i < Querier::Max; i++)
				{
					const auto state = m_Renderer->m_StatisticsStateList->AddNode();

					state->PushBehave("EndRenderer", nullptr);
					state->PushBehave("BeginStatistics", [=](RenderBehaveBuilder* builder, VkCommandBuffer commandBuffer) {
						builder->GetStatisticsRendererPass()->BeginStatistics(commandBuffer, static_cast<Querier::StatisticsBits>(1 << i));
					});
					state->PushBehave("EndStatistics", [=](RenderBehaveBuilder* builder, VkCommandBuffer commandBuffer) {
						builder->GetStatisticsRendererPass()->EndStatistics(commandBuffer, static_cast<Querier::StatisticsBits>(1 << i));
					});
				}
			}

			// Submit store task to threadPool.
			{
				const auto state = m_Renderer->m_StatisticsStateList->AddNode();

				state->PushBehave("EndRenderer", [](RenderBehaveBuilder* builder, VkCommandBuffer commandBuffer) {
					AsyncTask(ThreadPoolEnum::Custom, [&](std::shared_ptr<RendererSubPass> subPass) { 
						subPass->StoreStatistics(); 
					}, builder->GetStatisticsRendererPass());
				});
				state->PushBehave("BeginStatistics", nullptr);
				state->PushBehave("EndStatistics", nullptr);
			}

			// Do nothing.
			{
				const auto state = m_Renderer->m_StatisticsStateList->AddNode();

				state->PushBehave("EndRenderer", nullptr);
				state->PushBehave("BeginStatistics", nullptr);
				state->PushBehave("EndStatistics", nullptr);
			}

			m_Renderer->m_StatisticsStateList->SetState(Querier::Max + 2);
		}
	}

	std::shared_ptr<RendererSubPass>& Renderer::RenderBehaveBuilder::GetStatisticsRendererPass()
	{
		SPICES_PROFILE_ZONE;

		return *m_Renderer->m_Pass->GetSubPasses()->first();
	}

	void Renderer::RenderBehaveBuilder::Recording(const std::string& caption) const
	{
		SPICES_PROFILE_ZONE;

		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, caption)
		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, caption, 1, m_CurrentFrame)
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, caption)
		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter Pass:" + caption)
	}

	void Renderer::RenderBehaveBuilder::EndRecording() const
	{
		SPICES_PROFILE_ZONE;

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 1)
		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Leave Pass")
	}

	std::future<VkCommandBuffer> Renderer::RenderBehaveBuilder::Async(std::function<void(const VkCommandBuffer& cmdBuffer)> func) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Submit Cmds to Thread Pool.
		*/
		return m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [=](const VkCommandBuffer& cmdBuffer) {
			func(cmdBuffer);
		});
	}

	void Renderer::RenderBehaveBuilder::Await(std::function<void(const VkCommandBuffer& cmdBuffer)> func) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Submit Cmds to Thread Pool.
		*/
		std::future<VkCommandBuffer> futureCmdBuffer = m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
			func(cmdBuffer);
		});

		/**
		* @brief Wait for merge.
		*/
		Wait(futureCmdBuffer);
	}

	void Renderer::RenderBehaveBuilder::Wait(std::future<VkCommandBuffer>& futureCmdBuffer) const
	{
		SPICES_PROFILE_ZONE;

		const VkCommandBuffer buffers = futureCmdBuffer.get();

		/**
		* @brief Merge secondary commandbuffer to main commandbuffer.
		*/
		vkCmdExecuteCommands(m_CommandBuffer, 1, &buffers);
	}

	void Renderer::RenderBehaveBuilder::Wait(std::vector<std::future<VkCommandBuffer>>& futureCmdBuffers) const
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkCommandBuffer> buffers(futureCmdBuffers.size());

		for (size_t i = 0; i < buffers.size(); i++)
		{
			buffers[i] = futureCmdBuffers[i].get();
		}
		
		/**
		* @brief Merge secondary commandbuffer to main commandbuffer.
		*/
		vkCmdExecuteCommands(m_CommandBuffer, buffers.size(), buffers.data());
	}

	void Renderer::RenderBehaveBuilder::BindPipeline(const std::string& materialName, VkCommandBuffer cmdBuffer, VkPipelineBindPoint  bindPoint)
	{
		SPICES_PROFILE_ZONE;

		vkCmdBindPipeline(
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
			bindPoint,
			m_Renderer->m_Pipelines.Find(materialName)->GetPipeline()
		);
	}

	void Renderer::RenderBehaveBuilder::BindPipeline(VkPipeline pipeline, VkCommandBuffer cmdBuffer, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		vkCmdBindPipeline(
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
			bindPoint,
			pipeline
		);
	}

	void Renderer::RenderBehaveBuilder::BindPipelineAsync(const std::string& materialName, VkPipelineBindPoint bindPoint)
	{
		SPICES_PROFILE_ZONE;

		m_Renderer->SubmitCmdsParallel(
		m_CommandBuffer,
		m_SubPassIndex,
		[&](const VkCommandBuffer& cmdBuffer)
		{
			vkCmdBindPipeline(
				cmdBuffer,
				bindPoint,
				m_Renderer->m_Pipelines.Find(materialName)->GetPipeline()
			);
		});
	}

	void Renderer::RenderBehaveBuilder::SetViewPort(const VkCommandBuffer& cmdBuffer) const
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
		* @brief Instance a VkRect2D.
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

		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
			
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

	void Renderer::RenderBehaveBuilder::BeginNextSubPass(const std::string& subPassName)
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->find_value(subPassName);
		++m_SubPassIndex;
		m_HandledDGCData = m_Renderer->m_DGCData[subPassName];

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		//NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter SubPass:" + m_HandledSubPass->GetName())

		vkCmdNextSubpass(m_CommandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::ComputeRenderBehaveBuilder::BeginNextSubPass(const std::string& subPassName)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief End RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_ENDSTATISTICS(this, m_CommandBuffer)
		RENDERPASS_STATISTICS_ENDRENDERER(this)

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->find_value(subPassName);
		++m_SubPassIndex;
		m_HandledDGCData = m_Renderer->m_DGCData[subPassName];

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		//NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter SubPass:" + m_HandledSubPass->GetName())

		/**
		* @brief Begin RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_BEGINSTATISTICS(this, m_CommandBuffer)
	}

	void Renderer::RenderBehaveBuilder::BeginNextSubPassAsync(const std::string& subPassName)
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->find_value(subPassName);
		++m_SubPassIndex;
		m_HandledDGCData = m_Renderer->m_DGCData[subPassName];

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

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->first();
		m_SubPassIndex = 0;
		m_HandledDGCData = m_Renderer->m_DGCData[m_HandledSubPass->GetName()];

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
		* @brief Begin RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_BEGINSTATISTICS(this, m_CommandBuffer)

		vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	std::shared_ptr<RendererSubPass>& Renderer::ComputeRenderBehaveBuilder::GetStatisticsRendererPass()
	{
		SPICES_PROFILE_ZONE;

		return m_HandledSubPass;
	}

	void Renderer::ComputeRenderBehaveBuilder::BeginRenderPass()
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->first();
		m_SubPassIndex = 0;
		m_HandledDGCData = m_Renderer->m_DGCData[m_HandledSubPass->GetName()];

		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_Renderer->m_Pass->GetName())
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_Renderer->m_Pass->GetName(), 1, m_CurrentFrame)
		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_Renderer->m_Pass->GetName())
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())

		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter Pass:" + m_Renderer->m_Pass->GetName())

		/**
		* @brief Begin RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_BEGINSTATISTICS(this, m_CommandBuffer)
	}

	void Renderer::RenderBehaveBuilder::BeginRenderPassAsync()
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->first();
		m_SubPassIndex = 0;
		m_HandledDGCData = m_Renderer->m_DGCData[m_HandledSubPass->GetName()];

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
		* @brief Begin RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_BEGINSTATISTICS(this, m_CommandBuffer)

		/**
		* @brief This command not allow async.
		*/
		vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
	}

	void Renderer::RenderBehaveBuilder::EndRenderPass()
	{
		SPICES_PROFILE_ZONE;

		vkCmdEndRenderPass(m_CommandBuffer);

		/**
		* @brief End RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_ENDSTATISTICS(this, m_CommandBuffer)
		RENDERPASS_STATISTICS_ENDRENDERER(this)

		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Leave Pass:" + m_Renderer->m_Pass->GetName())

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_ENDLABEL(m_CommandBuffer)

		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 1)

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
	}

	void Renderer::ComputeRenderBehaveBuilder::EndRenderPass()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief End RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_ENDSTATISTICS(this, m_CommandBuffer)
		RENDERPASS_STATISTICS_ENDRENDERER(this)

		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Leave Pass:" + m_Renderer->m_Pass->GetName())

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_ENDLABEL(m_CommandBuffer)

		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 1)

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
	}

	Renderer::RayTracingRenderBehaveBuilder::RayTracingRenderBehaveBuilder(
		Renderer*         renderer     ,
		VulkanRayTracing* rayTracing   ,
		uint32_t          currentFrame ,
		uint32_t          currentImage
	)
		: RenderBehaveBuilder(renderer, currentFrame, currentImage)
		, m_VulkanRayTracing(rayTracing)
	{
		SPICES_PROFILE_ZONE;
		
		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->first();
	}

	std::shared_ptr<RendererSubPass>& Renderer::RayTracingRenderBehaveBuilder::GetStatisticsRendererPass()
	{
		SPICES_PROFILE_ZONE;

		return m_HandledSubPass;
	}

	void Renderer::RayTracingRenderBehaveBuilder::BeginRenderPass()
	{
		SPICES_PROFILE_ZONE;

		m_HandledSubPass      = *m_Renderer->m_Pass->GetSubPasses()->first();
		m_SubPassIndex        = 0;
		m_HandledDGCData      = m_Renderer->m_DGCData[m_HandledSubPass->GetName()];

		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_Renderer->m_Pass->GetName())
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_Renderer->m_Pass->GetName(), 1, m_CurrentFrame)
		NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_Renderer->m_Pass->GetName())
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())

		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter Pass:" + m_Renderer->m_Pass->GetName())

		/**
		* @brief Begin RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_BEGINSTATISTICS(this, m_CommandBuffer)
	}

	void Renderer::RayTracingRenderBehaveBuilder::EndRenderPass()
	{
		/**
		* @brief End RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_ENDSTATISTICS(this, m_CommandBuffer)
		RENDERPASS_STATISTICS_ENDRENDERER(this)

		NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Leave Pass:" + m_Renderer->m_Pass->GetName())

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_ENDLABEL(m_CommandBuffer)

		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 1)

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
	}

	void Renderer::RayTracingRenderBehaveBuilder::BeginNextSubPass(const std::string& subPassName)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief End RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_ENDSTATISTICS(this, m_CommandBuffer)
		RENDERPASS_STATISTICS_ENDRENDERER(this)

		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->find_value(subPassName);
		++m_SubPassIndex;
		m_HandledDGCData = m_Renderer->m_DGCData[subPassName];

		NSIGHTPERF_GPUPROFILERREPORT_POPRANGE(m_CommandBuffer)
		NSIGHTPERF_GPUPROFILERREPORF_PUSHRANGE(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTPERF_GPUPROFILERONESHOT_ENDRANGE(m_CommandBuffer, 2)
		//NSIGHTPERF_GPUPROFILERONESHOT_BEGINRANGE(m_CommandBuffer, m_HandledSubPass->GetName(), 2, m_CurrentFrame)

		DEBUGUTILS_ENDLABEL(m_CommandBuffer)
		DEBUGUTILS_BEGINLABEL(m_CommandBuffer, m_HandledSubPass->GetName())

		//NSIGHTAFTERMATH_GPUCRASHTRACKER_SETCHECKPOINT(m_CommandBuffer, m_Renderer->m_VulkanState.m_VkFunc, "Enter SubPass:" + m_HandledSubPass->GetName())

		/**
		* @brief Begin RenderPass Statistics.
		*/
		RENDERPASS_STATISTICS_BEGINSTATISTICS(this, m_CommandBuffer)
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindPipeline(
		const std::string&  materialName , 
		VkCommandBuffer     cmdBuffer    , 
		VkPipelineBindPoint bindPoint
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindPipeline(materialName, cmdBuffer, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindPipeline(
		VkPipeline          pipeline , 
		VkCommandBuffer     cmdBuffer, 
		VkPipelineBindPoint bindPoint
	)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::BindPipeline(pipeline, cmdBuffer, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindPipelineAsync(
		const std::string&  materialName , 
		VkPipelineBindPoint bindPoint
	)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::BindPipelineAsync(materialName, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSet(
		const DescriptorSetInfo& infos     , 
		VkCommandBuffer          cmdBuffer , 
		VkPipelineBindPoint      bindPoint
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindDescriptorSet(infos, cmdBuffer, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSetAsync(
		const DescriptorSetInfo& infos     , 
		VkPipelineBindPoint      bindPoint
	)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::BindDescriptorSetAsync(infos, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSet(
		const DescriptorSetInfo& infos     , 
		const std::string&       name      , 
		VkCommandBuffer          cmdBuffer , 
		VkPipelineBindPoint      bindPoint
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindDescriptorSet(infos, name, cmdBuffer, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::BindDescriptorSetAsync(
		const DescriptorSetInfo& infos , 
		const std::string&       name  , 
		VkPipelineBindPoint      bindPoint
	)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::BindDescriptorSetAsync(infos, name, bindPoint);
	}

	void Renderer::RayTracingRenderBehaveBuilder::TraceRays() const
	{
		SPICES_PROFILE_ZONE;
		
		const uint32_t width  = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().x);
		const uint32_t height = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().y);

		/*
		* @attention Vulkan not allow dynamic state in mixing raytracing pipeline and custom graphic pipeline.
		* @see https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/8038.
		*/
		m_Renderer->m_VulkanState.m_VkFunc.vkCmdTraceRaysKHR(
			m_CommandBuffer                      ,
			&m_VulkanRayTracing->GetRgenRegion() ,
			&m_VulkanRayTracing->GetMissRegion() ,
			&m_VulkanRayTracing->GetHitRegion()  ,
			&m_VulkanRayTracing->GetCallRegion() ,
			width                                ,
			height                               ,
			1
		);
	}

	void Renderer::RayTracingRenderBehaveBuilder::TraceRays(
		const VkStridedDeviceAddressRegionKHR* rgenRegion,
		const VkStridedDeviceAddressRegionKHR* missRegion,
		const VkStridedDeviceAddressRegionKHR* hitRegion,
		const VkStridedDeviceAddressRegionKHR* callRegion
	) const
	{
		SPICES_PROFILE_ZONE;
		
		const uint32_t width  = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().x);
		const uint32_t height = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().y);

		/**
		* @brief Trace rays.
		*/
		m_Renderer->m_VulkanState.m_VkFunc.vkCmdTraceRaysKHR(
			m_CommandBuffer ,
			rgenRegion      ,
			missRegion      ,
			hitRegion       ,
			callRegion      ,
			width           ,
			height          ,
			1
		);
	}

	void Renderer::RayTracingRenderBehaveBuilder::TraceRaysAsync(
		const VkStridedDeviceAddressRegionKHR* rgenRegion , 
		const VkStridedDeviceAddressRegionKHR* missRegion , 
		const VkStridedDeviceAddressRegionKHR* hitRegion  , 
		const VkStridedDeviceAddressRegionKHR* callRegion
	) const
	{
		SPICES_PROFILE_ZONE;

		const uint32_t width = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().x);
		const uint32_t height = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().y);

		/*
		* @attention Vulkan not allow dynamic state in mixing raytracing pipeline and custom graphic pipeline.
		* @see https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/8038.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
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

	void Renderer::RenderBehaveBuilder::BindDescriptorSet(
		const DescriptorSetInfo& infos     , 
		VkCommandBuffer          cmdBuffer , 
		VkPipelineBindPoint      bindPoint
	)
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
				m_Renderer->m_Pipelines.Find(name)->GetPipelineLayout(),
				pair.first,
				1,
				&pair.second->Get(),
				0,
				nullptr
			);
		}
	}

	void Renderer::RenderBehaveBuilder::BindDescriptorSetAsync(
		const DescriptorSetInfo& infos , 
		VkPipelineBindPoint      bindPoint
	)
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

		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& secCmdBuffer) {

			/**
			* @brief Iter all desctiptorsets.
			*/
			for (const auto& pair : infos)
			{
				vkCmdBindDescriptorSets(
					secCmdBuffer,
					bindPoint,
					m_Renderer->m_Pipelines.Find(name)->GetPipelineLayout(),
					pair.first,
					1,
					&pair.second->Get(),
					0,
					nullptr
				);
			}
		});
	}

	void Renderer::RenderBehaveBuilder::RunDGC(VkCommandBuffer cmdBuffer) const
	{
		SPICES_PROFILE_ZONE;

		PreprocessDGC_NV(cmdBuffer);

		InternalRegionBarrier(
			VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV   ,
			VK_ACCESS_INDIRECT_COMMAND_READ_BIT         ,
			VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV ,
			VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT         ,
			cmdBuffer
		);

		ExecuteDGC_NV(cmdBuffer);
	}

	void Renderer::RenderBehaveBuilder::RunDGCAsync() const
	{
		SPICES_PROFILE_ZONE;

		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
			PreprocessDGC_NV(cmdBuffer);

			InternalRegionBarrier(
				VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV   ,
				VK_ACCESS_INDIRECT_COMMAND_READ_BIT         ,
				VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV ,
				VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT         ,
				cmdBuffer
			);

			ExecuteDGC_NV(cmdBuffer);
		});
	}

	void Renderer::RenderBehaveBuilder::PreprocessDGC_NV(VkCommandBuffer cmdBuffer) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call vkCmdPreprocessGeneratedCommandsNV.
		*/
		m_HandledDGCData->PreprocessDGC(cmdBuffer ? cmdBuffer : m_CommandBuffer);
	}

	void Renderer::RenderBehaveBuilder::PreprocessDGCAsync_NV() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call vkCmdPreprocessGeneratedCommandsNV.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
			m_HandledDGCData->PreprocessDGC(cmdBuffer);
		});
	}

	void Renderer::RenderBehaveBuilder::ExecuteDGC_NV(const VkCommandBuffer& cmdBuffer) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call vkCmdExecuteGeneratedCommandsNV.
		*/
		m_HandledDGCData->ExecuteDGC(cmdBuffer ? cmdBuffer : m_CommandBuffer);
	}

	void Renderer::RenderBehaveBuilder::ExecuteDGCAsync_NV() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call vkCmdExecuteGeneratedCommandsNV.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
			m_HandledDGCData->ExecuteDGC(cmdBuffer);
		});
	}

	void Renderer::RenderBehaveBuilder::DrawFullScreenTriangle(const VkCommandBuffer& cmdBuffer) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call vkCmdDraw.
		*/
		vkCmdDraw(cmdBuffer ? cmdBuffer : m_CommandBuffer, 3, 1, 0, 0);
	}

	void Renderer::RenderBehaveBuilder::DrawFullScreenTriangleAsync() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call vkCmdDraw.
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
			vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
		});
	}

	void Renderer::RenderBehaveBuilder::InternalRegionBarrier(
		VkAccessFlags          srcAccessMask , 
		VkAccessFlags          dstAccessMask , 
		VkPipelineStageFlags   srcStageMask  , 
		VkPipelineStageFlags   dstStageMask  ,
		VkCommandBuffer        cmdBuffer
	) const
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

	void Renderer::RenderBehaveBuilder::InternalRegionBarrierAsync(
		VkAccessFlags          srcAccessMask  , 
		VkAccessFlags          dstAccessMask  , 
		VkPipelineStageFlags   srcStageMask   , 
		VkPipelineStageFlags   dstStageMask
	) const
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
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
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

		m_HandledSubPass  = *renderer->m_Pass->GetSubPasses()->find_value(subPassName);
		m_DescriptorSetId = { m_Renderer->m_Pass->GetName(), m_HandledSubPass->GetName() };
	}

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddPushConstant(uint32_t size)
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
		uint32_t                                         set           , 
		uint32_t                                         binding       , 
		VkShaderStageFlags                               stageFlags    , 
		const std::vector<std::string>&                  textureNames  , 
		VkFormat                                         format        ,
		TextureType                                      type
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

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddStorageTextureMipmaps(
		uint32_t                                         set           , 
		uint32_t                                         binding       , 
		VkShaderStageFlags                               stageFlags    , 
		const std::string&                               textureName   , 
		VkFormat                                         format        ,
		TextureType                                      type          ,
		std::function<void(RendererResourceCreateInfo&)> func
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		RendererResourceCreateInfo info;
		info.name                                = textureName;
		info.type                                = type;
		info.width                               = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.width;
		info.height                              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height;
		info.description.samples                 = VK_SAMPLE_COUNT_1_BIT;
		info.description.format                  = format;
		info.usage                               = VK_IMAGE_USAGE_STORAGE_BIT;

		if (func)
		{
			func(info);
		}

		for(int i = 0; i < info.mipLevel; i++)
		{
			VkDescriptorImageInfo* imageInfo         = m_Renderer->m_RendererResourcePool->AccessResource(info, i);
			imageInfo->imageLayout                   = VK_IMAGE_LAYOUT_GENERAL;

			m_ImageInfos[set][binding].push_back(*imageInfo);
		}

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, stageFlags, static_cast<uint32_t>(info.mipLevel));

		return *this;
	}

	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddAttachmentTexture(
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
			RendererResourceCreateInfo resInfo;
			resInfo.name = textureNames[i];
			const auto info = m_Renderer->m_RendererResourcePool->AccessResource(resInfo);

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
			RendererResourceCreateInfo resInfo;
			resInfo.name = inputAttachmentNames[i];
			const auto info = m_Renderer->m_RendererResourcePool->AccessResource(resInfo);

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

		const auto& descriptorSets = DescriptorSetManager::GetByName(m_DescriptorSetId);
		
		for (auto& pair : descriptorSets)
		{
			/**
			* @brief UpdateDescriptorSet, skip bindless texture set.
			*/
			if (pair.first == SpicesShader::BINDLESS_TEXTURE_SET)
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

	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddSubPass(
		const std::string&       subPassName , 
		Querier::StatisticsFlags flags
	)
	{
		SPICES_PROFILE_ZONE;

		const size_t size        = m_Renderer->m_Pass->GetSubPasses()->size();
		m_HandledRendererSubPass = m_Renderer->m_Pass->AddSubPass(subPassName, static_cast<uint32_t>(size), flags);

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

		const size_t index = m_Renderer->m_Pass->GetSubPasses()->size() - 1;
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
		uint32_t  currentImage  ,
		const std::array<VkCommandBuffer, MaxFrameInFlight>& cmdBuffers
	)
		: RenderBehaveBuilder(renderer, currentFrame, currentImage)
	{
		SPICES_PROFILE_ZONE;
		
		m_HandledSubPass = *m_Renderer->m_Pass->GetSubPasses()->first();
		m_CommandBuffer  = cmdBuffers[currentFrame];
	}

	void Renderer::ComputeRenderBehaveBuilder::BindPipeline(
		const std::string&  materialName , 
		VkCommandBuffer     cmdBuffer    , 
		VkPipelineBindPoint bindPoint
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindPipeline(materialName, cmdBuffer, bindPoint);
	}

	void Renderer::ComputeRenderBehaveBuilder::BindDescriptorSet(
		const DescriptorSetInfo& infos     , 
		VkCommandBuffer          cmdBuffer , 
		VkPipelineBindPoint      bindPoint
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindDescriptorSet(infos, cmdBuffer, bindPoint);
	}

	void Renderer::ComputeRenderBehaveBuilder::BindDescriptorSet(
		const DescriptorSetInfo& infos     , 
		const std::string&       name      , 
		VkCommandBuffer          cmdBuffer , 
		VkPipelineBindPoint      bindPoint
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::BindDescriptorSet(infos, name, cmdBuffer, bindPoint);
	}

	void Renderer::ComputeRenderBehaveBuilder::Dispatch(uint32_t x, uint32_t y, uint32_t z) const
	{
		SPICES_PROFILE_ZONE;
		
		vkCmdDispatch(m_CommandBuffer, x, y, z);
	}

	void Renderer::RenderBehaveBuilder::AddBarriers(
		VulkanImage*          image               ,
		VkAccessFlags         srcAccessMask       , 
		VkAccessFlags         dstAccessMask       , 
		VkPipelineStageFlags  srcStageMask        , 
		VkPipelineStageFlags  dstStageMask        ,
		uint32_t              srcQueueFamilyIndex ,
		uint32_t              dstQueueFamilyIndex ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		image->Barrier(
			cmdBuffer ? cmdBuffer : m_CommandBuffer , 
			srcAccessMask                           , 
			dstAccessMask                           , 
			srcStageMask                            , 
			dstStageMask                            , 
			srcQueueFamilyIndex                     , 
			dstQueueFamilyIndex
		);
	}

	void Renderer::RenderBehaveBuilder::ReleaseBarriers(
		VulkanImage*          image               ,
		VkAccessFlags         srcAccessMask       , 
		VkAccessFlags         dstAccessMask       , 
		VkPipelineStageFlags  srcStageMask        , 
		VkPipelineStageFlags  dstStageMask        ,
		uint32_t              srcQueueFamilyIndex ,
		uint32_t              dstQueueFamilyIndex ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		image->Barrier(
			cmdBuffer ? cmdBuffer : m_CommandBuffer , 
			srcAccessMask                           , 
			dstAccessMask                           , 
			srcStageMask                            , 
			dstStageMask                            , 
			srcQueueFamilyIndex                     , 
			dstQueueFamilyIndex
		);
	}

	void Renderer::RenderBehaveBuilder::InternalBarriers(
		VulkanImage*          image         ,
		VkAccessFlags         srcAccessMask , 
		VkAccessFlags         dstAccessMask , 
		VkPipelineStageFlags  srcStageMask  , 
		VkPipelineStageFlags  dstStageMask  ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;

		image->Barrier(
			cmdBuffer ? cmdBuffer : m_CommandBuffer , 
			srcAccessMask                           , 
			dstAccessMask                           , 
			srcStageMask                            , 
			dstStageMask                            , 
			VK_QUEUE_FAMILY_IGNORED                 , 
			VK_QUEUE_FAMILY_IGNORED
		);
	}

	void Renderer::ComputeRenderBehaveBuilder::InternalBarriers(
		VulkanImage*          image         ,
		VkAccessFlags         srcAccessMask , 
		VkAccessFlags         dstAccessMask , 
		VkPipelineStageFlags  srcStageMask  , 
		VkPipelineStageFlags  dstStageMask  ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::InternalBarriers(
			srcAccessMask , 
			dstAccessMask , 
			srcStageMask  , 
			dstStageMask  , 
			cmdBuffer ? cmdBuffer : m_CommandBuffer
		);
	}

	void Renderer::RenderBehaveBuilder::AddBarriers(
		VkBuffer              buffer              , 
		VkAccessFlags         srcAccessMask       , 
		VkAccessFlags         dstAccessMask       , 
		VkPipelineStageFlags  srcStageMask        , 
		VkPipelineStageFlags  dstStageMask        ,
		uint32_t              srcQueueFamilyIndex ,
		uint32_t              dstQueueFamilyIndex ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		VkBufferMemoryBarrier                   bufferBarrier {};
		bufferBarrier.sType                   = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		bufferBarrier.srcAccessMask           = srcAccessMask;
		bufferBarrier.dstAccessMask           = dstAccessMask;
		bufferBarrier.srcQueueFamilyIndex     = srcQueueFamilyIndex;   // Fetch From Graphic to Compute.
		bufferBarrier.dstQueueFamilyIndex     = dstQueueFamilyIndex;
		bufferBarrier.size                    = VK_WHOLE_SIZE;
		bufferBarrier.buffer                  = buffer;

		vkCmdPipelineBarrier(
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			1, &bufferBarrier,
			0, nullptr
		);
	}

	void Renderer::RenderBehaveBuilder::ReleaseBarriers(
		VkBuffer              buffer              , 
		VkAccessFlags         srcAccessMask       , 
		VkAccessFlags         dstAccessMask       , 
		VkPipelineStageFlags  srcStageMask        , 
		VkPipelineStageFlags  dstStageMask        ,
		uint32_t              srcQueueFamilyIndex ,
		uint32_t              dstQueueFamilyIndex ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		VkBufferMemoryBarrier                   bufferBarrier {};
		bufferBarrier.sType                   = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		bufferBarrier.srcAccessMask           = srcAccessMask;
		bufferBarrier.dstAccessMask           = dstAccessMask;
		bufferBarrier.srcQueueFamilyIndex     = srcQueueFamilyIndex;  // Release From Compute to Graphic
		bufferBarrier.dstQueueFamilyIndex     = dstQueueFamilyIndex;
		bufferBarrier.size                    = VK_WHOLE_SIZE;
		bufferBarrier.buffer                  = buffer;

		vkCmdPipelineBarrier(
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			1, &bufferBarrier,
			0, nullptr
		);
	}

	void Renderer::RenderBehaveBuilder::InternalBarriers(
		VkBuffer              buffer        , 
		VkAccessFlags         srcAccessMask , 
		VkAccessFlags         dstAccessMask , 
		VkPipelineStageFlags  srcStageMask  , 
		VkPipelineStageFlags  dstStageMask  ,
		VkCommandBuffer       cmdBuffer
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
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
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
		VkPipelineStageFlags  dstStageMask  ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder::InternalBarriers(
			buffer        , 
			srcAccessMask , 
			dstAccessMask , 
			srcStageMask  , 
			dstStageMask  , 
			cmdBuffer ? cmdBuffer : m_CommandBuffer
		);
	}

	void Renderer::RenderBehaveBuilder::InternalBarriers(
		VkAccessFlags         srcAccessMask , 
		VkAccessFlags         dstAccessMask , 
		VkPipelineStageFlags  srcStageMask  , 
		VkPipelineStageFlags  dstStageMask  ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		VkMemoryBarrier                   barrier {};
		barrier.sType                   = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		barrier.srcAccessMask           = srcAccessMask;
		barrier.dstAccessMask           = dstAccessMask;

		vkCmdPipelineBarrier(
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			1, &barrier,
			0, nullptr,
			0, nullptr
		);
	}

	void Renderer::ComputeRenderBehaveBuilder::InternalBarriers(
		VkAccessFlags         srcAccessMask , 
		VkAccessFlags         dstAccessMask , 
		VkPipelineStageFlags  srcStageMask  , 
		VkPipelineStageFlags  dstStageMask  ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::InternalBarriers(
			srcAccessMask ,
			dstAccessMask , 
			srcStageMask  , 
			dstStageMask  , 
			cmdBuffer ? cmdBuffer : m_CommandBuffer
		);
	}

	void Renderer::ComputeRenderBehaveBuilder::AddBarriers(
		VulkanImage*          image               ,
		VkAccessFlags         srcAccessMask       , 
		VkAccessFlags         dstAccessMask       , 
		VkPipelineStageFlags  srcStageMask        , 
		VkPipelineStageFlags  dstStageMask        ,
		uint32_t              srcQueueFamilyIndex ,
		uint32_t              dstQueueFamilyIndex ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::AddBarriers(
			image               , 
			srcAccessMask       , 
			dstAccessMask       , 
			srcStageMask        , 
			dstStageMask        , 
			srcQueueFamilyIndex , 
			dstQueueFamilyIndex , 
			cmdBuffer ? cmdBuffer : m_CommandBuffer
		);
	}

	void Renderer::ComputeRenderBehaveBuilder::ReleaseBarriers(
		VulkanImage*          image               ,
		VkAccessFlags         srcAccessMask       , 
		VkAccessFlags         dstAccessMask       , 
		VkPipelineStageFlags  srcStageMask        , 
		VkPipelineStageFlags  dstStageMask        ,
		uint32_t              srcQueueFamilyIndex ,
		uint32_t              dstQueueFamilyIndex ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::ReleaseBarriers(
			image               , 
			srcAccessMask       , 
			dstAccessMask       , 
			srcStageMask        , 
			dstStageMask        , 
			srcQueueFamilyIndex , 
			dstQueueFamilyIndex , 
			cmdBuffer ? cmdBuffer : m_CommandBuffer
		);
	}

	void Renderer::ComputeRenderBehaveBuilder::AddBarriers(
		VkBuffer              buffer              , 
		VkAccessFlags         srcAccessMask       , 
		VkAccessFlags         dstAccessMask       , 
		VkPipelineStageFlags  srcStageMask        , 
		VkPipelineStageFlags  dstStageMask        ,
		uint32_t              srcQueueFamilyIndex ,
		uint32_t              dstQueueFamilyIndex ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::AddBarriers(
			buffer              , 
			srcAccessMask       , 
			dstAccessMask       , 
			srcStageMask        , 
			dstStageMask        , 
			srcQueueFamilyIndex , 
			dstQueueFamilyIndex , 
			cmdBuffer ? cmdBuffer : m_CommandBuffer
		);
	}

	void Renderer::ComputeRenderBehaveBuilder::ReleaseBarriers(
		VkBuffer              buffer              , 
		VkAccessFlags         srcAccessMask       , 
		VkAccessFlags         dstAccessMask       , 
		VkPipelineStageFlags  srcStageMask        , 
		VkPipelineStageFlags  dstStageMask        ,
		uint32_t              srcQueueFamilyIndex ,
		uint32_t              dstQueueFamilyIndex ,
		VkCommandBuffer       cmdBuffer
	)
	{
		SPICES_PROFILE_ZONE;
		
		RenderBehaveBuilder::ReleaseBarriers(
			buffer              , 
			srcAccessMask       , 
			dstAccessMask       , 
			srcStageMask        , 
			dstStageMask        , 
			srcQueueFamilyIndex ,
			dstQueueFamilyIndex , 
			cmdBuffer ? cmdBuffer : m_CommandBuffer
		);
	}

	Renderer::DGCLayoutBuilder::DGCLayoutBuilder(
		const std::string& subPassName , 
		Renderer*          renderer
	)
		: m_Renderer(renderer)
		, m_SubPassName(subPassName)
	{
		SPICES_PROFILE_ZONE;

		if (!m_Renderer->m_DGCData[subPassName])
		{
			m_Renderer->m_DGCData[subPassName] = std::make_shared<VulkanDeviceGeneratedCommandsNV>(m_Renderer->m_VulkanState);
		}

		m_Renderer->m_DGCData[subPassName]->ResetCommandsLayout();
		m_HandledDGCData = m_Renderer->m_DGCData[subPassName];
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

		input.pIndexTypes                  = nullptr;
		input.pIndexTypeValues             = nullptr;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledDGCData->AddInputStride(sizeof(VkBindShaderGroupIndirectCommandNV));

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

		input.pIndexTypes                  = nullptr;
		input.pIndexTypeValues             = nullptr;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledDGCData->AddInputStride(sizeof(VkBindVertexBufferIndirectCommandNV));

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

		input.pIndexTypes                  = nullptr;
		input.pIndexTypeValues             = nullptr;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledDGCData->AddInputStride(sizeof(VkBindIndexBufferIndirectCommandNV));

		return *this;
	}

	Renderer::DGCLayoutBuilder& Renderer::DGCLayoutBuilder::AddPushConstantInput()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_SubPassName << ".Default";

		/**
		* @brief Instance a VkIndirectCommandsLayoutTokenNV.
		*/
		VkIndirectCommandsLayoutTokenNV      input{}; 
		input.sType                        = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
		input.tokenType                    = VK_INDIRECT_COMMANDS_TOKEN_TYPE_PUSH_CONSTANT_NV;

		input.pushconstantPipelineLayout   = m_Renderer->m_Pipelines.Find(ss.str())->GetPipelineLayout();
		input.pushconstantShaderStageFlags = VK_SHADER_STAGE_ALL;
		input.pushconstantOffset           = 0;
		input.pushconstantSize             = sizeof(VkDeviceAddress);

		input.stream                       = static_cast<uint32_t>(m_InputInfos.size());
		input.offset                       = 0;

		input.pIndexTypes                  = nullptr;
		input.pIndexTypeValues             = nullptr;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledDGCData->AddInputStride(sizeof(VkDeviceAddress));

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

		input.pIndexTypes                  = nullptr;
		input.pIndexTypeValues             = nullptr;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledDGCData->AddInputStride(sizeof(VkDrawIndexedIndirectCommand));

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

		input.pIndexTypes                  = nullptr;
		input.pIndexTypeValues             = nullptr;

		/**
		* @brief Store Input.
		*/
		m_InputInfos.push_back(input);
		m_HandledDGCData->AddInputStride(sizeof(VkDrawMeshTasksIndirectCommandNV));

		return *this;
	}

	void Renderer::DGCLayoutBuilder::Build() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create IndirectCommandsLayout.
		*/
		m_HandledDGCData->BuildCommandLayout(m_InputInfos);
	}

	Renderer::PipelineBuilder::PipelineBuilder(
		std::shared_ptr<RendererSubPass> subPass,
		std::shared_ptr<Material>        material,
		Renderer*                        renderer
	)
		: m_Renderer(renderer)
		, m_Material(std::move(material))
		, m_HandledSubPass(std::move(subPass))
		, m_pipelineConfig{}
	{}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::SetDefault()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Init Config.
		*/
		VulkanPipeline::DefaultPipelineConfigInfo(m_pipelineConfig);

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::NullBindingDescriptions()
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.bindingDescriptions = {};

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::NullAttributeDescriptions()
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.attributeDescriptions = {};

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::SetBindingDescriptions(
		const std::vector<VkVertexInputBindingDescription>& inputs
	)
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.bindingDescriptions = inputs;

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::SetAttributeDescriptions(
		const std::vector<VkVertexInputAttributeDescription>& inputs
	)
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.attributeDescriptions = inputs;

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::SetRenderPass()
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.renderPass = m_Renderer->m_Pass;

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::SetSubPassIndex()
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.subpass = m_HandledSubPass->GetIndex();

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::SetPipelineLayout(VkPipelineLayout& layout)
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.pipelineLayout = layout;

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::SetCullMode(VkCullModeFlags cullMode)
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.rasterizationInfo.cullMode = cullMode;

		return *this;
	}

	Renderer::PipelineBuilder& Renderer::PipelineBuilder::SetColorAttachments()
	{
		SPICES_PROFILE_ZONE;

		m_pipelineConfig.colorBlendInfo.attachmentCount = static_cast<uint32_t>(m_HandledSubPass->GetColorBlend().size());
		m_pipelineConfig.colorBlendInfo.pAttachments    = m_HandledSubPass->GetColorBlend().data();

		return *this;
	}

	void Renderer::PipelineBuilder::Build()
	{
		SPICES_PROFILE_ZONE;

		const auto pipeline = std::make_shared<VulkanPipeline>(
			m_Renderer->m_VulkanState   , 
			m_Material->GetName()       ,
			m_Material->GetShaderPath() ,
			m_pipelineConfig
		);

		if (m_Renderer->m_Pipelines.HasKey(m_Material->GetName()))
		{
			m_Renderer->m_RenderCache->PushToCaches(m_Renderer->m_Pipelines.Find(m_Material->GetName()));
		}

		m_Renderer->m_Pipelines.Insert(m_Material->GetName(), pipeline);
	}

	void Renderer::PipelineBuilder::BuildMesh()
	{
		SPICES_PROFILE_ZONE;

		const auto pipeline = std::make_shared<VulkanMeshPipeline>(
			m_Renderer->m_VulkanState   , 
			m_Material->GetName()       ,
			m_Material->GetShaderPath() ,
			m_pipelineConfig
		);

		if (m_Renderer->m_Pipelines.HasKey(m_Material->GetName()))
		{
			m_Renderer->m_RenderCache->PushToCaches(m_Renderer->m_Pipelines.Find(m_Material->GetName()));
		}

		m_Renderer->m_Pipelines.Insert(m_Material->GetName(), pipeline);
	}

	void Renderer::PipelineBuilder::BuildCompute()
	{
		SPICES_PROFILE_ZONE;

		const auto pipeline = std::make_shared<VulkanComputePipeline>(
			m_Renderer->m_VulkanState   , 
			m_Material->GetName()       ,
			m_Material->GetShaderPath() ,
			m_pipelineConfig
		);

		if (m_Renderer->m_Pipelines.HasKey(m_Material->GetName()))
		{
			m_Renderer->m_RenderCache->PushToCaches(m_Renderer->m_Pipelines.Find(m_Material->GetName()));
		}

		m_Renderer->m_Pipelines.Insert(m_Material->GetName(), pipeline);
	}

	void Renderer::PipelineBuilder::BuildRayTracing(const std::unordered_map<std::string, uint32_t>& hitGroups)
	{
		SPICES_PROFILE_ZONE;

		std::unordered_map<std::string, std::vector<std::string>> stages(m_Material->GetShaderPath());
		for (auto& pair : hitGroups)
		{
			stages["rchit"].push_back(pair.first);
		}

		const auto pipeline = std::make_shared<VulkanRayTracingPipeline>(
			m_Renderer->m_VulkanState   , 
			m_Material->GetName()       ,
			stages                      ,
			m_pipelineConfig
		);

		if (m_Renderer->m_Pipelines.HasKey(m_Material->GetName()))
		{
			m_Renderer->m_RenderCache->PushToCaches(m_Renderer->m_Pipelines.Find(m_Material->GetName()));
		}
		
		m_Renderer->m_Pipelines.Insert(m_Material->GetName(), pipeline);
	}

	void Renderer::PipelineBuilder::BuildDeviceGeneratedCommand(const std::string& pipelineName, const std::string& materialName, VulkanDeviceGeneratedCommandsNV* indirectPtr)
	{
		SPICES_PROFILE_ZONE;

		indirectPtr->CreateMeshPipeline(pipelineName, materialName, m_pipelineConfig);
	}
}