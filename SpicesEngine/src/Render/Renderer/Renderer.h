/**
* @file Renderer.h
* @brief The Renderer Class Definitions.
* @author Spices.
*/

#pragma once
/******************************Core Header**********************************************************/
#include "Core/Core.h"
#include "DescriptorSetManager/DescriptorSetManager.h"
#include "Render/Renderer/RendererPass/RendererPass.h"
#include "RenderPassStatistics\PipelineStatisticsQuerier.h"
#include "..\..\..\assets\Shaders\src\Header\ShaderCommon.h"
#include "Debugger/Aftermath/NsightAftermathGpuCrashTracker.h"
#include "Debugger/Perf/NsightPerfGPUProfilerReportGenerator.h"
#include "Debugger/Perf/NsightPerfGPUProfilerOneshotCollection.h"
#include "Core/Container/BehaveStateList.h"
#include "..\..\Core\Thread\ThreadModel.h"
#include "Render/Renderer/RendererCache/RendererCache.h"
#include "Core/Container/ThreadUnorderedMap.h"
/***************************************************************************************************/

/******************************Vulkan Backend Header************************************************/
#include "Render/FrameInfo.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Render/Vulkan/VulkanPipeline.h"
#include "Render/Vulkan/VulkanUtils.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "Render/Vulkan/VulkanRenderPass.h"
#include "Render/Vulkan/VulkanDeviceGeneratedCommandsNV.h"
/***************************************************************************************************/

/******************************World Component Header***********************************************/
#include "World/World/World.h"
#include "World/Components/MeshComponent.h"
#include "World/Components/TransformComponent.h"
/***************************************************************************************************/

/******************************STL Header***********************************************************/
#include <memory>
#include <unordered_map>
/***************************************************************************************************/

