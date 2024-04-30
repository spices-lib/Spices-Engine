/**
* @file SlateRegister.cpp.
* @brief The SlateRegister Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SlateRegister.h"

namespace Spiecs {

	void SlateRegister::OnUpdate(TimeStep& ts)
	{
		for (auto& pair : m_SlatesRenderContainer)
		{
			pair.second->OnUpdate(ts);
		}
	}

	void SlateRegister::OnRender()
	{
		for (auto& pair : m_SlatesRenderContainer)
		{
			pair.second->OnRender();
		}
	}

	void SlateRegister::OnEvent(Event& event)
	{
		for (auto& pair : m_SlatesEventContainer)
		{
			pair.second->OnEvent(event);
		}
	}

	std::shared_ptr<ImguiViewport> SlateRegister::GetViewPort()
	{
		return std::static_pointer_cast<ImguiViewport>(m_SlatesRenderContainer["ViewPort"]);
	}
}