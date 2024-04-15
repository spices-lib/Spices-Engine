#include "GameEditorWorld.h"
#include "World/Entity.h"
#include "Render/Resources/Loader/MaterialLoader.h"
#include "Render/Material/MeshMaterial.h"
#include "GamePlay/MeshController.h"

namespace Spiecs {

	void GameEditorWorld::OnPreActivate()
	{
		EditorWorld::OnPreActivate();

		// mesh2
		{
			Entity& meshentity = CreateEntity("DefaultMesh");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();

			std::shared_ptr<FilePack> pack = std::make_shared<FilePack>("111");
			//std::shared_ptr<BoxPack> pack = std::make_shared<BoxPack>(20, 20);
			pack->SetMaterial(std::make_shared<MeshMaterial>("MeshRenderer.0"));
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);

			//meshentity.AddComponent<NativeScriptComponent>(std::make_shared<MeshController>());
		}
		
		// mesh3
		{
			//Entity& meshentity = CreateEntity("DefaultMesh");
			//MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			//TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
			//transformComp1.SetPostion({ 1.5f, 1.5f, 0.0f });

			//std::shared_ptr<FilePack> pack = std::make_shared<FilePack>("111");
			////std::shared_ptr<SquarePack> pack = std::make_shared<SquarePack>();
			//pack->SetMaterial(std::make_shared<MeshMaterial>("MeshRenderer.1"));
			//std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			//meshComp.SetMesh(mesh);
		}
		
		// pointlight
		{
			Entity& plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>();
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			transformComp.SetPostion({ 1.0f, 1.0f, -5.0f });
		}
		{
			Entity& plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>();
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			transformComp.SetPostion({ -1.0f, -1.0f, -5.0f });
		}
	}

	void GameEditorWorld::OnActivate(TimeStep& ts)
	{
		EditorWorld::OnActivate(ts);
	}

	void GameEditorWorld::OnDeactivate()
	{
		EditorWorld::OnDeactivate();
	}

}