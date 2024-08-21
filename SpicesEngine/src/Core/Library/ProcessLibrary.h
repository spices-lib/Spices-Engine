/**
* @file ProcessLibrary.h
* @brief The ProcessLibrary Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

#include <Windows.h>

namespace Spices {

	/**
	* @brief Process Static Function Library.
	*/
	class ProcessLibrary
	{
	public:
		
		/**
		* @brief Open a Process with command.
		* @param[in] processPath Process Path.
		* @param[in] commandLine Commands.
		* @return Returns true if Open Process Successfully.
		*/
		static bool OpenProcess(const char* processPath, const char* commandLine = "");

		/**
		* @brief Close a Process with command.
		* @param[in] processPath Process Path.
		* @return Returns true if Open Process Successfully.
		*/
		static bool CloseProcess(const char* processName);

		/**
		* @brief Get this Process Memory used( GB ).
		* @return Returns this Process Memory used( GB ).
		*/
		static float ProcessMemoryInUsed();
	};
}