/**
* @file SlateInfoBar.cpp.
* @brief The SlateInfoBar Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SlateInfoBar.h"
#include "Systems/SlateSystem.h"
#include "Slate/Imgui/ImguiInfoBar.h"

namespace Spices {

	void SlateInfoBar::Create(const std::string& info, std::function<float()> rateFunc)
	{
		SPICES_PROFILE_ZONE;

		auto ptr = dynamic_cast<ImguiInfoBar*>(SlateSystem::GetRegister()->GetSlate("InfoBar").get());
		ptr->Push(std::make_shared<SlateInfoBar>(info, rateFunc));
	}
}