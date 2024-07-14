/**
* @file Application.h
* @brief The Application Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core.h"
#include "World/World/World.h"

#include <memory>

namespace Spices {

	/**
	* @brief Application Class.
    * Our Engine Start here.
	*/
	class Application
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Application();

		/**
		* @brief Destructor Function.
		*/
		virtual ~Application();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		Application(const Application&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		Application& operator=(const Application&) = delete;

		/**
		* @brief Run Our World.
		*/
		static void Run();
	};

	/**
	* @brief extern WorldCreation definition in Game.
	*/
	std::shared_ptr<World> CreateWorld();
}

/**
* @brief extern WorldCreation definition in Game.
* @return Returns World Pointer.
*/
extern std::shared_ptr<Spices::World> Spices::CreateWorld();