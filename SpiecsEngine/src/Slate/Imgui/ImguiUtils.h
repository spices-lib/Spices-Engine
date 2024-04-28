#pragma once
#include "Core/Core.h"
#include "Core/TimeStep.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"
#include "ImguiHelper.h"

namespace Spiecs{

	class ImguiSlate
	{
	public:
		enum class Side
		{
			Left,
			Right,
			Up,
			Down,
			Scene,
		};

	public:
		ImguiSlate(const std::string& panelName) : m_PanelName(panelName){};
		virtual ~ImguiSlate() {};

		virtual void OnUpdate(TimeStep& ts) = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event& event) = 0;

	protected:

		void BeginMainDock(Side side = Side::Scene, float alpha = 1.0f);
		void Begin(float alpha = 0.5f);
		void End() { ImGui::End(); ImGui::PopStyleVar(3); };

	protected:
		ImVec2 m_PanelSize;
		std::string m_PanelName;
		bool m_IsFocused = false;
		bool m_IsHovered = false;

		static ImGuiID dockspaceID;
	};
}