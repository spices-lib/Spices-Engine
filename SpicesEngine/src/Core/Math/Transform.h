/**
* @file Transform.h.
* @brief The Transform Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

    /**
    * @brief Wrapper of 3D Transform
    */
    struct Transform
    {
        glm::vec3 position{ 0.0f };  /* @brief Position Component. */
        glm::vec3 rotation{ 0.0f };  /* @brief Rotation Component. */
        glm::vec3 scale   { 1.0f };  /* @brief Scale    Component. */

        /**
        * @brief Get Model Matrix from this Transfrom.
        * @reutrn Returns this transfrom Matrix.
        */
        glm::mat4 ToMatrix();

        /**
        * @brief Get Rotation Matrix.
        * @param[in] rotation Rotation Component.
        * @return Returns Rotation Matrix.
        */
        static glm::mat4 GetRoatationMatrix(const glm::vec3& rotation);

        /**
        * @brief Get Model Matrix from specific Transfrom.
        * @param[in] transform Specific transfrom.
        * @reutrn Returns specific transfrom Matrix.
        */
        static glm::mat4 ToMatrix(const Transform& transform);
    };
}