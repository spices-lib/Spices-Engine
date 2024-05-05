#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	struct GBufferID
	{
		ImTextureID FinalColorID;
		ImTextureID BaseColorID;
		ImTextureID NormalID;
		
		/**
		* @brief ID Resource, pure red, so not show.
		*/
		ImTextureID IDID;

		/**
		* @brief Depth Resource, pure red, so not show.
		*/
		ImTextureID DepthID;

		void Free() {
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(FinalColorID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(BaseColorID));
			ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(NormalID));
		};
	};

	class ImguiGBufferVisualizer : public ImguiSlate
	{
	public:
		ImguiGBufferVisualizer(const std::string& panelName);
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