/**
* @file ThreadLibrary.cpp.
* @brief The ThreadLibrary Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ThreadLibrary.h"
#include "StringLibrary.h"

namespace Spices {
    
    bool ThreadLibrary::SetThreadName(const std::string& name)
    {
        SPICES_PROFILE_ZONE;
        
        const std::wstring n = StringLibrary::CharToWChar(name.c_str());
        
        HRESULT r = SetThreadDescription(GetCurrentThread(), n.c_str());

        if(r != S_OK)
        {
            SPICES_CORE_WARN("Thread name failed.")
            return false;
        }
        
        return true;
    }
}
