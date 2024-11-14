#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfAsset : public GltfObject
	{
	public:

		struct Item
		{
			std::string generator;
			std::string extras;
			std::string version;
		};

		GltfAsset(const Json& data) 
			: GltfObject(GltfObjectType::asset, data) 
		{
			m_AssetData.generator = data["generator"];
			m_AssetData.extras = data["extras"];
			m_AssetData.version = data["version"];
		}

		virtual ~GltfAsset() override = default;

	private:

		Item m_AssetData;
	};
}
