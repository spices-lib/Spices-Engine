/**
* @file Renderer.h
* @brief The Renderer Class Definitions.
* @author Spices.
*/

#pragma once
/******************************Core Header**********************************************************/
#include "Core/Core.h"
#include "RendererManager.h"
#include "Core/Library/ContainerLibrary.h"
#include "DescriptorSetManager/DescriptorSetManager.h"
#include "Render/Renderer/RendererPass/RendererPass.h"
#include "..\..\..\assets\Shaders\src\Header\ShaderCommon.h"
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

#define MAX_DIRECTIONALLIGHT_NUM 10
#define MAX_POINTLIGHT_MUN 100

namespace Spices {

	/**
	* @brief Renderer Class.
	* This class defines the basic behaves of renderer.
	* When we add an new Renderer, we need inherit from this.
	*/
	class Renderer
	{
	public:

		/******************************Basic Function**************************************************/

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] DescriptorPool The shared pointer of DescriptorPool, used for allocate descriptor and free descriptor.
		* @param[in] device The shared pointer of VulkanDevice, used for render pass's formats query.
		* @param[in] rendererResourcePool The shared pointer of RendererResourcePool, used for registry/access RT.
		* @param[in] isLoadDefaultMaterial True if need load a default material.
		*/
		Renderer
		(
			const std::string&                           rendererName            , 
			VulkanState&                                 vulkanState             ,
			const std::shared_ptr<VulkanDescriptorPool>& DescriptorPool          ,
			const std::shared_ptr<VulkanDevice>&         device                  ,
			const std::shared_ptr<RendererResourcePool>& rendererResourcePool    ,
			bool                                         isLoadDefaultMaterial = true
		);

