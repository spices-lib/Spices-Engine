/**
* @file WorldFunctions.cpp.
* @brief The WorldFunctions Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "WorldFunctions.h"
#include "World/Entity.h"

namespace Spices {

    void WorldFunctions::CreateMeshEntity(
        World*                                 world         , 
        const std::string&                     name          , 
        std::function<std::shared_ptr<Mesh>()> onMeshCreated , 
        std::function<void(Entity&)>           onAdded
    )
    {
        AsyncTask(ThreadPoolEnum::Custom, [=]() {

            SPICES_PROFILE_ZONEN("CreateBasicMeshEntity");

            auto mesh = onMeshCreated();

            CreateMeshEntity(world, name, mesh, onAdded);
        });
    }

    void WorldFunctions::CreateMeshEntity(
        World*                       world   , 
        const std::string&           name    , 
        const std::shared_ptr<Mesh>& mesh    , 
        std::function<void(Entity&)> onAdded
    )
    {
        AsyncMainTask(ThreadPoolEnum::Main, [=]() {

    		SPICES_PROFILE_ZONEN("CreateMeshEntity");
    		
    		Entity entity  = world->CreateEntity(name);
            auto& meshComp = entity.AddComponent<MeshComponent>();
    	
			meshComp.SetMesh(mesh);

            if(onAdded) onAdded(entity);

            /**
            * @brief Mark World with MeshAddedToWorld bits.
            */
            world->Mark(World::WorldMarkBits::MeshAddedToWorld);
    	});
    }
}