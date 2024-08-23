#include "Pchheader.h"
#include "WorldFunctions.h"
#include "World/Entity.h"

namespace Spices {


	std::future<Entity> WorldFunctions::CreateCubeEntityAsync(World* world)
	{
		SPICES_PROFILE_ZONE;

		return ThreadPool::Get()->SubmitPoolTask([world]() {

			std::shared_ptr<CubePack> pack = std::make_shared<CubePack>();
			pack->SetMaterial("BasePassRenderer.Mesh.Default");

			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();

			Entity entity = world->CreateEntity("Cube");
			entity.AddComponent<MeshComponent>().SetMesh(mesh);

			return entity;
		});
	}
}