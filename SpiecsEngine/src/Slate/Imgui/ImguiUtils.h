#pragma once
#include "Core/Core.h"
#include "Core/TimeStep.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

namespace Spiecs{

	class ImguiSlate
	{
	public:
		ImguiSlate() {};
		virtual ~ImguiSlate() {};

		virtual void OnUpdate(TimeStep& ts) = 0;
		virtual void OnRender();
		virtual void OnEvent(Event& event) = 0;

	protected:
		ImVec2 m_PanelSize;
	};
}