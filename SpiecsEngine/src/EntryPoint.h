/**
* @file EntryPoint.h.
* @brief EntryPoint Definitions.
* @author Spiecs.
*/

#include "Core/Core.h"
#include "core/Application.h"

/**
* @brief Main Function.
*/
int main() {

	try 
	{
		/**
		* @brief Create Application
		*/
		auto engine = Spiecs::Application();

		/**
		* @brief Application Run.
		*/
		engine.Run();
	}
	catch (const spdlog::spdlog_ex& ex) 
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}