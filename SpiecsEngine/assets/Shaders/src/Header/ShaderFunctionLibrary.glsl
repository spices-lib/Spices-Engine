/**
* @file ShaderFunctionLibrary.glsl.
* @brief This Shader Header File Defines Shader Functions.
* @author Spiecs.
*/

/************************************Pre Compile*******************************************/

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

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(-v.z, v.x), asin(-v.y));
    uv *= invAtan;
    uv += 0.5;

    return uv;
}

/*****************************************************************************************/