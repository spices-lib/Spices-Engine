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
#include "DescriptorSetManager/DescriptorSetManager.h"
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
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) = 0;

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

		void RegistyMaterial(const std::string& materialName);

	private:

		/**
		* @brief The interface is called during OnSystemInitialize().
		* Create specific renderpass.
		* @todo Implementate specific renderpass
		*/
		virtual void CreateRenderPass() = 0;

		virtual void CreateDescriptorSet() = 0;

		VkPipelineLayout CreatePipelineLayout(
			const DescriptorSetInfo& preRendererSetInfo,
			const DescriptorSetInfo& specificRendererSetInfo,
			const DescriptorSetInfo& materialSetInfo
		);

		std::shared_ptr<VulkanPipeline> CreatePipeline(
			std::shared_ptr<Material> material, 
			VkRenderPass&             renderPass, 
			VkPipelineLayout&         layout
		);

		/***************************************************************************************************/

	protected:

		/******************************Renderer Help Function**********************************************/

		/**
		* @brief Get sahder path string.
		* @param[in] shaderType What shader type is.
		* @return Returns the sahder path string.
		*/
		std::string GetSahderPath(const std::string& name, const std::string& shaderType);

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

	protected:

		/******************************Help Calss for quick build*******************************************/

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

			/**
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaverBuilder instanced.
			*/
			void BindPipeline(const std::string& materialName);

		public:

			void BindDescriptorSet(DescriptorSetInfo& infos);

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

		class DescriptorSetBuilder
		{
		public:
			/**
			* @brief Constructor Function.
			* @param[in] renderer When instanecd during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			*/
			DescriptorSetBuilder(Renderer* renderer) : m_Renderer(renderer) {};

			/**
			* @brief Destructor Function.
			*/
			virtual ~DescriptorSetBuilder() {};


			/**
			* @brief Set VkPushConstantRange by a specific pushconstant struct.
			* @param[in] T Specific pushconstant struct.
			* @return Returns this reference.
			*/
			template<typename T>
			inline DescriptorSetBuilder& AddPushConstant();

			/**
			* @brief Create local buffer object in collection, and add it's set binding to descriptorsetlayout, and sets descriptorwriter using it's buffer info.
			* @param[in] T Buffer struct.
			* @param[in] set Which set this buffer wil use.
			* @param[in] binding Which binding this buffer will use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @return Returns this reference.
			*/
			template<typename T>
			inline DescriptorSetBuilder& AddBuffer(
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
			inline DescriptorSetBuilder& AddTexture(
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
			DescriptorSetBuilder& AddInput(
				uint32_t set,
				uint32_t binding,
				uint32_t arrayNum,
				VkShaderStageFlags stageFlags,
				const std::vector<std::string>& inputAttachmentNames
			);

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

			std::unordered_map<uint32_t, std::unordered_map<uint32_t, VkDescriptorBufferInfo>> m_BufferInfos;
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
		* @brief Specific renderer name, Passed by instanced.
		*/
		std::string m_RendererName;

		/**
		* @brief Allow this class access all data.
		* Maybe remove.
		*/
		friend class DescriptorSetBuilder;

		std::unordered_map<std::string, std::shared_ptr<VulkanPipeline>> m_Pipelines;

		VkPushConstantRange m_PushConstantRange{};

		/**
		* @brief True if the specific renderer enable pushconstant in pipelinelayout.
		*/
		bool isUsePushConstant = false;

		std::unordered_map<Int2, std::unique_ptr<VulkanBuffer>> m_Buffers;
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
			m_Renderer->m_Pipelines["Default"]->GetPipelineLayout(),
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
		m_Renderer->m_Buffers[{set, binding}]->WriteToBuffer(&ubo);
		m_Renderer->m_Buffers[{set, binding}]->Flush();
	}

	template<typename T>
	inline Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddPushConstant()
	{
		/**
		* @brief Set variable.
		*/
		m_Renderer->isUsePushConstant = true;

		/**
		* @brief Fill in data.
		*/
		m_Renderer->m_PushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		m_Renderer->m_PushConstantRange.offset = 0;
		m_Renderer->m_PushConstantRange.size = sizeof(T);

		return *this;
	}

	template<typename T>
	inline Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddBuffer(
		uint32_t           set        , 
		uint32_t           binding    , 
		VkShaderStageFlags stageFlags
	)
	{
		Int2 id(set, binding);

		/**
		* @brief Creating VulkanBuffer.
		*/
		m_Renderer->m_Buffers[id] = std::make_unique<VulkanBuffer>(
			m_Renderer->m_VulkanState,
			sizeof(T),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		/**
		* @brief Map with host memory and video memory.
		*/
		m_Renderer->m_Buffers[id]->Map();

		/**
		* @brief fill in bufferInfos.
		*/
		m_BufferInfos[set][binding] = *m_Renderer->m_Buffers[id]->GetBufferInfo();

		auto descriptorSet = DescriptorSetManager::Registy(m_Renderer->m_RendererName, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags, 1);

		return *this;
	}

	template<typename T>
	inline Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddTexture(
		uint32_t           set        ,
		uint32_t           binding    ,
		uint32_t           arrayNum   ,
		VkShaderStageFlags stageFlags )
	{
		return *this;
	}
}