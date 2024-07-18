/**
* @file Camera.cpp.
* @brief The Camera Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Camera.h"
#include "Core/Math/Math.h"
#include "Render/FrameInfo.h"

namespace Spices {

	void Camera::SetPerspective(float fov, float nearPlane, float farPlane, float aspectRatio)
	{
		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveParam.fov         = fov;
		m_PerspectiveParam.nearPlane   = nearPlane;
		m_PerspectiveParam.farPlane    = farPlane;
		m_PerspectiveParam.aspectRatio = aspectRatio;
	}

	void Camera::SetPerspective(float aspectRatio)
	{
		m_ProjectionType = ProjectionType::Perspective;
		
		SetPerspective(m_PerspectiveParam.fov, m_PerspectiveParam.nearPlane, m_PerspectiveParam.farPlane, aspectRatio);
	}

	void Camera::SetOrthographic(float left, float right, float top, float bottom, float nearPlane, float farPlane)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthographicParam.left        = left;
		m_OrthographicParam.right       = right;
		m_OrthographicParam.top         = top;
		m_OrthographicParam.bottom      = bottom;
		m_OrthographicParam.nearPlane   = nearPlane;
		m_OrthographicParam.farPlane    = farPlane;
	}

	void Camera::ResetStableFrames()
	{
		if (FrameInfo::Get().m_RendererType == RendererType::Rasterization) return;
		m_StableFrames = 0;
	}

	void Camera::CalculatePMatrix()
	{
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			m_ProjectionMatrix = PerspectiveMatrix(
				m_PerspectiveParam.fov        , 
				m_PerspectiveParam.nearPlane  ,
				m_PerspectiveParam.farPlane   ,
				m_PerspectiveParam.aspectRatio
			);
			break;
		case ProjectionType::Orthographic:
			m_ProjectionMatrix = OtrhographicMatrix(
				m_OrthographicParam.left      ,
				m_OrthographicParam.right     ,
				m_OrthographicParam.top       ,
				m_OrthographicParam.bottom    ,
				m_OrthographicParam.nearPlane ,
				m_OrthographicParam.farPlane
			);
			break;
		}
	}
}
