#pragma once
#include "Component.h"
#include "entt.hpp"

namespace Spiecs
{
	class Entity;

	class Component
	{
	public:
		Component() {};
		virtual ~Component() {};

		virtual void OnSerialize() = 0;
		virtual void OnDeSerialize() = 0;

		virtual void OnComponentAdded(entt::entity entity) { m_Owner = entity; };

	protected:
		entt::entity m_Owner{ entt::null };
	};

}
