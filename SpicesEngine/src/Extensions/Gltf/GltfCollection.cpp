#include "Pchheader.h"
#include "GltfCollection.h"
#include "Resources/Loader/GltfLoader.h"
#include "World/Entity.h"
#include "World/World/World.h"
#include "World/Components/MeshComponent.h"
#include "Resources/Mesh/GltfPack.h"
#include "Core/Math/Math.h"

namespace Spices {

	GltfCollection::GltfCollection(const std::string& path)
	{
		SPICES_PROFILE_ZONE;

		GltfLoader::Load(path, this);
	}

	void GltfCollection::CreateEntity(World* world, const std::string& tag)
	{
		SPICES_PROFILE_ZONE;

		for (auto& item : m_Scenes->m_ScenesData)
		{
			for (auto& node : item.nodes)
			{
				CreateEntityRecursive(world, tag, node, glm::mat4(1.0f));
			}
		}
	}

	void GltfCollection::CreateEntityRecursive(World* world, const std::string& tag, uint32_t node, const glm::mat4& model)
	{
		/**
		* @brief Recursive in nodes.
		*/ 
		GltfNodes::Item& item = m_Nodes->m_NodesData[node];
		for (auto& n : item.children)
		{
			CreateEntityRecursive(world, tag, n, model * item.matrix);
		}

		/**
		* @todo Create Empty Entity here.
		*/
		if (item.mesh < -0.5f) return;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		DecomposeTransform(model * item.matrix, position, rotation, scale);

		Entity entity = world->CreateEntity(tag);
		TransformComponent& transformComp = entity.GetComponent<TransformComponent>();
		transformComp.SetPosition(position);
		transformComp.SetRotation(rotation);
		transformComp.SetScale(scale);

		MeshComponent& meshComp = entity.AddComponent<MeshComponent>();
		Mesh::Builder builder;

		for (int j = 0; j < m_Meshes->m_MeshesData[item.mesh].primitives.size(); j++)
		{
			std::stringstream ss;
			ss << m_Meshes->m_MeshesData[item.mesh].name << '_' << node;
			std::shared_ptr<GltfPack> pack = std::make_shared<GltfPack>(ss.str(), [&](GltfPack* pack)
			{
				GltfLoader::LoadPack(pack, m_Meshes->m_MeshesData[item.mesh].primitives[j], m_Accessors.get(), m_Buffers.get(), m_BufferViews.get());
			});

			pack->SetMaterial("BasePassRenderer.Mesh.ground");

			builder.AddPack(pack);
		}

		std::shared_ptr<Mesh> mesh = builder.Build();
		meshComp.SetMesh(mesh);
	}
}