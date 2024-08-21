/**
* @file ProcessLibrary.cpp
* @brief The ProcessLibrary Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ProcessLibrary.h"
#include "StringLibrary.h"

#include <psapi.h>
#include <stdio.h>

namespace Spices {

	bool ProcessLibrary::OpenProcess(const char* processPath, const char* commandLine)
	{
		SPICES_PROFILE_ZONE;

		STARTUPINFO StartInfo;
		PROCESS_INFORMATION info;

		ZeroMemory(&StartInfo, sizeof(StartInfo));
		StartInfo.cb = sizeof(STARTUPINFO);
		ZeroMemory(&info, sizeof(PROCESS_INFORMATION));

		const std::wstring path    = StringLibrary::CharToWChar(processPath);
		const std::wstring command = StringLibrary::CharToWChar(commandLine);

		if (!CreateProcess(
			path.c_str(),                             // Process Name
			const_cast<wchar_t*>(command.c_str()),    // Command line
			nullptr,                                  // Process handle not inheritable
			nullptr,                                  // Thread handle not inheritable
			FALSE,                                    // Set handle inheritance to FALSE
			NORMAL_PRIORITY_CLASS,                    // No creation flags
			nullptr,                                  // Use parent's environment block
			nullptr,                                  // Use parent's starting directory
			&StartInfo,                               // Pointer to STARTUP INFO structure
			&info                                     // Pointer to PROCESS_INFORMATION structure
		))
		{
			std::stringstream ss;
			ss << "Process: " << processPath << " Open Failed";

			SPICES_CORE_WARN(ss.str());
			return false;
		}

		return true;
	}

	bool ProcessLibrary::CloseProcess(const char* processName)
	{
		SPICES_PROFILE_ZONE;

		const std::string temp = std::string("TASKKILL /F /IM ") + processName;
		system(temp.c_str());

		return true;
	}

	float ProcessLibrary::ProcessMemoryInUsed()
	{
		SPICES_PROFILE_ZONE;

		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			return pmc.WorkingSetSize / 1024.0f / 1024.0f / 1024.0f; // GB.
		}

		return 0;
	}
}