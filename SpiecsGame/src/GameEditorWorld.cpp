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

			std::shared_ptr<FilePack> pack1 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_01");
			std::shared_ptr<FilePack> pack2 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_02");
			std::shared_ptr<FilePack> pack3 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_03");
			std::shared_ptr<FilePack> pack4 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_04");
			std::shared_ptr<FilePack> pack5 = std::make_shared<FilePack>("interior_stair_wl3ieamdw/interior_stair_wl3ieamdw_05");
			//std::shared_ptr<SpherePack> pack = std::make_shared<SpherePack>(20, 20);
			pack1->SetMaterial(std::make_shared<MeshMaterial>("MeshRenderer.interior_stair_wl3ieamdw"));
			pack2->SetMaterial(std::make_shared<MeshMaterial>("MeshRenderer.interior_stair_wl3ieamdw"));
			pack3->SetMaterial(std::make_shared<MeshMaterial>("MeshRenderer.interior_stair_wl3ieamdw"));
			pack4->SetMaterial(std::make_shared<MeshMaterial>("MeshRenderer.interior_stair_wl3ieamdw"));
			pack5->SetMaterial(std::make_shared<MeshMaterial>("MeshRenderer.interior_stair_wl3ieamdw"));
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).AddPack(pack2).AddPack(pack3).AddPack(pack4).AddPack(pack5).Build();
			meshComp.SetMesh(mesh);

			//meshentity.AddComponent<NativeScriptComponent>(std::make_shared<MeshController>());
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