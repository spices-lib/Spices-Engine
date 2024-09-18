#include "GameEditorWorld.h"
#include <World/Entity.h>
#include <Resources/Loader/MaterialLoader.h>
#include <GamePlay/MeshController.h>
#include <Resources/ResourcePool/ResourcePool.h>
#include <Systems/ResourceSystem.h>

namespace Spices {

	void GameEditorWorld::OnPreActivate()
	{
		SPICES_PROFILE_ZONE;

		ResourceSystem::RegistryResourceFolder(SPICES_GAME_ASSETS_PATH);

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
			Entity meshentity = CreateEntity("DefaultMesh");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
			transformComp1.SetPosition({10.0f, 2.0f, 30.0f});
			transformComp1.SetRotation({-180.0f, 0.0f, -180.0f});
			
			std::shared_ptr<FilePack> pack1 = std::make_shared<FilePack>("interior_stair_wl3ieamdw_01");
			//std::shared_ptr<FilePack> pack2 = std::make_shared<FilePack>("interior_stair_wl3ieamdw_02");
			//std::shared_ptr<FilePack> pack3 = std::make_shared<FilePack>("not_a_stanford_bunny");
			//std::shared_ptr<FilePack> pack4 = std::make_shared<FilePack>("interior_stair_wl3ieamdw_04");
			//std::shared_ptr<FilePack> pack5 = std::make_shared<FilePack>("interior_stair_wl3ieamdw_05");

			pack1->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			//pack2->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			//pack3->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			//pack4->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			//pack5->SetMaterial("BasePassRenderer.Mesh.interior_stair_wl3ieamdw");
			//std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).AddPack(pack2).AddPack(pack3).AddPack(pack4).AddPack(pack5).Build();
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
			meshComp.SetMesh(mesh);
		}

		// CornellBox
		/*{
			for(int i = 0; i < 1; i++)
			{
				std::stringstream ss;
				ss << "CornellBox_" << i;
				Entity meshentity = CreateEntity("CornellBox");
				MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
				TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
				transformComp1.SetPosition({-7.0f, 5.0f, 5.0f + i * 10.0f});
				transformComp1.SetRotation({0.0f, -90.0f, 0.0f});

				std::shared_ptr<FilePack> pack1 = std::make_shared<FilePack>("Test_room");

				std::stringstream mss;
				mss << "BasePassRenderer.Mesh.CornellBox" << i;
				pack1->SetMaterial(mss.str());
				std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
				meshComp.SetMesh(mesh);
			}
		}*/

		// sphere
		//{
		//	for(int i = 0; i < 1; i++)  // range in albedo
		//	{
		//		for(int j = 0; j < 1; j++)  // range in roughness
		//		{
		//			std::stringstream ss;
		//			ss << "Sphere_" << 10 * i + j;
		//			Entity meshentity = CreateEntity(ss.str());
		//			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
		//			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
		//			transformComp1.SetPosition({3.0f * i, 0.0f, 3.0f * j});

		//			std::shared_ptr<SpherePack> pack1 = std::make_shared<SpherePack>(100, 100);

		//			std::stringstream mss;
		//			mss << "BasePassRenderer.Mesh." << 10 * i + j;
		//			pack1->SetMaterial(mss.str());
		//			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
		//			meshComp.SetMesh(mesh);
		//		}
		//	}
		//}

		// testsphere
		//{
		//	for (int i = 0; i < 1; i++)  // range in albedo
		//	{
		//		for (int j = 0; j < 100; j++)  // range in roughness
		//		{
		//			for (int k = 0; k < 100; k++)
		//			{
		//				std::stringstream ss;
		//				ss << "TestSphere_" << 10000 * i + 100 * j + k;
		//				std::cout << 10000 * i + 100 * j + k << std::endl;
		//				Entity meshentity = CreateEntity(ss.str());
		//				MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
		//				TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
		//				transformComp1.SetPosition({ 3.0f * i, 3.0f * k, 3.0f * j });

		//				std::shared_ptr<SpherePack> pack1 = std::make_shared<SpherePack>(15, 24, false);

		//				std::stringstream mss;
		//				mss << "BasePassRenderer.Mesh.0";
		//				pack1->SetMaterial(mss.str());
		//				std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
		//				meshComp.SetMesh(mesh);
		//			}
		//		}
		//	}
		//}

		// ground
		{
			Entity meshentity = CreateEntity("Ground");
			MeshComponent& meshComp = meshentity.AddComponent<MeshComponent>();
			TransformComponent& transformComp1 = meshentity.GetComponent<TransformComponent>();
			transformComp1.SetPosition({0.0f, -1.1f, 15.0f});
			transformComp1.SetRotation({90.0f, 0.0f, 0.0f});
			transformComp1.SetScale({500.0f, 500.0f, 500.0f});

			std::shared_ptr<PlanePack> pack1 = std::make_shared<PlanePack>(18, 18);

			pack1->SetMaterial("BasePassRenderer.Mesh.ground");
			std::shared_ptr<Mesh> mesh = Mesh::Builder().AddPack(pack1).Build();
			meshComp.SetMesh(mesh);
		}

		//WorldFunctions::CreateCubeEntity(this);

		// pointlight
		{
			Entity plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>();
			plightComp.SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
			plightComp.SetIntensity(20.0f);
			SpriteComponent& sprite = plightentity.AddComponent<SpriteComponent>();
			sprite.SetMaterial("SpriteRenderer.Sprite.S_LightPoint");
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			transformComp.SetPosition({ -7.0f, 7.0f, 5.0f });
		}
		{
			Entity plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>();
			plightComp.SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
			plightComp.SetIntensity(20.0f);
			SpriteComponent& sprite = plightentity.AddComponent<SpriteComponent>();
			sprite.SetMaterial("SpriteRenderer.Sprite.S_LightPoint");
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			transformComp.SetPosition({ -7.0f, 5.0f, 15.0f });
		}
		{
			Entity plightentity = CreateEntity("PointLight");
			PointLightComponent& plightComp = plightentity.AddComponent<PointLightComponent>();
			plightComp.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
			plightComp.SetIntensity(20.0f);
			SpriteComponent& sprite = plightentity.AddComponent<SpriteComponent>();
			sprite.SetMaterial("SpriteRenderer.Sprite.S_LightPoint");
			TransformComponent& transformComp = plightentity.GetComponent<TransformComponent>();
			transformComp.SetPosition({ -7.0f, 5.0f, 25.0f });
		}

	}

	void GameEditorWorld::OnActivate(TimeStep& ts)
	{
		SPICES_PROFILE_ZONE;

		EditorWorld::OnActivate(ts);
	}

	void GameEditorWorld::OnDeactivate()
	{
		SPICES_PROFILE_ZONE;

		EditorWorld::OnDeactivate();
	}

}