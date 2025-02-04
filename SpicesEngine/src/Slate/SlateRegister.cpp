/**
* @file SlateRegister.cpp.
* @brief The SlateRegister Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SlateRegister.h"
#include "Core/Thread/ThreadPoolBasic.h"
#include "Core/Event/KeyEvent.h"

namespace Spices {

	void SlateRegister::OnUpdate(TimeStep& ts) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter hash map.
		*/
		for (auto& pair : m_SlatesEventContainer)
		{
			pair.second->OnUpdate(ts);
		}
	}

	void SlateRegister::OnRender() const
	{
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

		return std::static_pointer_cast<ImguiViewport>(m_SlatesRenderContainer["ViewPort"]);
	}

	std::shared_ptr<ImguiSlate> SlateRegister::GetSlate(const std::string& name)
	{
		SPICES_PROFILE_ZONE;

		if (m_SlatesEventContainer.find(name) != m_SlatesEventContainer.end())
		{
			return m_SlatesEventContainer[name];
		}

		return nullptr;
	}
}