/**
* @file Camera.cpp.
* @brief The Camera Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Camera.h"
#include "Core/Math/Math.h"
#include "Render/FrameInfo.h"
#include "Slate/SlateInfoBar.h"

namespace Spices {

	void Camera::SetPerspective(float fov, float nearPlane, float aspectRatio)
	{
		SPICES_PROFILE_ZONE;
		
		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveParam.fov         = fov;
		m_PerspectiveParam.nearPlane   = nearPlane;
		m_PerspectiveParam.aspectRatio = aspectRatio;
	}

	void Camera::SetPerspective(float aspectRatio)
	{
		SPICES_PROFILE_ZONE;
		
		m_ProjectionType = ProjectionType::Perspective;
		
		SetPerspective(m_PerspectiveParam.fov, m_PerspectiveParam.nearPlane, aspectRatio);
	}

	void Camera::SetOrthographic(float left, float right, float top, float bottom, float nearPlane, float farPlane)
	{
		SPICES_PROFILE_ZONE;
		
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
		SPICES_PROFILE_ZONE;
		
		if (FrameInfo::Get().m_RendererType == RendererType::Rasterization) return;
		
		if (m_StableFrames > 100)
		{
			SlateInfoBar::Create<float>("Refrush frame", [=]() -> float { 
				return static_cast<float>(this->m_StableFrames) / 100.0f; 
			}, [=](SlateInfoBar* that) {
				return std::any_cast<float>(that->GetRate()) >= 1.0f;
			});
		}

		m_StableFrames = 0;
	}

	const glm::mat4 Camera::GetPMatrix() const
	{
		SPICES_PROFILE_ZONE;
		
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			return PerspectiveMatrix(
				m_PerspectiveParam.fov        , 
				m_PerspectiveParam.nearPlane  ,
				100000000.0f,
				m_PerspectiveParam.aspectRatio
			);
		case ProjectionType::Orthographic:
			return OtrhographicMatrix(
				m_OrthographicParam.left      ,
				m_OrthographicParam.right     ,
				m_OrthographicParam.top       ,
				m_OrthographicParam.bottom    ,
				m_OrthographicParam.nearPlane ,
				m_OrthographicParam.farPlane
			);
		}

		return glm::mat4(1.0f);
	}

	void Camera::CalculatePMatrixReverseZ()
	{
		SPICES_PROFILE_ZONE;
		
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			m_ProjectionMatrix = PerspectiveMatrixInverseZ(
				m_PerspectiveParam.fov        , 
				m_PerspectiveParam.nearPlane  ,
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
