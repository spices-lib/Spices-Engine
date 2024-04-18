/**
* @file Component.h
* @brief The Component Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Component.h"
#include "entt.hpp"

namespace Spiecs
{
	/**
	* @brief Forward Declear
	*/
	class Entity;

	/**
	* @brief Component Class.
	* This class defines the basic behaver of component.
	* When we add an new Component, we need inherit from this.
	*/
	class Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Component() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~Component() {};

		/**
		* @brief This interface defines how to serialize. 
		*/
		virtual void OnSerialize() = 0;

		/**
		* @brief This interface defines how to dserialize.
		*/
		virtual void OnDeSerialize() = 0;

		/**
		* @brief This interface defines the behaver on specific component added.
		* Init with variable.
		* @param[in] entity this component's Owner entity.
		*/
		virtual void OnComponentAdded(const entt::entity& entity) { m_Owner = entity; };

	protected:

		/**
		* @brief This component's Owner entity.
		*/
		entt::entity m_Owner{ entt::null };
	};

}
