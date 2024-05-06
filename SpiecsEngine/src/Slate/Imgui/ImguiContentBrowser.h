#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiContentBrowser : public ImguiSlate
	{
	public:
		ImguiContentBrowser(const std::string& panelName, FrameInfo& frameInfo) : ImguiSlate(panelName, frameInfo) {};
		virtual ~ImguiContentBrowser() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
	};
}