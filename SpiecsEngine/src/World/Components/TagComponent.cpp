/**
* @file TagComponent.cpp.
* @brief The TagComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "TagComponent.h"

namespace Spiecs {

	TagComponent::TagComponent(const std::string& tag)
	{
		m_Tags.insert(tag);
	}

	void TagComponent::OnSerialize()
	{

	}

	void TagComponent::OnDeSerialize()
	{

	}
}