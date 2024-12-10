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

        switch (r)
        {
        case S_OK: 
            return true;
        case E_ABORT:
            SPICES_CORE_WARN("Operation aborted")
            return false;
        case E_ACCESSDENIED:
            SPICES_CORE_WARN("General access denied error")
            return false;
        case E_FAIL:
            SPICES_CORE_WARN("Unspecified failure")
            return false;
        case E_HANDLE:
            SPICES_CORE_WARN("Handle that is not valid")
            return false;
        case E_INVALIDARG:
            SPICES_CORE_WARN("One or more arguments are not valid")
            return false;
        case E_NOINTERFACE:
            SPICES_CORE_WARN("No such interface supported")
            return false;
        case E_NOTIMPL:
            SPICES_CORE_WARN("Not implemented")
            return false;
        case E_OUTOFMEMORY:
            SPICES_CORE_WARN("Failed to allocate necessary memory")
            return false;
        case E_POINTER:
            SPICES_CORE_WARN("Pointer that is not valid")
            return false;
        case E_UNEXPECTED:
            SPICES_CORE_WARN("Unexpected failure")
            return false;
        }
        
        return true;
    }
}
