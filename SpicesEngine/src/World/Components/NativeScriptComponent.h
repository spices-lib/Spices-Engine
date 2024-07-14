/**
* @file NativeScriptComponent.h.
* @brief The NativeScriptComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Core/Event/Event.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"
#include "Core/Event/SlateEvent.h"

namespace Spices {

	/**
	* @brief NativeScriptComponent Class.
	* This class defines the specific behaves of NativeScriptComponent.
	* When we add an new NativeScriptComponent, we need inherit from this.
	*/
	class NativeScriptComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		NativeScriptComponent() = default;

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] realComp The real NativeScriptComponent.
		*/
		NativeScriptComponent(std::shared_ptr<NativeScriptComponent> realComp);

		/**
		* @brief Destructor Function.
		*/
		virtual ~NativeScriptComponent() override = default;

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override {}

		/**
		* @brief This interface defines how to deserialize.
		* @todo Finish it.
		*/
		virtual void OnDeSerialize() override {}

		/**
		* @brief This interface defines the behaves on specific component added.
		* Init with variable.
		* @param[in] entity this component's Owner entity.
		*/
		virtual void OnComponentAdded(const entt::entity& entity) override final;

		/**
		* @brief This interface defines the behaves on specific component added.
		*/
		virtual void OnConstruction();

		/**
		* @brief This interface defines the behaves on specific component tick every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnTick(TimeStep& ts);

		/**
		* @brief This interface defines the behaves on specific component event happened.
		* @param[in] e Event.
		*/
		virtual void OnEvent(Event& e);

	private:

		/**
		* @brief Real component
		*/
		std::shared_ptr<NativeScriptComponent> m_RealNScriptComp;
	};
}