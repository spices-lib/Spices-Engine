#pragma once
#include "Core/Core.h"

namespace Spiecs {

    static size_t StrType2Size(const std::string& type)
    {
        if (type == "float4") return sizeof(glm::vec4);
        else if (type == "float3") return sizeof(glm::vec4);
        else if (type == "float2") return sizeof(glm::vec2);
        else if (type == "float") return sizeof(float);
    };
}
