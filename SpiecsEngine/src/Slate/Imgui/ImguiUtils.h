#pragma once
#include "Core/Core.h"
#include "Core/TimeStep.h"

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
		virtual void OnRender() = 0;

	protected:
	};
}