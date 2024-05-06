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

namespace Spiecs {

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

		if (!m_ViewPort) m_ViewPort = SlateSystem::GetRegister()->GetViewPort();

		if (m_ViewPort->IsHovered() && e.GetMouseButton() == Mouse::ButtonLeft)
		{
			/**
			* @breif Single Select.
			*/
			FrameInfo::Get().m_SelectFrontEntityID.clear();

			auto pair = m_ViewPort->GetMousePosInViewport();

			VulkanRenderBackend::GetRendererResourcePool()->AccessRowResource("ID")->CopyImageTexelToBuffer(pair.first, pair.second, (void*)&m_WorldPickID[0]);	
			FrameInfo::Get().m_SelectFrontEntityID.push_back(static_cast<int>(m_WorldPickID[0]));

			Entity entity((entt::entity)m_WorldPickID[0], FrameInfo::Get().m_World.get());
			std::string entityName = entity.GetComponent<TagComponent>().GetTag()[0];

			std::stringstream ss;
			ss << "Select entity: " << entityName;

			SPIECS_CORE_TRACE(ss.str());
		}

		/**
		* @breif Cancle Select.
		*/
		if (e.GetMouseButton() == Mouse::ButtonRight)
		{
			FrameInfo::Get().m_SelectFrontEntityID.clear();

			SPIECS_CORE_TRACE("Cancel all selected entity");
		}

		return false;
	}
}