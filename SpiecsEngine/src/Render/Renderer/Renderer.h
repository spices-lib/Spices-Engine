/**
* @file Renderer.h
* @brief The Renderer Class Definitions.
* @author Spiecs.
*/

#pragma once
/******************************Core Header**********************************************************/
#include "Core/Core.h"
#include "RendererManager.h"
#include "Core/Library/ContainerLibrary.h"
/***************************************************************************************************/

/******************************Vulkan Backend Header************************************************/
#include "Render/FrameInfo.h"
#include "Render/Vulkan/VulkanPipeline.h"
#include "Render/Vulkan/VulkanUtils.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "Render/Vulkan/VulkanRenderPass.h"
/***************************************************************************************************/

/******************************World Component Header***********************************************/
#include "World/World/World.h"
#include "World/Components/MeshComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/CameraComponent.h"
#include "World/Components/UUIDComponent.h"
#include "World/Components/SkyBoxComponent.h"
#include "World/Components/SpriteComponent.h"
/***************************************************************************************************/

/******************************STL Header***********************************************************/
#include <memory>
#include <unordered_map>
/***************************************************************************************************/

namespace Spiecs {

	/**
	* @brief Renderer Class.
	* This class defines the basic behaver of renderer.
	* When we add an new Renderer, we need inherit from this.
	*/
	class Renderer
	{
	public:

		/******************************Basic Function**************************************************/

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The shared pointer of DesctiptorPool, used for allocate descriptor and free descriptor.
		* @param[in] device The shared pointer of VulkanDevice, used for renderpass's formats query.
		* @param[in] rendererResourcePool The shared pointer of RendererResourcePool, used for regist/access rendertarget.
		*/
		Renderer
		(
			const std::string&                      rendererName            , 
			VulkanState&                            vulkanState             , 
			std::shared_ptr<VulkanDescriptorPool>   desctiptorPool          , 
			std::shared_ptr<VulkanDevice>           device                  , 
			std::shared_ptr<RendererResourcePool>   rendererResourcePool
		);

		/**
		* @brief Destructor Function.
		* We destroy pipeline layout and free descriptors that holed by this renderer here.
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

		/***************************************************************************************************/

		/******************************The interface do not needs override now******************************/

		/**
		* @brief This interface is called on rendersystem is registed.
		* Initialize the specific renderer's pipeline, renderpass. framebuffer, descriptor, and so on...
		* @note Though this is a interface, Usually not makeing scene in overriding it.
		*/
		virtual void OnSystemInitialize();

		/***************************************************************************************************/

		/******************************The interface needs override*****************************************/

		/**
		* @brief The interface is called every frame.
		* Defines what data needs to be renderer and how to render.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(FrameInfo& frameInfo) = 0;

		/**
		* @brief This interface is called on Window resized over (regist by swapchain).
		* If the specific renderer uses swpachianimage attachment during CreateRenderPass(), 
		* this interface needs to override, call CreateRenderPass() here just will be fine.
		*/
		virtual void OnWindowResizeOver() {};

		/**
		* @breif This interface is called on Viewport resize (regist by ImguiViewport).
		* If the specific renderer uses the attachment that needs recreated during CreateRenderPass(), 
		* this interface needs to override, call CreateRenderPass() here just will be fine.
		* If the specific renderer uses the input attachment during CreateRenderPass(), 
		* this interface needs to override, see SceneComposeRenderer::OnSlateResize() for sample.
		*/
		virtual void OnSlateResize() {};

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

		/***************************************************************************************************/

	protected:

		/******************************Renderer Help Function**********************************************/

		/**
		* @brief Get sahder path string.
		* @param[in] shaderType What shader type is.
		* @return Returns the sahder path string.
		*/
		std::string GetSahderPath(const std::string& shaderType);

		/**
		* @brief Free all descriptors that the specific renderer holded.
		* Called during Destructor Function, and OnSlateResize() if renderpass uses input attachment.
		* @return Returns true if free successfully.
		*/
		bool FreeResource();

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
		* @todo Mutiple directional light.
		*/
		DirectionalLightComponent::DirectionalLight GetDirectionalLight(FrameInfo& frameInfo);

		/**
		* @brief Get PointLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @return Returns the 10 PointLight render data.
		* @todo infinity pointlight.
		*/
		std::array<PointLightComponent::PointLight, 10> GetPointLight(FrameInfo& frameInfo);

		/***************************************************************************************************/

		/******************************Help Struct*********************************************************/

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
	
