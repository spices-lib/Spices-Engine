#include "Pchheader.h"
#include "WorldFunctions.h"
#include "World/Entity.h"

namespace Spices {

    void WorldFunctions::CreateMeshEntity(World* world, const std::string& name, const std::shared_ptr<Mesh>& mesh)
    {
    	AsyncTask(ThreadPoolEnum::Game, [=](){

    		SPICES_PROFILE_ZONEN("CreateMeshEntity");
    		
    		Entity entity  = world->CreateEntity(name);
            auto& meshComp = entity.AddComponent<MeshComponent>();
    	
			meshComp.SetMesh(mesh);
    	});
    }
}