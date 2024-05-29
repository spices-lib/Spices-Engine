/**
* @file SkyBoxComponent.cpp.
* @brief The SkyBoxComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SkyBoxComponent.h"

namespace Spiecs {

	SkyBoxComponent::SkyBoxComponent(const std::string& path)
	{
		/**
		* @brief Use SpherePack Instead
		*/
		std::shared_ptr<BoxPack> pack = std::make_shared<BoxPack>(2, 2);
		pack->SetMaterial(path);
		m_Mesh = Mesh::Builder().AddPack(pack).Build();
	}

	void SkyBoxComponent::OnSerialize()
	{
	}

	void SkyBoxComponent::OnDeSerialize()
	{
	}

	void SkyBoxComponent::DefineStageIcon()
	{
		m_SlateStageIconPath = "slate/stage/Stage.Light.png";
	}
}