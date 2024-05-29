/**
* @file Component.h
* @brief The Component Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"

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
		* @brief Defines Component icon on stage.
		* Set m_SlateStageIconPath here.
		* @return Returns Specific m_SlateStageIconPath.
		*/
		virtual const std::string& DefineStageIcon() = 0;

		/**
		* @brief Get Component's icon in Stage.
		* @return Returns the icon ImTextureID.
		*/
		inline ImTextureID& GetStageIcon() { return m_SlateStageIcon; };
		
	protected:

		/**
		* @brief Load a Texture from ResourcePool.
		* @param[out] id The ImTextureID Created with inconFilePath.
		* @param[in] iconFile The file path of icon.
		*/
		void LoadSlateIcon(ImTextureID& id, const std::string& iconFile);

	protected:

		/**
		* @brief This component's Owner entity.
		*/
		entt::entity m_Owner{ entt::null };

		/**
		* @brief This component's icon on stage slate.
		*/
		ImTextureID m_SlateStageIcon;

		/**
		* @brief This component's icon file path.
		*/
		std::string m_SlateStageIconPath;
	};

}
