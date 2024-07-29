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
#include "Plugins/HoudiniLiveLink/HoudiniSession.h"

namespace Spices {

	void EditorWorld::OnPreActivate()
	{
		SPICES_PROFILE_ZONE;
		
		// camera
		{
			Entity cameraentity = CreateEntity("EditorCamera");
			CameraComponent& camComp = cameraentity.AddComponent<CameraComponent>(true);
			camComp.SetCamera(std::make_shared<Camera>());
			camComp.GetCamera()->SetPerspective(45.0f, 0.001f, 100000.0f);
			//camComp.GetCamera()->SetOrthographic(-100.0f, 100.0f, 100.0f, -100.0f, 0.001f, 100000.0f);
			TransformComponent& transformComp = cameraentity.GetComponent<TransformComponent>();
			transformComp.SetPosition({ 30.0f, 6.0f, 15.0f });
			transformComp.SetRotation({8.0f, -90.0f, 0.0f});

			cameraentity.AddComponent<NativeScriptComponent>(std::make_shared<CameraController>());
		}
		
		// skybox
		{
			Entity skyboxentity = CreateEntity("SkyBox");
			SkyBoxComponent& skyboxComp = skyboxentity.AddComponent<SkyBoxComponent>("BasePassRenderer.SkyBox.little_paris_eiffel_tower_4k");
			TransformComponent& transformComp = skyboxentity.GetComponent<TransformComponent>();
			transformComp.SetScale({5000, 5000, 5000});
		}

		// directionallight
		{
			Entity dirlightentity = CreateEntity("DirectionalLight_0");
			TransformComponent& transformComp = dirlightentity.GetComponent<TransformComponent>();
			DirectionalLightComponent& dirlightComp = dirlightentity.AddComponent<DirectionalLightComponent>();
			transformComp.SetRotation({0.0f, 25.0f, 50.0f});
			dirlightComp.SetColor({ 1.0f, 1.0f, 1.0f });
			dirlightComp.SetIntensity(5.0f);
		}

		HoudiniEngine::HoudiniSession session {};
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