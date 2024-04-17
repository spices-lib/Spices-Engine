/**
* @file Application.h
* @brief The Application Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core.h"
#include "World/World/EditorWorld.h"
#include "World/World/RuntimeWorld.h"

#include <memory>

namespace Spiecs {

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
		* @return no return.
		*/
		void Run();

	private:
		
	};

	/**
	* @brief extern WorldCreation definition in Game.
	*/
	std::shared_ptr<World> CreateWorld();
}

/**
* @brief extern WorldCreation definition in Game.
*/
extern std::shared_ptr<Spiecs::World> Spiecs::CreateWorld();