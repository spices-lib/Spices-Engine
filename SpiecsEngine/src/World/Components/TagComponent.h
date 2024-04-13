#pragma once
#include "Core/Core.h"
#include "Component.h"

#include <string>
#include <set>

namespace Spiecs {

	class TagComponent : Component
	{
	public:
		TagComponent() {};
		virtual ~TagComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;

		void AddTag(const std::string& tag) { m_Tags.insert(tag); };
		void RemoveTag(const std::string& tag) { if (m_Tags.find(tag) != m_Tags.end()) m_Tags.erase(tag); };
		std::set<std::string> GetTag() { return m_Tags; };

	private:
		std::set<std::string> m_Tags;
	};
}