/**
* @file TransformComponent.h.
* @brief The TransformComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Spices {

	/**
	* @brief TransformComponent Class.
	* This class defines the specific behaves of TransformComponent.
	*/
	class TransformComponent : public Component
	{
	public:

		enum TransformComponentBits
		{
			Clean = 0,
			NeedUpdateTLAS = 1,
			MAX = 0x7FFFFFFF
		};

		typedef uint32_t TransformComponentFlags;

	public:

		/**
		* @brief Constructor Function.
		*/
		TransformComponent() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~TransformComponent() override = default;

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override;

		/**
		* @brief This interface defines how to deserialize.
		* @todo Finish it.
		*/
		virtual void OnDeSerialize() override;

		/**
		* @brief This interface defines how to draw this component to property panel.
		*/
		virtual void DrawThis() override;

		/**
		* @brief Set the position this component handled.
		* Call CalMatrix() during this API.
		* @param[in] position The entity's world position.
		*/
		void SetPosition(const glm::vec3& position) { m_Position = position; CalMatrix(); }

		/**
		* @brief Set the rotation this component handled.
		* Call CalMatrix() during this API.
		* @param[in] rotation The entity's world rotation.
		*/
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; CalMatrix(); }

		/**
		* @brief Set the scale this component handled.
		* Call CalMatrix() during this API.
		* @param[in] scale The entity's world scale.
		*/
		void SetScale(const glm::vec3& scale) { m_Scale = scale; CalMatrix(); }

		/**
		* @brief Add the position to this component handled.
		* Call CalMatrix() during this API.
		* @param[in] position The entity's world position.
		*/
		void AddPosition(const glm::vec3& position) { m_Position += position; CalMatrix(); }

		/**
		* @brief Add the rotation to this component handled.
		* Call CalMatrix() during this API.
		* @param[in] rotation The entity's world rotation.
		*/
		void AddRotation(const glm::vec3& rotation) { m_Rotation += rotation; CalMatrix(); }

		/**
		* @brief Add the scale to this component handled.
		* Call CalMatrix() during this API.
		* @param[in] scale The entity's world scale.
		*/
		void AddScale(const glm::vec3& scale) { m_Scale += scale; CalMatrix(); }

		/**
		* @brief Get the modelMatrix variable.
		* @return Returns the modelMatrix variable.
		*/
		const glm::mat4& GetModelMatrix() { CalMatrix(); return m_ModelMatrix; }

		/**
		* @brief Get Rotate Matrix.
		* @return Returns the Rotate Matrix.
		*/
		glm::mat4 GetRotateMatrix() const;
		
		/**
		* @brief Get the position variable.
		* @return Returns the position variable.
		*/
		const glm::vec3& GetPosition() const { return m_Position; }

		/**
		* @brief Get the rotation variable.
		* @return Returns the rotation variable.
		*/
		const glm::vec3& GetRotation() const { return m_Rotation; }

		/**
		* @brief Get the scale variable.
		* @return Returns the scale variable.
		*/
		const glm::vec3& GetScale() const { return m_Scale; }

		/**
		* @brief Get WorldMarkFlags this frame.
		* @return Returns the TransformComponentFlags.
		*/
		inline TransformComponentFlags GetMarker() const { return m_Marker; }

		/**
		* @brief Mark TransformComponentFlags with flags.
		* @param[in] flags In flags.
		*/
		void Mark(TransformComponentFlags flags) { m_Marker |= flags; }

		/**
		* @brief Clear TransformComponentFlags with flags.
		* @param[in] flags In flags.
		*/
		void ClearMarkerWithBits(TransformComponentFlags flags);

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

		/**
		* @brief World State this frame.
		*/
		TransformComponentFlags m_Marker = TransformComponentBits::Clean;
	};
}