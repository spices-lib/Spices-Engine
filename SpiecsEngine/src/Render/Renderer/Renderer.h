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
#include "Render/Renderer/RendererPass/RendererPass.h"
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
			std::shared_ptr<RendererResourcePool>   rendererResourcePool    ,
			bool isLoadDefaultMaterial = true
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
		* this interface needs to override, callOnSlateResize here just will be fine.
		*/
		virtual void OnWindowResizeOver() {};

		/**
		* @breif This interface is called on Viewport resize (regist by ImguiViewport).
		* If the specific renderer uses the attachment that needs recreated during CreateRenderPass(), 
		* this interface needs to override, call CreateRenderPass() and CreateDescriptorSet() here just will be fine.
		*/
		virtual void OnSlateResize();

		/**
		* @brief Register material to Specific Renderer.
		* @param[in] materialName Material Name.
		* @param[in] subpassName SubPass Name.
		*/
		void RegistyMaterial(
			const std::string& materialName , 
			const std::string& subpassName
		);

		/**
		* @brief Get RendererPass.
		* @return Returns the RendererPass.
		*/
		inline std::shared_ptr<RendererPass>& GetPasses() { return m_Pass; };

	private:

		/**
		* @brief The interface is called during OnSystemInitialize().
		* Create specific renderpass.
		* @todo Implementate specific renderpass
		*/
		virtual void CreateRendererPass() = 0;

		/**
		* @brief This interface is called buring OnSystemInitialize().
		* Create all subpass descriptorSet.
		*/
		virtual void CreateDescriptorSet() = 0;

		/**
		* @brief Create Specific Renderer Default Material. 
		*/
		void CreateDefaultMaterial();

		/**
		* @brief Create Pipeline Layout with material's descriptorset and renderer's descriptorset.
		* @param[in] rowSetLayouts All descriptorset collected.
		* @param[in] subPass material used subpass.
		*/
		VkPipelineLayout CreatePipelineLayout(
			std::vector<VkDescriptorSetLayout>& rowSetLayouts , 
			std::shared_ptr<RendererSubPass>    subPass
		);

		/**
		* @brief Create Specific Material Pipeline.
		* @param[in] material Specific Material.
		* @param[in] layout VkPipelineLayout.
		* @param[in] subPass RendererSubPass.
		* @todo Not be virtual and configurable with material.
		*/
		virtual std::shared_ptr<VulkanPipeline> CreatePipeline(
			std::shared_ptr<Material>        material  , 
			VkPipelineLayout&                layout    ,
			std::shared_ptr<RendererSubPass> subPass
		);

		/***************************************************************************************************/

	protected:

		/******************************Renderer Help Function**********************************************/

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
		* @param[out] directionalLight DirectionalLight.
		* @todo Mutiple directional light.
		*/
		void GetDirectionalLight(FrameInfo& frameInfo, DirectionalLightComponent::DirectionalLight& directionalLight);

		/**
		* @brief Get PointLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @param[out] pLightArrat PointLight.
		* @todo infinity pointlight.
		*/
		void GetPointLight(FrameInfo& frameInfo, std::array<PointLightComponent::PointLight, 1000>& pLightArrat);

		/***************************************************************************************************/

	protected:

		/******************************Help Calss for quick build*******************************************/

		/**
		* @brief This Class is a helpper for Buinding RendererPass.
		* Only instanced during CreateRendererPass().
		*/
		class RendererPassBuilder
		{
		public:

			/**
			* @brief Constructor Function.
			* @param[in] rendererPassName The RendererPass Name.
			* @param[in] renderer The Owner, pass this pointer.
			*/
			RendererPassBuilder(
				const std::string& rendererPassName , 
				Renderer*          renderer
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~RendererPassBuilder() {};

			/**
			* @brief Add a new SubPass to RedererPass.
			* @param[in] subPassName SunPass Name.
			* @return Returns the RendererPassBuilder.
			*/
			RendererPassBuilder& AddSubPass(const std::string& subPassName);

			/**
			* @brief Add a swpachianimage attachment.
			* @param[in] T Defines Specific VkAttachmentDescription.
			* @return Returns the RendererPassBuilder.
			*/
			template<typename T>
			RendererPassBuilder& AddSwapChainAttachment(T func);

			/**
			* @brief Add a color attachment.
			* @param[in] attachmentName Attachment Name.
			* @param[in] type Attachment TextureType.
			* @param[in] T Defines Specific VkAttachmentDescription.
			* @return Returns the RendererPassBuilder.
			*/
			template<typename T>
			RendererPassBuilder& AddColorAttachment(
				const std::string& attachmentName ,
				TextureType        type           ,
				T                  func
			);

			/**
			* @brief Add a depth attachment.
			* @param[in] T Defines Specific VkAttachmentDescription.
			* @return Returns the RendererPassBuilder.
			*/
			template<typename T>
			RendererPassBuilder& AddDepthAttachment(T func);

			/**
			* @brief Add a input attachment.
			* @param[in] attachmentName Attachment Name.
			* @param[in] type Attachment TextureType.
			* @param[in] T Defines Specific VkAttachmentDescription.
			* @return Returns the RendererPassBuilder.
			*/
			template<typename T>
			RendererPassBuilder& AddInputAttachment(
				const std::string& attachmentName , 
				TextureType        type           ,
				T                  func
			);

			/**
			* @brief End recording a subpass.
			* @return Returns the RendererPassBuilder.
			*/
			RendererPassBuilder& EndSubPass();

			/**
			* @brief Build the RendererPass.
			*/
			void Build();

		private:

			/**
			* @brief RendererPass Name.
			*/
			std::string m_RendererPassName;

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanecd.
			*/
			Renderer* m_Renderer;

			/**
			* @brief Handled Subpass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledRendererSubPass;
		};

		/**
		* @brief This Class is a helpper for Building Specific Renderer DescriptorSet.
		* Only instanced during CreateDescriptorSets().
		*/
		class DescriptorSetBuilder
		{
		public:

			/**
			* @brief Constructor Function.
			* @param[in] subPassName Subpass name.
			* @param[in] renderer When instanecd during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			*/
			DescriptorSetBuilder(
				const std::string& subPassName      ,
				Renderer*          renderer
			);

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
			inline DescriptorSetBuilder& AddUniformBuffer(
				uint32_t            set      ,
				uint32_t            binding  ,
				VkShaderStageFlags  stageFlags
			);

			/**
			* @brief Create local buffer object in collection, and add it's set binding to descriptorsetlayout, and sets descriptorwriter using it's buffer info.
			* @param[in] T Buffer struct.
			* @param[in] set Which set this buffer wil use.
			* @param[in] binding Which binding this buffer will use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @return Returns this reference.
			*/
			template<typename T>
			inline DescriptorSetBuilder& AddStorageBuffer(
				uint32_t            set      ,
				uint32_t            binding  ,
				VkShaderStageFlags  stageFlags
			);

			/**
			* @brief Add the texture set binding to descriptorsetlayout.
			* @param[in] T Texture Type.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @param[in] textureNames All Texture's Name.
			* @return Returns this reference.
			*/
			template<typename T>
			inline DescriptorSetBuilder& AddTexture(
				uint32_t                         set        ,
				uint32_t                         binding    ,
				VkShaderStageFlags               stageFlags ,
				const std::vector<std::string>&  textureNames
			);

			/**
			* @brief Add the attachment as texture to descriptorsetlayout.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @param[in] textureNames All Texture's Name.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddAttachmentTexture(
				uint32_t                         set        ,
				uint32_t                         binding    ,
				VkShaderStageFlags               stageFlags ,
				const std::vector<std::string>&  textureNames
			);

			/**
			* @brief Add the texture set binding to descriptorsetlayout.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @param[in] inputAttachmentNames All Input Texture's Name..
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddInput(
				uint32_t                         set                 ,
				uint32_t                         binding             ,
				VkShaderStageFlags               stageFlags          ,
				const std::vector<std::string>&  inputAttachmentNames
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

			/**
			* @brief Handled Subpass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledSubPass;

			/**
			* @brief The DesctiptorSetManager ID descriptorSet belongs to.
			*/
			String2 m_DescriptorSetId;

			/**
			* @brief DescriptorSet Buffer type binginginfo.
			* [set] - [binding] - [info]
			*/
			std::unordered_map<uint32_t, std::unordered_map<uint32_t, VkDescriptorBufferInfo>> m_BufferInfos;

			/**
			* @brief DescriptorSet Image type binginginfo.
			* [set] - [binding] - [info]
			*/
			std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>>> m_ImageInfos;
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
			RenderBehaverBuilder(
				Renderer* renderer     , 
				uint32_t  currentFrame , 
				uint32_t  currentImage
			);

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

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			*/
			void BindDescriptorSet(DescriptorSetInfo& infos);

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			*/
			void BindDescriptorSet(DescriptorSetInfo& infos, const std::string& name);

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
			void UpdateUniformBuffer(uint32_t set, uint32_t binding, F func);

			/**
			* @brief Update a local buffer.
			* @param[in] T Specific buffer struct Type.
			* @param[in] F A function pointer, which defines what data inside the buffer.
			* @param[in] set Which set the descriptor will use.
			* @param[in] binding Which binding the descriptor will use.
			*/
			template<typename T, typename F>
			void UpdateStorageBuffer(uint32_t set, uint32_t binding, F func);

			/**
			* @brief End a preview subpass and stat next subpass.
			* @param[in] subpassName The name of next subpass.
			*/
			void BeginNextSubPass(const std::string& subpassName);

			/**
			* @brief Begin this Renderer's RenderPass.
			*/
			void BeginRenderPass();

			/**
			* @brief End this Renderer's RenderPass.
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

			/**
			* @brief Handled Subpass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledSubPass;
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

		/**
		* @brief VulkanDevice , Passed by instanced.
		*/
		std::shared_ptr<VulkanDevice> m_Device;

		/**
		* @brief RendererResourcePool, Passed by instanced.
		*/
		std::shared_ptr<RendererResourcePool> m_RendererResourcePool;

		/**
		* @brief RendererPass.
		*/
		std::shared_ptr<RendererPass> m_Pass;
		
		/**
		* @brief Specific renderer name, Passed by instanced.
		*/
		std::string m_RendererName;

		/**
		* @brief Renderer stored material pipelines.
		*/
		std::unordered_map<std::string, std::shared_ptr<VulkanPipeline>> m_Pipelines;
		
		/**
		* @brief Whether should load a default renderer material.
		*/
		bool m_IsLoadDefaultMaterial;

		/**
		* @brief Allow this class access all data.
		*/
		friend class DescriptorSetBuilder;
		friend class RendererPassBuilder;
	};

	template<typename T, typename F>
	inline void Renderer::IterWorldComp(FrameInfo& frameInfo, F func)
	{
		SPIECS_PROFILE_ZONE;

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
		SPIECS_PROFILE_ZONE;

		/**
		* @breif Create pushconstant object.
		*/
		T push{};

		/**
		* @breif Write in data
		* @param[in] push pushconstant object.
		*/
		func(push);

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		/**
		* @breif Update PushConstants
		*/
		vkCmdPushConstants(
			m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
			m_Renderer->m_Pipelines[ss.str()]->GetPipelineLayout(),
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(T),
			&push
		);
	}

	template<typename T, typename F>
	inline void Renderer::RenderBehaverBuilder::UpdateUniformBuffer(uint32_t set, uint32_t binding, F func)
	{
		SPIECS_PROFILE_ZONE;

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
		m_HandledSubPass->SetBuffer({ set, binding }, &ubo);
	}

	template<typename T, typename F>
	inline void Renderer::RenderBehaverBuilder::UpdateStorageBuffer(uint32_t set, uint32_t binding, F func)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @breif Create uniform buffer object
		*/
		T ssbo{};

		/**
		* @breif Write in data
		* @param[in] uniform buffer object.
		*/
		func(ssbo);

		/**
		* @breif Update uniform buffer.
		*/
		m_HandledSubPass->SetBuffer({ set, binding }, &ssbo);
	}

	template<typename T>
	inline Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddPushConstant()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Call RendererSubPass::SetPushConstant().
		*/
		m_HandledSubPass->SetPushConstant([&](auto& range) {
			range.stageFlags   = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
			range.offset       = 0;
			range.size         = sizeof(T);
		});

		return *this;
	}

	template<typename T>
	inline Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddUniformBuffer(
		uint32_t           set        , 
		uint32_t           binding    , 
		VkShaderStageFlags stageFlags
	)
	{
		SPIECS_PROFILE_ZONE;

		Int2 id(set, binding);

		/**
		* @brief Creating VulkanBuffer.
		*/
		m_HandledSubPass->GetBuffers(id) = std::make_unique<VulkanBuffer>(
			m_Renderer->m_VulkanState,
			sizeof(T),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		/**
		* @brief Map with host memory and video memory.
		*/
		m_HandledSubPass->GetBuffers(id)->Map();

		/**
		* @brief fill in bufferInfos.
		*/
		m_BufferInfos[set][binding] = *m_HandledSubPass->GetBuffers(id)->GetBufferInfo();

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags, 1);

		return *this;
	}

	template<typename T>
	inline Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddStorageBuffer(
		uint32_t           set        , 
		uint32_t           binding    , 
		VkShaderStageFlags stageFlags
	)
	{
		SPIECS_PROFILE_ZONE;

		Int2 id(set, binding);

		/**
		* @brief Creating VulkanBuffer.
		*/
		m_HandledSubPass->GetBuffers(id) = std::make_unique<VulkanBuffer>(
			m_Renderer->m_VulkanState,
			sizeof(T),
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		/**
		* @brief Map with host memory and video memory.
		*/
		m_HandledSubPass->GetBuffers(id)->Map();

		/**
		* @brief fill in bufferInfos.
		*/
		m_BufferInfos[set][binding] = *m_HandledSubPass->GetBuffers(id)->GetBufferInfo();

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, stageFlags, 1);

		return *this;
	}

	template<typename T>
	inline Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddTexture(
		uint32_t           set        ,
		uint32_t           binding    ,
		VkShaderStageFlags stageFlags ,
		const std::vector<std::string>& textureNames
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < textureNames.size(); i++)
		{
			std::shared_ptr<Texture> texture = ResourcePool<Texture>::Load<T>(textureNames[i]);
			m_ImageInfos[set][binding].push_back(*texture->GetResource<VulkanImage>()->GetImageInfo());
		}

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags, textureNames.size());

		return *this;
	}

	template<typename T>
	inline Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddSwapChainAttachment(T func)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Instance a VkAttachmentDescription.
		*/
		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format         = m_Renderer->m_Device->GetSwapChainSupport().format.format;
		attachmentDescription.samples        = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachmentDescription.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescription.initialLayout  = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		attachmentDescription.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		/**
		* @breif Write in data
		* @param[in] attachmentDescription VkAttachmentDescription.
		*/
		func(attachmentDescription);

		/**
		* @brief Instance a VkClearValue.
		*/
		VkClearValue clearValue{};
		clearValue.color                     = { 0.0f, 0.0f, 0.0f, 1.0f };

		/**
		* @brief Instance a VkPipelineColorBlendAttachmentState.
		*/
		VkPipelineColorBlendAttachmentState colorBlend{};
		colorBlend.colorWriteMask            = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			                                   VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlend.blendEnable               = VK_FALSE;
		colorBlend.srcColorBlendFactor       = VK_BLEND_FACTOR_ONE;
		colorBlend.dstColorBlendFactor       = VK_BLEND_FACTOR_ZERO;
		colorBlend.colorBlendOp              = VK_BLEND_OP_ADD;
		colorBlend.srcAlphaBlendFactor       = VK_BLEND_FACTOR_ONE;
		colorBlend.dstAlphaBlendFactor       = VK_BLEND_FACTOR_ZERO;
		colorBlend.alphaBlendOp              = VK_BLEND_OP_ADD;

		uint32_t index = m_Renderer->m_Pass->AddAttachment("SwapChainImage", attachmentDescription, clearValue);

		/**
		* @brief Instance a VkAttachmentReference.
		*/
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment             = index;
		attachmentRef.layout                 = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		m_HandledRendererSubPass->AddColorAttachmentReference(attachmentRef, colorBlend);

		return *this;
	}

	template<typename T>
	inline Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddColorAttachment(
		const std::string& attachmentName , 
		TextureType        type           ,
		T                  func
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Instance a VkAttachmentDescription.
		*/
		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format            = m_Renderer->m_Device->GetSwapChainSupport().format.format;
		attachmentDescription.samples           = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp            = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachmentDescription.storeOp           = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp     = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp    = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescription.initialLayout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachmentDescription.finalLayout       = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		/**
		* @brief Write in data.
		* @param[in] isEnableBlend True if This Attachment can be blend.
		* @param[in attachmentDescription VkAttachmentDescription.
		*/
		bool isEnableBlend = false;
		func(isEnableBlend, attachmentDescription);

		/**
		* @brief Instance a VkClearValue.
		*/
		VkClearValue clearValue{};
		clearValue.color                        = { 0.0f, 0.0f, 0.0f, 1.0f };

		/**
		* @brief Instance a VkPipelineColorBlendAttachmentState.
		*/
		VkPipelineColorBlendAttachmentState colorBlend{};
		colorBlend.colorWriteMask               = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			                                      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		if (!isEnableBlend)
		{
			colorBlend.blendEnable              = VK_FALSE;
			colorBlend.srcColorBlendFactor      = VK_BLEND_FACTOR_ONE;
			colorBlend.dstColorBlendFactor      = VK_BLEND_FACTOR_ZERO;
			colorBlend.colorBlendOp             = VK_BLEND_OP_ADD;
			colorBlend.srcAlphaBlendFactor      = VK_BLEND_FACTOR_ONE;
			colorBlend.dstAlphaBlendFactor      = VK_BLEND_FACTOR_ZERO;
			colorBlend.alphaBlendOp             = VK_BLEND_OP_ADD;
		}
		else
		{
			colorBlend.blendEnable              = VK_TRUE;
			colorBlend.srcColorBlendFactor      = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlend.dstColorBlendFactor      = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlend.colorBlendOp             = VK_BLEND_OP_ADD;
			colorBlend.srcAlphaBlendFactor      = VK_BLEND_FACTOR_ONE;
			colorBlend.dstAlphaBlendFactor      = VK_BLEND_FACTOR_ZERO;
			colorBlend.alphaBlendOp             = VK_BLEND_OP_ADD;
		}

		/**
		* @brief Instance a RendererResourceCreateInfo.
		*/
		RendererResourceCreateInfo Info;
		Info.name                               = attachmentName;
		Info.type                               = type;
		Info.description                        = attachmentDescription;
		Info.width                              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.width;
		Info.height                             = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height;

		VkImageView& view = m_Renderer->m_RendererResourcePool->AccessResource(Info)->imageView;

		uint32_t index = m_Renderer->m_Pass->AddAttachment(attachmentName, attachmentDescription, clearValue, view);

		/**
		* @brief Instance a VkAttachmentReference.
		*/
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment                = index;
		attachmentRef.layout                    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		m_HandledRendererSubPass->AddColorAttachmentReference(attachmentRef, colorBlend);
		
		return *this;
	}

	template<typename T>
	inline Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddDepthAttachment(T func)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Instance a VkAttachmentDescription.
		*/
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format                  = VulkanSwapChain::FindDepthFormat(m_Renderer->m_VulkanState.m_PhysicalDevice);
		depthAttachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_LOAD;
		depthAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
		depthAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout           = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthAttachment.finalLayout             = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		/**
		* @brief Write in data.
		* @param[in attachmentDescription VkAttachmentDescription.
		*/
		func(depthAttachment);

		/**
		* @brief Instance a VkClearValue.
		*/
		VkClearValue clearValue{};
		clearValue.depthStencil                 = { 1.0f, 0 };

		/**
		* @brief Instance a RendererResourceCreateInfo.
		*/
		RendererResourceCreateInfo Info;
		Info.name                               = "Depth";
		Info.description                        = depthAttachment;
		Info.width                              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.width;
		Info.height                             = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height;
		Info.isDepthResource                    = true;

		VkImageView& view = m_Renderer->m_RendererResourcePool->AccessDepthResource(Info)->imageView;

		uint32_t index = m_Renderer->m_Pass->AddAttachment("Depth", depthAttachment, clearValue, view);

		/**
		* @brief Instance a VkAttachmentReference.
		*/
		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment           = index;
		depthAttachmentRef.layout               = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		m_HandledRendererSubPass->AdDepthAttachmentReference(depthAttachmentRef);

		return *this;
	}

	template<typename T>
	inline Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddInputAttachment(
		const std::string& attachmentName , 
		TextureType        type           ,
		T                  func
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Instance a VkAttachmentDescription.
		*/
		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format            = m_Renderer->m_Device->GetSwapChainSupport().format.format;
		attachmentDescription.samples           = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp            = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachmentDescription.storeOp           = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp     = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp    = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescription.initialLayout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachmentDescription.finalLayout       = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		/**
		* @brief Write in data.
		* @param[in attachmentDescription VkAttachmentDescription.
		*/
		func(attachmentDescription);

		/**
		* @brief Instance a VkClearValue.
		*/
		VkClearValue clearValue{};
		clearValue.color                        = { 0.0f, 0.0f, 0.0f, 1.0f };

		/**
		* @brief Instance a RendererResourceCreateInfo.
		*/
		RendererResourceCreateInfo Info;
		Info.name                               = attachmentName;
		Info.type                               = type;
		Info.description                        = attachmentDescription;
		Info.width                              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.width;
		Info.height                             = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height;

		VkImageView& view = m_Renderer->m_RendererResourcePool->AccessResource(Info)->imageView;

		uint32_t index = m_Renderer->m_Pass->AddAttachment(attachmentName, attachmentDescription, clearValue, view);

		/**
		* @brief Instance a VkAttachmentReference.
		*/
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment                = index;
		attachmentRef.layout                    = attachmentDescription.finalLayout;

		m_HandledRendererSubPass->AddInputAttachmentReference(attachmentRef);

		return *this;
	}
}