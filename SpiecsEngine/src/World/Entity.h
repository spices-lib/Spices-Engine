#pragma once
#include "Core/Core.h"
#include "entt.hpp"

namespace Spiecs {

	class Entity
	{
	public:
		Entity() {};
		virtual ~Entity() {};



	private:
		entt::entity m_EntityHandle{ entt::null };
	};
}