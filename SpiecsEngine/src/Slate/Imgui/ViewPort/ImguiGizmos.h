#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	class ImguiGizmos : public ImguiSlate
	{
	public:
		ImguiGizmos(
			const std::string& panelName,
			FrameInfo&         frameInfo,
			ImguiSlate*        owner
		) 
			: ImguiSlate(panelName, frameInfo) 
			, m_Owner(owner)
		{};

		virtual ~ImguiGizmos() {};

		virtual void OnUpdate(TimeStep& ts) override {};
		virtual void OnRender() override;
		virtual void OnEvent(Event& event) override;

	public:
		bool IsOver() { return ImGuizmo::IsOver(); };

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		ImguiSlate* m_Owner;

		int m_GizmoType = -1;
		bool bEnableGizmo = false;
	};
}