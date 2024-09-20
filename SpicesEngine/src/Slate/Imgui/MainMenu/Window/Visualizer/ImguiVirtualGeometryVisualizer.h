/**
* @file ImguiVirtualGeometryVisualizer.h
* @brief The ImguiVirtualGeometryVisualizer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief The VirtualGeometryBufferID Texture's ImTextureID.
	*/
	struct VirtualGeometryBufferID
	{
		ImTextureID TriangleID;          /* @brief ImTextureID of TriangleID. */
		ImTextureID MeshletID;           /* @brief ImTextureID of MeshletID.  */

		/**
		* @brief Free in used DescripotorSet used by GBuffer Visualizer.
		*/
		void Free()
		{
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(TriangleID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(MeshletID));
		}
	};

	/*
	* @brief The ImguiVirtualGeometryVisualizer Class.
	* This class defines how to render a VirtualGeometry Visualizer.
	*/
	class ImguiVirtualGeometryVisualizer : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiVirtualGeometryVisualizer(
			const std::string& panelName,
			FrameInfo& frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiVirtualGeometryVisualizer() override = default;

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
		* @brief Query ID from RendererResourcePool.
		*/
		void QueryID();

	private:

		/**
		* @brief The instance of GBufferID.
		*/
		VirtualGeometryBufferID m_BufferID;

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