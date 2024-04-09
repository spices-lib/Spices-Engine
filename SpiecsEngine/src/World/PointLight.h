#pragma once
#include "Core/Core.h"
#include <glm/glm.hpp>

namespace Spiecs {

	class PointLight 
	{
	public:
		PointLight() {};
		virtual ~PointLight() {};

	private:
		glm::vec3 m_Color{ 1.0f, 1.0f, 1.0f };
		float m_Intensity = 1.0;
		float m_Constantf = 1.0;
		float m_Linear = 0.35;
		float m_Quadratic = 0.44;
	};
}