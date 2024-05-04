#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiVisualizer;

	class ImguiAbout : public ImguiSlate
	{
	public:
		ImguiAbout(const std::string& panelName);
		virtual ~ImguiAbout() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:

	};
}