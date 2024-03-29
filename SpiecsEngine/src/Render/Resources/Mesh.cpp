#include "pchheader.h"
#include "Mesh.h"

namespace Spiecs {

	Mesh::Mesh(MeshPack meshPack)
		: m_Pack(meshPack)
	{}

	Mesh::Mesh(const std::string& filePath)
	{
		m_Pack = FilePack(filePath);
	}

	void SquarePack::OnCreatePack()
	{
		m_Vertices =
		{
			{ {-1.0f,  1.0f, 0.0f}, {0.0f,  0.0f, 1.0f},{1.0f,  0.0f, 0.0f}, { 0.0f, 0.0f }},
			{ { 1.0f,  1.0f, 0.0f}, {0.0f,  0.0f, 1.0f},{0.0f,  1.0f, 0.0f}, { 1.0f, 0.0f }},
			{ { 1.0f, -1.0f, 0.0f}, {0.0f,  0.0f, 1.0f},{0.0f,  0.0f, 1.0f}, { 1.0f, 1.0f }},
			{ {-1.0f, -1.0f, 0.0f}, {0.0f,  0.0f, 1.0f},{1.0f,  1.0f, 1.0f}, { 0.0f, 1.0f }}
		};

		m_Indices = { 0, 1, 2, 2, 3, 0 };
	}

	CustomPack::CustomPack(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices)
	{
		m_Vertices = inVertices;
		m_Indices = inIndices;
	}

	void FilePack::OnCreatePack()
	{
		// TODO:
	}

}