#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spiecs {

	class ImguiViewport;

	class WorldPickIDQueryer : public NativeScript
	{
	public:
		WorldPickIDQueryer();
		virtual ~WorldPickIDQueryer() {};

		virtual void OnTick(TimeStep& ts);
		virtual void OnEvent(Event& e) {};

	private:
		float m_WorldPickID[4];
		float* m_WorldPickIDMemory;
		std::shared_ptr<ImguiViewport> m_ViewPort;
	};
}