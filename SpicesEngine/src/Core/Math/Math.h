#pragma once
#include "Core/Core.h"
#include "Core/Input/KeyCodes.h"

namespace Spices {

    struct UInt2
    {
        UInt2() = default;
        UInt2(const uint32_t& x, const uint32_t& y) : x(x), y(y) {}
        virtual ~UInt2() = default;

        uint32_t x = 0;
        uint32_t y = 0;

        bool operator==(const UInt2& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct String2
    {
        String2() = default;
        String2(const std::string& str0, const std::string& str1) : x(str0), y(str1) {}
        virtual ~String2() = default;

        std::string x;
        std::string y;

        bool operator==(const String2& other) const {
            return x == other.x && y == other.y;
        }
    };

    bool DecomposeTransform(
        const glm::mat4& transform, 
        glm::vec3&       outTranslation, 
        glm::vec3&       outRotation,
        glm::vec3&       outScale
    );

    glm::mat4 PerspectiveMatrix(float fov, float nearPlane, float farPlane, float aspectRatio);
    glm::mat4 OtrhographicMatrix(float left, float right, float top, float bottom, float nearPlane, float farPlane);
    
    glm::mat4 PerspectiveMatrixInverseZ(float fov, float nearPlane, float aspectRatio);

#ifdef RENDERAPI_VULKAN

    /**
    * @brief Convert a Mat4x4 to the matrix required by acceleration structures.
    */
    inline VkTransformMatrixKHR ToVkTransformMatrixKHR(const glm::mat4& matrix)
    {
        // VkTransformMatrixKHR uses a row-major memory layout, while glm::mat4
        // uses a column-major memory layout. We transpose the matrix so we can
        // memcpy the matrix's data directly.
        const glm::mat4 temp = glm::transpose(matrix);
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
    template<> struct hash<Spices::UInt2> {
        size_t operator()(Spices::UInt2 const& info) const noexcept
        {
            return (hash<uint32_t>()(info.x) ^
                (hash<uint32_t>()(info.y) << 1));
        }
    };

    template<> struct hash<Spices::String2> {
        size_t operator()(Spices::String2 const& info) const noexcept
        {
            return (hash<std::string>()(info.x) ^
                (hash<std::string>()(info.y) << 1));
        }
    };
}