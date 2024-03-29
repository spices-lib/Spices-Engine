#include "pchheader.h"
#include "UISystem.h"

namespace Spiecs {

	void UISystem::OnSystemInitialize()
	{
	}
	void UISystem::OnSystemShutDown()
	{
	}
	void UISystem::OnSystemUpdate()
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		SPIECS_LOG("UISystem :: fin");
	}
}