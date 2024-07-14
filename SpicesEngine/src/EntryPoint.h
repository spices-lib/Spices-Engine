/**
* @file EntryPoint.h.
* @brief EntryPoint Definitions.
* @author Spices.
*/
#pragma once

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
		auto engine = Spices::Application();

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