#pragma once
#include "Core/Core.h"

namespace Spiecs {

    struct Int2
    {
        Int2() {};
        Int2(const uint32_t& x, const uint32_t& y) : x(x), y(y) {};
        virtual ~Int2() {};

        int x = 0;
        int y = 0;

        bool operator==(const Int2& other) const {
            return x == other.x && y == other.y;
        };
    };

    struct String2
    {
        String2() {};
        String2(const std::string& str0, const std::string& str1) : x(str0), y(str1) {};
        virtual ~String2() {};

        std::string x;
        std::string y;

        bool operator==(const String2& other) const {
            return x == other.x && y == other.y;
        };
    };

    bool DecomposeTransform(
        const glm::mat4& transform, 
        glm::vec3&       outTranslation, 
        glm::vec3&       outRotation,
        glm::vec3&       outScale
    );

    glm::mat4 Otrhographic(float left, float right, float top, float bottom, float nearPlane, float farPlane);

#ifdef RENDERAPI_VULKAN

    /**
    * @brief Convert a Mat4x4 to the matrix required by acceleration structures.
    */
    inline VkTransformMatrixKHR ToVkTransformMatrixKHR(const glm::mat4& matrix)
    {
        // VkTransformMatrixKHR uses a row-major memory layout, while glm::mat4
        // uses a column-major memory layout. We transpose the matrix so we can
        // memcpy the matrix's data directly.
        glm::mat4            temp = glm::transpose(matrix);
        VkTransformMatrixKHR out_matrix;
        memcpy(&out_matrix, &temp, sizeof(VkTransformMatrixKHR));
        return out_matrix;
    }

#endif

}

namespace std {

    /**
    * @brief Hash function used for unordered_map.
    */
    template<> struct hash<Spiecs::Int2> {
        size_t operator()(Spiecs::Int2 const& info) const {
            return (hash<int>()(info.x) ^
                (hash<int>()(info.y) << 1));
        }
    };

    template<> struct hash<Spiecs::String2> {
        size_t operator()(Spiecs::String2 const& info) const {
            return (hash<std::string>()(info.x) ^
                (hash<std::string>()(info.y) << 1));
        }
    };
}