/**
* @file EntryPoint.h.
* @brief EntryPoint Definitions.
* @author Spices.
*/
#pragma once

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Memory/MemoryEntry.h"

/**
* @brief Main Function.
*/
int main() {

	PROCESS_INSTANCE_ENTRY

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
	catch (const std::exception& ex) 
	{
		SPICES_CORE_ERROR(ex.what())
		
		return EXIT_FAILURE;
	}

	PROCESS_INSTANCE_EXIT

	return EXIT_SUCCESS;
}