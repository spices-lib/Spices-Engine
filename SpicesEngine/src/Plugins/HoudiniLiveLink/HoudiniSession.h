#pragma once
#include "HoudiniCore.h"

namespace HoudiniEngine {

    class HoudiniSession
    {
    public:
        HoudiniSession() { 
            StartSession(); 
        }
        virtual ~HoudiniSession() = default;

        void StartSession();
    };
}