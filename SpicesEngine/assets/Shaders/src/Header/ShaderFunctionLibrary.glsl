/**
* @file ShaderFunctionLibrary.glsl.
* @brief This Shader Header File Defines Shader Functions.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#ifndef SHADER_FUNCTION_LIBRARY
#define SHADER_FUNCTION_LIBRARY

#include "ShaderCommon.h"

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Generate a random unsigned int from two unsigned int values, using 16 pairs
* of rounds of the Tiny Encryption Algorithm. See Zafar, Olano, and Curtis,
* "GPU Random Numbers via the Tiny Encryption Algorithm".
* @param[in] val0 Input Value 0.
* @param[in] val1 Input Value 1.
* @return Returns the Random uint.
*/
uint tea(uint val0, uint val1)
{
	uint v0 = val0;
	uint v1 = val1;
	uint s0 = 0;

	for (uint n = 0; n < 16; n++)
	{
		s0 += 0x9e3779b9;
		v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
		v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
	}

	return v0;
}

/**
* @brief Generate a random unsigned int in [0, 2^24) given the previous RNG state
* using the Numerical Recipes linear congruential generator.
* @param[in out] prev Input Output Value.
* @return Returns the Random uint.
*/
uint lcg(inout uint prev)
{
	uint LCG_A = 1664525u;
	uint LCG_C = 1013904223u;
	prev = (LCG_A * prev + LCG_C);
	return prev & 0x00FFFFFF;
}

/**
* @brief Generate a random float in [0, 1) given the previous PNG state
* @param[in out] prev Input Output Value.
* @return Returns the Random float.
*/
float rnd(inout uint prev)
{
	return (float(lcg(prev)) / float(0x01000000));
}

/**
* @brief Randomly samples from a cosine-weighted hemisphere oriented in the `z` direction.
* From Ray Tracing Gems section 16.6.1, "Cosine-Weighted Hemisphere Oriented to the Z-Axis".
* @param[in out] seed Random Select Seed.
* @param[in] x Coordinate X axis.
* @param[in] y Coordinate Y axis.
* @param[in] z Coordinate X axis.
* @return Return Sampled Vector in Hemisphere.
*/
vec3 SamplingHemisphere(inout uint seed, in vec3 x, in vec3 y, in vec3 z)
{    
	float r1 = rnd(seed);
	float r2 = rnd(seed);
	float sq = sqrt(r1);

	vec3 direction = vec3(cos(2 * PI * r2) * sq, sin(2 * PI * r2) * sq, sqrt(1. - r1));
	direction = direction.x * x + direction.y * y + direction.z * z;

	return normalize(direction);
}

/**
* @brief Return the tangent and binormal from the incoming normal.
* @param[in] N Normal.
* @param[out] Nt Tangent.
* @param[out] Nb Bitangent.
*/ 
void CreateCoordinateSystem(in vec3 N, out vec3 Nt, out vec3 Nb)
{
	if (abs(N.x) > abs(N.y))
		Nt = vec3(N.z, 0, -N.x) / sqrt(N.x * N.x + N.z * N.z);
	else
		Nt = vec3(0, -N.z, N.y) / sqrt(N.y * N.y + N.z * N.z);
	Nb = cross(N, Nt);
}

/**
* @brief Transform space TextureCube sampler uv to flatten uv.
* @param[in] v space TextureCube sampler uv.
* @return Returns flatten uv.
*/
vec2 SampleSphericalMap(in vec3 v)
{
    vec2 uv = vec2(atan(-v.z, v.x), asin(-v.y));
    uv *= invAtan;
    uv += 0.5;

    return uv;
}

/**
* @brief BRDF Diffuse part, use lambert model.
* @param[in] albedo .
* @return Returns diffuse.
*/
vec3 BRDF_Diffuse_Lambert(in vec3 albedo)
{
	return albedo / PI;
}

/**
* @brief BRDF Specular (Normal Distribution function) part.
* use Cook-Torrance Specular model.
* use Trowbridge-Reitz GGX for Normal distribution.
* @param[in] dotNH .
* @param[in] roughness .
* @return Returns specular (Normal Distribution function) part.
*/
float BRDF_Specular_D_GGX(in float dotNH, in float roughness)
{
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;
	float denom = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
	return alpha2 / (PI * denom * denom); 
}

/**
* @brief BRDF Specular (Geometric shadowing function) part.
* @param[in] dotNL dot(Normal, LightDir).
* @param[in] dotNV dot(Normal, ViewDir).
* @param[in] roughness .
* @return Returns specular (Geometric shadowing function) part.
*/
float BRDF_Specular_G_SchlicksmithGGX(in float dotNL, in float dotNV, in float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0f;
	float GL = dotNL / (dotNL * (1.0 - k) + k);
	float GV = dotNV / (dotNV * (1.0 - k) + k);
	return GL * GV;
}

/**
* @brief BRDF Specular (Fresnel function) part.
* use Schlick model.
* @param[in] cosTheta 
* @param[in] dotNV dot(Normal, ViewDir).
* @param[in] metallic .
* @return Returns specular (Fresnel function) part.
*/
vec3 BRDF_Specular_F_Schlick(in float dotNV, in vec3 albedo, in float metallic)
{
	vec3 F0 = mix(vec3(0.04f), albedo, metallic); // * material.specular
	vec3 F = F0 + (1.0 - F0) * pow(1.0 - dotNV, 5.0); 
	return F;
}

/**
* @brief BRDF Specular part, use Cook-Torrance model.
* @param[in] L LightDirection.
* @param[in] V ViewDirection.
* @param[in] N Normal.
* @param[in] lightColor .
* @param[in] albedo .
* @param[in] metallic .
* @param[in] roughness .
*/
vec3 BRDF_Specular_CookTorrance(in vec3 L, in vec3 V, in vec3 N, in vec3 lightColor, in vec3 albedo, in float metallic, in float roughness)
{
	if(dot(L, N) < 0) return vec3(0.0f);

	float rroughness = max(0.05f, roughness);
    vec3 H = normalize (V + L);
	float dotNV = clamp(dot(N, V), 0.0, 1.0);
	float dotNL = clamp(dot(N, L), 0.0, 1.0);
	float dotLH = clamp(dot(L, H), 0.0, 1.0);
	float dotNH = clamp(dot(N, H), 0.0, 1.0);

	/**
	* @brief Calaulate specular - Normal distribution (Distribution of the microfacets).
	*/
	float D = BRDF_Specular_D_GGX(dotNH, rroughness);
	
	/**
	* @brief Calaulate specular - Geometric shadowing term (Microfacets shadowing).
	*/
	float G = BRDF_Specular_G_SchlicksmithGGX(dotNL, dotNV, rroughness);
	
	/**
	* @brief Calaulate specular - Fresnel factor (Reflectance depending on angle of incidence)
	*/
	vec3 F = BRDF_Specular_F_Schlick(dotNV, albedo, metallic);

	vec3 spec = D * F * G / (4.0 * dotNL * dotNV);

    /**
    * @brief Use Clamp to Handle “firefly” effect.
    * @todo Bi-Directional Path Tracing.
    */
    //if(max(max(spec.x, spec.y), spec.z) > 5.0f) return vec3(0.0f);
	return spec * dotNL * lightColor;
}

/*****************************************************************************************/

#endif