		/***************************************************************************************************/

	protected:

		/******************************Help Calss for quick build*******************************************/

		/**
		* @brief This class helps to build a vkpipelinelayout.
		* Only instanced during CreatePipelineLayoutAndDescriptor().
		*/
		class PipelineLayoutBuilder
		{
		public:

			/**
			* @brief Constructor Function.
			* @param[in] renderer When instanecd during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			*/
			PipelineLayoutBuilder(Renderer* renderer);

			/**
			* @brief Destructor Function.
			*/
			virtual ~PipelineLayoutBuilder() {};

			/**
			* @brief Create specific renderer buffer type collection for renderer.
			* @param[in] T Specific collection struct.
			* @return Returns this reference.
			* @see Collection.
			*/
			template<typename T>
			inline PipelineLayoutBuilder& CreateCollection();

			/**
			* @brief Create local buffer object in collection, and add it's set binding to descriptorsetlayout, and sets descriptorwriter using it's buffer info.
			* @param[in] T Buffer struct.
			* @param[in] set Which set this buffer wil use.
			* @param[in] binding Which binding this buffer will use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @return Returns this reference.
			*/
			template<typename T>
			inline PipelineLayoutBuilder& AddBuffer(
				uint32_t set, 
				uint32_t binding, 
				VkShaderStageFlags stageFlags
			);

			/**
			* @brief Add the texture set binding to descriptorsetlayout.
			* @param[in] T Texture Type.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] arrayNum Which arrayNum this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @return Returns this reference.
			*/
			template<typename T>
			inline PipelineLayoutBuilder& AddTexture(
				uint32_t set,
				uint32_t binding,
				uint32_t arrayNum,
				VkShaderStageFlags stageFlags
			);

			/**
			* @brief Add the texture set binding to descriptorsetlayout.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @return Returns this reference.
			*/
			inline PipelineLayoutBuilder& AddInput(
				uint32_t set,
				uint32_t binding,
				uint32_t arrayNum,
				VkShaderStageFlags stageFlags,
				const std::vector<std::string>& inputAttachmentNames
			);

			/**
			* @brief Set VkPushConstantRange by a specific pushconstant struct.
			* @param[in] T Specific pushconstant struct.
			* @return Returns this reference.
			*/
			template<typename T>
			inline PipelineLayoutBuilder& AddPushConstant();

			/**
			* @brief Create all buffer type descriptor set.
			* Create pipeline layout.
			* @attention Texture type descriptor set is not created here, but in Material::BuildMaterial().
			*/
			void Build();

		public:

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanecd.
			*/
			Renderer* m_Renderer;
			
			/**
			* @brief True if the specific renderer enable pushconstant in pipelinelayout. 
			*/
			bool isUsePushConstant = false;

			/**
			* @brief If isUsePushConstant is true, this variable must be filled.
			*/
			VkPushConstantRange m_PushConstantRange{};
		};

		/**
		* @brief This class helps to bind pipeline and bind buffer.
		* Only instanced during Render().
		*/
		class RenderBehaverBuilder
		{
		public:

			/**
			* @brief Constructor Function.
			* Bind pipleine and all buffer type descriptorset.
			* @param[in] renderer When instanecd during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			* @param[in] currentFrame Passed from FrameInfo.
			*/
			RenderBehaverBuilder(Renderer* renderer, uint32_t currentFrame, uint32_t currentImage);

			/**
			* @brief Destructor Function.
			*/
			virtual ~RenderBehaverBuilder() {};

		private:

			/**
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaverBuilder instanced.
			*/
			void BindPipeline();

			/**
			* @brief Bind all buffer type descriptorset.
			* Called On RenderBehaverBuilder instanced.
			*/
			void BindAllBufferTyepDescriptorSet();

		public:

			/**
			* @brief Bind one single descriptor.
			* @param[in] set Which set the descriptor will use.
			* @param[in] descriptorset Which descriptor will be binded.
			* @attemtion For specific renderer, this API only used for texture bind.
			*/
			void BindDescriptorSet(uint32_t set, VkDescriptorSet& descriptorset);

			/**
			* @brief Update local pushconstant buffer.
			* @param[in] T Specific push constant struct Type.
			* @param[in] F A function pointer, which defines what data inside the buffer.
			*/
			template<typename T, typename F>
			void UpdatePushConstant(F func);

