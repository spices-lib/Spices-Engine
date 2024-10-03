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
#include "Header/ShaderSampleDigit.glsl"

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

const vec2 vFontSize = vec2(8.0, 15.0);

/*****************************************************************************************/

/******************************************Functions**************************************/

/**
* @brief Get a digital number's level by using POW(l) macros.
* @param[in] v digital number.
* @param[in] minl Min Level for search.
* @param[in] s scale ratio. 
* @return Returns level.
*/
int GetDigitalNumberLevel(in float v, in int minl, in float s)
{
    int l = minl;
    for(;;)
    {
        if (s * POW(l) > abs(v)) break;
        l++;
    }
    
    return l;
}

/**
* @brief Draw editor grid lines in world.
* @param[in] sd Grid World Position.
* @param[in] t distance from ro to sd.
* @param[in] l Specific Level of grid.
* @return Returns Color. 
*/
vec4 DrawEditorGridLines(in vec3 sd, in float t, in int l)
{
    vec2 uv        = sd.xz / POW(l);
    vec2 d         = fwidth(uv);
    float minz     = min(d.y, 1.0f);
    float minx     = min(d.x, 1.0f);
    
    /**
    * @brief Draw grid lines.
    */
    vec2 grid      = abs(fract(uv - 0.5f) - 0.5f) / d;
    float line     = 1.0f - min(min(grid.x, grid.y), 1.0f);
    vec4 color     = vec4(0.2f, 0.2f, 0.2f, line);
    color.w       *= 1.0f - smoothstep(POW(-1) * viewAdaption * POW(l), viewAdaption * POW(l), t);

    /**
    * @brief Draw x/z axis.
    */
    if (sd.x > -minx * POW(l) && sd.x < minx * POW(l)) color.z = 1.0f;
    if (sd.z > -minz * POW(l) && sd.z < minz * POW(l)) color.x = 1.0f;

    return color;
}

vec4 DrawEditorGridDigitalNumber(in vec4 color, in vec3 sd, in float t, in int l, in vec2 vFontSize)
{
    vec2 uv        = sd.xz / POW(l + 1);
    vec2 duv       = fract(uv) / fwidth(uv);
    vec2 v         = floor(uv) * POW(l + 1);
    
    if(abs(v.x) >= abs(v.y))
    {
        if(abs(uv.y - 0.5f) < 0.5f)
        {
            float xa       = 1.0f;
            int vxl        = GetDigitalNumberLevel(v.x, 0, 1.0f);
            if(floor(uv.x) < 0.1f) vxl++;
            if(floor(mod(v.x, POW(l + 2))) != 0.0f)
            {
                xa        *= 1.0f - smoothstep(POW(-1) * viewAdaption * POW(l), viewAdaption * POW(l), t);
            }
            return mix(color, vec4(1.0f, 0.0f, 0.0f, xa), PrintValue(duv, vec2(0.0f), vFontSize ,v.x , vxl, 0.0f));
        }
    }
    else
    {
        if(abs(uv.x - 0.5f) < 0.5f)
        {
            float ya       = 1.0f;
            int vyl        = GetDigitalNumberLevel(v.y, 0, 1.0f);
            if(floor(uv.y) < 0.1f) vyl++;
            if(floor(mod(v.y, POW(l + 2))) != 0.0f)
            {
                ya        *= 1.0f - smoothstep(POW(-1) * viewAdaption * POW(l), viewAdaption * POW(l), t);
            }
            return mix(color, vec4(0.0f, 0.0f, 1.0f, ya), PrintValue(duv, vec2(0.0f), vFontSize ,v.y , vyl, 0.0f));
        }
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
    * Start from 0.1m, multiple 10  per level.
    */
    int level = GetDigitalNumberLevel(t, -1, viewAdaption);

    /**
    * @brief Draw Grids.
    */
    vec4 a = DrawEditorGridLines(sd, t, level);
    vec4 b = DrawEditorGridLines(sd, t, level + 1);
    vec4 c = DrawEditorGridDigitalNumber(max(a, b), sd, t, level, vFontSize);
    
    c.w   *= mix(0.0f, 1.0f, max(dot(direction.xyz, vec3(0.0f, -1.0f, 0.0f)), 0.0f));
    if(c.w < 0.01f) discard;
    outSceneColor    = c;
}

/*****************************************************************************************/