#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiUtilities : public ImguiSlate
	{
	public:

		ImguiUtilities(
			const std::string& panelName,
			FrameInfo& frameInfo
		);

		virtual ~ImguiUtilities() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};
	};
}