#pragma once
#include "Core/Core.h"
#include "chrono"

namespace Spiecs {

	class TimeStep
	{
	public:
		TimeStep();
		virtual ~TimeStep() {};

		float ft() { return m_FrameTime; };
		float gt() { return m_GameTime; };

		void Flush();

	public:
		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_LastTime;

		float m_FrameTime = 0;
		float m_GameTime = 0;
	};
}