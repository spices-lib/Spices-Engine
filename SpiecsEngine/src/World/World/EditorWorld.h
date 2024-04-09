#pragma once
#include "Core/Core.h"
#include "World.h"

namespace Spiecs {

	class EditorWorld : public World
	{
	public:
		EditorWorld() {};
		virtual ~EditorWorld() {};

		virtual void OnPreActivate() override;
		virtual void OnActivate(TimeStep ts) override;
		virtual void OnDeactivate() override;

	private:

	};
}