		/**
		* @brief Destructor Function.
		* We destroy pipeline layout and free descriptors that holed by this renderer here.
		*/
		virtual ~Renderer() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		Renderer(const Renderer&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		Renderer& operator=(const Renderer&) = delete;

		/***************************************************************************************************/

		/******************************The interface do not needs override now******************************/

		/**
		* @brief This interface is called on render system is registered.
		* Initialize the specific renderer's pipeline, render pass. framebuffer, descriptor, and so on...
		* @note Though this is a interface, Usually not making scene in overriding it.
		*/
		virtual void OnSystemInitialize();

		/***************************************************************************************************/

		/******************************The interface needs override*****************************************/

		/**
		* @brief The interface is called every frame.
		* Defines what data needs to be renderer and how to render.
		* @param[in] ts TimeStep.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) = 0;

		/**
		* @brief This interface is called on Window resized over (registry by swapchain).
		* If the specific renderer uses swapchain image attachment during CreateRenderPass(), 
		* this interface needs to override, callOnSlateResize here just will be fine.
		*/
		virtual void OnWindowResizeOver() {}

		/**
		* @breif This interface is called on Viewport resize (registry by ImGui Viewport).
		* If the specific renderer uses the attachment that needs recreated during CreateRenderPass(), 
		* this interface needs to override, call CreateRenderPass() and CreateDescriptorSet() here just will be fine.
		*/
		virtual void OnSlateResize();

		/**
		* @breif This interface is called on world mark query tick (registry by MeshComponent).
		*/
		virtual void OnMeshAddedWorld() {}

		/**
		* @brief Register material to Specific Renderer.
		* @param[in] materialName Material Name.
		* @param[in] subpassName SubPass Name.
		*/
		void RegistryMaterial(
			const std::string& materialName , 
			const std::string& subpassName
		);

		/**
		* @brief Get RendererPass.
		* @return Returns the RendererPass.
		*/
		inline std::shared_ptr<RendererPass>& GetPasses() { return m_Pass; }

	private:

		/**
		* @brief The interface is called during OnSystemInitialize().
		* Create specific render pass.
		* @todo Implemented specific render pass
		*/
		virtual void CreateRendererPass() = 0;

		/**
		* @brief This interface is called during OnSystemInitialize().
		* Create all sub pass descriptorSet.
		*/
		virtual void CreateDescriptorSet() = 0;

		/**
		* @brief Create Specific Renderer Default Material. 
		*/
		void CreateDefaultMaterial() const;

		/**
		* @brief Create Pipeline Layout with material's descriptorset and renderer's descriptor set.
		* @param[in] rowSetLayouts All descriptor set collected.
		* @param[in] subPass material used sub pass.
		*/
		VkPipelineLayout CreatePipelineLayout(
			const std::vector<VkDescriptorSetLayout>& rowSetLayouts , 
			std::shared_ptr<RendererSubPass>          subPass
		) const;

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
		* @brief Iterator the specific Component in World.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldComp(FrameInfo& frameInfo, F func);

		/**
		* @brief Get The activated camera entity's view matrix and projection matrix.
		* @param[in] frameInfo The current frame data.
		* @return Returns a tuple of view matrix, projection matrix, stable frames.
		*/
		std::tuple<glm::mat4, glm::mat4, unsigned int> GetActiveCameraMatrix(FrameInfo& frameInfo);

		/**
		* @brief Get DirectionalLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @param[out] dLightBuffer DirectionalLight.
		* @todo Multiple directional light.
		*/
		void GetDirectionalLight(FrameInfo& frameInfo, std::array<SpicesShader::DirectionalLight, DIRECTIONALLIGHTBUFFERMAXNUM>& dLightBuffer);

		void GetDirectionalLightMatrix(FrameInfo& frameInfo, std::array<glm::mat4, MAX_DIRECTIONALLIGHT_NUM>& directionalLight);

		/**
		* @brief Get PointLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @param[out] pLightBuffer PointLight Buffer.
		* @todo infinity pointlight.
		*/
		void GetPointLight(FrameInfo& frameInfo, std::array<SpicesShader::PointLight, POINTLIGHTBUFFERMAXNUM>& pLightBuffer);

		/***************************************************************************************************/

	protected:

		/******************************Help Class for quick build*******************************************/

		/**
		* @brief This Class is a helper for Building RendererPass.
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
			virtual ~RendererPassBuilder() = default;

			/**
			* @brief Add a new SubPass to Renderer Pass.
			* @param[in] subPassName SunPass Name.
			* @return Returns the RendererPassBuilder.
			*/
			RendererPassBuilder& AddSubPass(const std::string& subPassName);

			/**
			* @brief Add a swapchain image attachment.
			* @param[in] func Defines Specific VkAttachmentDescription.
			* @return Returns the RendererPassBuilder.
			*/
			template<typename T>
			RendererPassBuilder& AddSwapChainAttachment(T func);

			/**
			* @brief Add a color attachment.
			* @param[in] attachmentName Attachment Name.
			* @param[in] type Attachment TextureType.
			* @param[in] func Defines Specific VkAttachmentDescription.
			* @return Returns the RendererPassBuilder.
			*/
			template<typename T>
			RendererPassBuilder& AddColorAttachment(
				const std::string& attachmentName ,
				const TextureType& type           ,
				T                  func
			);

			/**
			* @brief Add a depth attachment.
			* @param[in] attachmentName Attachment Name.
			* @param[in] type Attachment TextureType.
			* @param[in] func Defines Specific VkAttachmentDescription.
			* @return Returns the RendererPassBuilder.
			*/
			template<typename T>
			RendererPassBuilder& AddDepthAttachment(
				const std::string& attachmentName ,
				const TextureType& type           ,
				T                  func
			);

			/**
			* @brief Add a input attachment.
			* @param[in] attachmentName Attachment Name.
			* @param[in] type Attachment TextureType.
			* @param[in] func Defines Specific VkAttachmentDescription.
			* @return Returns the RendererPassBuilder.
			*/
			template<typename T>
			RendererPassBuilder& AddInputAttachment(
				const std::string& attachmentName , 
				const TextureType& type           ,
				T                  func
			);

			/**
			* @brief End recording a sub pass.
			* @return Returns the RendererPassBuilder.
			*/
			RendererPassBuilder& EndSubPass();

			/**
			* @brief Build the RendererPass.
			*/
			void Build() const;

		private:

			/**
			* @brief RendererPass Name.
			*/
			std::string m_RendererPassName;

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanced.
			*/
			Renderer* m_Renderer;

			/**
			* @brief Handled Sub pass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledRendererSubPass;
		};

		/**
		* @brief This Class is a helper for Building Specific Renderer DescriptorSet.
		* Only instanced during CreateDescriptorSets().
		*/
		class DescriptorSetBuilder
		{
		public:

			/**
			* @brief Constructor Function.
			* @param[in] subPassName Sub pass name.
			* @param[in] renderer When instanced during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			*/
			DescriptorSetBuilder(
				const std::string& subPassName      ,
				Renderer*          renderer
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~DescriptorSetBuilder() = default;

			/**
			* @brief Set VkPushConstantRange by a specific push constant struct.
			* @tparam T Specific push constant struct.
			* @return Returns this reference.
			*/
			template<typename T>
			inline DescriptorSetBuilder& AddPushConstant();

			/**
			* @brief Create local buffer object in collection, and add it's set binding to descriptorsetlayout, and sets descriptorwriter using it's buffer info.
			* @tparam T Buffer struct.
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
			* @tparam T Buffer struct.
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
			* @brief Add the texture set binding to descriptor set layout.
			* @tparam T Texture Type.
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
			* @brief Add the storage texture set binding to descriptor set layout.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @param[in] textureNames All Texture's Name.
			* @param[in] format Texture Format.
			* @param[in] type Texture's type.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddStorageTexture(
				uint32_t                         set                                ,
				uint32_t                         binding                            ,
				VkShaderStageFlags               stageFlags                         ,
				const std::vector<std::string>&  textureNames                       ,
				VkFormat                         format = VK_FORMAT_R8G8B8A8_UNORM  ,
				TextureType                      type = TextureType::Texture2D
			);

			/**
			* @brief Add the attachment as texture to descriptor set layout.
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
			* @brief Add the texture set binding to descriptor set layout.
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
			* @brief Add the Acceleration Structure set binding to descriptor set layout.
			* @param[in] set Which set this Acceleration Structure wil use.
			* @param[in] binding Which binding this Acceleration Structure wil use.
			* @param[in] stageFlags Which buffer stage this Acceleration Structure will use.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddAccelerationStructure(
				uint32_t                         set         ,
				uint32_t                         binding     ,
				VkShaderStageFlags               stageFlags
			);

			/**
			* @brief Create all buffer type descriptor set.
			* Create pipeline layout.
			* @param[in] accel AccelerationStructure. Pass it if call AddAccelerationStructure before.
			* @attention Texture type descriptor set is not created here, but in Material::BuildMaterial().
			*/
			void Build(const VkAccelerationStructureKHR& accel = VK_NULL_HANDLE);

		public:

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanced.
			*/
			Renderer* m_Renderer;

			/**
			* @brief Handled Sub pass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledSubPass;

			/**
			* @brief The Descriptor Set Manager ID descriptorSet belongs to.
			*/
			String2 m_DescriptorSetId;

			/**
			* @brief DescriptorSet Buffer type binging info.
			* [set] - [binding] - [info]
			*/
			std::unordered_map<uint32_t, std::unordered_map<uint32_t, VkDescriptorBufferInfo>> m_BufferInfos;

			/**
			* @brief DescriptorSet Image type binging info.
			* [set] - [binding] - [info]
			*/
			std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>>> m_ImageInfos;
		};

		/**
		* @brief This class helps to bind pipeline and bind buffer.
		* Only instanced during Render().
		*/
		class RenderBehaveBuilder
		{
		public:

			/**
			* @brief Constructor Function.
			* Bind pipeline and all buffer type descriptor set.
			* @param[in] renderer When instanced during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			* @param[in] currentFrame Passed from FrameInfo.
			* @param[in] currentImage Passed from FrameInfo.
			*/
			RenderBehaveBuilder(
				Renderer* renderer     , 
				uint32_t  currentFrame , 
				uint32_t  currentImage
			)
				: m_Renderer(renderer)
				, m_CurrentFrame(currentFrame)
				, m_CurrentImage(currentImage)
			{
				m_CommandBuffer = m_Renderer->m_VulkanState.m_GraphicCommandBuffer[currentFrame];
			}

			/**
			* @brief Destructor Function.
			*/
			virtual ~RenderBehaveBuilder() = default;

			/**
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaveBuilder instanced.
			* @param[in] materialName also pipelineName.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipeline(const std::string& materialName, VkPipelineBindPoint  bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);

			/**
			* @brief DynamicState Set Viewport and Scissor.
			*/
			void SetViewPort() const;

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                   , 
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
			);

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                       , 
				const std::string&         name                                        , 
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
			);

			/******************************Update By Value**********************************************************/

			/**
			* @brief Update local push constant buffer.
			* @tparam T Specific push constant struct Type.
			* @param[in] func A function pointer, which defines what data inside the buffer.
			*/
			template<typename T, typename F>
			void UpdatePushConstant(F func);

			/**
			* @brief Update a local buffer.
			* @tparam T Specific buffer struct Type.
			* @param[in] func A function pointer, which defines what data inside the buffer.
			* @param[in] set Which set the descriptor will use.
			* @param[in] binding Which binding the descriptor will use.
			*/
			template<typename T, typename F>
			void UpdateUniformBuffer(uint32_t set, uint32_t binding, F func);

			/**
			* @brief Update a local buffer.
			* @tparam T Specific buffer struct Type.
			* @param[in] func A function pointer, which defines what data inside the buffer.
			* @param[in] set Which set the descriptor will use.
			* @param[in] binding Which binding the descriptor will use.
			*/
			template<typename T, typename F>
			void UpdateStorageBuffer(uint32_t set, uint32_t binding, F func);

			/*******************************************************************************************************/

			/**
			* @brief Update local push constant buffer.
			* @tparam T Specific push constant struct Type.
			* @param[in] data push constant data pointer.
			*/
			template<typename T>
			void UpdatePushConstant(void* data) const;

			/**
			* @brief Update a local buffer.
			* @param[in] set Which set the descriptor will use.
			* @param[in] binding Which binding the descriptor will use.
			* @param[in] data ubo data pointer.
			*/
			void UpdateUniformBuffer(uint32_t set, uint32_t binding, void* data) const;

			/**
			* @brief Update a local buffer.
			* @param[in] set Which set the descriptor will use.
			* @param[in] binding Which binding the descriptor will use.
			* @param[in] data ssbo data pointer.
			*/
			void UpdateStorageBuffer(uint32_t set, uint32_t binding, void* data) const;

			/**
			* @brief End a preview sub pass and stat next sub pass.
			* @param[in] subpassName The name of next sub pass.
			*/
			void BeginNextSubPass(const std::string& subpassName);

			/**
			* @brief Begin this Renderer's RenderPass.
			*/
			void BeginRenderPass();

			/**
			* @brief End this Renderer's RenderPass.
			*/
			void EndRenderPass() const;

		protected:

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanced.
			*/
			Renderer* m_Renderer;

			/**
			* @brief Current frame index.
			* Passed while this class instanced.
			* @see FrameInfo.
			*/
			uint32_t m_CurrentFrame;

			/**
			* @brief Current frame index.
			* Passed while this class instanced.
			* @see FrameInfo.
			*/
			uint32_t m_CurrentImage;

			/**
			* @brief Current CommandBuffer.
			*/
			VkCommandBuffer m_CommandBuffer;

			/**
			* @brief Handled Sub pass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledSubPass;
		};

		/**
		* @brief This class helps to bind pipeline and bind buffer.
		* Specific for RayTracing Renderer.
		* Only instanced during Render().
		*/
		class RayTracingRenderBehaveBuilder : public RenderBehaveBuilder
		{
		public:
			
			/**
			* @brief Constructor Function.
			* Bind pipeline and all buffer type descriptor set.
			* @param[in] renderer When instanced during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			* @param[in] currentFrame Passed from FrameInfo.
			* @param[in] currentImage Passed from FrameInfo.
			*/
			RayTracingRenderBehaveBuilder(
				Renderer* renderer     , 
				uint32_t  currentFrame , 
				uint32_t  currentImage
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~RayTracingRenderBehaveBuilder() override = default;

			void Recording(const std::string& caption);
			void Endrecording();
			
			/**
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaveBuilder instanced.
			* @param[in] materialName also pipelineName.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipeline(const std::string& materialName, VkPipelineBindPoint  bindPoint = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR) override;
			
			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                       , 
				VkPipelineBindPoint  bindPoint = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
			) override;

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                             , 
				const std::string&         name                                              , 
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
			) override;

			/**
			* @brief Call  vkCmdTraceRaysKHR here.
			* @param[in] rgenRegion RayGen Shader Group.
			* @param[in] missRegion Miss Shader Group.
			* @param[in] hitRegion Hit Shader Group.
			* @param[in] callRegion Callable Shader Group.
			*/
			void TraceRays(
				const VkStridedDeviceAddressRegionKHR* rgenRegion,
				const VkStridedDeviceAddressRegionKHR* missRegion,
				const VkStridedDeviceAddressRegionKHR* hitRegion,
				const VkStridedDeviceAddressRegionKHR* callRegion
			);

		private:
			
			/**
			* @brief Function Pointer of vkCmdTraceRaysKHR.
			*/
			PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
		};
		
		/**
		* @brief This class helps to bind pipeline and bind buffer.
		* Specific for pure Compute Renderer.
		* Only instanced during Render().
		*/
		class ComputeRenderBehaveBuilder : public RenderBehaveBuilder
		{
		public:
			
			/**
			* @brief Constructor Function.
			* Bind pipeline and all buffer type descriptor set.
			* @param[in] renderer When instanced during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			* @param[in] currentFrame Passed from FrameInfo.
			* @param[in] currentImage Passed from FrameInfo.
			*/
			ComputeRenderBehaveBuilder(
				Renderer* renderer     , 
				uint32_t  currentFrame , 
				uint32_t  currentImage
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~ComputeRenderBehaveBuilder() override = default;

			void Recording(const std::string& caption);
			void Endrecording();
			
			/**
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaveBuilder instanced.
			* @param[in] materialName also pipelineName.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipeline(const std::string& materialName, VkPipelineBindPoint  bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE) override;
			
			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                       , 
				VkPipelineBindPoint  bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE
			) override;

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                       , 
				const std::string&   name                                              , 
				VkPipelineBindPoint  bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE
			) override;

			/**
			* @brief Dispatch Compute Shader.
			* @param[in] x X group.
			* @param[in] y Y group.
			* @param[in] z Z group.
			*/
			void Dispatch(uint32_t x, uint32_t y, uint32_t z);
		};

	protected:
		
		/**
		* @brief This variable is passed while renderer instanced.
		*/
		VulkanState& m_VulkanState;

		/**
		* @brief This variable is passed while renderer instanced.
		*/
		std::shared_ptr<VulkanDescriptorPool> m_DescriptorPool;

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
	void Renderer::IterWorldComp(FrameInfo& frameInfo, F func)
	{
		SPICES_PROFILE_ZONE;

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
			if (func(static_cast<int>(e), transComp, tComp)) break;
		}
	}

	template<typename T, typename F>
	void Renderer::RenderBehaveBuilder::UpdatePushConstant(F func)
	{
		SPICES_PROFILE_ZONE;

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
			m_CommandBuffer,
			m_Renderer->m_Pipelines[ss.str()]->GetPipelineLayout(),
			VK_SHADER_STAGE_ALL,
			0,
			sizeof(T),
			&push
		);
	}

	template<typename T, typename F>
	void Renderer::RenderBehaveBuilder::UpdateUniformBuffer(uint32_t set, uint32_t binding, F func)
	{
		SPICES_PROFILE_ZONE;

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
	void Renderer::RenderBehaveBuilder::UpdateStorageBuffer(uint32_t set, uint32_t binding, F func)
	{
		SPICES_PROFILE_ZONE;

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
	void Renderer::RenderBehaveBuilder::UpdatePushConstant(void* data) const
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		/**
		* @breif Update PushConstants
		*/
		vkCmdPushConstants(
			m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
			m_Renderer->m_Pipelines[ss.str()]->GetPipelineLayout(),
			VK_SHADER_STAGE_ALL,
			0,
			sizeof(T),
			data
		);
	}

	inline void Renderer::RenderBehaveBuilder::UpdateUniformBuffer(uint32_t set, uint32_t binding, void* data) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif Update uniform buffer.
		*/
		m_HandledSubPass->SetBuffer({ set, binding }, data);
	}

	inline void Renderer::RenderBehaveBuilder::UpdateStorageBuffer(uint32_t set, uint32_t binding, void* data) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif Update uniform buffer.
		*/
		m_HandledSubPass->SetBuffer({ set, binding }, data);
	}

