/**
* @file EditorWorld.cpp.
* @brief The EditorWorld Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "EditorWorld.h"
#include "World/Entity.h"
#include "Render/FrameInfo.h"
#include "Systems/SystemManager.h"
#include "GamePlay/CameraController.h"

namespace Spiecs {

	void EditorWorld::OnPreActivate()
	{
		// camera
		{
			Entity& cameraentity = CreateEntity("EditorCamera");
			CameraComponent& camComp = cameraentity.AddComponent<CameraComponent>(true);
			camComp.SetCamera(std::make_shared<Camera>());
			camComp.GetCamera()->SetPerspective(glm::radians(45.0f), 0.001f, 100000.0f, 1920.f/1080.f);
			TransformComponent& transformComp = cameraentity.GetComponent<TransformComponent>();
			transformComp.SetPostion({ 5.0f, 7.0f, -10.0f });

			cameraentity.AddComponent<NativeScriptComponent>(std::make_shared<CameraController>());
		}
		
		// skybox
		{
			Entity& skyboxentity = CreateEntity("SkyBox");
			SkyBoxComponent& skyboxComp = skyboxentity.AddComponent<SkyBoxComponent>("BasePassRenderer.SkyBox.little_paris_eiffel_tower_4k");
			TransformComponent& transformComp = skyboxentity.GetComponent<TransformComponent>();
			transformComp.SetScale({5000, 5000, 5000});
		}

		// directionallight
		{
			Entity& dirlightentity = CreateEntity("DirectionalLight");
			DirectionalLightComponent& dirlightComp = dirlightentity.AddComponent<DirectionalLightComponent>();
			dirlightComp.SetDirection({1.0f, 1.0f, -1.0f});
			dirlightComp.SetColor({ 1.0f, 0.0f, 0.0f });
			dirlightComp.SetIntensity(1.0f);
		}
	}

	void EditorWorld::OnActivate(TimeStep& ts)
	{
		SPIECS_PROFILE_ZONE;

		SystemManager::Run(ts);
	}

	void EditorWorld::OnDeactivate()
	{

	}
}