#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiGizmos : public ImguiSlate
	{
	public:
		ImguiGizmos(const std::string& panelName, FrameInfo& frameInfo) : ImguiSlate(panelName, frameInfo) {};
		virtual ~ImguiGizmos() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
	};
}