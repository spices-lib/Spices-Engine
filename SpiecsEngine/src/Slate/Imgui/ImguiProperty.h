#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiProperty : public ImguiSlate
	{
	public:
		ImguiProperty(const std::string& panelName) : ImguiSlate(panelName) {};
		virtual ~ImguiProperty() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
	};
}