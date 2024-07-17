#include "GameEditorWorld.h"
#include <World/Entity.h>
#include <Resources/Loader/MaterialLoader.h>
#include <GamePlay/MeshController.h>
#include <Resources/ResourcePool/ResourcePool.h>

#include <new>

namespace Spices {

	void GameEditorWorld::OnPreActivate()
	{
		EditorWorld::OnPreActivate();

		// 3dsmax poly canton
		{
			/*Entity& meshentity = CreateEntity("DefaultMesh");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
			transformComp1.SetScale({ 25.0f, 25.0f, 25.0f });
			transformComp1.SetPostion({ 0.0f, -5.0f, 0.0f });

			std::shared_ptr<FilePack> pack1 = std::make_shared<FilePack>("112");
			pack1->SetMaterial("BasePassRenderer.Mesh.0");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
			meshComp.SetMesh(mesh);*/
		}

		// bridge pbr model
		{
			Entity& meshentity = CreateEntity("DefaultMesh");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
			transformComp1.SetPosition({-10.0f, 0.0f, 0.0f});
			
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

		// CornellBox
		{
			for(int i = 0; i < 2; i++)
			{
				std::stringstream ss;
				ss << "CornellBox_" << i;
				Entity meshentity = CreateEntity("CornellBox");
				MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
				TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
				transformComp1.SetPosition({-10.0f, 10.0f, i * 5.0f});

				std::shared_ptr<FilePack> pack1 = std::make_shared<FilePack>("Test_room");

				std::stringstream mss;
				mss << "BasePassRenderer.Mesh.CornellBox" << i;
				pack1->SetMaterial(mss.str());
				std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
				meshComp.SetMesh(mesh);
			}
		}

		// sphere
		{
			for(int i = 0; i < 3; i++)  // range in albedo
			{
				for(int j = 0; j < 10; j++)  // range in roughness
				{
					std::stringstream ss;
					ss << "SphereRangeInRoughness_" << 10 * i + j;
					Entity meshentity = CreateEntity(ss.str());
					MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
					TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
					transformComp1.SetPosition({3.0f * i, 0.0f, 3.0f * j});

					std::shared_ptr<SpherePack> pack1 = std::make_shared<SpherePack>(100, 100);

					std::stringstream mss;
					mss << "BasePassRenderer.Mesh." << 10 * i + j;
					pack1->SetMaterial(mss.str());
					std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
					meshComp.SetMesh(mesh);
				}
			}
		}

		// ground
		{
			Entity meshentity = CreateEntity("Ground");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
			transformComp1.SetPosition({0.0f, -1.1f, 15.0f});
			transformComp1.SetRotation({glm::radians(90.0f), 0.0f, 0.0f});
			transformComp1.SetScale({50.0f, 50.0f, 50.0f});

			std::shared_ptr<SquarePack> pack1 = std::make_shared<SquarePack>();

			pack1->SetMaterial("BasePassRenderer.Mesh.ground");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
			meshComp.SetMesh(mesh);
		}
		
		
		// pointlight
		{
			Entity& plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>();
			plightComp.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
			//plightentity.AddComponent<SpriteComponent>("SpriteRenderer.Sprite.S_LightPoint");
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			transformComp.SetPosition({ 1.0f, 3.0f, -4.0f });
		}
		{
			Entity& plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>();
			plightComp.SetColor(glm::vec3(1.0f));
			//plightentity.AddComponent<SpriteComponent>("SpriteRenderer.Sprite.S_LightPoint");
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			//transformComp.SetPostion({ -1.0f, 3.0f, 4.0f });
			transformComp.SetPosition({ 0.0f, 10.0f, 0.0f });
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