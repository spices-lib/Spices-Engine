#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiFloattingInfo : public ImguiSlate
	{
	public:
		ImguiFloattingInfo(const std::string& panelName, FrameInfo& frameInfo, ImguiSlate* owner) : ImguiSlate(panelName, frameInfo), m_Owner(owner) {};
		virtual ~ImguiFloattingInfo() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
		ImguiSlate* m_Owner;
	};
}