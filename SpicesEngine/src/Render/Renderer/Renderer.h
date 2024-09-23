/**
* @file Renderer.h
* @brief The Renderer Class Definitions.
* @author Spices.
*/

#pragma once
/******************************Core Header**********************************************************/
#include "Core/Core.h"
#include "RendererManager.h"
#include "DescriptorSetManager/DescriptorSetManager.h"
#include "Render/Renderer/RendererPass/RendererPass.h"
#include "Render/Vulkan/VulkanCmdThreadPool.h"
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
#include "Render/Vulkan/VulkanIndirectDrawNV.h"
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
		* @param[in] cmdThreadPool ThreadPool of submit Cmd parallel.
		* @param[in] isLoadDefaultMaterial True if need load a default material.
		* @param[in] isRegistryDGCPipeline True if need registry dgc pipeline.
		*/
		Renderer
		(
			const std::string&                           rendererName            , 
			VulkanState&                                 vulkanState             ,
			const std::shared_ptr<VulkanDescriptorPool>& DescriptorPool          ,
			const std::shared_ptr<VulkanDevice>&         device                  ,
			const std::shared_ptr<RendererResourcePool>& rendererResourcePool    ,
			const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool           ,
			bool                                         isLoadDefaultMaterial = true ,
			bool                                         isRegistryDGCPipeline = false
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
		* @brief Registry material to Specific Renderer.
		* @param[in] materialName Material Name.
		* @param[in] subpassName SubPass Name.
		*/
		void RegistryMaterial(
			const std::string& materialName , 
			const std::string& subpassName
		);

		/**
		* @brief Registry dgc pipeline to Specific Renderer.
		* @param[in] materialName Material Name.
		* @param[in] subpassName SubPass Name.
		*/
		void RegistryDGCPipeline(
			const std::string& materialName,
			const std::string& subpassName
		);

		/**
		* @brief Fill in World Renderable data to IndirectBuffer.
		* @tparam T Specific Component.
		* @param[in] subpassName .
		*/
		template<typename T>
		void FillIndirectRenderData(const std::string& subpassName);

		/**
		* @brief Get RendererPass.
		* @return Returns the RendererPass.
		*/
		inline std::shared_ptr<RendererPass>& GetPasses() { return m_Pass; }

	private:

		/**
		* @brief The interface is called during OnSystemInitialize().
		* Create specific render pass.
		*/
		virtual void CreateRendererPass() = 0;

		/**
		* @brief This interface is called during OnSystemInitialize().
		* Create all sub pass descriptorSet.
		*/
		virtual void CreateDescriptorSet() = 0;

		/**
		* @brief This interface is called during OnSystemInitialize().
		* Create Device Generated Commands Layout.
		*/
		virtual void CreateDeviceGeneratedCommandsLayout() {};

		/**
		* @brief Create Specific Renderer Default Material. 
		*/
		void CreateDefaultMaterial();

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
		* @brief Create device generated command Pipeline.
		* @param[in] pipelineName Pipeline's name.
		* @param[in] materialName Material's name.
		* @param[in] layout PipelineLayout.
		* @param[in] subPass RendererSubPass.
		*/
		virtual std::shared_ptr<VulkanPipeline> CreateDGCPipeline(
			const std::string&               pipelineName ,
			const std::string&               materialName ,
			VkPipelineLayout&                layout       ,
			std::shared_ptr<RendererSubPass> subPass
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
		* @brief Submit a group of commands to secondary command buffer, and execute all of them.
		* @param[in] primaryCmdBuffer The main Command Buffer.
		* @param[in] subpass subpass index.
		* @param func Specific Commands.
		*/
		template<typename F>
		void SubmitCmdsParallel(VkCommandBuffer primaryCmdBuffer, uint32_t subpass, F&& func);

		/**
		* @brief Iterator the specific Component in World Parallel.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] subpass subpass index.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldCompSubmitCmdParallel(FrameInfo& frameInfo, uint32_t subpass, F func);

		/**
		* @brief Iterator the specific Component in World With break.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldCompWithBreak(FrameInfo& frameInfo, F func);

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
		void GetDirectionalLight(FrameInfo& frameInfo, std::array<SpicesShader::DirectionalLight, DIRECTIONALLIGHT_BUFFER_MAXNUM>& dLightBuffer);

		void GetDirectionalLightMatrix(FrameInfo& frameInfo, std::array<glm::mat4, MAX_DIRECTIONALLIGHT_NUM>& directionalLight);

		/**
		* @brief Get PointLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @param[out] pLightBuffer PointLight Buffer.
		* @todo infinity pointlight.
		*/
		void GetPointLight(FrameInfo& frameInfo, std::array<SpicesShader::PointLight, POINTLIGHT_BUFFER_MAXNUM>& pLightBuffer);

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
			* @brief Add subpass Self Dependency.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			*/
			RendererPassBuilder& AddSelfDependency(
				VkAccessFlags          srcAccessMask ,
				VkAccessFlags          dstAccessMask ,
				VkPipelineStageFlags   srcStageMask  ,
				VkPipelineStageFlags   dstStageMask
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
			* @param[in] size pushconstant size.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddPushConstant(uint64_t size);

			/**
			* @brief Create local buffer object in collection, and add it's set binding to descriptorsetlayout, and sets descriptorwriter using it's buffer info.
			* @tparam T Buffer struct.
			* @param[in] set Which set this buffer wil use.
			* @param[in] binding Which binding this buffer will use.
			* @param[in] size ubo size.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddUniformBuffer(
				uint32_t            set      ,
				uint32_t            binding  ,
				uint64_t            size     ,
				VkShaderStageFlags  stageFlags
			);

			/**
			* @brief Create local buffer object in collection, and add it's set binding to descriptorsetlayout, and sets descriptorwriter using it's buffer info.
			* @param[in] set Which set this buffer wil use.
			* @param[in] binding Which binding this buffer will use.
			* @param[in] size size of ssbo.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddStorageBuffer(
				uint32_t            set      ,
				uint32_t            binding  ,
				uint64_t            size     ,
				VkShaderStageFlags  stageFlags
			);

			/**
			* @brief Add buffer object in collection, and add it's set binding to descriptorsetlayout, and sets descriptorwriter using it's buffer info.
			* @param[in] set Which set this buffer wil use.
			* @param[in] binding Which binding this buffer will use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @param[in] buffer SSBO.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddStorageBuffer(
				uint32_t                      set        ,
				uint32_t                      binding    ,
				VkShaderStageFlags            stageFlags ,
				std::shared_ptr<VulkanBuffer> buffer
			);

			/**
			* @brief Init the Bindless texture set binding to descriptor set layout.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @return Returns this reference.
			*/
			template<typename T>
			inline DescriptorSetBuilder& AddBindLessTexture(
				uint32_t                         set        ,
				uint32_t                         binding    ,
				VkShaderStageFlags               stageFlags ,
				const std::vector<std::string>&  textureNames
			);

			/**
			* @brief Add the texture set binding to descriptor set layout.
			* @tparam T Texture Type.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] textureNames All Texture's Name.
			* @return Returns this reference.
			*/
			template<typename T>
			inline DescriptorSetBuilder& AddTexture(
				uint32_t                         set,
				uint32_t                         binding,
				VkShaderStageFlags               stageFlags,
				const std::vector<std::string>&  textureNames
			);

			/**
			* @brief Add the storage texture set binding to descriptor set layout.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @param[in] textureNames All Texture's Name.
			* @param[in] format Texture Format, used in init.
			* @param[in] type Texture's type, used in init.
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
		* @brief This Class is a helper for Building GDC Layout.
		* Only instanced during CreateDeviceGeneratedCommandsLayout().
		*/
		class DGCLayoutBuilder
		{
		public:

			/**
			* @brief Constructor Function.
			* @param[in] subPassName Sub pass name.
			* @param[in] renderer When instanced during CreatePipelineLayoutAndDescriptor(), pass this pointer.
			*/
			DGCLayoutBuilder(
				const std::string&  subPassName ,
				Renderer*           renderer
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~DGCLayoutBuilder() = default;

			/**
			* @brief Add Binding Shader Group Command to Input.
			* @return Returns this reference.
			*/
			DGCLayoutBuilder& AddShaderGroupInput();

			/**
			* @brief Add Binding Vertex Buffer to Input.
			* @return Returns this reference.
			*/
			DGCLayoutBuilder& AddVertexBufferInput();

			/**
			* @brief Add Binding Index Buffer to Input.
			* @return Returns this reference.
			*/
			DGCLayoutBuilder& AddIndexBufferInput();

			/**
			* @brief Add Binding PushConstant Command to Input.
			* @return Returns this reference.
			*/
			DGCLayoutBuilder& AddPushConstantInput();

			/**
			* @brief Add Draw Indexed Command to Input.
			* @return Returns this reference.
			*/
			DGCLayoutBuilder& AddDrawIndexedInput();

			/**
			* @brief Add Draw Mesh Task Command to Input.
			* @return Returns this reference.
			*/
			DGCLayoutBuilder& AddDrawMeshTaskInput();

			/**
			* @brief Create GDC Layout.
			*/
			void Build();

		private:

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanced.
			*/
			Renderer* m_Renderer;

			/**
			* @brief Specific Subpass Name.
			* Passed while this class instanced.
			*/
			std::string m_SubpassName;

			/**
			* @brief Stores command inputs.
			*/
			std::vector<VkIndirectCommandsLayoutTokenNV> m_InputInfos;

			/**
			* @brief Current Subpass IndirectData.
			*/
			std::shared_ptr<VulkanIndirectDrawNV> m_HandledIndirectData;
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
				SPICES_PROFILE_ZONE;

				m_CommandBuffer = m_Renderer->m_VulkanState.m_GraphicCommandBuffer[currentFrame];
			}

			/**
			* @brief Destructor Function.
			*/
			virtual ~RenderBehaveBuilder() = default;

			/**
			* @brief Recording all this behaver does.
			* @param[in] caption Recording Name
			*/
			void Recording(const std::string& caption);

			/**
			* @brief Endrecording all this behaver does.
			*/
			void Endrecording();

			/**
			* @brief Async Commands.
			* @param[in] func In Function Pointer
			*/
			void Async(std::function<void(VkCommandBuffer& cmdBuffer)> func);

			/**
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaveBuilder instanced.
			* @param[in] materialName also pipelineName.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipeline(
				const std::string&   materialName , 
				VkCommandBuffer      cmdBuffer    = VK_NULL_HANDLE,
				VkPipelineBindPoint  bindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS
			);

			/**
			* @brief Bind the pipeline created by CreatePipeline() Async.
			* Called on RenderBehaveBuilder instanced.
			* @param[in] materialName also pipelineName.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipelineAsync(
				const std::string&   materialName , 
				VkPipelineBindPoint  bindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS
			);

			/**
			* @brief DynamicState Set Viewport and Scissor.
			* @param[in] cmdBuffer VkCommandBuffer
			*/
			void SetViewPort(VkCommandBuffer cmdBuffer = VK_NULL_HANDLE) const;

			/**
			* @brief DynamicState Set Viewport and Scissor Async.
			*/
			void SetViewPortAsync() const;

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                       , 
				VkCommandBuffer            cmdBuffer = VK_NULL_HANDLE,
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
			);

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                       , 
				const std::string&         name                                        , 
				VkCommandBuffer            cmdBuffer = VK_NULL_HANDLE                  ,
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
			);

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo Async.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSetAsync(
				const DescriptorSetInfo&   infos                                       , 
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
			);

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name Async.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSetAsync(
				const DescriptorSetInfo&   infos                                       , 
				const std::string&         name                                        , 
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS
			);

			/**
			* @brief Run DGC Buffer.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void RunDGC(VkCommandBuffer cmdBuffer = VK_NULL_HANDLE);

			/**
			* @brief Run DGC Buffer Async.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void RunDGCAsync();

			/**
			* @brief Preprocess Device Generated CommandsBuffer.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void PreprocessDGC_NV(VkCommandBuffer cmdBuffer = VK_NULL_HANDLE);

			/**
			* @brief Preprocess Device Generated CommandsBuffer Async.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void PreprocessDGCAsync_NV();

			/**
			* @brief Execute Device Generated CommandsBuffer.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void ExecuteDGC_NV(VkCommandBuffer cmdBuffer = VK_NULL_HANDLE);

			/**
			* @brief Execute Device Generated CommandsBuffer Async.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void ExecuteDGCAsync_NV();

			/**
			* @brief Draw a Triangle full screen.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void DrawFullScreenTriangle(VkCommandBuffer cmdBuffer = VK_NULL_HANDLE);

			/**
			* @brief Draw a Triangle full screen Async.
			*/
			void DrawFullScreenTriangleAsync();

			/**
			* @brief Add a memory Barrier.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void PipelineMemoryBarrier(
				VkAccessFlags          srcAccessMask ,
				VkAccessFlags          dstAccessMask ,
				VkPipelineStageFlags   srcStageMask  ,
				VkPipelineStageFlags   dstStageMask  ,
				VkCommandBuffer        cmdBuffer = VK_NULL_HANDLE
			);

			/**
			* @brief Add a memory Barrier.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			*/
			void PipelineMemoryBarrierAsync(
				VkAccessFlags          srcAccessMask ,
				VkAccessFlags          dstAccessMask ,
				VkPipelineStageFlags   srcStageMask  ,
				VkPipelineStageFlags   dstStageMask
			);

			/******************************Update By Value**********************************************************/

			/**
			* @brief Update local push constant buffer.
			* @tparam T Specific push constant struct Type.
			* @param[in] func A function pointer, which defines what data inside the buffer.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			template<typename T, typename F>
			void UpdatePushConstant(F func, VkCommandBuffer cmdBuffer = VK_NULL_HANDLE);

			/**
			* @brief Update local push constant buffer Async.
			* @tparam T Specific push constant struct Type.
			* @param[in] func A function pointer, which defines what data inside the buffer.
			*/
			template<typename T, typename F>
			void UpdatePushConstantAsync(F func);

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
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			template<typename T>
			void UpdatePushConstant(void* data, VkCommandBuffer cmdBuffer = VK_NULL_HANDLE) const;

			/**
			* @brief Update local push constant buffer Async.
			* @tparam T Specific push constant struct Type.
			* @param[in] data push constant data pointer.
			*/
			template<typename T>
			void UpdatePushConstantAsync(void* data) const;

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
			* @param[in] size data size.
			* @param[in] offset data size.
			*/
			void UpdateStorageBuffer(
				uint32_t set                    , 
				uint32_t binding                , 
				void*    data                   , 
				uint64_t size   = VK_WHOLE_SIZE ,
				uint64_t offset = 0
			) const;

			/**
			* @brief End a preview sub pass and stat next sub pass.
			* @param[in] subpassName The name of next sub pass.
			*/
			void BeginNextSubPass(const std::string& subpassName);

			/**
			* @brief End a preview sub pass and stat next sub pass.
			* @param[in] subpassName The name of next sub pass.
			*/
			void BeginNextSubPassAsync(const std::string& subpassName);

			/**
			* @brief Begin this Renderer's RenderPass.
			*/
			void BeginRenderPass();

			/**
			* @brief Begin this Renderer's RenderPass Async.
			*/
			void BeginRenderPassAsync();

			/**
			* @brief End this Renderer's RenderPass.
			*/
			void EndRenderPass() const;

		public:

			/**
			* @brief Get current subpass index in renderpass.
			* @return Returns index of subpass.
			*/
			uint32_t GetSubpassIndex() const { return m_SubpassIndex; }

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
			* @brief Current subpass Index.
			*/
			uint32_t m_SubpassIndex = 0;

			/**
			* @brief Current CommandBuffer.
			*/
			VkCommandBuffer m_CommandBuffer;

			/**
			* @brief Handled Sub pass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledSubPass;

			/**
			* @brief Current Subpass IndirectData.
			*/
			std::shared_ptr<VulkanIndirectDrawNV> m_HandledIndirectData;
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
			
			/**
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaveBuilder instanced.
			* @param[in] materialName also pipelineName.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipeline(
				const std::string&   materialName , 
				VkCommandBuffer      cmdBuffer    = VK_NULL_HANDLE,
				VkPipelineBindPoint  bindPoint    = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
			) override;

			/**
			* @brief Bind the pipeline created by CreatePipeline() Async.
			* Called on RenderBehaveBuilder instanced.
			* @param[in] materialName also pipelineName.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipelineAsync(
				const std::string&   materialName , 
				VkPipelineBindPoint  bindPoint    = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
			) override;
			
			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                             , 
				VkCommandBuffer            cmdBuffer = VK_NULL_HANDLE                        ,
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
			) override;

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo Async.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSetAsync(
				const DescriptorSetInfo&   infos,
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
			) override;

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                             , 
				const std::string&         name                                              , 
				VkCommandBuffer            cmdBuffer = VK_NULL_HANDLE                        ,
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
			) override;

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name Async.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSetAsync(
				const DescriptorSetInfo&   infos                                             , 
				const std::string&         name                                              , 
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
			) override;

			/**
			* @brief Call vkCmdTraceRaysKHR here.
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

			/**
			* @brief Call vkCmdTraceRaysKHR here Async.
			* @param[in] rgenRegion RayGen Shader Group.
			* @param[in] missRegion Miss Shader Group.
			* @param[in] hitRegion Hit Shader Group.
			* @param[in] callRegion Callable Shader Group.
			*/
			void TraceRaysAsync(
				const VkStridedDeviceAddressRegionKHR* rgenRegion,
				const VkStridedDeviceAddressRegionKHR* missRegion,
				const VkStridedDeviceAddressRegionKHR* hitRegion,
				const VkStridedDeviceAddressRegionKHR* callRegion
			);
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

			/**
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaveBuilder instanced.
			* @param[in] materialName also pipelineName.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipeline(
				const std::string&   materialName                                  , 
				VkCommandBuffer      cmdBuffer    = VK_NULL_HANDLE                 ,
				VkPipelineBindPoint  bindPoint    = VK_PIPELINE_BIND_POINT_COMPUTE
			) override;
			
			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo.
			* For Binding a Renderer DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                      , 
				VkCommandBuffer            cmdBuffer = VK_NULL_HANDLE                 ,
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE
			) override;

			/**
			* @brief Binding DescriptorSet with DescriptorSetInfo and name.
			* For Binding a Material DescriptorSet.
			* @param[in] infos DescriptorSetInfo.
			* @param[in] name The material name.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindDescriptorSet(
				const DescriptorSetInfo&   infos                                       , 
				const std::string&         name                                        , 
				VkCommandBuffer            cmdBuffer = VK_NULL_HANDLE                  ,
				VkPipelineBindPoint        bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE
			) override;

			/**
			* @brief Dispatch Compute Shader.
			* @param[in] x X group.
			* @param[in] y Y group.
			* @param[in] z Z group.
			*/
			void Dispatch(uint32_t x, uint32_t y, uint32_t z);

			/*void AddBarriers(
				VkBuffer              buffer         , 
				VkAccessFlags         srcAccessMask  , 
				VkAccessFlags         dstAccessMask  , 
				VkPipelineStageFlags  srcStageMask   , 
				VkPipelineStageFlags  dstStageMask
			);

			void ReleaseBarriers(
				VkBuffer              buffer         ,
				VkAccessFlags         srcAccessMask  ,
				VkAccessFlags         dstAccessMask  ,
				VkPipelineStageFlags  srcStageMask   ,
				VkPipelineStageFlags  dstStageMask
			);

			void InternalBarriers(
				VkBuffer              buffer                                               ,
				VkAccessFlags         srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT           ,
				VkAccessFlags         dstAccessMask = VK_ACCESS_SHADER_READ_BIT            ,
				VkPipelineStageFlags  srcStageMask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ,
				VkPipelineStageFlags  dstStageMask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT
			);*/
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
		* @brief ThreadPool of Submit Commands.
		*/
		std::shared_ptr<VulkanCmdThreadPool> m_CmdThreadPool;

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
		* @brief Pipelines Reference in DGC Pipeline.
		*/
		std::unordered_map<std::string, std::vector<VkPipeline>> m_PipelinesRef;

		/**
		* @brief Whether should load a default renderer material.
		*/
		bool m_IsLoadDefaultMaterial;

		/**
		* @brief Whether should registry dgc pipeline.
		*/
		bool m_IsRegistryDGCPipeline;

		/**
		* @brief Data of dgc Indirect Draw.
		*/
		std::unordered_map<std::string, std::shared_ptr<VulkanIndirectDrawNV>> m_IndirectData;

		/**
		* @brief Allow this class access all data.
		*/
		friend class DescriptorSetBuilder;
		friend class RendererPassBuilder;
		friend class DGCLayoutBuilder;
	};

	template<typename T>
	inline void Renderer::FillIndirectRenderData(const std::string& subpassName)
	{
		SPICES_PROFILE_ZONE;

		auto indirectPtr = m_IndirectData[subpassName];
		indirectPtr->ResetInput();

		/**
		* @brief Prepare ShaderGroup
		*/
		uint32_t nSequences = 0;
		auto view = FrameInfo::Get().m_World->GetRegistry().view<T>();
		{
			SPICES_PROFILE_ZONEN("FillIndirectRenderData::Prepare ShaderGroup");

			std::unordered_map<std::string, uint32_t> pipelineMap;

			for (auto& e : view)
			{
				auto& meshComp = FrameInfo::Get().m_World->GetRegistry().get<T>(e);

				meshComp.GetMesh()->GetPacks().for_each([&](const auto& k, const std::shared_ptr<MeshPack>& v) {

					if (pipelineMap.find(v->GetMaterial()->GetName()) == pipelineMap.end())
					{
						pipelineMap[v->GetMaterial()->GetName()] = pipelineMap.size();
					}

					v->SetShaderGroupHandle(pipelineMap[v->GetMaterial()->GetName()]);
					nSequences++;

					return false;
				});
			}
			indirectPtr->SetSequenceCount(nSequences);

			m_PipelinesRef[subpassName].resize(pipelineMap.size());

			for (auto& pair : pipelineMap)
			{
				m_PipelinesRef[subpassName][pair.second] = m_Pipelines[pair.first]->GetPipeline();
			}
		}

		/**
		* @brief Fill in Input Buffer
		*/
		std::vector<size_t> offset;
		size_t totalSize = 0;
		std::shared_ptr<VulkanBuffer> inputBuffer = nullptr;
		{
			SPICES_PROFILE_ZONEN("FillIndirectRenderData::Fill in Input Buffer");

			size_t alignSeqIndexMask = m_Device->GetDGCProperties().minSequencesIndexBufferOffsetAlignment - 1;
			size_t alignMask         = m_Device->GetDGCProperties().minIndirectCommandsBufferOffsetAlignment - 1;

			auto& inputStrides       = indirectPtr->GetInputStrides();
			
			for (int i = 0; i < inputStrides.size(); i++)
			{
				offset.push_back(totalSize);
				totalSize += ((inputStrides[i] * nSequences + alignMask) & (~alignMask));
			}

			VulkanBuffer stagingBuffer(
				m_VulkanState,
				"StagingBuffer",
				totalSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			auto& layoutTokens = indirectPtr->GetLayoutTokens();

			int index = 0;
			for (auto& e : view)
			{
				auto& meshComp = FrameInfo::Get().m_World->GetRegistry().get<T>(e);

				meshComp.GetMesh()->GetPacks().for_each([&](const auto& k, const std::shared_ptr<MeshPack>& v) {

					for (int i = 0; i < layoutTokens.size(); i++)
					{
						VkBindShaderGroupIndirectCommandNV  shader;
						VkBindVertexBufferIndirectCommandNV vbo;
						VkBindIndexBufferIndirectCommandNV  ibo;
						VkDeviceAddress                     push;
						VkDrawIndexedIndirectCommand        drawIndexed;
						VkDrawMeshTasksIndirectCommandNV    drawMesh;

						switch (layoutTokens[i].tokenType)
						{
						case VK_INDIRECT_COMMANDS_TOKEN_TYPE_SHADER_GROUP_NV:
							shader.groupIndex = v->GetShaderGroupHandle() + 1;
							stagingBuffer.WriteToBuffer(&shader, inputStrides[i], index * inputStrides[i] + offset[i]);
							break;

						case VK_INDIRECT_COMMANDS_TOKEN_TYPE_VERTEX_BUFFER_NV:
							vbo.bufferAddress = v->GetResource().positions.buffer->GetAddress();
							vbo.size          = sizeof(v->GetResource().positions.attributes);
							vbo.stride        = sizeof(glm::vec3);
							stagingBuffer.WriteToBuffer(&vbo, inputStrides[i], index * inputStrides[i] + offset[i]);
							break;

						case VK_INDIRECT_COMMANDS_TOKEN_TYPE_INDEX_BUFFER_NV:
							ibo.bufferAddress = v->GetResource().primitivePoints.buffer->GetAddress();
							ibo.size          = sizeof(v->GetResource().primitivePoints.attributes);
							ibo.indexType     = VK_INDEX_TYPE_UINT32;
							stagingBuffer.WriteToBuffer(&ibo, inputStrides[i], index * inputStrides[i] + offset[i]);
							break;

						case VK_INDIRECT_COMMANDS_TOKEN_TYPE_PUSH_CONSTANT_NV:
							push              = v->GetMeshDesc().GetBufferAddress();
							stagingBuffer.WriteToBuffer(&push, inputStrides[i], index * inputStrides[i] + offset[i]);
							break;

						case VK_INDIRECT_COMMANDS_TOKEN_TYPE_DRAW_INDEXED_NV:
							drawIndexed.firstIndex     = 0;
							drawIndexed.firstInstance  = 0;
							drawIndexed.indexCount     = v->GetResource().primitivePoints.attributes->size();
							drawIndexed.instanceCount  = 1;
							drawIndexed.vertexOffset   = 0;
							stagingBuffer.WriteToBuffer(&drawIndexed, inputStrides[i], index* inputStrides[i] + offset[i]);
							break;

						case VK_INDIRECT_COMMANDS_TOKEN_TYPE_DRAW_TASKS_NV:
							drawMesh = v->GetDrawCommand();
							stagingBuffer.WriteToBuffer(&drawMesh, inputStrides[i], index * inputStrides[i] + offset[i]);
							break;

						default:
							SPICES_CORE_ERROR("Not Supported Token Type.");
							break;
						}
					}

					index++;
					return false;
				});
			}
			stagingBuffer.Flush();

			inputBuffer = indirectPtr->CreateInputBuffer(totalSize);
			inputBuffer->CopyBuffer(stagingBuffer.Get(), inputBuffer->Get(), totalSize);
		}

		/**
		* @brief Fill in Streams
		*/
		{
			SPICES_PROFILE_ZONEN("FillIndirectRenderData::Fill in Streams");

			std::vector<VkIndirectCommandsStreamNV> inputs;
			inputs.resize(offset.size());
			for (int i = 0; i < offset.size(); i++)
			{
				inputs[i].buffer = inputBuffer->Get();
				inputs[i].offset = offset[i];
			}
			indirectPtr->SetInputStreams(inputs);
		}

		/**
		* @brief Regenerate dgc pipeline
		*/
		{
			SPICES_PROFILE_ZONEN("FillIndirectRenderData::Regenerate dgc pipeline");

			CreateDefaultMaterial();
		}

		/**
		* @brief Create ProcessBuffer.
		*/
		{
			SPICES_PROFILE_ZONEN("FillIndirectRenderData:: Create ProcessBuffer");

			VkGeneratedCommandsMemoryRequirementsInfoNV     memInfo{};
			memInfo.sType                                 = VK_STRUCTURE_TYPE_GENERATED_COMMANDS_MEMORY_REQUIREMENTS_INFO_NV;
			memInfo.maxSequencesCount                     = nSequences;
			memInfo.indirectCommandsLayout                = indirectPtr->GetCommandLayout();
			memInfo.pipeline                              = m_Pipelines["BasePassRenderer.Mesh.Default.DGC"]->GetPipeline();
			memInfo.pipelineBindPoint                     = VK_PIPELINE_BIND_POINT_GRAPHICS;

			VkMemoryRequirements2                           memReqs{};
			memReqs.sType                                 = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;

			m_VulkanState.m_VkFunc.vkGetGeneratedCommandsMemoryRequirementsNV(m_VulkanState.m_Device, &memInfo, &memReqs);

			indirectPtr->SetPreprocessSize(memReqs.memoryRequirements.size);
			indirectPtr->CreatePreprocessBuffer(memReqs.memoryRequirements.size);
		}
	}

	template<typename F>
	inline void Renderer::SubmitCmdsParallel(VkCommandBuffer primaryCmdBuffer, uint32_t subpass, F&& func)
	{
		SPICES_PROFILE_ZONE;

		VkCommandBufferInheritanceInfo         inheritanceInfo {};
		inheritanceInfo.sType                = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.renderPass           = m_Pass->Get();
		inheritanceInfo.subpass              = subpass;
		inheritanceInfo.framebuffer          = m_Pass->GetFramebuffer(FrameInfo::Get().m_Imageindex);
     
		VkCommandBufferBeginInfo               cmdBufferBeginInfo {};
		cmdBufferBeginInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufferBeginInfo.flags             = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		cmdBufferBeginInfo.pInheritanceInfo  = &inheritanceInfo;

		std::future<VkCommandBuffer> cmdBuffer = m_CmdThreadPool->SubmitPoolTask<VkCommandBuffer>([&](VkCommandBuffer cmdBuffer) {

			VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo));

			func(cmdBuffer);

			VK_CHECK(vkEndCommandBuffer(cmdBuffer));

			return cmdBuffer;
		});

		VkCommandBuffer buffer = cmdBuffer.get();
		vkCmdExecuteCommands(primaryCmdBuffer, 1, &buffer);
	}

	template<typename T, typename F>
	inline void Renderer::IterWorldCompSubmitCmdParallel(FrameInfo& frameInfo, uint32_t subpass, F func)
	{
		SPICES_PROFILE_ZONE;

		VkCommandBufferInheritanceInfo         inheritanceInfo {};
		inheritanceInfo.sType                = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.renderPass           = m_Pass->Get();
		inheritanceInfo.subpass              = subpass;
		inheritanceInfo.framebuffer          = m_Pass->GetFramebuffer(frameInfo.m_Imageindex);
						     
		VkCommandBufferBeginInfo               cmdBufferBeginInfo {};
		cmdBufferBeginInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufferBeginInfo.flags             = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		cmdBufferBeginInfo.pInheritanceInfo  = &inheritanceInfo;

		/**
		* @brief Begin all Command Buffer.
		*/
		{
			m_CmdThreadPool->SubmitThreadTask_LightWeight_ForEach([&](VkCommandBuffer cmdBuffer) {
				VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo));
			});
			m_CmdThreadPool->Wait();
		}

		/**
		* @brief Iter use view, not group.
		* @attention Group result nullptr here.
		*/
		{
			auto& view = frameInfo.m_World->GetRegistry().view<T>();
			for (auto& e : view)
			{
				m_CmdThreadPool->SubmitPoolTask<VkCommandBuffer>([&](VkCommandBuffer cmdBuffer) {

					auto& [tComp, transComp] = frameInfo.m_World->GetRegistry().get<T, TransformComponent>(e);

					/**
					* @brief This function defined how we use these components.
					* @param[in] e entityid.
					* @param[in] transComp TransformComponent.
					* @param[in] tComp TComponent.
					*/
					func(cmdBuffer, static_cast<int>(e), transComp, tComp);

					return cmdBuffer;
				});
			}
			m_CmdThreadPool->Wait();
		}

		/**
		* @brief End all Command Buffer.
		*/
		{
			m_CmdThreadPool->SubmitThreadTask_LightWeight_ForEach([&](VkCommandBuffer cmdBuffer) {
				VK_CHECK(vkEndCommandBuffer(cmdBuffer));
			});
			m_CmdThreadPool->Wait();
		}

		vkCmdExecuteCommands(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], m_CmdThreadPool->GetThreadsCount(), m_CmdThreadPool->GetCommandBuffers(frameInfo.m_FrameIndex).data());
	}

	template<typename T, typename F>
	void Renderer::IterWorldCompWithBreak(FrameInfo& frameInfo, F func)
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
	void Renderer::RenderBehaveBuilder::UpdatePushConstant(F func, VkCommandBuffer cmdBuffer)
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
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
			m_Renderer->m_Pipelines[ss.str()]->GetPipelineLayout(),
			VK_SHADER_STAGE_ALL,
			0,
			sizeof(T),
			&push
		);
	}

	template<typename T, typename F>
	inline void Renderer::RenderBehaveBuilder::UpdatePushConstantAsync(F func)
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
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubpassIndex, [&](VkCommandBuffer& cmdBuffer) {
			vkCmdPushConstants(
				cmdBuffer,
				m_Renderer->m_Pipelines[ss.str()]->GetPipelineLayout(),
				VK_SHADER_STAGE_ALL,
				0,
				sizeof(T),
				&push
			);
		});
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
	void Renderer::RenderBehaveBuilder::UpdatePushConstant(void* data, VkCommandBuffer cmdBuffer) const
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		/**
		* @breif Update PushConstants
		*/
		vkCmdPushConstants(
			cmdBuffer ? cmdBuffer : m_CommandBuffer,
			m_Renderer->m_Pipelines[ss.str()]->GetPipelineLayout(),
			VK_SHADER_STAGE_ALL,
			0,
			sizeof(T),
			data
		);
	}

	template<typename T>
	inline void Renderer::RenderBehaveBuilder::UpdatePushConstantAsync(void* data) const
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << m_Renderer->m_RendererName << "." << m_HandledSubPass->GetName() << ".Default";

		/**
		* @breif Update PushConstants
		*/
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, [&](VkCommandBuffer& cmdBuffer) {
			vkCmdPushConstants(
				cmdBuffer,
				m_Renderer->m_Pipelines[ss.str()]->GetPipelineLayout(),
				VK_SHADER_STAGE_ALL,
				0,
				sizeof(T),
				data
			);
		});
	}

	inline void Renderer::RenderBehaveBuilder::UpdateUniformBuffer(uint32_t set, uint32_t binding, void* data) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif Update uniform buffer.
		*/
		m_HandledSubPass->SetBuffer({ set, binding }, data);
	}

	inline void Renderer::RenderBehaveBuilder::UpdateStorageBuffer(
		uint32_t set     , 
		uint32_t binding , 
		void*    data    , 
		uint64_t size    , 
		uint64_t offset
	) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif Update uniform buffer.
		*/
		m_HandledSubPass->SetBuffer({ set, binding }, data, size, offset);
	}

	template<typename T>
	inline Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddBindLessTexture(
		uint32_t                        set          , 
		uint32_t                        binding      , 
		VkShaderStageFlags              stageFlags   , 
		const std::vector<std::string>& textureNames
	)
	{
		SPICES_PROFILE_ZONE;
		
		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < textureNames.size(); i++)
		{
			const std::shared_ptr<Texture> texture = ResourcePool<Texture>::Load<T>(textureNames[i], textureNames[i]);
			m_ImageInfos[set][binding].push_back(*texture->GetResource<VulkanImage>()->GetImageInfo());
		}

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags, BINDLESS_TEXTURE_MAXNUM);

		return *this;
	}

	template<typename T>
	Renderer::DescriptorSetBuilder& Renderer::DescriptorSetBuilder::AddTexture(
		uint32_t                        set        ,
		uint32_t                        binding    ,
		VkShaderStageFlags              stageFlags,
		const std::vector<std::string>& textureNames
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief fill in imageInfos.
		*/
		for (int i = 0; i < textureNames.size(); i++)
		{
			const std::shared_ptr<Texture> texture = ResourcePool<Texture>::Load<T>(textureNames[i], textureNames[i]);
			m_ImageInfos[set][binding].push_back(*texture->GetResource<VulkanImage>()->GetImageInfo());
		}

		/**
		* @brief Registy descriptor and add binging to it.
		*/
		const auto descriptorSet = DescriptorSetManager::Registry(m_DescriptorSetId, set);
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags, static_cast<uint32_t>(textureNames.size()));

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
		if (func)
		{
			func(isEnableBlend, attachmentDescription);
		}

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
		if (func)
		{
			func(depthAttachment);
		}

		/**
		* @brief Instance a VkClearValue.
		* Though we reverse z depth, clear buffer with 0.0f here.
		*/
		VkClearValue                              clearValue{};
		clearValue.depthStencil                 = { 0.0f, 0 };

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