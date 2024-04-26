#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiConsole : public ImguiSlate
	{
	public:
		ImguiConsole() {};
		virtual ~ImguiConsole() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;

	private:
	};
}