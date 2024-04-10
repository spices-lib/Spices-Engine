#include "GameEditorWorld.h"
#include "World/Entity.h"

namespace Spiecs {

	void GameEditorWorld::OnPreActivate()
	{
		EditorWorld::OnPreActivate();

		// mesh
		{
			Entity& meshentity = CreateEntity("DefaultMesh");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();

			//std::shared_ptr<FilePack> pack = std::make_shared<FilePack>("111");
			std::shared_ptr<SquarePack> pack = std::make_shared<SquarePack>();
			pack->GetMaterial()->LoadMaterial("MeshRenderer.1");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);
		}

		// mesh2
		{
			Entity& meshentity = CreateEntity("DefaultMesh");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
			transformComp1.SetPostion({ 1.5f, 1.5f, 0.0f });

			std::shared_ptr<FilePack> pack = std::make_shared<FilePack>("111");
			//std::shared_ptr<SquarePack> pack = std::make_shared<SquarePack>();
			pack->GetMaterial()->LoadMaterial("MeshRenderer.0");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack).Build();
			meshComp.SetMesh(mesh);
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