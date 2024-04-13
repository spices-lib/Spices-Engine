#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Core/UUID.h"

namespace Spiecs {

	class UUIDComponent : public Component
	{
	public:
		UUIDComponent() {};
		virtual ~UUIDComponent() {};

		virtual void OnSerialize();
		virtual void OnDeSerialize();

		void SetUUID(UUID uuid) { m_uuid = uuid; };
		UUID GetUUID() { return m_uuid; };

	private:
		UUID m_uuid;
	};
}