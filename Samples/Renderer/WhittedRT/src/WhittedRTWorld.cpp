#include "WhittedRTWorld.h"
#include <World/Entity.h>
#include <Resources/Loader/MaterialLoader.h>
#include <GamePlay/MeshController.h>
#include <Resources/ResourcePool/ResourcePool.h>
#include <GamePlay/CameraController.h>
#include <Systems/ResourceSystem.h>

namespace Spices {

	void WhittedRTWorld::OnPreActivate()
	{
		SPICES_PROFILE_ZONE;

		ResourceSystem::RegistryResourceFolder(SPICES_GAME_ASSETS_PATH);

		// camera
		{
			Entity cameraentity = CreateEntity("EditorCamera");
			CameraComponent& camComp = cameraentity.AddComponent<CameraComponent>(true);
			camComp.SetCamera(std::make_shared<Camera>());
			camComp.GetCamera()->SetPerspective(45.0f, 0.001f, 100000.0f);
			TransformComponent& transformComp = cameraentity.GetComponent<TransformComponent>();
			transformComp.SetPosition({ 30.0f, 6.0f, 15.0f });
			transformComp.SetRotation({ 8.0f, -90.0f, 0.0f });

			cameraentity.AddComponent<NativeScriptComponent>(std::make_shared<CameraController>());
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