#include "Pchheader.h"
#include "WorldPickIDQueryer.h"
#include "Systems/SlateSystem.h"
#include "Slate/Imgui/ViewPort/ImguiViewport.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Input/MouseButtonCodes.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "World/Entity.h"
#include "World/World/World.h"

namespace Spices {

	WorldPickIDQueryer::WorldPickIDQueryer()
	{
		m_WorldPickIDMemory = &m_WorldPickID[0];
	}

	void WorldPickIDQueryer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(WorldPickIDQueryer::OnMouseButtonPressed));
	}

	bool WorldPickIDQueryer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		/**
		* @brief The first frame, we will not get register pointer.
		*/
		if (!SlateSystem::GetRegister()) return false;

		/**
		* @brief Get viewport.
		*/
		if (!m_ViewPort) m_ViewPort = SlateSystem::GetRegister()->GetViewPort();

		if (!m_ViewPort->IsHovered() || m_ViewPort->GetGizmo()->IsOver()) return false;

		if (m_ViewPort->GetMousePosInViewport().first < 0                              ||
			m_ViewPort->GetMousePosInViewport().first > m_ViewPort->GetPanelSize().x   ||
			m_ViewPort->GetMousePosInViewport().second < 0                             ||
			m_ViewPort->GetMousePosInViewport().second > m_ViewPort->GetPanelSize().y
			)
		{
			return false;
		}

		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			/**
			* @brief Add pick.
			*/
			if (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift))
			{
				auto pair = m_ViewPort->GetMousePosInViewport();

				VulkanRenderBackend::GetRendererResourcePool()
				->AccessRowResource("ID")
				->CopyImageTexelToBuffer(
					pair.first, 
					pair.second, 
					reinterpret_cast<void*>(&m_WorldPickID[0])
				);

				Entity entity((entt::entity)m_WorldPickID[0], FrameInfo::Get().m_World.get());
				std::string entityName = *entity.GetComponent<TagComponent>().GetTag().begin();
				
				FrameInfo::Get().m_PickEntityID.push_back(static_cast<int>(m_WorldPickID[0]), entityName);
				
				std::stringstream ss;
				ss << "Select entity: " << entityName;

				SPICES_CORE_TRACE(ss.str());
			}

			/**
			* @breif Sub pick.
			*/
			else if (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl))
			{
				auto pair = m_ViewPort->GetMousePosInViewport();

				VulkanRenderBackend::GetRendererResourcePool()
				->AccessRowResource("ID")
				->CopyImageTexelToBuffer(
					pair.first, 
					pair.second, 
					reinterpret_cast<void*>(&m_WorldPickID[0])
				);

				std::string* ptr = FrameInfo::Get().m_PickEntityID.find_value(static_cast<int>(m_WorldPickID[0]));
				if (ptr)
				{
					std::stringstream ss;
					ss << "Deselect entity: " << *ptr;

					SPICES_CORE_TRACE(ss.str());

					FrameInfo::Get().m_PickEntityID.erase(static_cast<int>(m_WorldPickID[0]));
				}
			}

			/**
			* @breif Single Select.
			*/
			else
			{
				FrameInfo::Get().m_PickEntityID.clear();

				auto pair = m_ViewPort->GetMousePosInViewport();

				VulkanRenderBackend::GetRendererResourcePool()
				->AccessRowResource("ID")
				->CopyImageTexelToBuffer(
					pair.first, 
					pair.second, 
					reinterpret_cast<void*>(&m_WorldPickID[0])
				);

				Entity entity((entt::entity)m_WorldPickID[0], FrameInfo::Get().m_World.get());
				std::string entityName = *entity.GetComponent<TagComponent>().GetTag().begin();

				FrameInfo::Get().m_PickEntityID.push_back(static_cast<int>(m_WorldPickID[0]), entityName);

				std::stringstream ss;
				ss << "Select entity: " << entityName;

				SPICES_CORE_TRACE(ss.str());
			}
		}

		/**
		* @breif Cancle Select.
		*/
		else if (e.GetMouseButton() == Mouse::ButtonRight)
		{
			FrameInfo::Get().m_PickEntityID.clear();

			SPICES_CORE_TRACE("Cancel all selected entity");
		}

		return false;
	}
}