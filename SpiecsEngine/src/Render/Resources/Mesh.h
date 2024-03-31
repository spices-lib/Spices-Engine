#pragma once
#include "Core/Core.h"
#include "Vertex.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Spiecs {

	class MeshPack
	{
	public:
		MeshPack() {};
		virtual ~MeshPack() {};

		virtual void OnCreatePack() {};

		const std::vector<Vertex>& GetVertices() { return m_Vertices; };
		const std::vector<uint32_t>& GetIndices() { return m_Indices; };
	protected:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};

	// TODO: Move to ECS
	class TransformComponent
	{
	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetModelMatrix() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		};
	};

	class SquarePack : public MeshPack
	{
	public:
		SquarePack() : MeshPack() {};

		virtual void OnCreatePack() override;
	};

	class CustomPack : public MeshPack
	{
	public:
		CustomPack(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices);

		virtual void OnCreatePack() override {};
	};

	class FilePack : public MeshPack
	{
	public:
		FilePack(const std::string& filePath) : MeshPack(), m_Path(filePath) {};

		virtual void OnCreatePack() override;
	private:
		std::string m_Path;
	};



	class Mesh
	{
	public:
		Mesh(MeshPack meshPack);
		Mesh(const std::string& filePath);
		virtual ~Mesh() {};

		inline MeshPack& GetPack() { return m_Pack; };
		void Bind();

	private:
		MeshPack m_Pack;
	};

	
}