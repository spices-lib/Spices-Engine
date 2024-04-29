/**
* @file SlateSystem.cpp.
* @brief The SlateSystem Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SlateSystem.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"
#include "Core/Event/Event.h"
#include "Core/Log/Console.h"

namespace Spiecs {

	std::shared_ptr<SlateRegister> SlateSystem::m_SlateRegister;

	void SlateSystem::OnSystemInitialize()
	{
		m_SlateRegister = std::make_shared<SlateRegister>();

		m_SlateRegister->Register<ImguiMainMenu>("MainMenu");
		m_SlateRegister->Register<ImguiViewport>("Viewport");
		m_SlateRegister->Register<ImguiConsole>("Console", Console::Register("Console"));
		m_SlateRegister->Register<ImguiContentBrowser>("Content");
		m_SlateRegister->Register<ImguiProperty>("Property");
		m_SlateRegister->Register<ImguiStage>("Stage");
	}

	void SlateSystem::OnSystemShutDown()
	{

	}

	void SlateSystem::OnSystemUpdate(TimeStep& ts)
	{
	}

	void SlateSystem::OnEvent(Event& event)
	{
		m_SlateRegister->OnEvent(event);
	}
}