#include "pchheader.h"
#include "EditorWorld.h"
#include "World/Entity.h"
#include "Render/FrameInfo.h"
#include "Systems/SystemManager.h"
#include "World/Components/CameraComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/MeshComponent.h"

namespace Spiecs {

	void EditorWorld::OnPreActivate()
	{
		// camera
		Entity& cameraentity = CreateEntity("EditorCamera");
		CameraComponent& camComp = cameraentity.AddComponent<CameraComponent>(true);
		camComp.SetCamera(std::make_shared<Camera>());
		camComp.GetCamera()->SetPerspective(glm::radians(45.0f), 0.001f, 1000.0f, 1.777);
		TransformComponent& transformComp = cameraentity.GetComponent<TransformComponent>();
		transformComp.SetPostion({ 0.0f, 0.0f, -10.0f });

		// mesh
		Entity& meshentity = CreateEntity("DefaultMesh");
		MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();

		//std::shared_ptr<FilePack> pack = std::make_shared<FilePack>("111");
		std::shared_ptr<SquarePack> pack = std::make_shared<SquarePack>();
		pack->OnCreatePack();
		std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();

		meshComp.SetMesh(mesh);
	}

	void EditorWorld::OnActivate()
	{
		SystemManager::Run();
	}

	void EditorWorld::OnDeactivate()
	{

	}
}