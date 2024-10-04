#pragma once
#include "Core/Core.h"

namespace Spices {

    static size_t StrType2Size(const std::string& type)
    {
        if      (type == "float4") return sizeof(glm::vec4);
        else if (type == "float3") return sizeof(glm::vec3);
        else if (type == "float2") return sizeof(glm::vec2);
        else if (type == "float")  return sizeof(float);
        else if (type == "int")    return sizeof(int);
        else if (type == "bool")    return sizeof(bool);

        SPICES_CORE_ERROR("StrType2Size: Not supported type.");
        return 0;
    };
}
