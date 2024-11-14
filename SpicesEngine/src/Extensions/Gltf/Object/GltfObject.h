#pragma once
#include "Core/Core.h"
#include "../GltfUtils.h"

#include <nlohmann/json.hpp>

namespace Spices {

	class GltfObject
	{
	public:
		using Json = nlohmann::json;

	public:
		GltfObject(GltfObjectType type, const Json& data) : m_Type(type), m_Data(data) {}
		virtual ~GltfObject() = default;

		GltfObjectType GetType() const { return m_Type; }
		std::string GetStringType() const { return std::move(GltfUtils::ToString(m_Type)); }

	protected:

		GltfObjectType m_Type;
		Json m_Data;
	};

}