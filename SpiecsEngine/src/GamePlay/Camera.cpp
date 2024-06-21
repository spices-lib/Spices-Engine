/**
* @file Camera.cpp.
* @brief The Camera Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Camera.h"
#include "Core/Math/Math.h"

namespace Spiecs {

	void Camera::SetPerspective(float fov, float nearPlane, float farPlane, float aspectRatio)
	{
		m_ProjectionType = ProjectionType::Perspective;

		m_FOV = fov;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		m_AspectRatio = aspectRatio;

		const float tanHalfFovy = tan(fov / 2.0f);

		m_ProjectionMatrix = glm::mat4{ 0.0f };
		m_ProjectionMatrix[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
		m_ProjectionMatrix[1][1] = 1.0f / (tanHalfFovy);
		m_ProjectionMatrix[2][2] = farPlane / (farPlane - nearPlane);
		m_ProjectionMatrix[2][3] = 1.0f;
		m_ProjectionMatrix[3][2] = -(farPlane * nearPlane) / (farPlane - nearPlane);
	}

	void Camera::SetPerspective(float aspectRatio)
	{
		SetPerspective(m_FOV, m_NearPlane, m_FarPlane, aspectRatio);
	}

	void Camera::SetOrthographic(float left, float right, float top, float bottom, float nearPlane, float farPlane)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_ProjectionMatrix = Otrhographic(left, right, top, bottom, nearPlane, farPlane);
	}

}