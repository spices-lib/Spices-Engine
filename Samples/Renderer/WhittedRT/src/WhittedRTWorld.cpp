#include "WhittedRTWorld.h"
#include <World/Entity.h>
#include <Resources/Loader/MaterialLoader.h>
#include <GamePlay/MeshController.h>
#include <Resources/ResourcePool/ResourcePool.h>
#include <GamePlay/CameraController.h>
#include <Systems/ResourceSystem.h>
#include <Extensions/Gltf/GltfCollection.h>

namespace Spices {

	void WhittedRTWorld::OnPreActivate()
	{
		SPICES_PROFILE_ZONE;

		ResourceSystem::RegistryResourceFolder(SPICES_GAME_ASSETS_PATH);

		EditorWorld::OnPreActivate();

		{
			auto collection = std::make_shared<GltfCollection>("G:/Vulkan/ExtensionAssets/BistroExterior/BistroExterior.gltf");
			collection->CreateEntity(this, "BistroExterior");
		}
	}

	void WhittedRTWorld::OnActivate(TimeStep& ts)
	{
		SPICES_PROFILE_ZONE;

		EditorWorld::OnActivate(ts);
	}

	void WhittedRTWorld::OnDeactivate()
	{
		SPICES_PROFILE_ZONE;

		EditorWorld::OnDeactivate();
	}

}