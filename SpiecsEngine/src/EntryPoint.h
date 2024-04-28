/**
* @file EntryPoint.h.
* @brief EntryPoint Definitions.
* @author Spiecs.
*/

#include "Pchheader.h"
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
	catch (const std::exception& e) 
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}