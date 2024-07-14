#pragma once
#include "Core/Core.h"

#include <Windows.h>

namespace Spices {

	class ProcessLibrary
	{
	public:
		
		static bool OpenProcess(const char* processPath, const char* commandLine = "");

		static bool CloseProcess(const char* processName);

		static float ProcessMemoryInUsed();
	};
}