#include "pchheader.h"
#include "Mesh.h"

namespace Spiecs {

	Mesh::Mesh(std::unordered_map<uint32_t, std::shared_ptr<MeshPack>> meshPack)
		: m_Pack(meshPack)
	{}

	void Mesh::Draw(VkCommandBuffer& commandBuffer)
	{
		for (auto& pair : m_Pack)
		{
			pair.second->OnBind(commandBuffer);
			pair.second->OnDraw(commandBuffer);
		}
	}

	Mesh::Builder& Mesh::Builder::AddPack(std::shared_ptr<MeshPack> meshPack)
	{
		meshPack->OnCreatePack();

		m_Pack[m_PackNums] = meshPack;
		return *this;
	}

	std::shared_ptr<Mesh> Mesh::Builder::Build() const
	{
		return  std::make_shared<Mesh>(m_Pack);
	}

}