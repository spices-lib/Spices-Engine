#pragma once
#include <Core/Core.h>
#include <World/World/EditorWorld.h>

namespace Spices {

	class WhittedRTWorld : public EditorWorld
	{
	public:
		WhittedRTWorld() = default;
		virtual ~WhittedRTWorld() override = default;

		virtual void OnPreActivate() override;
		virtual void OnActivate(TimeStep& ts) override;
		virtual void OnDeactivate() override;

	private:
	};
}