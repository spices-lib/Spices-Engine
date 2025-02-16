/**
* @file Math.h.
* @brief The Math Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Core/Input/KeyCodes.h"

namespace Spices {

    /**
    * @brief double unsigned int
    */
    struct UInt2
    {
    public:

        /**
        * @brief Constructor Function.
        */
        UInt2() = default;

        /**
        * @brief Constructor Function.
        * @param[in] x x component.
        * @param[in] y y component.
        */
        UInt2(const uint32_t& x, const uint32_t& y) : x(x), y(y) {}

        /**
        * @brief Destructor Function.
        */
        virtual ~UInt2() = default;

        /**
        * @brief equal operation.
        * @param[in] other another UInt2.
        * @reutrn Returns true if equal.
        */
        bool operator==(const UInt2& other) const {
            return x == other.x && y == other.y;
        }

    public:

        /**
        * @brief x component.
        */
        uint32_t x = 0;

        /**
        * @brief y component.
        */
        uint32_t y = 0;
    };

    /**
    * @brief double string
    */
    struct String2
    {
    public:

        /**
        * @brief Constructor Function.
        */
        String2() = default;

        /**
        * @brief Constructor Function.
        * @param[in] str0 x component.
        * @param[in] str1 y component.
        */
        String2(const std::string& str0, const std::string& str1) : x(str0), y(str1) {}

        /**
        * @brief Destructor Function.
        */
        virtual ~String2() = default;

        /**
        * @brief equal operation.
        * @param[in] other another String2.
        * @reutrn Returns true if equal.
        */
        bool operator==(const String2& other) const {
            return x == other.x && y == other.y;
        }

    public:

        /**
        * @brief x component.
        */
        std::string x;

        /**
        * @brief y component.
        */
        std::string y;
    };

    /**
    * @brief Decompose matrix to split SRT transform.
    * @param[in] transform Transform Matrix.
    * @param[in] outTranslation split translate.
    * @param[in] outRotation split rotation.
    * @param[in] outScale split scale.
    * @return Returns true if split succeed.
    */
    bool DecomposeTransform(
        const glm::mat4& transform, 
        glm::vec3&       outTranslation, 
        glm::vec3&       outRotation,
        glm::vec3&       outScale
    );

    /**
    * @brief Calculate Perspective Matrix.
    * @param[in] fov .
    * @param[in] nearPlane .
    * @param[in] farPlane .
    * @param[in] aspectRatio .
    * @return Returns Perspective Matrix.
    */
    glm::mat4 PerspectiveMatrix(float fov, float nearPlane, float farPlane, float aspectRatio);

    /**
    * @brief Calculate Otrhographic Matrix.
    * @param[in] left .
    * @param[in] right .
    * @param[in] top .
    * @param[in] bottom .
    * @param[in] nearPlane .
    * @param[in] farPlane .
    * @return Returns Otrhographic Matrix.
    */
    glm::mat4 OtrhographicMatrix(float left, float right, float top, float bottom, float nearPlane, float farPlane);
    
    /**
    * @brief Calculate Perspective Matrix(reverse z version).
    * @param[in] fov .
    * @param[in] nearPlane .
    * @param[in] aspectRatio .
    * @return Returns Perspective Matrix(reverse z version).
    */
    glm::mat4 PerspectiveMatrixInverseZ(float fov, float nearPlane, float aspectRatio);

#ifdef RENDERAPI_VULKAN

    /**
    * @brief Convert a Mat4x4 to the matrix required by acceleration structures.
    * @param[in] matrix Transform Matrix.
    * @return Returns VkTransformMatrixKHR.
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