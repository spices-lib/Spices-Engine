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