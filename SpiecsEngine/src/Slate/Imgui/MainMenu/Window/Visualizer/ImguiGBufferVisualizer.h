#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	struct GBufferID
	{
		ImTextureID SceneColorID;
		ImTextureID AlbedoID;
		ImTextureID NormalID;
		ImTextureID SpecularID;
		
		/**
		* @brief ID Resource, pure red, so not show.
		*/
		ImTextureID IDID;

		/**
		* @brief Depth Resource, pure red, so not show.
		*/
		ImTextureID DepthID;

		void Free() {
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(SceneColorID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(AlbedoID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(NormalID));
		};
	};

	class ImguiGBufferVisualizer : public ImguiSlate
	{
	public:
		ImguiGBufferVisualizer(const std::string& panelName, FrameInfo& frameInfo);
		virtual ~ImguiGBufferVisualizer() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		bool OnSlateResized(SlateResizeEvent& event);
		void QueryGBufferID();

	private:
		GBufferID m_GBufferID;
		float m_Width = 500;
		float m_Height = 500;
	};
}