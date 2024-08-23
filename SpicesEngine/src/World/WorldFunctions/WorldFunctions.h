#pragma once
#include "Core/Core.h"
#include "Core/Thread/ThreadPool.h"

namespace Spices {

	class World;

	class Entity;

	class WorldFunctions
	{
	public:

		template<typename ...Args>
		static Entity CreateCubeEntity(World* world, Args... args);

		static std::future<Entity> CreateCubeEntityAsync(World* world);

		template<typename ...Args>
		static Entity CreateSphereEntity(World* world, Args... args);
	};

	template<typename ...Args>
	inline Entity WorldFunctions::CreateCubeEntity(World* world, Args ...args)
	{
		SPICES_PROFILE_ZONE;

		Entity entity = world->CreateEntity("Cube");
		MeshComponent& meshComp = entity.AddComponent<MeshComponent>();

		std::shared_ptr<CubePack> pack = std::make_shared<CubePack>(std::forward<Args>(args)...);

		pack->SetMaterial("BasePassRenderer.Mesh.ground");
		std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
		meshComp.SetMesh(mesh);

		return entity;
	}

	template<typename ...Args>
	inline Entity WorldFunctions::CreateSphereEntity(World* world, Args ...args)
	{
		return Entity();
	}

}