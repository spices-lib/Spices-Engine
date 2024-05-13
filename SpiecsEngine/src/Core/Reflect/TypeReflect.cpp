#include "Pchheader.h"
#include "TypeReflect.h"

namespace Spiecs {
    
    size_t StrType2Size(const std::string& type)
    {
        switch(type)
        {
        case "float4" : return sizeof(glm::vec4);
        case "float3" : return sizeof(glm::vec4);
        case "float2" : return sizeof(glm::vec2);
        }
    }
}
