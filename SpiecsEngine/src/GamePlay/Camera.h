/**
* @file Camera.h.
* @brief The Camera Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Spiecs {

	/**
	* @brief This Struct defines camera projection type.
	*/
	enum ProjectionType
	{
		/**
		* @brief perspective
		*/
		Perspective = 0,

		/**
		* @brief orthographic
		*/
		Orthographic = 1,
	};

	struct PerspectiveParam
	{
		float fov = glm::radians(45.0f);
		float nearPlane = 0.01f;
		float farPlane = 1000.0f;
		float aspectRatio = 1.777f;
	};

	struct OrthographicParam
	{
		float left;
		float right;
		float top;
		float bottom;
		float nearPlane = 0.01f;
		float farPlane = 1000.0f;
	};
	
	/**
	* @brief Camera Class.
	* This class just encapsulate Projection Matrix.
	* @todo Implemented projection type switch.
	*/
	class Camera
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Camera() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~Camera() = default;

		/**
		* @brief Set ProjectionMatrix by using perspective type.
		* @param[in] fov Field of View, usually 45.0 degree.
		* @param[in] nearPlane NearPlane, usually 0.01.
		* @param[in] farPlane FarPlane, usually infinity far.
		* @param[in] aspectRatio AspectRatio, viewport width / viewport height.
		*/
		void SetPerspective(float fov, float nearPlane, float farPlane, float aspectRatio);

		/**
		* @brief Set ProjectionMatrix by using perspective type with one param.
		* @param[in] aspectRatio AspectRatio, viewport width / viewport height.
		*/
		void SetPerspective(float aspectRatio);

		/**
		* @brief Set ProjectionMatrix by using orthographic type.
		* @param[in] left Orthographic Box Left size.
		* @param[in] right Orthographic Box right size.
		* @param[in] top Orthographic Box top size.
		* @param[in] bottom Orthographic Box bottom size.
		* @param[in] nearPlane Orthographic Box nearPlane size.
		* @param[in] farPlane Orthographic Box farPlane size.
		*/
		void SetOrthographic(float left, float right, float top, float bottom, float nearPlane, float farPlane);

		/**
		* @brief Reset m_StableFrames to 0.
		*/
		void ResetStableFrames() { m_StableFrames = 0; }

		/**
		* @brief InCreate 1 to m_StableFrames per frame.
		*/
		void IncreaseStableFrames() { m_StableFrames++; }

		/**
		* @brief Get camera projection matrix.
		* @return Returns the camera projection matrix.
		*/
		glm::mat4 GetPMatrix() const { return m_ProjectionMatrix; }

		/**
		* @brief Get camera projection type.
		* @return Returns the camera projection type.
		*/
		const ProjectionType& GetProjectionType() const { return m_ProjectionType; }

		/**
		* @brief Get camera AspectRatio.
		* @return Returns the camera AspectRatio.
		*/
		float GetAspectRatio() const { return m_PerspectiveParam.aspectRatio; }

		/**
		* @brief Get camera StableFrames.
		* @return Return the camera StableFrames.
		*/
		unsigned int GetStableFrames() const { return m_StableFrames; }

		PerspectiveParam& GetPerspectiveParam() { return m_PerspectiveParam; }

		OrthographicParam& GOrthographicParam() { return m_OrthographicParam; }

	private:

		/**
		* @brief ProjectionMatrix.
		* Init with identity Matrix.
		*/
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

		/**
		* @breif ProjectionType.
		* Init with Perspective.
		*/
		ProjectionType m_ProjectionType = Perspective;

		/**
		* @brief Camera Stable Frames Number.
		*/
		unsigned int m_StableFrames = 0;

		/**
		* @brief Camera PerspectiveParam.
		*/
		PerspectiveParam m_PerspectiveParam;

		/**
		* @brief Camera OrthographicParam.
		*/
		OrthographicParam m_OrthographicParam;
	};
}