#include "Pchheader.h"
#include "EditorWorld.h"
#include "World/Entity.h"
#include "Render/FrameInfo.h"
#include "Systems/SystemManager.h"
#include "GamePlay/CameraController.h"
#include "GamePlay/MeshController.h"

namespace Spiecs {

	void EditorWorld::OnPreActivate()
	{
		// camera
		{
			Entity& cameraentity = CreateEntity("EditorCamera");
			CameraComponent& camComp = cameraentity.AddComponent<CameraComponent>(true);
			camComp.SetCamera(std::make_shared<Camera>());
			camComp.GetCamera()->SetPerspective(glm::radians(45.0f), 0.001f, 1000.0f, 1280.f/720.f);
			TransformComponent& transformComp = cameraentity.GetComponent<TransformComponent>();
			transformComp.SetPostion({ 0.0f, 0.0f, -5.0f });
			cameraentity.AddComponent<NativeScriptComponent>(std::make_shared<CameraController>());
		}
		
		// skybox
		{
			Entity& skyboxentity = CreateEntity("SkyBox");
			SkyBoxComponent& skyboxComp = skyboxentity.AddComponent<SkyBoxComponent>("SkyBoxRenderer");
			TransformComponent& transformComp = skyboxentity.GetComponent<TransformComponent>();
			transformComp.SetScale({100, 100, 100});

			skyboxentity.AddComponent<NativeScriptComponent>(std::make_shared<MeshController>());
		}

		// directionallight
		{
			Entity& dirlightentity = CreateEntity("DirectionalLight");
			DirectionalLightComponent& dirlightComp = dirlightentity.AddComponent<DirectionalLightComponent>();
		}
	}

	void EditorWorld::OnActivate(TimeStep& ts)
	{
		SystemManager::Run(ts);
	}

	void EditorWorld::OnDeactivate()
	{

	}
}