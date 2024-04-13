#include "Pchheader.h"
#include "CameraController.h"
#include "World/Entity.h"
#include "Render/FrameInfo.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Input.h"

namespace Spiecs {

	void CameraController::OnConstruction()
	{
		Entity entity( m_Owner, FrameInfo::Get().m_World.get(), "" );
		m_CameraTranComp = &entity.GetComponent<TransformComponent>();
		m_Camera = &entity.GetComponent<CameraComponent>().GetCamera();
	}

	void CameraController::OnTick(TimeStep& ts)
	{
		if (Input::IsKeyPressed(Key::W))
		{
			std::any_cast<TransformComponent*>(m_CameraTranComp) ->AddPostion({ 0, 0, 1 * ts.ft() });
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			std::any_cast<TransformComponent*>(m_CameraTranComp)->AddPostion({ 0, 0, -1 * ts.ft() });
		}
		else if (Input::IsKeyPressed(Key::A))
		{
			std::any_cast<TransformComponent*>(m_CameraTranComp)->AddPostion({-1 * ts.ft(), 0, 0 });
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			std::any_cast<TransformComponent*>(m_CameraTranComp)->AddPostion({ 1 * ts.ft(), 0, 0 });
		}
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(CameraController::OnKeyPressed));
	}

	bool CameraController::OnKeyPressed(KeyPressedEvent& e)
	{
		return true;
	}
}