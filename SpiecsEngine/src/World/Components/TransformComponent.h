#pragma once
#include "Core/Core.h"
#include "Component.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Spiecs {

	class TransformComponent : public Component
	{
	public:
		TransformComponent() {};
		virtual ~TransformComponent() {};

		virtual void OnSerialize();
		virtual void OnDeSerialize();

		void SetPostion(glm::vec3 position) { m_Position = position; CalMatrix(); };
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; CalMatrix();  };
		void SetScale(glm::vec3 scale) { m_Scale = scale; CalMatrix(); };

		void AddPostion(glm::vec3 position) { m_Position += position; CalMatrix(); };
		void AddRotation(glm::vec3 rotation) { m_Rotation += rotation; CalMatrix(); };
		void AddScale(glm::vec3 scale) { m_Scale += scale; CalMatrix(); };

		glm::mat4& GetModelMatrix() { return m_ModelMatrix; };

		glm::vec3& GetPosition() { return m_Position; };
		glm::vec3& GetRotation() { return m_Rotation; };
		glm::vec3& GetScale() { return m_Scale; };

	private:
		void CalMatrix();
	private:
		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

		glm::vec3 m_Position { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale { 1.0f, 1.0f, 1.0f };
	};
}