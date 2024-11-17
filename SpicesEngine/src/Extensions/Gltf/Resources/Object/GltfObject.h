#pragma once
#include "Core/Core.h"
#include "../../GltfHelper.h"

#include <nlohmann/json.hpp>

namespace Spices {

	class GltfObject
	{
	public:
		using Json = nlohmann::json;

	public:
		GltfObject(const Json& data) : m_Data(data) {}
		virtual ~GltfObject() = default;

	protected:

		Json m_Data;
	};

}