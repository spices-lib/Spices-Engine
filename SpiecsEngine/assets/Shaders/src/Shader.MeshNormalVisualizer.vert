/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************Vertex Input*****************************************/

/**
* @brief Vertex Shader Input from InputAssembly.
*/
layout(location = 0) in vec3 position;             /*Vertex Position*/
layout(location = 1) in vec3 normal;               /*Vertex Normal*/
layout(location = 2) in vec3 color;                /*Vertex Color*/
layout(location = 3) in vec2 texCoord;             /*Vertex UV*/

/*****************************************************************************************/

/************************************Vertex Output****************************************/

/**
* @brief Vertex Shader Output to Geometry Shader.
*/
layout(location = 0) out struct VertOut {
    vec3 position;                                 /*World Position*/
    vec3 normal;                                   /*World Normal*/
} vertOut;

/*****************************************************************************************/

/*********************************Push Constant*******************************************/

/**
* @brief push constant.
*/
layout(push_constant) uniform Push {
    mat4 model;                                    /*Model Matrix*/
    int entityID;                                  /*Entity ID*/
} push;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    mat3 m3model = mat3(transpose(inverse(push.model)));
    vec3 normal = normalize(m3model * normal);

    vec4 pos = push.model * vec4(position, 1.0f);

    vertOut.position = pos.xyz;
    vertOut.normal = normal;

    gl_Position = pos;
}

/*****************************************************************************************/