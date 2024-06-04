/**
* @file TagComponent.h.
* @brief The TagComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"

#include <string>
#include <set>

namespace Spiecs {

	/**
	* @brief TagComponent Class.
	* This class defines the specific behaver of TagComponent.
	*/
	class TagComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		TagComponent() {};

		/**
		* @brief Constructor Function.
		*/
		TagComponent(const std::string& tag);

		/**
		* @brief Destructor Function.
		*/
		virtual ~TagComponent() {};

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override;

		/**
		* @brief This interface defines how to dserialize.
		* @todo Finsih it.
		*/
		virtual void OnDeSerialize() override;

		/**
		* @brief Add the tag this component handled.
		* @param[in] tag The string type.
		*/
		void AddTag(const std::string& tag) { m_Tags.insert(tag); };

		/**
		* @brief Remove a tag from this component handled.
		* @param[in] tag The string type.
		*/
		void RemoveTag(const std::string& tag) { if (m_Tags.find(tag) != m_Tags.end()) m_Tags.erase(tag); };

		/**
		* @brief Get the tags variable.
		* @return Returns the tags variable.
		*/
		inline const std::set<std::string>& GetTag() { return m_Tags; };

	private:

		/**
		* @brief The tags this component handled.
		*/
		std::set<std::string> m_Tags;
	};
}