			/**
			* @brief Update a local buffer.
			* @param[in] T Specific buffer struct Type.
			* @param[in] F A function pointer, which defines what data inside the buffer.
			* @param[in] set Which set the descriptor will use.
			* @param[in] binding Which binding the descriptor will use.
			*/
			template<typename T, typename F>
			void UpdateBuffer(uint32_t set, uint32_t binding, F func);


			void BindInputDescriptorSet();

			/**
			* @brief Begin this Renderer's RenderPass.
			* Call it auto.
			*/
			void BeginRenderPass();

			/**
			* @brief End this Renderer's RenderPass.
			* Call it manually.
			*/
			void EndRenderPass();

		private:

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanecd.
			*/
			Renderer* m_Renderer;

			/**
			* @brief Current frame index.
			* Passed while this class instanecd.
			* @see FrameInfo.
			*/
			uint32_t m_CurrentFrame;

			/**
			* @brief Current frame index.
			* Passed while this class instanecd.
			* @see FrameInfo.
			*/
			uint32_t m_CurrentImage;
		};

		/***************************************************************************************************/

		/**
		* @brief This struct placed the local buffer data.Specific for every renderer.
		* Needed to be inherited for specific renderer.
		* @attention Only place buffer data here, not allowed texture data.
		* Texture data is placed in Material::TextureParam.
		*/
		struct Collection 
		{
		public:

			/**
			* @brief Constructor Function.
			*/
			Collection() {};

			/**
			* @brief Destructor Function.
			*/
			virtual ~Collection() {};

			/**
			* @brief The interface of how to map the local buffer with specific set and binding.
			* @param[in] set Specific set.
			* @param[in] binding Specific binding.
			* @return Returns the local buffer smart pointor.
			*/
			virtual std::unique_ptr<VulkanBuffer>& GetBuffer(uint32_t set, uint32_t binding) = 0;
		};

	protected:
		
		/**
		* @brief This variable is passed while renderer instaned.
		*/
		VulkanState& m_VulkanState;

		/**
		* @brief This variable is passed while renderer instaned.
		*/
		std::shared_ptr<VulkanDescriptorPool> m_DesctiptorPool;

		std::shared_ptr<VulkanDevice> m_Device;

		std::shared_ptr<RendererResourcePool> m_RendererResourcePool;

		/**
		* @brief This variable is a Wapper of VkRenderPass.
		*/
		std::unique_ptr<VulkanRenderPass> m_RenderPass;

		/**
		* @brief This variable helps to set vkpipelinelayout.
		* Each set per element.
		* Fill with both buffer and texture.
		*/
		std::vector<std::unique_ptr<VulkanDescriptorSetLayout>> m_VulkanLayouts{};

		/**
		* @brief VkDescriptorSetLayout used while create pipeline layout.
		* Each set per element.
		* Fill with both buffer and texture.
		*/
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts{};

		/**
		* @brief This variable is descriptor warpper.
		* @note Only fill with buffer.
		*/
		std::array<DescriptorResource, MaxFrameInFlight> m_Resource{};

		/**
		* @brief This variable is local buffer warpper.
		* @note Only fill with buffer.
		*/
		std::array<std::unique_ptr<Collection>, MaxFrameInFlight> m_Collections;

		/**
		* @brief Specific renderer pipelinelayout, defined by CreatePipelineLayoutAndDescriptor().
		*/
		VkPipelineLayout m_PipelineLayout{};

		/**
		* @brief Specific renderer pipeline, defined by CreatePipeline().
		*/
		std::unique_ptr<VulkanPipeline> m_VulkanPipeline;
		
		/**
		* @brief Specific renderer name, Passed by instanced.
		*/
		std::string m_RendererName;

		/**
		* @brief Allow this class access all data.
		* Maybe remove.
		*/
		friend class PipelineLayoutBuilder;
	};

