#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiMainMenu : public ImguiSlate
	{
	public:
		ImguiMainMenu() {};
		virtual ~ImguiMainMenu() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
	};

}