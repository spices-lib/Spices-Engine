/**
* @file SpriteMaterial.h.
* @brief The SpriteMaterial Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Material.h"

namespace Spiecs {

    /**
    * @brief SpriteMaterial Class.
    */
    class SpriteMaterial : public Material
    {
    public:

        /**
        * @brief Constructor Function.
        */
        SpriteMaterial() {};

        /**
        * @brief Constructor Function.
        * Init member veriables and Dserialize.
        * @param[in] materialPath The path of material.
        */
        SpriteMaterial(const std::string& materialPath) : Material(materialPath) {};

        /**
        * @brief Destructor Function.
        */
        virtual ~SpriteMaterial() {};

    private:

        /**
        * @brief This interface is inherited from Material.
        * It defines how we build texture and descriptor set.
        */
        virtual void BuildMaterial() override;
    };

}