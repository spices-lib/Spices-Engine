#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanPipeline.h"
#include "Render/Vulkan/VulkanUtils.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/FrameInfo.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "RendererManager.h"
#include "Core/Library/ContainerLibrary.h"

#include "World/Components/MeshComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/CameraComponent.h"
#include "World/Components/UUIDComponent.h"

#include <memory>
#include <unordered_map>

namespace Spiecs {

	class Renderer
	{
	public:
		Renderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool);
		virtual ~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		virtual void Render(FrameInfo& frameInfo) = 0;

		void OnSystemInitialize();

	private:
		// TODO: specific renderpass
		virtual void CreateRenderPass() = 0;


		// specific desctiptor layout
		virtual void CreatePipelineLayoutAndDescriptor() = 0;

		// specific pipeline
		virtual void CreatePipeline(VkRenderPass renderPass) = 0;

	protected:
		std::string GetSahderPath(const std::string& shaderType);

		//void AddDescriptor();

		template<typename T, typename F>
		inline void IterWorldComp(FrameInfo& frameInfo, F func);

	private:
		struct DescriptorResource
		{
			std::vector<VkDescriptorSet> m_DescriptorSets{};
		};

	protected:
		class PipelineLayoutBuilder
		{
		public:
			PipelineLayoutBuilder(Renderer* renderer) : m_Renderer(renderer) {};

			virtual ~PipelineLayoutBuilder() {};

			template<typename T>
			inline PipelineLayoutBuilder& CreateCollection();

			template<typename T>
			inline PipelineLayoutBuilder& AddBuffer(
				uint32_t set, 
				uint32_t binding, 
				VkShaderStageFlags stageFlags
			);

			template<typename T>
			inline PipelineLayoutBuilder& AddPushConstant();

			void Build();

		private:
			Renderer* m_Renderer;
			std::vector<std::unique_ptr<VulkanDescriptorSetLayout>> m_VulkanLayouts{};

			bool isUsePushConstant = false;
			VkPushConstantRange m_PushConstantRange{};
		};

		struct Collection 
		{
		public:
			Collection() {};
			virtual ~Collection() {};

			virtual VkDescriptorBufferInfo GetSpecificBufferInfo(uint32_t set, uint32_t binding) = 0;
			virtual std::unique_ptr<VulkanBuffer>& GetBuffer(uint32_t set, uint32_t binding) = 0;
		};

	protected:
		// vulkan state
		VulkanState& m_VulkanState;
		std::shared_ptr<VulkanDescriptorPool> m_DesctiptorPool;

		// TODO: renderpass
		// VkRenderPass m_RenderPass;

		// descriptorset
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts{};
		std::array<DescriptorResource, MaxFrameInFlight> m_Resource{};

		// localdescriptorbuffer
		std::array<std::unique_ptr<Collection>, MaxFrameInFlight> m_Collections;

		// pipeline
		VkPipelineLayout m_PipelineLayout;
		std::unique_ptr<VulkanPipeline> m_VulkanPipeline;
		
		std::string m_RendererName;

		friend class PipelineLayoutBuilder;
	};

	template<typename T, typename F>
	inline void Renderer::IterWorldComp(FrameInfo& frameInfo, F func)
	{
		// group inter will be bug
		auto& view = frameInfo.m_World->GetRegistry().view<T>();
		for (auto& e : view)
		{
			auto& [tComp, transComp] = frameInfo.m_World->GetRegistry().get<T, TransformComponent>(e);

			bool isIterBreak = func(transComp, tComp);

			if (isIterBreak) break;
		}
	}

	template<typename T>
	inline Renderer::PipelineLayoutBuilder& Renderer::PipelineLayoutBuilder::CreateCollection()
	{
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			m_Renderer->m_Collections[i] = std::make_unique<T>();
		}

		return *this;
	}

	template<typename T>
	inline Renderer::PipelineLayoutBuilder& Renderer::PipelineLayoutBuilder::AddBuffer(uint32_t set, uint32_t binding, VkShaderStageFlags stageFlags)
	{
		// local data
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			m_Renderer->m_Collections[i]->GetBuffer(set, binding) = std::make_unique<VulkanBuffer>(
				m_Renderer->m_VulkanState,
				sizeof(T),
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			m_Renderer->m_Collections[i]->GetBuffer(set, binding)->Map();
		}

		// descriptorset layout
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorSetLayout>>(m_VulkanLayouts, set + 1);

		m_VulkanLayouts[set] = VulkanDescriptorSetLayout::Builder()
			.AddBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags)
			.Build(m_Renderer->m_VulkanState);

		ContainerLibrary::Resize<VkDescriptorSetLayout>(m_Renderer->m_DescriptorSetLayouts, set + 1);

		m_Renderer->m_DescriptorSetLayouts[set] = m_VulkanLayouts[set]->GetDescriptorSetLayout();

		for (int i = 0; i < MaxFrameInFlight; i++)
		{

			ContainerLibrary::Resize<VkDescriptorSet>(m_Renderer->m_Resource[i].m_DescriptorSets, binding + 1);

			auto bufferInfo = m_Renderer->m_Collections[i]->GetSpecificBufferInfo(set, binding);
			VulkanDescriptorWriter(*m_VulkanLayouts[set], *m_Renderer->m_DesctiptorPool)
				.WriteBuffer(binding, &bufferInfo)
				.Build(m_Renderer->m_Resource[i].m_DescriptorSets[binding]);
		}

		return *this;
	}

	template<typename T>
	inline Renderer::PipelineLayoutBuilder& Renderer::PipelineLayoutBuilder::AddPushConstant()
	{
		isUsePushConstant = true;

		m_PushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		m_PushConstantRange.offset = 0;
		m_PushConstantRange.size = sizeof(T);

		return *this;
	}
}