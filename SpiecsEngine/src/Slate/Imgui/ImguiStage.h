#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	struct StageIconID
	{
		ImTextureID filterIcon;
		ImTextureID optionsIcon;

		ImTextureID visibleIcon;
	};

	class ImguiStage : public ImguiSlate
	{
	public:
		ImguiStage(
			const std::string& panelName  , 
			FrameInfo&         frameInfo
		);
		virtual ~ImguiStage() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override {};

	private:
		StageIconID m_StageIconID;
	};
}