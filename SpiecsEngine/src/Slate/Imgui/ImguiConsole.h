#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiConsole : public ImguiSlate
	{
	public:
		ImguiConsole(const std::string& panelName) : ImguiSlate(panelName) {};
		virtual ~ImguiConsole() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
	};
}