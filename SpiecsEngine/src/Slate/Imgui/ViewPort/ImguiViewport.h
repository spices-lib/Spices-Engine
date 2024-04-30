#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiFloattingInfo;

	class ImguiViewport : public ImguiSlate
	{
	public:
		ImguiViewport(const std::string& panelName);
		virtual ~ImguiViewport() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;

	public:
		bool IsResizedThisFrame() { return isResized; };

	private:
		bool OnWindowResizeOver(WindowResizeOverEvent& event);
		void QueryIsResizedThisFrame(ImVec2 thisFrameSize);

	private:
		ImTextureID m_ViewportID;
		std::shared_ptr<ImguiFloattingInfo> m_FloattingInfo;
		bool isResized = false;
	};
}