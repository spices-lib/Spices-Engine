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
}