	template<typename T>
	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddPushConstant()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Call RendererSubPass::SetPushConstant().
		*/
		m_HandledSubPass->SetPushConstant([&](auto& range) {
			range.stageFlags   = VK_SHADER_STAGE_ALL;
			range.offset       = 0;
			range.size         = sizeof(T);
		});

		return *this;
	}

	template<typename T>
	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddUniformBuffer(
		uint32_t           set        , 
		uint32_t           binding    , 
		VkShaderStageFlags stageFlags
	)
	{
		SPICES_PROFILE_ZONE;

		const UInt2 id(set, binding);

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
		const auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, stageFlags, 1);

		return *this;
	}

	template<typename T>
	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddStorageBuffer(
		uint32_t           set        , 
		uint32_t           binding    , 
		VkShaderStageFlags stageFlags
	)
	{
		SPICES_PROFILE_ZONE;

		const UInt2 id(set, binding);

		/**
		* @brief Creating VulkanBuffer.
		*/
		m_HandledSubPass->GetBuffers(id) = std::make_unique<VulkanBuffer>(
			m_Renderer->m_VulkanState,
			sizeof(T),
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
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
		const auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, stageFlags, 1);

		return *this;
	}

	template<typename T>
	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddTexture(
		uint32_t                        set        ,
		uint32_t                        binding    ,
		VkShaderStageFlags              stageFlags ,
		const std::vector<std::string>& textureNames
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < textureNames.size(); i++)
		{
			const std::shared_ptr<Texture> texture = ResourcePool<Texture>::Load<T>(textureNames[i]);
			m_ImageInfos[set][binding].push_back(*texture->GetResource<VulkanImage>()->GetImageInfo());
		}

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		const auto descriptorSet = DescriptorSetManager::Registy(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags, textureNames.size());

		return *this;
	}

	template<typename T>
	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddSwapChainAttachment(T func)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkAttachmentDescription.
		*/
		VkAttachmentDescription                attachmentDescription{};
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
		VkClearValue                            clearValue{};
		clearValue.color                     = { 0.0f, 0.0f, 0.0f, 1.0f };

		/**
		* @brief Instance a VkPipelineColorBlendAttachmentState.
		*/
		VkPipelineColorBlendAttachmentState    colorBlend{};
		colorBlend.colorWriteMask            = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			                                   VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlend.blendEnable               = VK_FALSE;
		colorBlend.srcColorBlendFactor       = VK_BLEND_FACTOR_ONE;
		colorBlend.dstColorBlendFactor       = VK_BLEND_FACTOR_ZERO;
		colorBlend.colorBlendOp              = VK_BLEND_OP_ADD;
		colorBlend.srcAlphaBlendFactor       = VK_BLEND_FACTOR_ONE;
		colorBlend.dstAlphaBlendFactor       = VK_BLEND_FACTOR_ZERO;
		colorBlend.alphaBlendOp              = VK_BLEND_OP_ADD;

		const uint32_t index = m_Renderer->m_Pass->AddAttachment("SwapChainImage", attachmentDescription, 1, clearValue);

		/**
		* @brief Instance a VkAttachmentReference.
		*/
		VkAttachmentReference                  attachmentRef{};
		attachmentRef.attachment             = index;
		attachmentRef.layout                 = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		m_HandledRendererSubPass->AddColorAttachmentReference(attachmentRef, colorBlend);

		return *this;
	}

	template<typename T>
	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddColorAttachment(
		const std::string& attachmentName , 
		const TextureType& type           ,
		T                  func
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkAttachmentDescription.
		*/
		VkAttachmentDescription                   attachmentDescription{};
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

		/**
		* @brief Get layers.
		*/
		uint32_t layers = 1;
		switch (type)
		{
		case Spices::TextureType::Texture2DArray:
			layers = m_Renderer->m_RendererResourcePool->AccessRowResource(Info.name)->GetLayers();
			break;
		case Spices::TextureType::Texture2DCube:
			layers = 6;
			break;
		default:
			break;
		}

		uint32_t index = m_Renderer->m_Pass->AddAttachment(attachmentName, attachmentDescription, clearValue, layers, view);

		/**
		* @brief Instance a VkAttachmentReference.
		*/
		VkAttachmentReference                     attachmentRef{};
		attachmentRef.attachment                = index;
		attachmentRef.layout                    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		m_HandledRendererSubPass->AddColorAttachmentReference(attachmentRef, colorBlend);
		
		return *this;
	}

	template<typename T>
	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddDepthAttachment(
		const std::string& attachmentName ,
		const TextureType& type           ,
		T func
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkAttachmentDescription.
		*/
		VkAttachmentDescription                   depthAttachment{};
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
		VkClearValue                              clearValue{};
		clearValue.depthStencil                 = { 1.0f, 0 };

		/**
		* @brief Instance a RendererResourceCreateInfo.
		*/
		RendererResourceCreateInfo Info;
		Info.name                               = attachmentName;
		Info.type                               = type;
		Info.description                        = depthAttachment;
		Info.width                              = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.width;
		Info.height                             = m_Renderer->m_Device->GetSwapChainSupport().surfaceSize.height;
		Info.isDepthResource                    = true;

		const VkImageView& view = m_Renderer->m_RendererResourcePool->AccessResource(Info)->imageView;

		/**
		* @brief Get layers.
		*/
		uint32_t layers = 1;
		switch (type)
		{
		case Spices::TextureType::Texture2DArray:
			layers = m_Renderer->m_RendererResourcePool->AccessRowResource(Info.name)->GetLayers();
			break;
		case Spices::TextureType::Texture2DCube:
			layers = 6;
			break;
		default:
			break;
		}

		const uint32_t index = m_Renderer->m_Pass->AddAttachment(attachmentName, depthAttachment, clearValue, layers, view);

		/**
		* @brief Instance a VkAttachmentReference.
		*/
		VkAttachmentReference                     depthAttachmentRef{};
		depthAttachmentRef.attachment           = index;
		depthAttachmentRef.layout               = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		m_HandledRendererSubPass->AddDepthAttachmentReference(depthAttachmentRef);

		return *this;
	}

	template<typename T>
	Renderer::RendererPassBuilder& Renderer::RendererPassBuilder::AddInputAttachment(
		const std::string& attachmentName ,
		const TextureType& type           ,
		T                  func
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkAttachmentDescription.
		*/
		VkAttachmentDescription                   attachmentDescription{};
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
		VkClearValue                              clearValue{};
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

		const VkImageView& view = m_Renderer->m_RendererResourcePool->AccessResource(Info)->imageView;

		const uint32_t index = m_Renderer->m_Pass->AddAttachment(attachmentName, attachmentDescription, clearValue, 1, view); /*todo: layer config */

		/**
		* @brief Instance a VkAttachmentReference.
		*/
		VkAttachmentReference                    attachmentRef{};
		attachmentRef.attachment                = index;
		attachmentRef.layout                    = attachmentDescription.finalLayout;

		m_HandledRendererSubPass->AddInputAttachmentReference(attachmentRef);

		return *this;
	}
}