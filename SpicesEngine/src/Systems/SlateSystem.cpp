/**
* @file SlateSystem.cpp.
* @brief The SlateSystem Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SlateSystem.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"
#include "Core/Event/Event.h"
#include "Core/Log/Console.h"

namespace Spices {

	std::shared_ptr<SlateRegister> SlateSystem::m_SlateRegister;

	void SlateSystem::OnSystemInitialize()
	{
		SPICES_PROFILE_ZONE;

		m_SlateRegister = std::make_shared<SlateRegister>();

		/**
		* @brief Order is important.
		*/
		m_SlateRegister->Register<ImguiMainMenu>      (true, "MainMenu");
		m_SlateRegister->Register<ImguiViewport>      (true, "ViewPort");
		m_SlateRegister->Register<ImguiConsole>       (true, "Console"  , Console::Register("Console"));
		m_SlateRegister->Register<ImguiContentBrowser>(true, "Content" );
		m_SlateRegister->Register<ImguiProperty>      (true, "Property");
		m_SlateRegister->Register<ImguiStage>         (true, "Stage"   );
		m_SlateRegister->Register<ImguiOperation>     (true, "Operation");
		m_SlateRegister->Register<ImguiInfoBar>       (true, "InfoBar" );
	}

	void SlateSystem::OnSystemShutDown()
	{

	}

	void SlateSystem::OnSystemUpdate(TimeStep& ts)
	{
	}

	void SlateSystem::OnEvent(Event& event)
	{
		SPICES_PROFILE_ZONE;

		m_SlateRegister->OnEvent(event);
	}
}