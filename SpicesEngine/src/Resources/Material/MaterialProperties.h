/**
* @file MaterialProperties.h.
* @brief The MaterialProperties Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief Wapper of material render options.
	*/
	class MaterialProperties
	{
	public:
		
		/**
		* @brief Constructor Function.
		*/
		MaterialProperties();

		/**
		* @brief Destructor Function.
		*/
		virtual ~MaterialProperties() = default;

		bool bTwoSided;            // Enablerender two sides.
		bool bCastShadow;          // Enable cast shadow.
		bool bReceiveShadow;       // Enable receive shadow.
		bool bWireFrame;           // Enable wireframe render.
	};
}