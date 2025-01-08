/**
* @file Mesh.cpp.
* @brief The Mesh Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Mesh.h"

namespace Spices {

	Mesh::Mesh(std::shared_ptr<scl::linked_unordered_map<uint32_t, std::shared_ptr<MeshPack>>> meshPack)
		: m_Pack(meshPack)
	{}

	void Mesh::AddMaterialToHitGroup(std::unordered_map<std::string, uint32_t>& hitGroup)
	{
		SPICES_PROFILE_ZONE;

		m_Pack->for_each([&](const uint32_t& k, const std::shared_ptr<MeshPack>& v) {

			auto& stages = v->GetMaterial()->GetShaderPath("rchit");
			if (stages.empty())
			{
				std::stringstream ss;
				ss << "Material: " << v->GetMaterial()->GetName() << " do not has valid rchit shader.";

				SPICES_CORE_ERROR(ss.str())
			}

			if (hitGroup.find(stages[0]) == hitGroup.end())
			{
				hitGroup[stages[0]] = static_cast<uint32_t>(hitGroup.size());

				v->SetHitShaderHandle(hitGroup[stages[0]]);
			}
			else
			{
				v->SetHitShaderHandle(hitGroup[stages[0]]);
			}

			return false;
		});
	}

	Mesh::Builder::Builder()
	{
		SPICES_PROFILE_ZONE;

		m_Pack = std::make_shared<scl::linked_unordered_map<uint32_t, std::shared_ptr<MeshPack>>>();
	}

	Mesh::Builder& Mesh::Builder::AddPack(std::shared_ptr<MeshPack> meshPack)
	{
		SPICES_PROFILE_ZONE;

		meshPack->OnCreatePack();

		m_Pack->push_back(m_PackNums, meshPack);
		m_PackNums++;
		return *this;
	}

	std::shared_ptr<Mesh> Mesh::Builder::Build() const
	{
		SPICES_PROFILE_ZONE;

		return std::make_shared<Mesh>(m_Pack);
	}

}