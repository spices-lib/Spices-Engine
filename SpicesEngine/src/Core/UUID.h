#pragma once
#include "Core/Core.h"

namespace Spices {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

}

namespace std {

	template<>
	struct hash<Spices::UUID>
	{
		std::size_t operator()(const Spices::UUID& uuid) const noexcept
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}