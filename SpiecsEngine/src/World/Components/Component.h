#pragma once
#include "Component.h"

namespace Spiecs
{
	class Component
	{
	public:
		virtual void OnSerialize() = 0;
		virtual void OnDeSerialize() = 0;

		virtual void OnComponentAdded() = 0;
	};

}
