#include "pchheader.h"
#include "Camera.h"

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

	void Camera::SetOrthographic(float left, float right, float top, float bottom, float nearPlane, float farPlane)
	{
		m_ProjectionMatrix = glm::mat4{ 1.0f };
		m_ProjectionMatrix[0][0] = 2.0f / (right - left);
		m_ProjectionMatrix[1][1] = 2.0f / (bottom - top);
		m_ProjectionMatrix[2][2] = 1.0f / (farPlane - nearPlane);
		m_ProjectionMatrix[3][0] = -(right + left) / (right - left);
		m_ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		m_ProjectionMatrix[3][2] = -nearPlane / (farPlane - nearPlane);
	}

}