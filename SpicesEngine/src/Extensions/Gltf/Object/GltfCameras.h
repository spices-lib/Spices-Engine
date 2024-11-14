#pragma once
#include "Core/Core.h"
#include "GltfObject.h"

namespace Spices {

	class GltfCameras : public GltfObject
	{
	public:
		struct Item
		{
			float yfov;
			float znear;
			float zfar;
			int nodeIndex;
		};

	public:
		GltfCameras(const Json& data)
			: GltfObject(GltfObjectType::images, data)
		{
			m_CamerasData.resize(data.size());

			for (int i = 0; i < data.size(); i++)
			{
				Item& item = m_CamerasData[i];
				const Json& json = data[i];

				item.yfov      = GltfHelper::GetElementFloat(json, "perspective/yfov", 0.1f);
				item.znear     = GltfHelper::GetElementFloat(json, "perspective/znear", 0.1f);
				item.zfar      = GltfHelper::GetElementFloat(json, "perspective/zfar", 100.0f);
				item.nodeIndex = -1;
			}
		}

		virtual ~GltfCameras() override = default;

	private:

		std::vector<Item> m_CamerasData;
	};
}
