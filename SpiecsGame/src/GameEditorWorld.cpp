#include "GameEditorWorld.h"
#include <World/Entity.h>
#include <Resources/Loader/MaterialLoader.h>
#include <GamePlay/MeshController.h>
#include <Resources/ResourcePool/ResourcePool.h>

namespace Spiecs {

	void GameEditorWorld::OnPreActivate()
	{
		EditorWorld::OnPreActivate();

		// mesh1
		{
			Entity& meshentity = CreateEntity("DefaultMesh");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
			transformComp1.SetScale({ 25.0f, 25.0f, 25.0f });
			transformComp1.SetPostion({ 0.0f, -5.0f, 0.0f });

			std::shared_ptr<FilePack> pack1 = std::make_shared<FilePack>("112");
			//std::shared_ptr<SpherePack> pack1 = std::make_shared<SpherePack>(20, 20);
			pack1->SetMaterial("BasePassRenderer.Mesh.0");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
			meshComp.SetMesh(mesh);
		}

		// mesh2
		{
			Entity& meshentity = CreateEntity("DefaultMesh");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();

			std::shared_ptr<FilePack> pack1 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_01");
			std::shared_ptr<FilePack> pack2 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_02");
			std::shared_ptr<FilePack> pack3 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_03");
			std::shared_ptr<FilePack> pack4 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_04");
			std::shared_ptr<FilePack> pack5 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_05");

			pack1->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			pack2->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			pack3->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			pack4->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			pack5->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).AddPack(pack2).AddPack(pack3).AddPack(pack4).AddPack(pack5).Build();
			meshComp.SetMesh(mesh);
		}
		
		// pointlight
		{
			Entity& plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>(glm::vec3(1.0f, 0.0f, 0.0f), 2.0f, 1.0f, 0.35f, 0.44f);
			plightentity.AddComponent<SpriteComponent>("SpriteRenderer.Sprite.S_LightPoint");
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			transformComp.SetPostion({ 1.0f, 3.0f, -4.0f });
		}
		{
			Entity& plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>(glm::vec3(0.0f, 1.0f, 0.0f), 2.0f, 1.0f, 0.35f, 0.44f);
			plightentity.AddComponent<SpriteComponent>("SpriteRenderer.Sprite.S_LightPoint");
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			transformComp.SetPostion({ -1.0f, 3.0f, 4.0f });
		}
	}

	void GameEditorWorld::OnActivate(TimeStep& ts)
	{
		ZoneScoped;
		EditorWorld::OnActivate(ts);
	}

	void GameEditorWorld::OnDeactivate()
	{
		EditorWorld::OnDeactivate();
	}

}