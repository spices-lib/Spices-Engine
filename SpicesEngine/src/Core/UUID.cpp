/**
* @file UUID.cpp.
* @brief The UUID Class Implementation.
* @author The Cherno.
*/

#include "Pchheader.h"
#include "UUID.h"

namespace Spices {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	static std::unordered_set<uint64_t> s_IDs;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{}
}