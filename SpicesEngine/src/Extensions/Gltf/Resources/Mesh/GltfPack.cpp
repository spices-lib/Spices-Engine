#include "Pchheader.h"
#include "GltfPack.h"
#include "../Loader/GltfLoader.h"

namespace Spices {

	bool GltfPack::OnCreatePack(bool isCreateBuffer)
	{
		SPICES_PROFILE_ZONE;

		if (MeshPack::OnCreatePack(isCreateBuffer)) return true;

		m_Func(this);

		if (isCreateBuffer)
		{
			MeshProcessor::GenerateMeshLodClusterHierarchy(this);
			CreateBuffer();
		}

		return true;
	}
}