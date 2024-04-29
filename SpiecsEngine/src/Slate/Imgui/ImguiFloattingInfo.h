#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiFloattingInfo : public ImguiSlate
	{
	public:
		ImguiFloattingInfo(const std::string& panelName) : ImguiSlate(panelName) {};
		virtual ~ImguiFloattingInfo() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
	};
}