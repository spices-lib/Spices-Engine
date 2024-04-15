#pragma once
#include "Core/Core.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Spiecs {

	enum ProjectionType
	{
		Perspective = 0,
		Orthographic = 1,
	};

	class Camera
	{
	public:
		Camera() {};
		virtual ~Camera() {};

		void SetPerspective(float fov, float nearPlane, float farPlane, float aspectRatio);

		void SetPerspective(float aspectRatio);


		void SetOrthographic(float left, float right, float top, float bottom, float nearPlane, float farPlane);
		glm::mat4 GetPMatrix() { return m_ProjectionMatrix; };

	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

		ProjectionType m_ProjectionType = Perspective;

		float m_FOV = glm::radians(45.0f);
		float m_NearPlane = 0.01f;
		float m_FarPlane = 1000.0f;

		float m_AspectRatio = 1.777f;
	};
}