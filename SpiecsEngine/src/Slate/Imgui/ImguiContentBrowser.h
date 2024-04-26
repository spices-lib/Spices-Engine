#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiContentBrowser : public ImguiSlate
	{
	public:
		ImguiContentBrowser() {};
		virtual ~ImguiContentBrowser() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;

	private:
	};
}