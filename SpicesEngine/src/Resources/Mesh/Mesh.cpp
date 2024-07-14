/**
* @file Mesh.cpp.
* @brief The Mesh Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Mesh.h"

namespace Spices {

	Mesh::Mesh(std::unordered_map<uint32_t, std::shared_ptr<MeshPack>> meshPack)
		: m_Pack(meshPack)
	{}

#ifdef RENDERAPI_VULKAN

	std::vector<VulkanRayTracing::BlasInput> Mesh::CreateMeshPackASInput()
	{
		std::vector<VulkanRayTracing::BlasInput> allBlas;
		allBlas.reserve(m_Pack.size());

		for (auto& pair : m_Pack)
		{
			auto blas = pair.second->MeshPackToVkGeometryKHR();
			allBlas.emplace_back(blas);
		}

		return allBlas;
	}

	void Mesh::AddMaterialToHitGroup(std::unordered_map<std::string, uint32_t>& hitGroup)
	{
		for (auto& pair : m_Pack)
		{
			auto& stages = pair.second->GetMaterial()->GetShaderPath("rchit");
			if (stages.empty())
			{
				std::stringstream ss;
				ss << "Material: " << pair.second->GetMaterial()->GetName() << " do not has vaild rchit shader.";

				SPICES_CORE_ERROR(ss.str());
			}

			if (hitGroup.find(stages[0]) == hitGroup.end())
			{
				hitGroup[stages[0]] = static_cast<uint32_t>(hitGroup.size());

				pair.second->SetMaterialHandle(hitGroup[stages[0]]);
			}
			else
			{
				pair.second->SetMaterialHandle(hitGroup[stages[0]]);
			}
		}
	}

#endif

	Mesh::Builder& Mesh::Builder::AddPack(std::shared_ptr<MeshPack> meshPack)
	{
		meshPack->OnCreatePack();

		m_Pack[m_PackNums] = meshPack;
		m_PackNums++;
		return *this;
	}

	std::shared_ptr<Mesh> Mesh::Builder::Build() const
	{
		return std::make_shared<Mesh>(m_Pack);
	}

}