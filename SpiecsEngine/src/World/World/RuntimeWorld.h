#pragma once
#include "Core/Core.h"
#include "World.h"

namespace Spiecs {

	class RuntimeWorld : World
	{
	public:
		RuntimeWorld() {};
		virtual ~RuntimeWorld() {};

		virtual void OnPreActivate() override;
		virtual void OnActivate() override;
		virtual void OnDeactivate() override;

	private:

	};
}