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

	void Mesh::Bind()
	{
		VkBuffer buffers[] = { vertexBuffer->getBuffer() };
		VkDeviceSize offests[] = { 0 };
		vkCmdBindVertexBuffers(commandbuffer, 0, 1, buffers, offests);

		if (hasIndexBuffer)
		{
			vkCmdBindIndexBuffer(commandbuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		}
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
		: MeshPack()
	{
		m_Vertices = inVertices;
		m_Indices = inIndices;
	}

	void FilePack::OnCreatePack()
	{
		// TODO:
	}

}