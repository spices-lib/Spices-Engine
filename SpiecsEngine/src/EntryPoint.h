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

	/**
	* @brief Create Application
	*/
	auto engine = Spiecs::Application();

	/**
	* @brief Application Run.
	*/
	engine.Run();
}