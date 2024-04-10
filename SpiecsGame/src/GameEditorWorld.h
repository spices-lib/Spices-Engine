#pragma once
#include "Core/Core.h"
#include "World/World/EditorWorld.h"

namespace Spiecs {

	class GameEditorWorld : public EditorWorld
	{
	public:
		GameEditorWorld() {};
		virtual ~GameEditorWorld() {};

		virtual void OnPreActivate() override;
		virtual void OnActivate(TimeStep& ts) override;
		virtual void OnDeactivate() override;

	private:
	};
}