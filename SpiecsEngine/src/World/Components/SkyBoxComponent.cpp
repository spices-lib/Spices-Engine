#include "Pchheader.h"
#include "SkyBoxComponent.h"
#include "Render/Material/SkyBoxMaterial.h"

namespace Spiecs {

	SkyBoxComponent::SkyBoxComponent(const std::string& path)
	{
		// Use SpherePack Instead
		std::shared_ptr<BoxPack> pack = std::make_shared<BoxPack>();
		pack->SetMaterial(std::make_shared<SkyBoxMaterial>("SkyBoxRenderer"));
		m_Mesh = Mesh::Builder().AddPack(pack).Build();
	}

	void SkyBoxComponent::OnSerialize()
	{
	}

	void SkyBoxComponent::OnDeSerialize()
	{
	}

	void SkyBoxComponent::OnComponentAdded()
	{
	}

}