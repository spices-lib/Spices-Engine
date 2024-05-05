#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spiecs {

	class ImguiViewport;

	class WorldPickIDQueryer : public NativeScript
	{
	public:
		WorldPickIDQueryer() {};
		virtual ~WorldPickIDQueryer() {};

		virtual void OnTick(TimeStep& ts);
		virtual void OnEvent(Event& e) {};

	private:
		uint32_t m_WorldPickID;
		std::shared_ptr<ImguiViewport> m_ViewPort;
	};
}