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

		{
			Entity plane = CreateEntity("CornellBox_RightFace");
			TransformComponent& transformComp = plane.GetComponent<TransformComponent>();
			MeshComponent& meshComp = plane.AddComponent<MeshComponent>();
			transformComp.SetPosition({ 0.0f, 0.0f, 0.5f });
			transformComp.SetRotation({ 0.0f, 0.0f, 0.0f });

			std::shared_ptr<PlanePack> pack = std::make_shared<PlanePack>();

			pack->SetMaterial("BasePassRenderer.Mesh.ground");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);
		}

		{
			Entity plane = CreateEntity("CornellBox_LeftFace");
			TransformComponent& transformComp = plane.GetComponent<TransformComponent>();
			MeshComponent& meshComp = plane.AddComponent<MeshComponent>();
			transformComp.SetPosition({ 0.0f, 0.0f, -0.5f });
			transformComp.SetRotation({ -180.0f, 0.0f, -180.0f });

			std::shared_ptr<PlanePack> pack = std::make_shared<PlanePack>();

			pack->SetMaterial("BasePassRenderer.Mesh.ground");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);
		}

		{
			Entity plane = CreateEntity("CornellBox_FrontFace");
			TransformComponent& transformComp = plane.GetComponent<TransformComponent>();
			MeshComponent& meshComp = plane.AddComponent<MeshComponent>();
			transformComp.SetPosition({ -0.5f, 0.0f, 0.0f });
			transformComp.SetRotation({ -180.0f, -90.0f, -180.0f });

			std::shared_ptr<PlanePack> pack = std::make_shared<PlanePack>();

			pack->SetMaterial("BasePassRenderer.Mesh.ground");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);
		}

		{
			Entity plane = CreateEntity("CornellBox_TopFace");
			TransformComponent& transformComp = plane.GetComponent<TransformComponent>();
			MeshComponent& meshComp = plane.AddComponent<MeshComponent>();
			transformComp.SetPosition({ 0.0f, 0.5f, 0.0f });
			transformComp.SetRotation({ 90.0f, -90.0f, 180.0f });

			std::shared_ptr<PlanePack> pack = std::make_shared<PlanePack>();

			pack->SetMaterial("BasePassRenderer.Mesh.ground");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);
		}

		{
			Entity plane = CreateEntity("CornellBox_ButtomFace");
			TransformComponent& transformComp = plane.GetComponent<TransformComponent>();
			MeshComponent& meshComp = plane.AddComponent<MeshComponent>();
			transformComp.SetPosition({ 0.0f, -0.5f, 0.0f });
			transformComp.SetRotation({ -90.0f, -90.0f, 180.0f });

			std::shared_ptr<PlanePack> pack = std::make_shared<PlanePack>();

			pack->SetMaterial("BasePassRenderer.Mesh.ground");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);
		}

		{
			Entity sphere = CreateEntity("CornellBox_Sphere");
			MeshComponent& meshComp = sphere.AddComponent<MeshComponent>();
			TransformComponent& transformComp = sphere.GetComponent<TransformComponent>();
			transformComp.SetPosition({ 0.0f,  0.0f, 0.0f });
			transformComp.SetScale({ 0.14f, 0.14f, 0.14f });

			std::shared_ptr<SpherePack> pack = std::make_shared<SpherePack>(100, 100);

			pack->SetMaterial("BasePassRenderer.Mesh.ground");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);
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