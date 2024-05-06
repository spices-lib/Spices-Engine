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

	std::vector<std::string> TagComponent::GetTag()
	{
		std::vector<std::string> tags;

		for (auto& elem : m_Tags)
		{
			tags.push_back(elem);
		}

		return tags;
	}

}