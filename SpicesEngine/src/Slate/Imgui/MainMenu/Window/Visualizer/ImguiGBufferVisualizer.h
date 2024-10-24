/**
* @file ImguiVisualizer.h
* @brief The ImguiVisualizer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief The GBuffer Texture's ImTextureID.
	*/
	struct GBufferID
	{
		ImTextureID SceneColorID;   // ImTextureID of SceneColor.
		ImTextureID AlbedoID;       // ImTextureID of AlbedoID.
		ImTextureID NormalID;       // ImTextureID of NormalID.
		ImTextureID RoughnessID;    // ImTextureID of RoughnessID.
		ImTextureID MetallicID;     // ImTextureID of MetallicID.
		
		ImTextureID IDID;           // ImTextureID of ID Resource   , pure red, so not show.
		ImTextureID DepthID;        // ImTextureID of Depth Resource, pure red, so not show.

		/**
		* @brief Free in used DescripotorSet used by GBuffer Visualizer.
		*/
		void Free() 
		{
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(SceneColorID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(AlbedoID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(NormalID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(RoughnessID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(MetallicID));
		}
	};

	/*
	* @brief The ImguiGBufferVisualizer Class.
	* This class defines how to render a GBuffer Visualizer.
	*/
	class ImguiGBufferVisualizer : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiGBufferVisualizer(
			const std::string& panelName , 
			FrameInfo&         frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiGBufferVisualizer() override = default;

		/**
		* @brief This interface is called On SlateSystem Update.
		* @param[in] ts TimeStep.
		*/
		virtual void OnUpdate(TimeStep& ts) override {}

		/**
		* @brief This interface is called On SlateRenderer Render.
		*/
		virtual void OnRender() override;

		/**
		* @brief This interface is called On Global Event Function Pointer is called.
		*/
		virtual void OnEvent(Event& event) override;

	private:

		/**
		* @brief Event Dispatcher target. Registy on Slate(Viewport) Resized.
		* @param[in] event SlateResizeEvent.
		* @return Returns true if need block the event.
		*/
		bool OnSlateResized(SlateResizeEvent& event);

		/**
		* @brief Query GBufferID from RendererResourcePool.
		*/
		void QueryGBufferID();

	private:

		/**
		* @brief The instance of GBufferID.
		*/
		GBufferID m_GBufferID;

		/**
		* @brief The slate width.
		*/
		float m_Width = 500;

		/**
		* @brief The slate height.
		*/
		float m_Height = 500;
	};
}