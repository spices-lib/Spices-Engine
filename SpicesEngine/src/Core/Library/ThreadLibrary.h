/**
* @file ThreadLibrary.h
* @brief The ThreadLibrary Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

    /**
    * @brief Thread Static Function Library.
    */
    class ThreadLibrary
    {
    public:

        /**
        * @brief Set Thread name.
        * @param name thread name.
        * @return Returns true if succeed.
        */
        static bool SetThreadName(const std::string& name);
        
    };
}