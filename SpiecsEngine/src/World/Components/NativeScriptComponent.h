#pragma once
#include "Core/core.h"
#include "Component.h"

namespace Spiecs {

	class NativeScriptComponent : public Component
	{
	public:
		NativeScriptComponent() {};
		virtual ~NativeScriptComponent() {};

		virtual void OnSerialize() = 0;
		virtual void OnDeSerialize() = 0;
		virtual void OnComponentAdded() = 0;

		virtual void OnConstruction() {};
		virtual void OnTick() {};

	};
}