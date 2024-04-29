#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiGBufferVisualizer;

	class ImguiCreateEntity : public ImguiSlate
	{
	public:
		ImguiCreateEntity(const std::string& panelName);
		virtual ~ImguiCreateEntity() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:

	};
}