/**
* @file Component.h
* @brief The Component Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiHelper.h"

#include <entt.hpp>
#include <imgui.h>

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
		Component();

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
		virtual void OnComponentAdded(const entt::entity& entity);

		/**
		* @brief This interface defines how to draw this component to property panel.
		*/
		virtual void DrawThis() {};

	protected:

		/**
		* @brief This component's Owner entity.
		*/
		entt::entity m_Owner{ entt::null };

		/**
		* @brief This component's icon file path.
		*/
		std::string m_SlateStageIconPath;
	};

}
