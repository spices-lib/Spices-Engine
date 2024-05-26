#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiFloattingInfo;
	class ImguiGizmos;

	class ImguiViewport : public ImguiSlate
	{
	public:
		ImguiViewport(const std::string& panelName, FrameInfo& frameInfo);
		virtual ~ImguiViewport() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;

	public:
		bool IsResizedThisFrame() { return isResized; };
		std::pair<uint32_t, uint32_t> GetMousePosInViewport();
		std::shared_ptr<ImguiGizmos> GetGizmo() { return m_Gizmos; };

	private:
		bool OnSlateResize(SlateResizeEvent& event);
		void QueryIsResizedThisFrame(ImVec2 thisFrameSize);

	private:
		ImTextureID m_ViewportID;
		std::shared_ptr<ImguiFloattingInfo> m_FloattingInfo;
		std::shared_ptr<ImguiGizmos> m_Gizmos;
		bool isResized = false;
	};
}