#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	struct GBufferID
	{
		ImTextureID FinalColorID;
		ImTextureID BaseColorID;
		ImTextureID NormalID;
	};

	class ImguiGBufferVisualizer : public ImguiSlate
	{
	public:
		ImguiGBufferVisualizer(const std::string& panelName);
		virtual ~ImguiGBufferVisualizer() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;

		void SetWindowState(bool isGBufferSlateOn) { m_IsGBufferSlateOn = isGBufferSlateOn; };

	private:
		bool OnWindowResized(WindowResizeOverEvent& event);
		void QueryGBufferID();

	private:
		GBufferID m_GBufferID;

		bool m_IsGBufferSlateOn = false;
	};
}