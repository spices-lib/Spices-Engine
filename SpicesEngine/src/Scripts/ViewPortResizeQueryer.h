#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spices {

	class ImguiViewport;

	class ViewPortResizeQueryer : public NativeScript
	{
	public:
		ViewPortResizeQueryer() {};
		virtual ~ViewPortResizeQueryer() {};

		virtual void OnTick(TimeStep& ts);
		virtual void OnEvent(Event& e) {};

	private:
		std::shared_ptr<ImguiViewport> m_ViewPort;
	};

}