#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiStage : public ImguiSlate
	{
	public:
		ImguiStage(const std::string& panelName) : ImguiSlate(panelName) {};
		virtual ~ImguiStage() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
	};
}