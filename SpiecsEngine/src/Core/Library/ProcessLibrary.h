#pragma once
#include "Core/Core.h"

#include <Windows.h>

namespace Spiecs {

	class ProcessLibrary
	{
	public:
		
		static bool OpenProcess(const char* processName);

		static bool CloseProcess(const char* processName);
	};
}