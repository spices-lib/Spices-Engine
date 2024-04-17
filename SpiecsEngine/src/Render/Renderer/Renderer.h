/**
* @file Renderer.h
* @brief The Renderer Class Definitions.
* @author Spiecs.
*/

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
#include "World/World/World.h"

#include "World/Components/MeshComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/CameraComponent.h"
#include "World/Components/UUIDComponent.h"
#include "World/Components/SkyBoxComponent.h"

#include <memory>
#include <unordered_map>

namespace Spiecs {

	/**
	* @brief  Renderer Class.
	* This class defines the basic behaver of renderer.
	* When we add an new Renderer, we need inherit from this.
	*/
	class Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		Renderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool);

		/**
		* @brief Destructor Function.
		* We destroy pipeline layout here.
		*/
		virtual ~Renderer();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		Renderer(const Renderer&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		Renderer& operator=(const Renderer&) = delete;

		/**
		* @brief The interface of how to render.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(FrameInfo& frameInfo) = 0;

		/**
		* @brief The Function is called on this attached.
		*/
		void OnSystemInitialize();

	private:

		/**
		* @brief The interface is called during OnSystemInitialize().
		* Create specific renderpass.
		* @todo Implementate specific renderpass
		*/
		virtual void CreateRenderPass() = 0;

		/**
		* @brief The interface is called during OnSystemInitialize().
		* Create specific pipelinelayout and buffer type descriptor.
		*/
		virtual void CreatePipelineLayoutAndDescriptor() = 0;

		/**
		* @brief The interface is called during OnSystemInitialize().
		* Create specific pipeline.
		* @param[in] renderPass Renderer specific renderpass.
		*/
		virtual void CreatePipeline(VkRenderPass renderPass) = 0;

	protected:

		/**
		* @brief Get sahder path string.
		* @param[in] shaderType What shader type is.
		* @return Returns the sahder path string.
		*/
		std::string GetSahderPath(const std::string& shaderType);

		/**
		* @brief Iterater the specific Component in World.
		* @param[in] T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] F The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldComp(FrameInfo& frameInfo, F func);

		/**
		* @brief Get The actived camera entity's view matrix and projection matrix.
		* @param[in] frameInfo The current frame data.
		* @return Returns a pair of view matrix and projection matrix.
		*/
		std::pair<glm::mat4, glm::mat4> GetActiveCameraMatrix(FrameInfo& frameInfo);

		/**
		* @brief Get DirectionalLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @return Returns the only one DirectionalLight render data.
		*/
		DirectionalLightComponent::DirectionalLight GetDirectionalLight(FrameInfo& frameInfo);

		/**
		* @brief Get PointLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @return Returns the 10 PointLight render data.
		* @todo infinity pointlight.
		*/
		std::array<PointLightComponent::PointLight, 10> GetPointLight(FrameInfo& frameInfo);

	private:

		/**
		* @brief The struct is VkDescriptorSet container Warpper.
		* Specific for MaxFrameInFlight.
		*/
		struct DescriptorResource
		{
			/**
			* @brief VkDescriptorSet vector.
			* Each set per element.
			*/
			std::vector<VkDescriptorSet> m_DescriptorSets{};
		};

	public:

		/**
		* @brief This struct is copyed from Material Class.
		* Only remains the date that needs to be transfered to shaders.
		*/
		struct TextureParam
		{
			/**
			* @brief The constant instead texture if isn't in use.
			*/
			glm::vec3 constant;

			/**
			* @brief 
			*/
			int isInUse;
			alignas(16) float intensity;

			void CopyFromMaterial(const Material::TextureParam& materialTextureParam) 
			{
				isInUse = materialTextureParam.isInUse.has_value() ? materialTextureParam.isInUse.value() : 0;
				constant = materialTextureParam.constant;
				intensity = materialTextureParam.intensity;
			}
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
			inline PipelineLayoutBuilder& AddTexture(
				uint32_t set,
				uint32_t binding,
				uint32_t arrayNum,
				VkShaderStageFlags stageFlags
			);

			template<typename T>
			inline PipelineLayoutBuilder& AddPushConstant();

			void Build();

		private:
		public:
			Renderer* m_Renderer;
			
			bool isUsePushConstant = false;
			VkPushConstantRange m_PushConstantRange{};
		};

		class RenderBehaverBuilder
		{
		public:
			RenderBehaverBuilder(Renderer* renderer, uint32_t currentFrame);
			virtual ~RenderBehaverBuilder() {};

			void BindPipeline();
			void BindAllBufferTyepDescriptorSet();
			void BindDescriptorSet(uint32_t set, VkDescriptorSet& descriptorset);

			template<typename T, typename F>
			void UpdatePushConstant(F func);

			template<typename T, typename F>
			void UpdateBuffer(uint32_t set, uint32_t binding, F func);

		private:
			Renderer* m_Renderer;
			uint32_t m_CurrentFrame;
		};

		struct Collection 
		{
		public:
			Collection() {};
			virtual ~Collection() {};

			virtual std::unique_ptr<VulkanBuffer>& GetBuffer(uint32_t set, uint32_t binding) = 0;
		};

	protected:
		// vulkan state
		VulkanState& m_VulkanState;
		std::shared_ptr<VulkanDescriptorPool> m_DesctiptorPool;

		// TODO: renderpass
		// VkRenderPass m_RenderPass;

		// descriptorset
		std::vector<std::unique_ptr<VulkanDescriptorSetLayout>> m_VulkanLayouts{};
		std::vector<std::unique_ptr<VulkanDescriptorWriter>> m_VulkanLayoutWriters{};
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
		// group iter will be bug
		auto& view = frameInfo.m_World->GetRegistry().view<T>();
		for (auto& e : view)
		{
			auto& [tComp, transComp] = frameInfo.m_World->GetRegistry().get<T, TransformComponent>(e);

			bool isIterBreak = func((int)e, transComp, tComp);

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
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorSetLayout>>(m_Renderer->m_VulkanLayouts, set + 1);

		m_Renderer->m_VulkanLayouts[set] = VulkanDescriptorSetLayout::Builder(m_Renderer->m_VulkanLayouts[set].get())
			.AddBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags)
			.Build(m_Renderer->m_VulkanState);

		ContainerLibrary::Resize<VkDescriptorSetLayout>(m_Renderer->m_DescriptorSetLayouts, set + 1);

		m_Renderer->m_DescriptorSetLayouts[set] = m_Renderer->m_VulkanLayouts[set]->GetDescriptorSetLayout();


		// writers
		auto bufferInfo = m_Renderer->m_Collections[0]->GetBuffer(set, binding)->GetBufferInfo();
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorWriter>>(m_Renderer->m_VulkanLayoutWriters, set + 1);

		if (m_Renderer->m_VulkanLayoutWriters[set])
		{
			auto writters = m_Renderer->m_VulkanLayoutWriters[set]->GetWritters();
			m_Renderer->m_VulkanLayoutWriters[set] = std::make_unique<VulkanDescriptorWriter>(*m_Renderer->m_VulkanLayouts[set], *m_Renderer->m_DesctiptorPool, writters);
		}
		else
		{
			m_Renderer->m_VulkanLayoutWriters[set] = std::make_unique<VulkanDescriptorWriter>(*m_Renderer->m_VulkanLayouts[set], *m_Renderer->m_DesctiptorPool);
		}
		m_Renderer->m_VulkanLayoutWriters[set]->WriteBuffer(binding, bufferInfo);

		return *this;
	}

	template<typename T>
	inline Renderer::PipelineLayoutBuilder& Renderer::PipelineLayoutBuilder::AddTexture(uint32_t set, uint32_t binding, uint32_t arrayNum, VkShaderStageFlags stageFlags)
	{
		// local data
		/*for (int i = 0; i < MaxFrameInFlight; i++)
		{
			m_Renderer->m_Collections[i]->GetImage(set, binding) = std::make_unique<VulkanImage>(
				m_Renderer->m_VulkanState,
				SPIECS_ENGINE_ASSETS_PATH + "Textures/src/street.jpg"
			);
		}*/


		// descriptorset layout
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorSetLayout>>(m_Renderer->m_VulkanLayouts, set + 1);

		m_Renderer->m_VulkanLayouts[set] = VulkanDescriptorSetLayout::Builder(m_Renderer->m_VulkanLayouts[set].get())
			.AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags, arrayNum)
			.Build(m_Renderer->m_VulkanState);

		ContainerLibrary::Resize<VkDescriptorSetLayout>(m_Renderer->m_DescriptorSetLayouts, set + 1);

		m_Renderer->m_DescriptorSetLayouts[set] = m_Renderer->m_VulkanLayouts[set]->GetDescriptorSetLayout();


		// writers
		/*auto imageInfo = m_Renderer->m_Collections[0]->GetImage(set, binding)->GetImageInfo();
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorWriter>>(m_Renderer->m_VulkanLayoutWriters, set + 1);

		if (m_Renderer->m_VulkanLayoutWriters[set])
		{
			auto writters = m_Renderer->m_VulkanLayoutWriters[set]->GetWritters();
			m_Renderer->m_VulkanLayoutWriters[set] = std::make_unique<VulkanDescriptorWriter>(*m_Renderer->m_VulkanLayouts[set], *m_Renderer->m_DesctiptorPool, writters);
		}
		else
		{
			m_Renderer->m_VulkanLayoutWriters[set] = std::make_unique<VulkanDescriptorWriter>(*m_Renderer->m_VulkanLayouts[set], *m_Renderer->m_DesctiptorPool);
		}
		m_Renderer->m_VulkanLayoutWriters[set]->WriteImage(binding, imageInfo);*/

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

	template<typename T, typename F>
	inline void Renderer::RenderBehaverBuilder::UpdatePushConstant(F func)
	{
		// create pushconstant
		T push{};

		// write in data
		func(push);

		// update 
		vkCmdPushConstants(
			m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
			m_Renderer->m_PipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(T),
			&push
		);
	}

	template<typename T, typename F>
	inline void Renderer::RenderBehaverBuilder::UpdateBuffer(uint32_t set, uint32_t binding, F func)
	{
		// create uniform buffer object
		T ubo{};

		// write in data
		func(ubo);

		// update
		m_Renderer->m_Collections[m_CurrentFrame]->GetBuffer(set, binding)->WriteToBuffer(&ubo);
		m_Renderer->m_Collections[m_CurrentFrame]->GetBuffer(set, binding)->Flush();
	}
}