/**
* @file TransformComponent.h.
* @brief The TransformComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Spiecs {

	/**
	* @brief TransformComponent Class.
	* This class defines the specific behaver of TransformComponent.
	*/
	class TransformComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		TransformComponent() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~TransformComponent() {};

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize();

		/**
		* @brief This interface defines how to dserialize.
		* @todo Finsih it.
		*/
		virtual void OnDeSerialize();

		/**
		* @brief This interface defines how to draw this component to property panel.
		*/
		virtual void DrawThis();

		/**
		* @brief Set the position this component handled.
		* Call CalMatrix() during this API.
		* @param[in] position The entity's world position.
		*/
		void SetPostion(const glm::vec3& position) { m_Position = position; CalMatrix(); };

		/**
		* @brief Set the rotation this component handled.
		* Call CalMatrix() during this API.
		* @param[in] rotation The entity's world rotation.
		*/
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; CalMatrix(); };

		/**
		* @brief Set the scale this component handled.
		* Call CalMatrix() during this API.
		* @param[in] scale The entity's world scale.
		*/
		void SetScale(const glm::vec3& scale) { m_Scale = scale; CalMatrix(); };

		/**
		* @brief Add the position to this component handled.
		* Call CalMatrix() during this API.
		* @param[in] position The entity's world position.
		*/
		void AddPostion(const glm::vec3& position) { m_Position += position; CalMatrix(); };

		/**
		* @brief Add the rotation to this component handled.
		* Call CalMatrix() during this API.
		* @param[in] rotation The entity's world rotation.
		*/
		void AddRotation(const glm::vec3& rotation) { m_Rotation += rotation; CalMatrix(); };

		/**
		* @brief Add the scale to this component handled.
		* Call CalMatrix() during this API.
		* @param[in] scale The entity's world scale.
		*/
		void AddScale(const glm::vec3& scale) { m_Scale += scale; CalMatrix(); };

		/**
		* @brief Get the modelMatrix variable.
		* @return Returns the modelMatrix variable.
		*/
		inline const glm::mat4& GetModelMatrix() { CalMatrix(); return m_ModelMatrix; };

		/**
		* @brief Get the position variable.
		* @return Returns the position variable.
		*/
		inline const glm::vec3& GetPosition() { return m_Position; };

		/**
		* @brief Get the rotation variable.
		* @return Returns the rotation variable.
		*/
		inline const glm::vec3& GetRotation() { return m_Rotation; };

		/**
		* @brief Get the scale variable.
		* @return Returns the scale variable.
		*/
		inline const glm::vec3& GetScale() { return m_Scale; };

	private:

		/**
		* @brief Calculate Model Matrix.
		*/
		void CalMatrix();

	private:

		/**
		* @brief The modelMatrix this component handled.
		*/
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

		/**
		* @brief The position this component handled.
		*/
		glm::vec3 m_Position { 0.0f, 0.0f, 0.0f };

		/**
		* @brief The rotation this component handled.
		*/
		glm::vec3 m_Rotation { 0.0f, 0.0f, 0.0f };

		/**
		* @brief The scale this component handled.
		*/
		glm::vec3 m_Scale { 1.0f, 1.0f, 1.0f };
	};
}