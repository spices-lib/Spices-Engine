#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiStage : public ImguiSlate
	{
	public:
		ImguiStage() {};
		virtual ~ImguiStage() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;

	private:
	};
}