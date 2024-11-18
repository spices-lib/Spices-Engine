/**
* @file GltfHelper.cpp
* @brief The GltfHelper Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "GltfHelper.h"

namespace Spices {

    VkFormat GltfHelper::GetFormat(const std::string& str, int id)
    {
        SPICES_PROFILE_ZONE;

        if (str == "SCALAR")
        {
            switch (id)
            {
            case 5120: return VK_FORMAT_R8_SINT;      //(BYTE)
            case 5121: return VK_FORMAT_R8_UINT;      //(UNSIGNED_BYTE)1
            case 5122: return VK_FORMAT_R16_SINT;     //(SHORT)2
            case 5123: return VK_FORMAT_R16_UINT;     //(UNSIGNED_SHORT)2
            case 5124: return VK_FORMAT_R32_SINT;     //(SIGNED_INT)4
            case 5125: return VK_FORMAT_R32_UINT;     //(UNSIGNED_INT)4
            case 5126: return VK_FORMAT_R32_SFLOAT;   //(FLOAT)
            }
        }
        else if (str == "VEC2")
        {
            switch (id)
            {
            case 5120: return VK_FORMAT_R8G8_SINT;      //(BYTE)
            case 5121: return VK_FORMAT_R8G8_UINT;      //(UNSIGNED_BYTE)1
            case 5122: return VK_FORMAT_R16G16_SINT;    //(SHORT)2
            case 5123: return VK_FORMAT_R16G16_UINT;    //(UNSIGNED_SHORT)2
            case 5124: return VK_FORMAT_R32G32_SINT;    //(SIGNED_INT)4
            case 5125: return VK_FORMAT_R32G32_UINT;    //(UNSIGNED_INT)4
            case 5126: return VK_FORMAT_R32G32_SFLOAT;  //(FLOAT)
            }
        }
        else if (str == "VEC3")
        {
            switch (id)
            {
            case 5120: return VK_FORMAT_UNDEFINED;         //(BYTE)
            case 5121: return VK_FORMAT_UNDEFINED;         //(UNSIGNED_BYTE)1
            case 5122: return VK_FORMAT_UNDEFINED;         //(SHORT)2
            case 5123: return VK_FORMAT_UNDEFINED;         //(UNSIGNED_SHORT)2
            case 5124: return VK_FORMAT_R32G32B32_SINT;    //(SIGNED_INT)4
            case 5125: return VK_FORMAT_R32G32B32_UINT;    //(UNSIGNED_INT)4
            case 5126: return VK_FORMAT_R32G32B32_SFLOAT;  //(FLOAT)
            }
        }
        else if (str == "VEC4")
        {
            switch (id)
            {
            case 5120: return VK_FORMAT_R8G8B8A8_SINT;        //(BYTE)
            case 5121: return VK_FORMAT_R8G8B8A8_UINT;        //(UNSIGNED_BYTE)1
            case 5122: return VK_FORMAT_R16G16B16A16_SINT;    //(SHORT)2
            case 5123: return VK_FORMAT_R16G16B16A16_UINT;    //(UNSIGNED_SHORT)2
            case 5124: return VK_FORMAT_R32G32B32A32_SINT;    //(SIGNED_INT)4
            case 5125: return VK_FORMAT_R32G32B32A32_UINT;    //(UNSIGNED_INT)4
            case 5126: return VK_FORMAT_R32G32B32A32_SFLOAT;  //(FLOAT)
            }
        }

        return VK_FORMAT_UNDEFINED;
    }

    uint32_t GltfHelper::SizeOfFormat(VkFormat format)
    {
        SPICES_PROFILE_ZONE;

        switch (format)
        {
        case VK_FORMAT_R8_SINT: return 1;                 //(BYTE)
        case VK_FORMAT_R8_UINT: return 1;                 //(UNSIGNED_BYTE)1
        case VK_FORMAT_R16_SINT: return 2;                //(SHORT)2
        case VK_FORMAT_R16_UINT: return 2;                //(UNSIGNED_SHORT)2
        case VK_FORMAT_R32_SINT: return 4;                //(SIGNED_INT)4
        case VK_FORMAT_R32_UINT: return 4;                //(UNSIGNED_INT)4
        case VK_FORMAT_R32_SFLOAT: return 4;              //(FLOAT)

        case VK_FORMAT_R8G8_SINT: return 2 * 1;           //(BYTE)
        case VK_FORMAT_R8G8_UINT: return 2 * 1;           //(UNSIGNED_BYTE)1
        case VK_FORMAT_R16G16_SINT: return 2 * 2;         //(SHORT)2
        case VK_FORMAT_R16G16_UINT: return 2 * 2;         // (UNSIGNED_SHORT)2
        case VK_FORMAT_R32G32_SINT: return 2 * 4;         //(SIGNED_INT)4
        case VK_FORMAT_R32G32_UINT: return 2 * 4;         //(UNSIGNED_INT)4
        case VK_FORMAT_R32G32_SFLOAT: return 2 * 4;       //(FLOAT)

        case VK_FORMAT_UNDEFINED: return 0;               //(BYTE) (UNSIGNED_BYTE) (SHORT) (UNSIGNED_SHORT)
        case VK_FORMAT_R32G32B32_SINT: return 3 * 4;      //(SIGNED_INT)4
        case VK_FORMAT_R32G32B32_UINT: return 3 * 4;      //(UNSIGNED_INT)4
        case VK_FORMAT_R32G32B32_SFLOAT: return 3 * 4;    //(FLOAT)

        case VK_FORMAT_R8G8B8A8_SINT: return 4 * 1;       //(BYTE)
        case VK_FORMAT_R8G8B8A8_UINT: return 4 * 1;       //(UNSIGNED_BYTE)1
        case VK_FORMAT_R16G16B16A16_SINT: return 4 * 2;   //(SHORT)2
        case VK_FORMAT_R16G16B16A16_UINT: return 4 * 2;   //(UNSIGNED_SHORT)2
        case VK_FORMAT_R32G32B32A32_SINT: return 4 * 4;   //(SIGNED_INT)4
        case VK_FORMAT_R32G32B32A32_UINT: return 4 * 4;   //(UNSIGNED_INT)4
        case VK_FORMAT_R32G32B32A32_SFLOAT: return 4 * 4; //(FLOAT)
        }

        return 0;
    }

    int GltfHelper::GetFormatSize(int id)
    {
        SPICES_PROFILE_ZONE;

        switch (id)
        {
            case 5120: return 1;                          //(BYTE)
            case 5121: return 1;                          //(UNSIGNED_BYTE)1
            case 5122: return 2;                          //(SHORT)2
            case 5123: return 2;                          //(UNSIGNED_SHORT)2
            case 5124: return 4;                          //(SIGNED_INT)4
            case 5125: return 4;                          //(UNSIGNED_INT)4
            case 5126: return 4;                          //(FLOAT)
        }

        return -1;
    }

    int GltfHelper::GetDimensions(const std::string& str)
    {
        SPICES_PROFILE_ZONE;

        if      (str == "SCALAR")    return  1;
        else if (str == "VEC2")      return  2;
        else if (str == "VEC3")      return  3;
        else if (str == "VEC4")      return  4;
        else if (str == "MAT4")      return  4 * 4;
        else return  -1;
    }

    void GltfHelper::SplitGltfAttribute(std::string attribute, std::string* semanticName, uint32_t* semanticIndex)
    {
        SPICES_PROFILE_ZONE;

        *semanticIndex = 0;

        if (isdigit(attribute.back()))
        {
            *semanticIndex = attribute.back() - '0';

            attribute.pop_back();
            if (attribute.back() == '_')
            {
                attribute.pop_back();
            }
        }

        *semanticName = attribute;
    }

    glm::vec4 GltfHelper::GetVector(const Json::array_t& accessor)
    {
        SPICES_PROFILE_ZONE;

        if (accessor.size() == 0)
        {
            return glm::vec4(0.0f);
        }
        else if(accessor.size() == 1)
        {
            return glm::vec4(accessor[0], 0.0f, 0.0f, 0.0f);
        }
        else if (accessor.size() == 2)
        {
            return glm::vec4(accessor[0], accessor[1], 0.0f, 0.0f);
        }
        else if (accessor.size() == 3)
        {
            return glm::vec4(accessor[0], accessor[1], accessor[2], 0.0f);
        }
        else
        {
            return glm::vec4(accessor[0], accessor[1], accessor[2], accessor[3]);
        }
    }

    glm::mat4 GltfHelper::GetMatrix(const Json::array_t& accessor)
    {
        SPICES_PROFILE_ZONE;

        return glm::mat4(
            glm::vec4(accessor[0],  accessor[1],  accessor[2],  accessor[3]),
            glm::vec4(accessor[4],  accessor[5],  accessor[6],  accessor[7]),
            glm::vec4(accessor[8],  accessor[9],  accessor[10], accessor[11]),
            glm::vec4(accessor[12], accessor[13], accessor[14], accessor[15])
        );
    }

    std::string GltfHelper::GetElementString(const Json::object_t& root, const char* path, const std::string& pDefault)
    {
        SPICES_PROFILE_ZONE;

        return GetElement<std::string>(&root, path, pDefault);
    }

    bool GltfHelper::GetElementBoolean(const Json::object_t& root, const char* path, bool pDefault)
    {
        SPICES_PROFILE_ZONE;

        return GetElement<bool>(&root, path, pDefault);
    }

    float GltfHelper::GetElementFloat(const Json::object_t& root, const char* path, float pDefault)
    {
        SPICES_PROFILE_ZONE;

        return GetElement<float>(&root, path, pDefault);
    }

    int GltfHelper::GetElementInt(const Json::object_t& root, const char* path, int pDefault)
    {
        SPICES_PROFILE_ZONE;

        return GetElement<int>(&root, path, pDefault);
    }

    GltfHelper::Json::array_t GltfHelper::GetElementJsonArray(const Json::object_t& root, const char* path, Json::array_t pDefault)
    {
        SPICES_PROFILE_ZONE;

        return GetElement<Json::array_t>(&root, path, pDefault);
    }

    glm::vec4 GltfHelper::GetElementVector(Json::object_t& root, const char* path, glm::vec4 pDefault)
    {
        SPICES_PROFILE_ZONE;

        if (root.find(path) != root.end() && !root[path].is_null())
        {
            return GetVector(root[path].get<Json::array_t>());
        }
        else
        {
            return pDefault;
        }
    }
}