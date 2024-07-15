/**
* @file EditorWorld.cpp.
* @brief The EditorWorld Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "EditorWorld.h"
#include "World/Entity.h"
#include "Render/FrameInfo.h"
#include "Systems/SystemManager.h"
#include "GamePlay/CameraController.h"

namespace Spices {

	void EditorWorld::OnPreActivate()
	{
		// camera
		{
			Entity& cameraentity = CreateEntity("EditorCamera");
			CameraComponent& camComp = cameraentity.AddComponent<CameraComponent>(true);
			camComp.SetCamera(std::make_shared<Camera>());
			camComp.GetCamera()->SetPerspective(45.0f, 0.001f, 100000.0f, 1920.f/1080.f);
			//camComp.GetCamera()->SetOrthographic(-100.0f, 100.0f, 100.0f, -100.0f, 0.001f, 100000.0f);
			TransformComponent& transformComp = cameraentity.GetComponent<TransformComponent>();
			transformComp.SetPosition({ 5.0f, 7.0f, -10.0f });

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
			TransformComponent& transformComp = dirlightentity.GetComponent<TransformComponent>();
			DirectionalLightComponent& dirlightComp = dirlightentity.AddComponent<DirectionalLightComponent>();
			transformComp.SetRotation({0.0f, 0.0f, 0.0f});
			dirlightComp.SetColor({ 1.0f, 1.0f, 1.0f });
			dirlightComp.SetIntensity(0.5f);
		}
	}
 
	void EditorWorld::OnActivate(TimeStep& ts)
	{
		SPICES_PROFILE_ZONE;

		SystemManager::Run(ts);
	}

	void EditorWorld::OnDeactivate()
	{

	}
}