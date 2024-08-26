#include "Pchheader.h"
#include "WorldFunctions.h"
#include "World/Entity.h"

namespace Spices {

    Entity WorldFunctions::CreateMeshEntity(World* world, const std::string& name, std::shared_ptr<MeshPack> pack)
    {
		SPICES_PROFILE_ZONE;

		Entity entity = world->CreateEntity(name);
		MeshComponent& meshComp = entity.AddComponent<MeshComponent>();

		std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
		meshComp.SetMesh(mesh);

		return entity;
    }
}