#include "Pchheader.h"
#include "ProcessLibrary.h"

#include <string>
#include <locale>
#include <codecvt>

namespace Spiecs {

	bool ProcessLibrary::OpenProcess(const char* processName)
	{
		STARTUPINFO StartInfo;
		PROCESS_INFORMATION pinfo;

		ZeroMemory(&StartInfo, sizeof(StartInfo));
		StartInfo.cb = sizeof(STARTUPINFO);
		ZeroMemory(&pinfo, sizeof(PROCESS_INFORMATION));

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring name = converter.from_bytes(processName);

		if (!CreateProcess(
			name.c_str(),                // Process Name
			L"",                         // Command line
			NULL,                        // Process handle not inheritable
			NULL,                        // Thread handle not inheritable
			FALSE,                       // Set handle inheritance to FALSE
			NORMAL_PRIORITY_CLASS,       // No creation flags
			NULL,                        // Use parent's environment block
			NULL,                        // Use parent's starting directory
			&StartInfo,                  // Pointer to STARTUPINFO structure
			&pinfo                       // Pointer to PROCESS_INFORMATION structure
		))
		{
			std::stringstream ss;
			ss << "Process: " << processName << " Open Failed";

			SPIECS_CORE_WARN(ss.str());
			return false;
		}

		return true;
	}

	bool ProcessLibrary::CloseProcess(const char* processName)
	{
		std::string temp = std::string("TASKKILL /F /IM ") + processName;
		system(temp.c_str());

		return true;
	}
}