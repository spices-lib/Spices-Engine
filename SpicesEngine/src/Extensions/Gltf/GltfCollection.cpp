/**
* @file GltfCollection.cpp
* @brief The GltfCollection Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "GltfCollection.h"
#include "Resources/Loader/GltfLoader.h"
#include "World/Entity.h"
#include "World/World/World.h"
#include "World/Components/MeshComponent.h"
#include "World/Components/EntityComponent.h"
#include "Resources/Mesh/GltfPack.h"
#include "Core/Math/Math.h"
#include "Slate/SlateInfoBar.h"

namespace Spices {

	GltfCollection::GltfCollection(const std::string& path)
	{
		SPICES_PROFILE_ZONE;

		GltfLoader::Load(path, this);
	}

	void GltfCollection::CreateEntity(World* world, const std::string& tag, Transform transform)
	{
		SPICES_PROFILE_ZONE;

		const std::shared_ptr<LoadingState> loadingState = std::make_shared<LoadingState>();

		/**
		* @brief Calaulate model matrix.
		*/
		for (auto& item : m_Scenes->m_ScenesData)
		{
			for (const auto& node : item.nodes)
			{
				CreateEntityRecursive(world, tag, node, transform.ToMatrix(), loadingState);
			}
		}

		/**
		* @brief InfoBar output.
		*/
		std::stringstream ss;
		ss << "GLTF: " << tag << " is on Loading...";

		auto self = shared_from_this();
		SlateInfoBar::Create<float>(ss.str(), [=]() -> float {
			return static_cast<float>(loadingState->loadedMeshes.load()) / static_cast<float>(self->m_Meshes->GetNMeshes());
		}, [=](SlateInfoBar* that) {
			return std::any_cast<float>(that->GetRate()) >= 1.0f;
		});

		world->Mark(World::WorldMarkBits::MeshAddedToWorld);
	}

	Entity GltfCollection::CreateEntityRecursive(
		World*             world , 
		const std::string& tag   , 
		uint32_t           node  , 
		const glm::mat4&   model , 
		std::shared_ptr<LoadingState> loadingState
	)
	{
		GltfNodes::Item& item = m_Nodes->m_NodesData[node];

		/**
		* @brief Create this entity.
		*/
		auto self   = shared_from_this();


		Entity entity = world->CreateEntity(tag);

		auto& transformComp = entity.GetComponent<TransformComponent>();

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		DecomposeTransform(model * item.matrix, position, rotation, scale);
		rotation = glm::vec3(glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));

		transformComp.SetPosition(position);
		transformComp.SetRotation(rotation);
		transformComp.SetScale(scale);


		std::vector<Entity> childrens;

		for (auto& n : item.children)
		{
			childrens.push_back(CreateEntityRecursive(world, tag, n, glm::mat4(1.0f), loadingState));
		}

		if (!childrens.empty())
		{
			auto& entityComp = entity.AddComponent<EntityComponent>();

			for (auto& e : childrens)
			{
				entityComp.AddEntity(e);
			}
		}

		if (item.mesh < -0.5f)
		{
			return entity;
		}

		Mesh::Builder builder;

		for (int i = 0; i < self->m_Meshes->m_MeshesData[item.mesh].primitives.size(); i++)
		{
			std::stringstream ss;
			ss << self->m_Meshes->m_MeshesData[item.mesh].name << '_' << node;

			std::shared_ptr<GltfPack> pack = std::make_shared<GltfPack>(ss.str(), [&](GltfPack* gltfPack) {
				GltfLoader::LoadPack(gltfPack, self->m_Meshes->m_MeshesData[item.mesh].primitives[i], self->m_Accessors.get(), self->m_Buffers.get(), self->m_BufferViews.get());
			});

			std::shared_ptr<Material> material = GltfLoader::LoadMaterial(self->m_Materials->m_MaterialsData[self->m_Meshes->m_MeshesData[item.mesh].primitives[i].material], self->m_Images.get());

			pack->SetMaterial(material);

			builder.AddPack(pack);
		}

		std::shared_ptr<Mesh> mesh = builder.Build();

		auto& meshComp = entity.AddComponent<MeshComponent>();

		meshComp.SetMesh(mesh);

		//++loadingState->loadedMeshes;

		///**
		//* @brief Mark the world with MeshAddedToWorld bit.
		//*/
		//if (loadingState->loadedMeshes.load() == self->m_Meshes->GetNMeshes())
		//{
		//	world->Mark(World::WorldMarkBits::MeshAddedToWorld);
		//}

		return entity;

	}
}