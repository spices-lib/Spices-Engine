#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfMaterials : public GltfObject
	{
	public:


		struct Item
		{

		};

	public:
		GltfMaterials(const Json& data) 
			: GltfObject(GltfObjectType::materials, data)
		{
			
		}

		virtual ~GltfMaterials() override = default;

	private:
		std::vector<Item> m_MaterialsData;
	};
}
