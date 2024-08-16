/**
* @file BasePassRenderer.cpp.
* @brief The BasePassRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "BasePassRenderer.h"

namespace Spices {

	void BasePassRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "BassPass", this }
		.AddSubPass("Mesh")
		.AddColorAttachment("Albedo", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Normal", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Roughness", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Metallic", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Position", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("ID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R32_SFLOAT;
		})
		.AddDepthAttachment("Depth", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;   /* @attention It seams that layout transform is not work? */
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.EndSubPass()
		.AddSubPass("SkyBox")
		.AddColorAttachment("Albedo", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {})
		.AddColorAttachment("Position", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("ID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                = VK_FORMAT_R32_SFLOAT;
		})
		.AddDepthAttachment("Depth", TextureType::Texture2D, [](VkAttachmentDescription& description) {})
		.EndSubPass()
		.Build();
	}

	void BasePassRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Mesh", this }
		.AddStorageBuffer(2, 0, sizeof(SpicesShader::MeshDesc) * MESH_BUFFER_MAXNUM, VK_SHADER_STAGE_TASK_BIT_EXT | VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddPushConstant(sizeof(SpicesShader::MeshDesc))
		.Build();

		DescriptorSetBuilder{ "SkyBox", this }
		.AddPushConstant(sizeof(SpicesShader::MeshDesc))
		.Build();
	}

	void BasePassRenderer::OnMeshAddedWorld()
	{
		SPICES_PROFILE_ZONE;

		m_BaseMeshDrawCommandsBuffer.clear();

		std::unordered_map<std::string, std::vector<VkDrawMeshTasksIndirectCommandEXT>> commandsMap;

		auto& view = FrameInfo::Get().m_World->GetRegistry().view<MeshComponent>();
		for (auto& e : view)
		{
			MeshComponent meshComp;
			TransformComponent tranComp;

			std::tie(meshComp, tranComp) = FrameInfo::Get().m_World->GetRegistry().get<MeshComponent, TransformComponent>(e);

			meshComp.GetMesh()->GetPacks().for_each([&](const uint32_t& k, const std::shared_ptr<MeshPack>& v) {
				commandsMap[v->GetMaterial()->GetName()].push_back(v->GetDrawCommand());

				SpicesShader::MeshDesc            desc;
				desc.modelAddredd               = tranComp.GetModelBufferAddress();
				desc.vertexAddress              = v->GetVerticesBufferAddress();
				desc.indexAddress               = v->GetIndicesBufferAddress();
				desc.materialParameterAddress   = v->GetMaterial()->GetMaterialParamsAddress();
				desc.meshletAddress             = v->GetMeshletsBufferAddress();
				desc.nMeshlets                  = static_cast<uint32_t>(v->GetMeshlets().size());
				desc.entityID                   = static_cast<unsigned int>(e);

				m_DescsMap[v->GetMaterial()->GetName()].descs.push_back(std::move(desc));

				return false;
			});
		}

		for (auto& pair : commandsMap)
		{
			uint64_t bufferSize = pair.second.size() * sizeof(VkDrawMeshTasksIndirectCommandEXT);

			VulkanBuffer stagingBuffer(
				m_VulkanState                        , 
				bufferSize                           ,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT     ,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			stagingBuffer.WriteToBuffer(pair.second.data());

			std::unique_ptr<VulkanBuffer> commandBuffer = std::make_unique<VulkanBuffer>(
				m_VulkanState                             ,
				bufferSize                                ,
				VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT       |
				VK_BUFFER_USAGE_TRANSFER_DST_BIT          |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			commandBuffer->CopyBuffer(stagingBuffer.Get(), commandBuffer->Get(), bufferSize);

			m_BaseMeshDrawCommandsBuffer[pair.first] = std::move(commandBuffer);
			m_BaseMeshDrawCommandsBufferCount[pair.first] = pair.second.size();
		}
	}

	std::shared_ptr<VulkanPipeline> BasePassRenderer::CreatePipeline(
		std::shared_ptr<Material> material,
		VkPipelineLayout& layout,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass                      = m_Pass->Get();
		pipelineConfig.subpass                         = subPass->GetIndex();
		pipelineConfig.pipelineLayout                  = layout;
		pipelineConfig.rasterizationInfo.cullMode      = VK_CULL_MODE_NONE;
		pipelineConfig.colorBlendInfo.attachmentCount  = static_cast<uint32_t>(subPass->GetColorBlend().size());
		pipelineConfig.colorBlendInfo.pAttachments     = subPass->GetColorBlend().data();

		return std::make_shared<VulkanMeshPipeline>(
			m_VulkanState,
			material->GetName(),
			material->GetShaderPath(),
			pipelineConfig
		);
	}

	void BasePassRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::Rasterization) return;
		
		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };
		
		//builder.BeginRenderPassAsync();

		////builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }));

		//IterWorldCompSubmitCmdParallel<MeshComponent>(frameInfo, builder.GetSubpassIndex(), [&](VkCommandBuffer& cmdBuffer, int entityId, TransformComponent& transComp, MeshComponent& meshComp) {

		//	builder.SetViewPort(cmdBuffer);

		//	builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"), cmdBuffer);

		//	builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }), cmdBuffer);

		//	meshComp.GetMesh()->DrawMeshTasks(cmdBuffer, [&](const uint32_t& meshpackId, const auto& meshPack) {

		//		builder.BindPipeline(meshPack->GetMaterial()->GetName(), cmdBuffer);

		//		builder.UpdatePushConstant<SpicesShader::MeshDesc>([&](auto& push) {
		//			push.modelAddredd              = transComp.GetModelBufferAddress();
		//			push.vertexAddress             = meshPack->GetVerticesBufferAddress();
		//			push.indexAddress              = meshPack->GetIndicesBufferAddress();
		//			push.materialParameterAddress  = meshPack->GetMaterial()->GetMaterialParamsAddress();
		//			push.meshletAddress            = meshPack->GetMeshletsBufferAddress();
		//			push.nMeshlets                 = static_cast<uint32_t>(meshPack->GetMeshlets().size());
		//			push.entityID                  = entityId;
		//		}, cmdBuffer);
		//	});
		//});

		builder.BeginRenderPass();

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }));

		for (auto& pair : m_BaseMeshDrawCommandsBuffer)
		{
			//builder.UpdateStorageBuffer(2, 0, m_DescsMap[pair.first].descs.data(), m_DescsMap[pair.first].descs.size());

			//builder.BindPipeline(pair.first);
			uint32_t count = m_BaseMeshDrawCommandsBufferCount[pair.first];
			uint32_t stride = sizeof(VkDrawMeshTasksIndirectCommandEXT);
			//vkCmdDrawMeshTasksIndirectEXT(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], pair.second->Get(), 0, count, stride);
		}

		builder.BeginNextSubPass("SkyBox");

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SkyBox" }));

		IterWorldCompWithBreak<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			skyboxComp.GetMesh()->DrawMeshTasks(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](const uint32_t& meshpackId, const auto& meshPack) {

				builder.BindPipeline(meshPack->GetMaterial()->GetName());

				builder.UpdatePushConstant<SpicesShader::MeshDesc>([&](auto& push) {
					push.modelAddredd              = transComp.GetModelBufferAddress();
					push.vertexAddress             = meshPack->GetVerticesBufferAddress();
					push.indexAddress              = meshPack->GetIndicesBufferAddress();
					push.materialParameterAddress  = meshPack->GetMaterial()->GetMaterialParamsAddress();
					push.meshletAddress            = meshPack->GetMeshletsBufferAddress();
					push.nMeshlets                 = static_cast<uint32_t>(meshPack->GetMeshlets().size());
					push.entityID                  = entityId;
				});
			});

			return true;
		});

		builder.EndRenderPass();
	}
}