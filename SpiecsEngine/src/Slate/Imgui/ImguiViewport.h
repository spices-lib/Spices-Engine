#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	class ImguiViewport : public ImguiSlate
	{
	public:
		ImguiViewport();
		virtual ~ImguiViewport() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;

	private:
		ImTextureID m_ViewportID;
	};
}