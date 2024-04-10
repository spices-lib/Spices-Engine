#include "Pchheader.h"
#include "EditorWorld.h"
#include "World/Entity.h"
#include "Render/FrameInfo.h"
#include "Systems/SystemManager.h"

namespace Spiecs {

	void EditorWorld::OnPreActivate()
	{
		// camera
		{
			Entity& cameraentity = CreateEntity("EditorCamera");
			CameraComponent& camComp = cameraentity.AddComponent<CameraComponent>(true);
			camComp.SetCamera(std::make_shared<Camera>());
			camComp.GetCamera()->SetPerspective(glm::radians(45.0f), 0.001f, 1000.0f, 1.333);
			TransformComponent& transformComp = cameraentity.GetComponent<TransformComponent>();
			transformComp.SetPostion({ 0.0f, 0.0f, -3.0f });
		}
		
		// skybox
		{
			Entity& skyboxentity = CreateEntity("SkyBox");
			SkyBoxComponent& skyboxComp = skyboxentity.AddComponent<SkyBoxComponent>();
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