/**
* @file ShaderMaterialAttributes.glsl.
* @brief This Shader Header File Defines MaterialAttributes Structure and Functions.
* @author Spices.
*/

/************************************Pre Compile******************************************/

#ifndef SHADER_MATERIAL_ATTRIBUTES
#define SHADER_MATERIAL_ATTRIBUTES

#include "ShaderCommon.h"

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Get Material Attributes, must be implementated by specific rchit shader.
* @param[in] pi Intersected Pixel.
* @param[in,out] attributes specific MaterialAttributes.
* @attention This function must be implement in specific shader.
*/
void GetMaterialAttributes(const in Pixel pi, inout MaterialAttributes attributes);

/**
* @brief Reverse Back face normal.
* @param[in] materialAttributes MaterialAttributes.
* @attention This function must be implement in specific shader.
*/
void ReverseBackFaceNormal(inout MaterialAttributes attr);

/**
* @brief Init Material Attributes.
* @param[in] pi Intersected Pixel.
* @return Return specific MaterialAttributes.
*/
MaterialAttributes InitMaterialAttributes(const in Pixel pi)
{
    MaterialAttributes attributes;
    
    attributes.albedo           = vec3(0.5f);     /* @brief 50% energy reflect.                   */
    attributes.roughness        = 1.0f;           /* @brief 100% random direction reflect.        */
    attributes.metallic         = 0.0f;
    attributes.emissive         = vec3(0.0f);     /* @brief self no energy.                       */
    attributes.normal           = pi.normal;      /* @brief Pixel World Normal.                   */
    attributes.maxRayDepth      = 1;              /* @brief Pixel Ray Tracing Max Depth.          */
    attributes.maxLightDepth    = 1;              /* @brief Pixel Ray Tracing Max Light Depth.    */
    attributes.maxShadowDepth   = 1;              /* @brief Pixel Ray Tracing Max Shadow Depth.   */
    
    return attributes;
}

/**
* @brief MaterialAttributes Post Handle.
* @param[in,out] attr MaterialAttributes.
*/
void PostHandleWithMaterialAttributes(inout MaterialAttributes attr)
{
    ReverseBackFaceNormal(attr);

    attr.albedo             = clamp(attr.albedo, vec3(0.0f), vec3(1.0f));    /* @brief Clamp to  0.0f - 1.0f.       */
    attr.roughness          = clamp(attr.roughness, 0.0f, 1.0f);             /* @brief Clamp to  0.0f - 1.0f.       */
    attr.metallic           = clamp(attr.metallic, 0.0f, 1.0f);              /* @brief Clamp to  0.0f - 1.0f.       */
    attr.normal             = normalize(attr.normal);
    attr.maxRayDepth        = min(max(0, attr.maxRayDepth), 12);             /* @brief Clamp to  0 - 12.            */
    attr.maxLightDepth      = min(max(0, attr.maxLightDepth), 12);           /* @brief Clamp to  0 - 12.            */
    attr.maxShadowDepth     = min(max(0, attr.maxShadowDepth), 12);          /* @brief Clamp to  0 - 12.            */
}

/*****************************************************************************************/

#endif