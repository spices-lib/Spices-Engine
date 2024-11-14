#include "Pchheader.h"
#include "GltfHelper.h"

namespace Spices {

    int GltfHelper::GetFormatSize(int id)
    {
        SPICES_PROFILE_ZONE;

        switch (id)
        {
            case 5120: return 1; //(BYTE)
            case 5121: return 1; //(UNSIGNED_BYTE)1
            case 5122: return 2; //(SHORT)2
            case 5123: return 2; //(UNSIGNED_SHORT)2
            case 5124: return 4; //(SIGNED_INT)4
            case 5125: return 4; //(UNSIGNED_INT)4
            case 5126: return 4; //(FLOAT)
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

        return glm::vec4(accessor[0], accessor[1], accessor[2], (accessor.size() == 4) ? accessor[3] : 0);
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

    std::string GltfHelper::GetElementString(const Json::object_t& root, const char* path, std::string pDefault)
    {
        return GetElement<std::string>(&root, path, pDefault);
    }

    bool GltfHelper::GetElementBoolean(const Json::object_t& root, const char* path, bool default)
    {
        return GetElement<bool>(&root, path, default);
    }

    float GltfHelper::GetElementFloat(const Json::object_t& root, const char* path, float default)
    {
        return GetElement<float>(&root, path, default);
    }

    int GltfHelper::GetElementInt(const Json::object_t& root, const char* path, int default)
    {
        return GetElement<int>(&root, path, default);
    }

    GltfHelper::Json::array_t GltfHelper::GetElementJsonArray(const Json::object_t& root, const char* path, Json::array_t default)
    {
        return GetElement<Json::array_t>(&root, path, default);
    }

    glm::vec4 GltfHelper::GetElementVector(Json::object_t& root, const char* path, glm::vec4 default)
    {
        if (root.find(path) != root.end() && !root[path].is_null())
        {
            return GetVector(root[path].get<Json::array_t>());
        }
        else
        {
            return default;
        }
    }
}