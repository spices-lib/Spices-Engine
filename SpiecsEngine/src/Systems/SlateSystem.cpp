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

		m_SlateRegister->Register<ImguiMainMenu>(true, "MainMenu");
		m_SlateRegister->Register<ImguiViewport>(true, "Viewport");
		m_SlateRegister->Register<ImguiConsole>(true, "Console", Console::Register("Console"));
		m_SlateRegister->Register<ImguiContentBrowser>(true, "Content");
		m_SlateRegister->Register<ImguiProperty>(true, "Property");
		m_SlateRegister->Register<ImguiStage>(true, "Stage");
		m_SlateRegister->Register<ImguiFloattingInfo>(true, "FloattingInfo");
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