static constexpr uint32_t MAX_DIRECTIONALLIGHT_NUM = 10;
static constexpr uint32_t MAX_POINTLIGHT_MUN = 100;

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
		virtual void OnMeshAddedWorld();

		/**
		* @brief Registry material to Specific Renderer.
		* @param[in] materialName Material Name.
		* @param[in] subPassName SubPass Name.
		*/
		void RegistryMaterial(
			const std::string& materialName , 
			const std::string& subPassName
		);

		/**
		* @brief Registry dgc pipeline to Specific Renderer.
		* @param[in] materialName Material Name.
		* @param[in] subPassName SubPass Name.
		* @param[in] indirectPtr VulkanDeviceGeneratedCommandsNV.
		*/
		void RegistryDGCPipeline(
			const std::string& materialName,
			const std::string& subPassName ,
			VulkanDeviceGeneratedCommandsNV* indirectPtr
		);

		/**
		* @brief Fill in World RenderAble data to IndirectBuffer.
		* @tparam T Specific Component.
		* @param[in] subPassName .
		* @param[in] view Component View.
		* @return Returns new VulkanDeviceGeneratedCommandsNV.
		*/
		template<typename T>
		std::shared_ptr<VulkanDeviceGeneratedCommandsNV> FillIndirectRenderData(const std::string& subPassName, std::shared_ptr<std::vector<uint32_t>> view);

		/**
		* @brief Get entities with component in world.
		* @tparam T Specific Component.
		* @param[in] world world pointer .
		* @return Returns entities.
		*/
		template<typename T>
		std::shared_ptr<std::vector<uint32_t>> GetEntityWithComponent(World* world);

		/**
		* @brief Get RendererPass.
		* @return Returns the RendererPass.
		*/
		inline std::shared_ptr<RendererPass>& GetPasses() { return m_Pass; }

		/**
		* @brief Get default material using sub pass Name.
		* @param subPassName sub pass Name.
		* @return default material.
		*/
		std::shared_ptr<Material> GetDefaultMaterial(const std::string& subPassName) const;
		
		/**
		* @brief Iterator the specific RenderPass Statistics in Renderer.
		* @param[in] func The function pointer that handle Statistics.
		*/
		template<typename F>
		inline void IterStatistics(F&& func);

		/**
		* @brief Reset Renderer State to disActive.
		*/
		void ResetRendererState() { m_IsActive = false; }

	protected:

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
		virtual void CreateDeviceGeneratedCommandsLayout() {}

		/**
		* @brief Create Specific Material Pipeline.
		* @param[in] material Specific Material.
		* @param[in] layout VkPipelineLayout.
		* @param[in] subPass RendererSubPass.
		*/
		virtual void CreatePipeline(
			std::shared_ptr<Material>        material  , 
			VkPipelineLayout&                layout    ,
			std::shared_ptr<RendererSubPass> subPass
		) = 0;

		/**
		* @brief Create Specific Renderer Default Material. 
		*/
		void CreateDefaultMaterial();
		
		/**
		* @brief Create Specific Renderer DGC Material.
		* @param[in] subPass SubPass Name.
		* @param[in] indirectPtr VulkanDeviceGeneratedCommandsNV.
		*/
		void CreateDGCMaterial(const std::string& subPass, VulkanDeviceGeneratedCommandsNV* indirectPtr);
		
		/**
		* @brief Create Pipeline Layout with material's descriptorset and renderer's descriptor set.
		* @param[in] rowSetLayouts All descriptor set collected.
		* @param[in] subPass material used sub pass.
		*/
		VkPipelineLayout CreatePipelineLayout(
			const std::vector<VkDescriptorSetLayout>& rowSetLayouts ,
			const std::shared_ptr<RendererSubPass>&   subPass
		) const;

		/**
		* @brief Create device generated command Pipeline.
		* @param[in] pipelineName Pipeline's name.
		* @param[in] materialName Material's name.
		* @param[in] layout PipelineLayout.
		* @param[in] subPass RendererSubPass.
		* @param[in] indirectPtr VulkanDeviceGeneratedCommandsNV.
		*/
		virtual void CreateDeviceGeneratedCommandsPipeline(
			const std::string&               pipelineName ,
			const std::string&               materialName ,
			VkPipelineLayout&                layout       ,
			std::shared_ptr<RendererSubPass> subPass      ,
			VulkanDeviceGeneratedCommandsNV* indirectPtr
		) {}

		/***************************************************************************************************/

	protected:

		/******************************Renderer Help Function**********************************************/

		/**
		* @brief Submit a group of commands to secondary command buffer, and execute all of them.
		* @param[in] primaryCmdBuffer The main Command Buffer.
		* @param[in] subPass subPass index.
		* @param func Specific Commands.
		* @return Returns future VkCommandBuffer.
		*/
		template<typename F>
		std::future<VkCommandBuffer> SubmitCmdsParallel(VkCommandBuffer primaryCmdBuffer, uint32_t subPass, F&& func);

		/**
		* @brief Iterator the specific Component in World.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldComp(FrameInfo& frameInfo, F func);

		/**
		* @brief Iterator the specific Component in World With break.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldCompWithBreak(FrameInfo& frameInfo, F func);

		/**
		* @brief Iterator the specific Component in World With range.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] ranges Entities View ranges.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldCompWithRange(FrameInfo& frameInfo, const std::vector<uint32_t>& ranges, F func);

		/**
		* @brief Iterator the specific Component in World With range.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] ranges Entities View ranges.
		* @param[in] floor ranges floor.
		* @param[in] ceil ranges ceil.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldCompWithRange(FrameInfo& frameInfo, const std::vector<uint32_t>& ranges, uint32_t floor, uint32_t ceil, F func);

		/**
		* @brief Iterator the specific Component in World With break With range.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] ranges Entities View ranges.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldCompWithBreakWithRange(FrameInfo& frameInfo, const std::vector<uint32_t>& ranges, F func);

		/**
		* @brief Iterator the specific Component in World With break With range.
		* @tparam T The specific Component class.
		* @param[in] frameInfo The current frame data.
		* @param[in] ranges Entities View ranges.
		* @param[in] floor ranges floor.
		* @param[in] ceil ranges ceil.
		* @param[in] func The function pointer that need to execute during this function.
		*/
		template<typename T, typename F>
		inline void IterWorldCompWithBreakWithRange(FrameInfo& frameInfo, const std::vector<uint32_t>& ranges, uint32_t floor, uint32_t ceil, F func);

		/**
		* @brief Get The activated camera entity's view matrix and projection matrix.
		* @param[in] frameInfo The current frame data.
		* @return Returns a tuple of view matrix, projection matrix, stable frames, fov.
		*/
		std::tuple<glm::mat4, glm::mat4, unsigned int, float> GetActiveCameraMatrix(FrameInfo& frameInfo);

		/**
		* @brief Get DirectionalLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @param[out] dLightBuffer DirectionalLight.
		* @todo Multiple directional light.
		*/
		void GetDirectionalLight(FrameInfo& frameInfo, std::array<SpicesShader::DirectionalLight, SpicesShader::DIRECTIONALLIGHT_BUFFER_MAXNUM>& dLightBuffer);

		void GetDirectionalLightMatrix(FrameInfo& frameInfo, std::array<glm::mat4, MAX_DIRECTIONALLIGHT_NUM>& directionalLight);

		/**
		* @brief Get PointLightComponent's render data in World.
		* @param[in] frameInfo The current frame data.
		* @param[out] pLightBuffer PointLight Buffer.
		* @todo infinity pointlight.
		*/
		void GetPointLight(FrameInfo& frameInfo, std::array<SpicesShader::PointLight, SpicesShader::POINTLIGHT_BUFFER_MAXNUM>& pLightBuffer);

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
			* @param[in] flags Statistics Flags.
			* @return Returns the RendererPassBuilder.
			*/
			RendererPassBuilder& AddSubPass(
				const std::string&       subPassName   , 
				Querier::StatisticsFlags flags = Querier::ALL
			);

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
			* @brief Add subPass Self Dependency.
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
			* @param[in] size push constant size.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddPushConstant(uint32_t size);

			/**
			* @brief Create local buffer object in collection, and add it's set binding to descriptorsetlayout, and sets descriptorwriter using it's buffer info.
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
			* @brief Init the BindLess texture set binding to descriptor set layout.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @param[in] textureNames Texture Debug Name.
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
			* @param[in] stageFlags Texture shader usage flags.
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
				uint32_t                                         set                                ,
				uint32_t                                         binding                            ,
				VkShaderStageFlags                               stageFlags                         ,
				const std::vector<std::string>&                  textureNames                       ,
				VkFormat                                         format = VK_FORMAT_R8G8B8A8_UNORM  ,
				TextureType                                      type   = TextureType::Texture2D           
			);

			/**
			* @brief Add the storage texture set binding with mipmap to descriptor set layout.
			* @param[in] set Which set this texture wil use.
			* @param[in] binding Which binding this texture wil use.
			* @param[in] stageFlags Which buffer stage this buffer will use.
			* @param[in] textureName Texture's Name.
			* @param[in] format Texture Format, used in init.
			* @param[in] type Texture's type, used in init.
			* @param[in] func Function of define specific RendererResourceCreateInfo.
			* @return Returns this reference.
			*/
			DescriptorSetBuilder& AddStorageTextureMipmaps(
				uint32_t                                         set                                ,
				uint32_t                                         binding                            ,
				VkShaderStageFlags                               stageFlags                         ,
				const std::string&                               textureName                        ,
				VkFormat                                         format = VK_FORMAT_R8G8B8A8_UNORM  ,
				TextureType                                      type   = TextureType::Texture2D    ,
				std::function<void(RendererResourceCreateInfo&)> func   = nullptr             
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
		* @brief This Class is a helper for Building Specific Renderer Pipeline.
		* Only instanced during CreatePipeline().
		*/
		class PipelineBuilder
		{
		public:

			/**
			* @brief Constructor Function.
			* @param[in] subPass The RendererSubPass.
			* @param[in] material Referenced Material.
			* @param[in] renderer The Owner, pass this pointer.
			*/
			PipelineBuilder(
				std::shared_ptr<RendererSubPass> subPass ,
				std::shared_ptr<Material>        material,
				Renderer*                        renderer
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~PipelineBuilder() = default;

			/**
			* @brief Set PipelineConfigInfo to default value.
			* @return Returns this reference.
			*/
			PipelineBuilder& SetDefault();

			/**
			* @brief Set bindingDescriptions to NULL.
			* @return Returns this reference.
			*/
			PipelineBuilder& NullBindingDescriptions();

			/**
			* @brief Set attributeDescriptions to NULL.
			* @return Returns this reference.
			*/
			PipelineBuilder& NullAttributeDescriptions();

			/**
			* @brief Set bindingDescriptions.
			* @param[in] inputs Vector of VkVertexInputBindingDescription.
			* @return Returns this reference.
			*/
			PipelineBuilder& SetBindingDescriptions(
				const std::vector<VkVertexInputBindingDescription>& inputs
			);

			/**
			* @brief Set attributeDescriptions.
			* @param[in] inputs Vector of VkVertexInputBindingDescription.
			* @return Returns this reference.
			*/
			PipelineBuilder& SetAttributeDescriptions(
				const std::vector<VkVertexInputAttributeDescription>& inputs
			);

			/**
			* @brief Set renderPass to correct value..
			* @return Returns this reference.
			*/
			PipelineBuilder& SetRenderPass();

			/**
			* @brief Set subpass to correct value.
			* @return Returns this reference.
			*/
			PipelineBuilder& SetSubPassIndex();

			/**
			* @brief Set pipelineLayout to given value.
			* @param[in] layout VkPipelineLayout.
			* @return Returns this reference.
			*/
			PipelineBuilder& SetPipelineLayout(VkPipelineLayout& layout);

			/**
			* @brief Set cullMode to given value.
			* @param[in] cullMode VkCullModeFlags.
			* @return Returns this reference.
			*/
			PipelineBuilder& SetCullMode(VkCullModeFlags cullMode);

			/**
			* @brief Set pAttachments to correct value.
			* @return Returns this reference.
			*/
			PipelineBuilder& SetColorAttachments();

			/**
			* @brief Build Pipeline.
			*/
			void Build();

			/**
			* @brief Build Mesh Pipeline.
			*/
			void BuildMesh();

			/**
			* @brief Build Compute Pipeline.
			*/
			void BuildCompute();

			/**
			* @brief Build Raytracing Pipeline.
			* @param[in] hitGroups scene hit shader groups.
			*/
			void BuildRayTracing(const std::unordered_map<std::string, uint32_t>& hitGroups);

			/**
			* @brief Build Raytracing Pipeline.
			* @param[in] pipelineName pipeline's name.
			* @param[in] materialName material's name.
			* @param[in] indirectPtr VulkanDeviceGeneratedCommandsNV.
			*/
			void BuildDeviceGeneratedCommand(const std::string& pipelineName, const std::string& materialName, VulkanDeviceGeneratedCommandsNV* indirectPtr);

		private:

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanced.
			*/
			Renderer* m_Renderer;

			/**
			* @brief Referenced Material.
			*/
			std::shared_ptr<Material> m_Material;

			/**
			* @brief Handled Sub pass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledSubPass;

			/**
			* @brief PipelineConfigInfo for create Pipeline.
			*/
			PipelineConfigInfo m_pipelineConfig;
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
			void Build() const;

		private:

			/**
			* @brief Specific Renderer pointer.
			* Passed while this class instanced.
			*/
			Renderer* m_Renderer;

			/**
			* @brief Specific SubPass Name.
			* Passed while this class instanced.
			*/
			std::string m_SubPassName;

			/**
			* @brief Stores command inputs.
			*/
			std::vector<VkIndirectCommandsLayoutTokenNV> m_InputInfos;

			/**
			* @brief Current SubPass DGC Data.
			*/
			std::shared_ptr<VulkanDeviceGeneratedCommandsNV> m_HandledDGCData;
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
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~RenderBehaveBuilder() = default;

			/**
			* @brief Get RenderPass in statistics.
			* @reutrn Return RenderPass.
			*/
			virtual std::shared_ptr<RendererSubPass>& GetStatisticsRendererPass();

			/**
			* @brief Recording all this behaves does.
			* @param[in] caption Recording Name
			*/
			void Recording(const std::string& caption) const;

			/**
			* @brief End recording all this behaveS does.
			*/
			void EndRecording() const;

			/**
			* @brief Async Commands.
			* @param[in] func In Function Pointer.
			* @return Returns future VkCommandBuffer.
			*/
			std::future<VkCommandBuffer> Async(std::function<void(const VkCommandBuffer& cmdBuffer)> func) const;

			/**
			* @brief Await Async Commands.
			* @param[in] func In Function Pointer.
			*/
			void Await(std::function<void(const VkCommandBuffer& cmdBuffer)> func) const;

			/**
			* @brief Wait for merge secondary commandBuffers.
			* @param[in] futureCmdBuffer secondary commandBuffer.
			*/
			void Wait(std::future<VkCommandBuffer>& futureCmdBuffer) const;

			/**
			* @brief Wait for merge secondary commandBuffer.
			* @param[in] futureCmdBuffers secondary commandBuffer.
			*/
			void Wait(std::vector<std::future<VkCommandBuffer>>& futureCmdBuffers) const;

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
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaveBuilder instanced.
			* @param[in] pipeline VkPipeline.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipeline(
				VkPipeline           pipeline , 
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
			void SetViewPort(const VkCommandBuffer& cmdBuffer = VK_NULL_HANDLE) const;

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
			void RunDGC(VkCommandBuffer cmdBuffer = VK_NULL_HANDLE) const;

			/**
			* @brief Run DGC Buffer Async.
			*/
			void RunDGCAsync() const;

			/**
			* @brief Preprocess Device Generated CommandsBuffer.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void PreprocessDGC_NV(VkCommandBuffer cmdBuffer = VK_NULL_HANDLE) const;

			/**
			* @brief Preprocess Device Generated CommandsBuffer Async.
			*/
			void PreprocessDGCAsync_NV() const;

			/**
			* @brief Execute Device Generated CommandsBuffer.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void ExecuteDGC_NV(const VkCommandBuffer& cmdBuffer = VK_NULL_HANDLE) const;

			/**
			* @brief Execute Device Generated CommandsBuffer Async.
			*/
			void ExecuteDGCAsync_NV() const;

			/**
			* @brief Draw a Triangle full screen.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void DrawFullScreenTriangle(const VkCommandBuffer& cmdBuffer = VK_NULL_HANDLE) const;

			/**
			* @brief Draw a Triangle full screen Async.
			*/
			void DrawFullScreenTriangleAsync() const;

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
			* @brief Update a local buffer.
			* @param[in] set Which set the descriptor will use.
			* @param[in] binding Which binding the descriptor will use.
			* @param[in] buffer Specific VulkanBuffer.
			*/
			void UpdateStorageBuffer(
				uint32_t                      set     , 
				uint32_t                      binding , 
				std::shared_ptr<VulkanBuffer> buffer
			) const;

			/**
			* @brief Update AccelerationStructure in descriptorSet.
			* @param[in] set descriptorSet index.
			* @param[in] binding descriptorSet binding.
			* @param[in] accel VkAccelerationStructureKHR.
			*/
			void UpdateAccelerationStructure(
				uint32_t set     , 
				uint32_t binding , 
				const VkAccelerationStructureKHR& accel
			) const;

			/**
			* @brief End a preview sub pass and stat next sub pass.
			* @param[in] subPassName The name of next sub pass.
			*/
			virtual void BeginNextSubPass(const std::string& subPassName);

			/**
			* @brief End a preview sub pass and stat next sub pass.
			* @param[in] subPassName The name of next sub pass.
			*/
			void BeginNextSubPassAsync(const std::string& subPassName);

			/**
			* @brief Begin this Renderer's RenderPass.
			*/
			virtual void BeginRenderPass();

			/**
			* @brief Begin this Renderer's RenderPass Async.
			*/
			void BeginRenderPassAsync();

			/**
			* @brief End this Renderer's RenderPass.
			*/
			virtual void EndRenderPass();

			/****************************************************************************/

			/**
			* @brief Add a Image Barrier from compute queue to this graphic queue.
			* @param[in] image VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] srcQueueFamilyIndex source queue family.
			* @param[in] dstQueueFamilyIndex dest queue family.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void AddBarriers(
				VulkanImage*          image               ,
				VkAccessFlags         srcAccessMask       , 
				VkAccessFlags         dstAccessMask       , 
				VkPipelineStageFlags  srcStageMask        , 
				VkPipelineStageFlags  dstStageMask        ,
				uint32_t              srcQueueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily,
				uint32_t              dstQueueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily,
				VkCommandBuffer       cmdBuffer           = VK_NULL_HANDLE
			);

			/**
			* @brief Add a Image Barrier from this graphic queue to compute queue.
			* @param[in] image VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] srcQueueFamilyIndex source queue family.
			* @param[in] dstQueueFamilyIndex dest queue family.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void ReleaseBarriers(
				VulkanImage*          image          ,
				VkAccessFlags         srcAccessMask  ,
				VkAccessFlags         dstAccessMask  ,
				VkPipelineStageFlags  srcStageMask   ,
				VkPipelineStageFlags  dstStageMask   ,
				uint32_t              srcQueueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily,
				uint32_t              dstQueueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily,
				VkCommandBuffer       cmdBuffer           = VK_NULL_HANDLE
			);

			/**
			* @brief Add a Image Barrier from this queue to this queue.
			* @param[in] image VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void InternalBarriers(
				VulkanImage*          image                                                ,
				VkAccessFlags         srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT           ,
				VkAccessFlags         dstAccessMask = VK_ACCESS_SHADER_READ_BIT            ,
				VkPipelineStageFlags  srcStageMask  = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ,
				VkPipelineStageFlags  dstStageMask  = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT    ,
				VkCommandBuffer       cmdBuffer     = VK_NULL_HANDLE
			);

			/****************************************************************************/

			/**
			* @brief Add a Buffer Barrier from compute queue to this graphic queue.
			* @param[in] buffer VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] srcQueueFamilyIndex source queue family.
			* @param[in] dstQueueFamilyIndex dest queue family.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void AddBarriers(
				VkBuffer              buffer         , 
				VkAccessFlags         srcAccessMask  , 
				VkAccessFlags         dstAccessMask  , 
				VkPipelineStageFlags  srcStageMask   , 
				VkPipelineStageFlags  dstStageMask   ,
				uint32_t              srcQueueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily,
				uint32_t              dstQueueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily,
				VkCommandBuffer       cmdBuffer           = VK_NULL_HANDLE
			);

			/**
			* @brief Add a Buffer Barrier from this graphic queue to compute queue.
			* @param[in] buffer VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] srcQueueFamilyIndex source queue family.
			* @param[in] dstQueueFamilyIndex dest queue family.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void ReleaseBarriers(
				VkBuffer              buffer         ,
				VkAccessFlags         srcAccessMask  ,
				VkAccessFlags         dstAccessMask  ,
				VkPipelineStageFlags  srcStageMask   ,
				VkPipelineStageFlags  dstStageMask   ,
				uint32_t              srcQueueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily,
				uint32_t              dstQueueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily,
				VkCommandBuffer       cmdBuffer           = VK_NULL_HANDLE
			);

			/**
			* @brief Add a Buffer Barrier from this queue to this queue.
			* @param[in] buffer VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void InternalBarriers(
				VkBuffer              buffer                                               ,
				VkAccessFlags         srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT           ,
				VkAccessFlags         dstAccessMask = VK_ACCESS_SHADER_READ_BIT            ,
				VkPipelineStageFlags  srcStageMask  = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ,
				VkPipelineStageFlags  dstStageMask  = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT    ,
				VkCommandBuffer       cmdBuffer     = VK_NULL_HANDLE
			);

			/**
			* @brief Add a Memory Barrier from this queue to this queue.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void InternalBarriers(
				VkAccessFlags         srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT           , 
				VkAccessFlags         dstAccessMask = VK_ACCESS_SHADER_READ_BIT            , 
				VkPipelineStageFlags  srcStageMask  = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ,
				VkPipelineStageFlags  dstStageMask  = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT    ,
				VkCommandBuffer       cmdBuffer     = VK_NULL_HANDLE
			);

			/**
			* @brief Add a memory Barrier.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			void InternalRegionBarrier(
				VkAccessFlags          srcAccessMask ,
				VkAccessFlags          dstAccessMask ,
				VkPipelineStageFlags   srcStageMask  ,
				VkPipelineStageFlags   dstStageMask  ,
				VkCommandBuffer        cmdBuffer = VK_NULL_HANDLE
			) const;

			/**
			* @brief Add a memory Barrier.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			*/
			void InternalRegionBarrierAsync(
				VkAccessFlags          srcAccessMask ,
				VkAccessFlags          dstAccessMask ,
				VkPipelineStageFlags   srcStageMask  ,
				VkPipelineStageFlags   dstStageMask
			) const;

			/****************************************************************************/

		public:

			/**
			* @brief Get current subPass index in renderPass.
			* @return Returns index of subPass.
			*/
			uint32_t GetSubPassIndex() const { return m_SubPassIndex; }

		public:

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
			* @brief Current subPass Index.
			*/
			uint32_t m_SubPassIndex = 0;

			/**
			* @brief Current CommandBuffer.
			*/
			VkCommandBuffer m_CommandBuffer;

			/**
			* @brief Handled Sub pass.
			*/
			std::shared_ptr<RendererSubPass> m_HandledSubPass;

			/**
			* @brief Current subPass DGCData.
			*/
			std::shared_ptr<VulkanDeviceGeneratedCommandsNV> m_HandledDGCData;
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
			* @param[in] rayTracing VulkanRayTracing.
			* @param[in] currentFrame Passed from FrameInfo.
			* @param[in] currentImage Passed from FrameInfo.
			*/
			RayTracingRenderBehaveBuilder(
				Renderer*         renderer     , 
				VulkanRayTracing* rayTracing   ,
				uint32_t          currentFrame , 
				uint32_t          currentImage
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~RayTracingRenderBehaveBuilder() override = default;
			
			/**
			* @brief Get RenderPass in statistics.
			* @reutrn Return RenderPass.
			*/
			virtual std::shared_ptr<RendererSubPass>& GetStatisticsRendererPass() override;

			/**
			* @brief Begin this Renderer's RenderPass.
			*/
			virtual void BeginRenderPass() override;

			/**
			* @brief End this Renderer's RenderPass.
			*/
			virtual void EndRenderPass() override;

			/**
			* @brief End a preview sub pass and stat next sub pass.
			* @param[in] subPassName The name of next sub pass.
			*/
			virtual void BeginNextSubPass(const std::string& subPassName) override;

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
			* @brief Bind the pipeline created by CreatePipeline().
			* Called on RenderBehaveBuilder instanced.
			* @param[in] pipeline VkPipeline.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			* @param[in] bindPoint VkPipelineBindPoint.
			*/
			virtual void BindPipeline(
				VkPipeline           pipeline , 
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
			*/
			void TraceRays() const;

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
			) const;

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
			) const;

		private:

			/**
			* @brief Referenced VulkanRayTracing.
			*/
			VulkanRayTracing* m_VulkanRayTracing;
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
			* @param[in] cmdBuffers Buffers that collect commands.
			*/
			ComputeRenderBehaveBuilder(
				Renderer* renderer      , 
				uint32_t  currentFrame  , 
				uint32_t  currentImage  ,
				const std::array<VkCommandBuffer, MaxFrameInFlight>& cmdBuffers
			);

			/**
			* @brief Destructor Function.
			*/
			virtual ~ComputeRenderBehaveBuilder() override = default;

			/**
			* @brief Get RenderPass in statistics.
			* @reutrn Return RenderPass.
			*/
			virtual std::shared_ptr<RendererSubPass>& GetStatisticsRendererPass() override;

			/**
			* @brief Begin this Renderer's RenderPass.
			*/
			virtual void BeginRenderPass() override;

			/**
			* @brief End this Renderer's RenderPass.
			*/
			virtual void EndRenderPass() override;

			/**
			* @brief End a preview sub pass and stat next sub pass.
			* @param[in] subPassName The name of next sub pass.
			*/
			virtual void BeginNextSubPass(const std::string& subPassName) override;

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
			void Dispatch(uint32_t x, uint32_t y, uint32_t z) const;

			/****************************************************************************/

			/**
			* @brief Add a Image Barrier from graphic queue to this compute queue.
			* @param[in] image VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] srcQueueFamilyIndex source queue family.
			* @param[in] dstQueueFamilyIndex dest queue family.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void AddBarriers(
				VulkanImage*          image               , 
				VkAccessFlags         srcAccessMask       , 
				VkAccessFlags         dstAccessMask       , 
				VkPipelineStageFlags  srcStageMask        , 
				VkPipelineStageFlags  dstStageMask        ,
				uint32_t              srcQueueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily,
				uint32_t              dstQueueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily,
				VkCommandBuffer       cmdBuffer           = VK_NULL_HANDLE
			) override;

			/**
			* @brief Add a Image Barrier from this compute queue to graphic queue.
			* @param[in] image VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] srcQueueFamilyIndex source queue family.
			* @param[in] dstQueueFamilyIndex dest queue family.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void ReleaseBarriers(
				VulkanImage*          image          ,
				VkAccessFlags         srcAccessMask  ,
				VkAccessFlags         dstAccessMask  ,
				VkPipelineStageFlags  srcStageMask   ,
				VkPipelineStageFlags  dstStageMask   ,
				uint32_t              srcQueueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily,
				uint32_t              dstQueueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily,
				VkCommandBuffer       cmdBuffer           = VK_NULL_HANDLE
			) override;

			/**
			* @brief Add a Image Barrier from this queue to this queue.
			* @param[in] image VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void InternalBarriers(
				VulkanImage*          image                                                ,
				VkAccessFlags         srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT           ,
				VkAccessFlags         dstAccessMask = VK_ACCESS_SHADER_READ_BIT            ,
				VkPipelineStageFlags  srcStageMask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ,
				VkPipelineStageFlags  dstStageMask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ,
				VkCommandBuffer       cmdBuffer     = VK_NULL_HANDLE
			) override;

			/****************************************************************************/

			/**
			* @brief Add a Buffer Barrier from graphic queue to this compute queue.
			* @param[in] buffer VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] srcQueueFamilyIndex source queue family.
			* @param[in] dstQueueFamilyIndex dest queue family.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void AddBarriers(
				VkBuffer              buffer         , 
				VkAccessFlags         srcAccessMask  , 
				VkAccessFlags         dstAccessMask  , 
				VkPipelineStageFlags  srcStageMask   , 
				VkPipelineStageFlags  dstStageMask   ,
				uint32_t              srcQueueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily,
				uint32_t              dstQueueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily,
				VkCommandBuffer       cmdBuffer           = VK_NULL_HANDLE
			) override;

			/**
			* @brief Add a Buffer Barrier from this compute queue to graphic queue.
			* @param[in] buffer VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] srcQueueFamilyIndex source queue family.
			* @param[in] dstQueueFamilyIndex dest queue family.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void ReleaseBarriers(
				VkBuffer              buffer         ,
				VkAccessFlags         srcAccessMask  ,
				VkAccessFlags         dstAccessMask  ,
				VkPipelineStageFlags  srcStageMask   ,
				VkPipelineStageFlags  dstStageMask   ,
				uint32_t              srcQueueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily,
				uint32_t              dstQueueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily,
				VkCommandBuffer       cmdBuffer           = VK_NULL_HANDLE
			) override;

			/**
			* @brief Add a Buffer Barrier from this queue to this queue.
			* @param[in] buffer VulkanImage.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void InternalBarriers(
				VkBuffer              buffer                                               ,
				VkAccessFlags         srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT           ,
				VkAccessFlags         dstAccessMask = VK_ACCESS_SHADER_READ_BIT            ,
				VkPipelineStageFlags  srcStageMask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ,
				VkPipelineStageFlags  dstStageMask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ,
				VkCommandBuffer       cmdBuffer     = VK_NULL_HANDLE
			) override;

			/**
			* @brief Add a Memory Barrier from this queue to this queue.
			* @param[in] srcAccessMask VkAccessFlags.
			* @param[in] dstAccessMask VkAccessFlags.
			* @param[in] srcStageMask VkPipelineStageFlags.
			* @param[in] dstStageMask VkPipelineStageFlags.
			* @param[in] cmdBuffer Input a VkCommandBuffer if needs, otherwise use self variable.
			*/
			virtual void InternalBarriers(
				VkAccessFlags         srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT           , 
				VkAccessFlags         dstAccessMask = VK_ACCESS_SHADER_READ_BIT            , 
				VkPipelineStageFlags  srcStageMask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ,
				VkPipelineStageFlags  dstStageMask  = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ,
				VkCommandBuffer       cmdBuffer     = VK_NULL_HANDLE
			) override;

			/****************************************************************************/
		};

	public:

		/**
		* @brief Statistics StateList.
		*/
		static std::shared_ptr<scl::behave_state_list<void, Renderer::RenderBehaveBuilder*, VkCommandBuffer>> m_StatisticsStateList;

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
		scl::thread_unordered_map<std::string, std::shared_ptr<VulkanPipeline>> m_Pipelines;

		/**
		* @brief Combination of DescriptorSet.
		*/
		DescriptorSetManager::DescriptorSetCombineFlags m_DescriptorSetCombine;

		/**
		* @brief Whether should load a default renderer material.
		*/
		bool m_IsLoadDefaultMaterial;

		/**
		* @brief Whether this renderer is active(submit commands in RendererBehaveBuilder).
		*/
		bool m_IsActive;

		/**
		* @brief Data of dgc DGC Draw.
		*/
		std::unordered_map<std::string, std::shared_ptr<VulkanDeviceGeneratedCommandsNV>> m_DGCData;

		/**
		* @brief RendererCache.
		*/
		std::shared_ptr<RendererCache> m_RenderCache;

		/**
		* @brief Allow this class access all data.
		*/
		friend class DescriptorSetBuilder;
		friend class RendererPassBuilder;
		friend class DGCLayoutBuilder;
	};

	template<typename T>
	inline std::shared_ptr<VulkanDeviceGeneratedCommandsNV> Renderer::FillIndirectRenderData(const std::string& subPassName, std::shared_ptr<std::vector<uint32_t>> view)
	{
		SPICES_PROFILE_ZONE;

		auto indirectPtr = std::make_shared<VulkanDeviceGeneratedCommandsNV>(m_VulkanState);
		auto srcPtr      = m_DGCData[subPassName];

		/**
		* @brief Cache original data.
		*/
		indirectPtr->SetInputStride(srcPtr->GetInputStrides());
		indirectPtr->SetStrides(srcPtr->GetStrides());
		indirectPtr->SetCommandLayout(srcPtr->GetCommandLayout());
		indirectPtr->SetLayoutTokens(srcPtr->GetLayoutTokens());

		/**
		* @brief Move original to caches.
		*/
		m_RenderCache->PushToCaches(srcPtr);

		/**
		* @brief Prepare ShaderGroup
		*/
		uint32_t nSequences = 0;
		{
			SPICES_PROFILE_ZONEN("FillIndirectRenderData::Prepare ShaderGroup");

			std::unordered_map<std::string, uint32_t> pipelineMap;
			
			FrameInfo::Get().m_World->ViewComponent<T>(*view, [&](const auto& e, auto& meshComp){
				
				meshComp.GetMesh()->GetPacks().for_each([&](const auto& k, const std::shared_ptr<MeshPack>& v) {

					if (pipelineMap.find(v->GetMaterial()->GetName()) == pipelineMap.end())
					{
						pipelineMap[v->GetMaterial()->GetName()] = pipelineMap.size();
					}

					v->SetShaderGroupHandle(pipelineMap[v->GetMaterial()->GetName()]);
					nSequences++;

					return false;
				});
			});
			indirectPtr->SetSequenceCount(nSequences);

			indirectPtr->GetPipelineRef().resize(pipelineMap.size(), nullptr);

			for (auto& pair : pipelineMap)
			{
				indirectPtr->GetPipelineRef()[pair.second] = m_Pipelines.Find(pair.first)->GetPipeline();
			}
		}

		/**
		* @brief Fill in Input Buffer
		*/
		std::vector<size_t> offset;
		std::shared_ptr<VulkanBuffer> inputBuffer = nullptr;
		{
			SPICES_PROFILE_ZONEN("FillIndirectRenderData::Fill in Input Buffer");

			size_t totalSize         = 0;
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
			FrameInfo::Get().m_World->ViewComponent<T>(*view, [&](const auto& e, auto& meshComp){
					
				meshComp.GetMesh()->GetPacks().for_each([&](const auto& k, const std::shared_ptr<MeshPack>& v) {

					for (int i = 0; i < layoutTokens.size(); i++)
					{
						switch (layoutTokens[i].tokenType)
						{
							case VK_INDIRECT_COMMANDS_TOKEN_TYPE_SHADER_GROUP_NV:
							{
								VkBindShaderGroupIndirectCommandNV shader;
								shader.groupIndex = v->GetShaderGroupHandle() + 1;
								stagingBuffer.WriteToBuffer(&shader, inputStrides[i], index * inputStrides[i] + offset[i]);
								break;
							}
							case VK_INDIRECT_COMMANDS_TOKEN_TYPE_VERTEX_BUFFER_NV:
							{
								VkBindVertexBufferIndirectCommandNV vbo;
								vbo.bufferAddress = v->GetResource().positions.buffer->GetAddress();
								vbo.size          = sizeof(v->GetResource().positions.attributes);
								vbo.stride        = sizeof(glm::vec3);
								stagingBuffer.WriteToBuffer(&vbo, inputStrides[i], index * inputStrides[i] + offset[i]);
								break;
							}
							case VK_INDIRECT_COMMANDS_TOKEN_TYPE_INDEX_BUFFER_NV:
							{
								VkBindIndexBufferIndirectCommandNV ibo;
								ibo.bufferAddress = v->GetResource().primitivePoints.buffer->GetAddress();
								ibo.size          = sizeof(v->GetResource().primitivePoints.attributes);
								ibo.indexType     = VK_INDEX_TYPE_UINT32;
								stagingBuffer.WriteToBuffer(&ibo, inputStrides[i], index * inputStrides[i] + offset[i]);
								break;
							}
							case VK_INDIRECT_COMMANDS_TOKEN_TYPE_PUSH_CONSTANT_NV:
							{
								VkDeviceAddress push;
								push              = v->GetMeshDesc().GetBufferAddress();
								stagingBuffer.WriteToBuffer(&push, inputStrides[i], index * inputStrides[i] + offset[i]);
								break;
							}
							case VK_INDIRECT_COMMANDS_TOKEN_TYPE_DRAW_INDEXED_NV:
							{
								VkDrawIndexedIndirectCommand drawIndexed;
								drawIndexed.firstIndex     = 0;
								drawIndexed.firstInstance  = 0;
								drawIndexed.indexCount     = v->GetResource().primitivePoints.attributes->size();
								drawIndexed.instanceCount  = 1;
								drawIndexed.vertexOffset   = 0;
								stagingBuffer.WriteToBuffer(&drawIndexed, inputStrides[i], index* inputStrides[i] + offset[i]);
								break;
							}
							case VK_INDIRECT_COMMANDS_TOKEN_TYPE_DRAW_TASKS_NV:
							{
								VkDrawMeshTasksIndirectCommandNV drawMesh;
								drawMesh = v->GetDrawCommand();
								stagingBuffer.WriteToBuffer(&drawMesh, inputStrides[i], index * inputStrides[i] + offset[i]);
								break;
							}
							default:
								SPICES_CORE_ERROR("Not Supported Token Type.");
								break;
						}
					}

					index++;
					return false;
				});
			});
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

			CreateDGCMaterial(subPassName, indirectPtr.get());
		}

		/**
		* @brief Create ProcessBuffer.
		*/
		{
			SPICES_PROFILE_ZONEN("FillIndirectRenderData:: Create ProcessBuffer");

			indirectPtr->CreatePreprocessBuffer();
		}

		return indirectPtr;
	}

	template<typename T>
	inline std::shared_ptr<std::vector<uint32_t>> Renderer::GetEntityWithComponent(World* world)
	{
		SPICES_PROFILE_ZONE;

		auto entities = std::make_shared<std::vector<uint32_t>>();

		world->ViewComponent<MeshComponent>([&](auto entityID, auto& tComp) {
			
			entities->push_back(static_cast<uint32_t>(entityID));
			return false;
			
		});

		return entities;
	}

	template<typename F>
	inline std::future<VkCommandBuffer> Renderer::SubmitCmdsParallel(VkCommandBuffer primaryCmdBuffer, uint32_t subPass, F&& func)
	{
		SPICES_PROFILE_ZONE;

		return AsyncRHITask(ThreadPoolEnum::RHI, [=](VkCommandBuffer cmdBuffer) {

			VkCommandBufferInheritanceInfo         inheritanceInfo {};
			inheritanceInfo.sType                = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			inheritanceInfo.renderPass           = m_Pass->Get();
			inheritanceInfo.subpass              = subPass;
			inheritanceInfo.framebuffer          = m_Pass->GetFramebuffer(FrameInfo::Get().m_ImageIndex);
			inheritanceInfo.occlusionQueryEnable = VK_TRUE;

#ifdef SPICES_DEBUG

			inheritanceInfo.pipelineStatistics   = static_cast<VkQueryPipelineStatisticFlags>(PipelineStatisticEnum::ALL);
     
#endif

			VkCommandBufferBeginInfo               cmdBufferBeginInfo {};
			cmdBufferBeginInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			cmdBufferBeginInfo.flags             = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
			cmdBufferBeginInfo.pInheritanceInfo  = &inheritanceInfo;

			VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo))

			func(cmdBuffer);

			VK_CHECK(vkEndCommandBuffer(cmdBuffer))

			return cmdBuffer;
		});
	}

	template<typename T, typename F>
	inline void Renderer::IterWorldComp(FrameInfo& frameInfo, F func)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter use view, not group.
		* @attention Group result nullptr here.
		*/
		frameInfo.m_World->ViewComponent<T>([&](auto e, auto& tComp) {
			auto& transComp = frameInfo.m_World->GetRegistry().get<TransformComponent>(e);

			/**
			* @brief This function defined how we use these components.
			* @param[in] e entityid.
			* @param[in] transComp TransformComponent.
			* @param[in] tComp TComponent.
			*/
			func(static_cast<int>(e), transComp, tComp);

			return false;
		});
	}

	template<typename T, typename F>
	void Renderer::IterWorldCompWithBreak(FrameInfo& frameInfo, F func)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter use view, not group.
		* @attention Group result nullptr here.
		*/
		frameInfo.m_World->ViewComponent<T>([&](auto e, auto& tComp) {
			auto& transComp = frameInfo.m_World->GetRegistry().get<TransformComponent>(e);

			/**
			* @brief This function defined how we use these components.
			* @param[in] e entityid.
			* @param[in] transComp TransformComponent.
			* @param[in] tComp TComponent.
			* @return Returns true if need break for for loop.
			*/
			return func(static_cast<int>(e), transComp, tComp);
		});
	}

	template<typename T, typename F>
	inline void Renderer::IterWorldCompWithRange(FrameInfo& frameInfo, const std::vector<uint32_t>& ranges, F func)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter use view, not group.
		* @attention Group result nullptr here.
		*/
		frameInfo.m_World->ViewComponent<T>(ranges, [&](auto e, auto& tComp) {
			auto& transComp = frameInfo.m_World->GetRegistry().get<TransformComponent>(e);

			/**
			* @brief This function defined how we use these components.
			* @param[in] e entityid.
			* @param[in] transComp TransformComponent.
			* @param[in] tComp TComponent.
			* @return Returns true if need break for for loop.
			*/
			func(static_cast<int>(e), transComp, tComp);

			return false;
		});
	}

	template<typename T, typename F>
	inline void Renderer::IterWorldCompWithRange(FrameInfo& frameInfo, const std::vector<uint32_t>& ranges, uint32_t floor, uint32_t ceil, F func)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter use view, not group.
		* @attention Group result nullptr here.
		*/
		frameInfo.m_World->ViewComponent<T>(ranges, floor, ceil, [&](auto e, auto& tComp) {
			auto& transComp = frameInfo.m_World->GetRegistry().get<TransformComponent>(e);

			/**
			* @brief This function defined how we use these components.
			* @param[in] e entityid.
			* @param[in] transComp TransformComponent.
			* @param[in] tComp TComponent.
			* @return Returns true if need break for for loop.
			*/
			func(static_cast<int>(e), transComp, tComp);

			return false;
		});
	}

	template<typename T, typename F>
	inline void Renderer::IterWorldCompWithBreakWithRange(FrameInfo& frameInfo, const std::vector<uint32_t>& ranges, F func)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter use view, not group.
		* @attention Group result nullptr here.
		*/
		frameInfo.m_World->ViewComponent<T>(ranges, [&](auto e, auto& tComp) {
			auto& transComp = frameInfo.m_World->GetRegistry().get<TransformComponent>(e);

			/**
			* @brief This function defined how we use these components.
			* @param[in] e entityid.
			* @param[in] transComp TransformComponent.
			* @param[in] tComp TComponent.
			* @return Returns true if need break for for loop.
			*/
			return func(static_cast<int>(e), transComp, tComp);
		});
	}

	template<typename T, typename F>
	inline void Renderer::IterWorldCompWithBreakWithRange(FrameInfo& frameInfo, const std::vector<uint32_t>& ranges, uint32_t floor, uint32_t ceil, F func)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter use view, not group.
		* @attention Group result nullptr here.
		*/
		frameInfo.m_World->ViewComponent<T>(ranges, floor, ceil, [&](auto e, auto& tComp) {
			auto& transComp = frameInfo.m_World->GetRegistry().get<TransformComponent>(e);

			/**
			* @brief This function defined how we use these components.
			* @param[in] e entityid.
			* @param[in] transComp TransformComponent.
			* @param[in] tComp TComponent.
			* @return Returns true if need break for for loop.
			*/
			return func(static_cast<int>(e), transComp, tComp);
		});
	}

	template<typename F>
	inline void Renderer::IterStatistics(F&& func)
	{
		SPICES_PROFILE_ZONE;

		if (!m_IsActive) return;

		/**
		* @brief Iter valid statistics in rendererPass.
		*/
		m_Pass->GetSubPasses()->for_each([&](const std::string& name, const std::shared_ptr<RendererSubPass>& subPass) {

			const auto& ptr = subPass->GetStatistics();

			if (!ptr) return false;

			return func(name, ptr);
		});
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
			m_Renderer->m_Pipelines.Find(ss.str())->GetPipelineLayout(),
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
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, m_SubPassIndex, [&](const VkCommandBuffer& cmdBuffer) {
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
		m_Renderer->SubmitCmdsParallel(m_CommandBuffer, [&](const VkCommandBuffer& cmdBuffer) {
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

	inline void Renderer::RenderBehaveBuilder::UpdateStorageBuffer(
		uint32_t                      set     , 
		uint32_t                      binding , 
		std::shared_ptr<VulkanBuffer> buffer
	) const
	{
		SPICES_PROFILE_ZONE;

		const String2 m_DescriptorSetId = { m_Renderer->m_Pass->GetName(), m_HandledSubPass->GetName() };

		const auto& descriptorSets = DescriptorSetManager::GetByName(m_DescriptorSetId);

		descriptorSets.find(set)->second->UpdateDescriptorSet(binding, buffer);
	}

	inline void Renderer::RenderBehaveBuilder::UpdateAccelerationStructure(
		uint32_t set     , 
		uint32_t binding , 
		const VkAccelerationStructureKHR& accel
	) const
	{
		SPICES_PROFILE_ZONE;

		const String2 m_DescriptorSetId = { m_Renderer->m_Pass->GetName(), m_HandledSubPass->GetName() };

		const auto& descriptorSets = DescriptorSetManager::GetByName(m_DescriptorSetId);

		descriptorSets.find(set)->second->UpdateDescriptorSet(binding, accel);
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
		descriptorSet->AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags, SpicesShader::BINDLESS_TEXTURE_MAXNUM);

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
			colorBlend.dstAlphaBlendFactor      = VK_BLEND_FACTOR_ONE; // Stable alpha channel to 1, for ImGui::Image will rendering background wrongly.
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