	template<typename T, typename F>
	inline void Renderer::IterWorldComp(FrameInfo& frameInfo, F func)
	{
		/**
		* @brief Iter use view, not group.
		* @attention Group result nullptr here.
		*/
		auto& view = frameInfo.m_World->GetRegistry().view<T>();
		for (auto& e : view)
		{
			auto& [tComp, transComp] = frameInfo.m_World->GetRegistry().get<T, TransformComponent>(e);

			/**
			* @brief This function defined how we use these components.
			* @param[in] e entityid.
			* @param[in] transComp TransformComponent.
			* @param[in] tComp TComponent.
			* @return Returns true if need break for for loop.
			*/
			bool isIterBreak = func((int)e, transComp, tComp);

			/**
			* @brief Whether break this for loop.
			*/
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
		/**
		* @breif Create local buffer for each Frame.
		*/
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


		/**
		* @brief Create descriptorsetlayout.
		*/
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorSetLayout>>(m_Renderer->m_VulkanLayouts, set + 1);

		m_Renderer->m_VulkanLayouts[set] = VulkanDescriptorSetLayout::Builder(m_Renderer->m_VulkanLayouts[set].get())
			.AddBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags)
			.Build(m_Renderer->m_VulkanState);

		ContainerLibrary::Resize<VkDescriptorSetLayout>(m_Renderer->m_DescriptorSetLayouts, set + 1);

		m_Renderer->m_DescriptorSetLayouts[set] = m_Renderer->m_VulkanLayouts[set]->GetDescriptorSetLayout();


		/**
		* @brief Create descriptorWriter.
		*/
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
		/**
		* @brief Create descriptorsetlayout.
		*/
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorSetLayout>>(m_Renderer->m_VulkanLayouts, set + 1);

		m_Renderer->m_VulkanLayouts[set] = VulkanDescriptorSetLayout::Builder(m_Renderer->m_VulkanLayouts[set].get())
			.AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags, arrayNum)
			.Build(m_Renderer->m_VulkanState);

		ContainerLibrary::Resize<VkDescriptorSetLayout>(m_Renderer->m_DescriptorSetLayouts, set + 1);

		m_Renderer->m_DescriptorSetLayouts[set] = m_Renderer->m_VulkanLayouts[set]->GetDescriptorSetLayout();


		/**
		* @brief Create descriptorWriter.
		* Not execte here.
		*/
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

	inline Renderer::PipelineLayoutBuilder::PipelineLayoutBuilder(Renderer* renderer)
		: m_Renderer(renderer)
	{
		m_Renderer->m_VulkanLayouts.clear();
		m_Renderer->m_DescriptorSetLayouts.clear();
		m_Renderer->m_VulkanLayoutWriters.clear();
	}

	inline Renderer::PipelineLayoutBuilder& Renderer::PipelineLayoutBuilder::AddInput(uint32_t set, uint32_t binding, uint32_t arrayNum, VkShaderStageFlags stageFlags, const std::vector<std::string>& inputAttachmentNames)
	{
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorSetLayout>>(m_Renderer->m_VulkanLayouts, set + 1);

		m_Renderer->m_VulkanLayouts[set] = VulkanDescriptorSetLayout::Builder(m_Renderer->m_VulkanLayouts[set].get())
			.AddBinding(binding, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, stageFlags, arrayNum)
			.Build(m_Renderer->m_VulkanState);

		ContainerLibrary::Resize<VkDescriptorSetLayout>(m_Renderer->m_DescriptorSetLayouts, set + 1);

		m_Renderer->m_DescriptorSetLayouts[set] = m_Renderer->m_VulkanLayouts[set]->GetDescriptorSetLayout();

		std::vector<VkDescriptorImageInfo> imageInfos{};

		for (auto& pair : inputAttachmentNames)
		{
			imageInfos.push_back(*m_Renderer->m_RendererResourcePool->AccessResource(pair));
		}

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
		m_Renderer->m_VulkanLayoutWriters[set]->WriteInput(binding, imageInfos);

		return *this;
	}

	template<typename T>
	inline Renderer::PipelineLayoutBuilder& Renderer::PipelineLayoutBuilder::AddPushConstant()
	{
		/**
		* @brief Set variable.
		*/
		isUsePushConstant = true;

		/**
		* @brief Fill in data.
		*/
		m_PushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		m_PushConstantRange.offset = 0;
		m_PushConstantRange.size = sizeof(T);

		return *this;
	}

	template<typename T, typename F>
	inline void Renderer::RenderBehaverBuilder::UpdatePushConstant(F func)
	{
		/**
		* @breif Create pushconstant object.
		*/
		T push{};

		/**
		* @breif Write in data
		* @param[in] push pushconstant object.
		*/
		func(push);

		/**
		* @breif Update PushConstants
		*/
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
		/**
		* @breif Create uniform buffer object
		*/
		T ubo{};

		/**
		* @breif Write in data
		* @param[in] uniform buffer object.
		*/
		func(ubo);

		/**
		* @breif Update uniform buffer.
		*/
		m_Renderer->m_Collections[m_CurrentFrame]->GetBuffer(set, binding)->WriteToBuffer(&ubo);
		m_Renderer->m_Collections[m_CurrentFrame]->GetBuffer(set, binding)->Flush();
	}
}