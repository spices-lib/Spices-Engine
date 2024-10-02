/**
* @file Shader.ViewportGridRenderer.ViewportGrid.Default.frag.
* @brief This Shader Defines ViewportGrid Renderer ViewportGrid SubPass Default Fragemt Shader Behaver.
* @author Spices.
*/

/************************************Pre Compile*******************************************/

#version 460

#extension GL_GOOGLE_include_directive : enable     /* @brief Enable include Macro. */

#include "Header/ShaderCommon.h"
#include "Header/ShaderPreRendererLayout.glsl"
#include "Header/ShaderFunctionLibrary.glsl"

/*****************************************************************************************/

/************************************Fragment Input***************************************/

/**
* @brief Fragment Shader Input From Vertex Shader.
*/
layout(location = 0) in struct FragInput
{ 
    vec2 texCoord;
}
fragInput;

/*****************************************************************************************/

/************************************Fragment Output**************************************/

/**
* @brief Fragment Shader Output to FrameBuffer.
*/
layout(location = 0) out vec4 outSceneColor;    /* @brief SceneColor Attachment. */

/*****************************************************************************************/

/********************************Specific Renderer Data***********************************/

/**
* @brief Simplify format of pow by 10.0f.
*/
#define POW(l) pow(10.0f, l)

/**
* @brief View grid Adaption.
*/
const float viewAdaption = POW(2);

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Draw editor grid lines in world.
* @param[in] sd Grid World Position.
* @param[in] l Specific Level of grid.
* @return Returns Color.
*/
vec4 DrawEditorGridLines(in vec3 sd, in float t, in int l)
{
    vec2 uv      = sd.xz / POW(l);
    vec2 d       = fwidth(uv) ;
    vec2 grid    = abs(fract(uv - 0.5f) - 0.5f) / d;
    float c      = 0.001f * POW(l);

    float line   = min(grid.x, grid.y);
    float minz   = min(d.y, 1.0f);
    float minx   = min(d.x, 1.0f);
    
    vec4 color   = vec4(0.2f, 0.2f, 0.2f, 1.0f - min(line, 1.0f));
    color.w     *= 1.0f - smoothstep(0.1f * viewAdaption * POW(l), viewAdaption * POW(l), t);

    // z axis
    if (sd.x > -c && sd.x < c)
    {
        color.z = 1.0f;
    }
    // x axis
    if (sd.z > -c && sd.z < c)
    {
        color.x = 1.0f;
    }

    return color;
}

/**
* @brief Compute depth from position.
* @param[in] p Position.
* @return Depth.
*/
float ComputeDepth(in vec3 p)
{
    vec4 cp = view.projection * view.view * vec4(p.xyz, 1.0);
    return cp.z / cp.w;
}

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    vec2 d         = fragInput.texCoord * 2.0f - 1.0f;

    /**
    * @brief Calculate Screen World Direction.
    */
    vec4 origin    = view.inView * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 target    = inverse(view.nprojection) * vec4(d.x, d.y, 1.0f, 1.0f);
	vec4 direction = view.inView * vec4(normalize(target.xyz), 0.0f);

    /**
    * @brief Only draw ground grid.
    */
	if (direction.y > 0.0f) discard;

    /**
    * @brief Get sd.
    */
    float t        = -origin.y / direction.y;
    float px       =  origin.x + direction.x * t;
    float pz       =  origin.z + direction.z * t;
    vec3  sd       = vec3(px, 0.0f, pz);

    /**
    * @brief Write depth.
    */
    gl_FragDepth   = ComputeDepth(sd);

    /**
    * @brief Get specific view level.
    */
    int level = -1;  // Start from 0.1m, multiple 10  per level.
    for (;;)
    {
        if (viewAdaption * POW(level) > t)
        {
            break;
        }

        level++;
    }

    /**
    * @brief Draw Grids.
    */
    vec4 a = DrawEditorGridLines(sd, t, level);
    vec4 b = DrawEditorGridLines(sd, t, level + 1);

    outSceneColor  = max(a, b);
    outSceneColor *= mix(0.0f, 1.0f, max(dot(direction.xyz, vec3(0.0f, -1.0f, 0.0f)), 0.0f));
}

/*****************************************************************************************/