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
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Iter hash map.
		*/
		for (auto& pair : m_SlatesRenderContainer)
		{
			pair.second->OnUpdate(ts);
		}
	}

	void SlateRegister::OnRender()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Iter hash map.
		*/
		for (auto& pair : m_SlatesRenderContainer)
		{
			pair.second->OnRender();
		}
	}

	void SlateRegister::OnEvent(Event& event)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Iter hash map.
		*/
		for (auto& pair : m_SlatesEventContainer)
		{
			pair.second->OnEvent(event);
		}
	}

	std::shared_ptr<ImguiViewport> SlateRegister::GetViewPort()
	{
		SPIECS_PROFILE_ZONE;

		return std::static_pointer_cast<ImguiViewport>(m_SlatesRenderContainer["ViewPort"]);
	}
}