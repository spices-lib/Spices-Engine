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
* @brief Vertex Shader Output to Fragment Shader.
*/
layout(location = 0) out struct VertOut {
    vec3 position;                                 /*World Position*/
    vec3 normal;                                   /*World Position*/
    vec3 color;                                    /*Fragment Color*/
    vec2 texCoord;                                 /*Fragment UV*/
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

/*************************************Pre Renderer Data***********************************/

/**
* @brief Global View Struct.
*/
layout(set = 0, binding = 0) uniform View {
    mat4 projection;          /*Projection Matrix from major Camera Entity*/
    mat4 view;                /*View Matrix from major Camera Entity*/
    mat4 inView;              /*Inverse View Matrix from major Camera Entity*/
    vec4 sceneTextureSize;    /*Scene Texture Size*/
    vec4 windowSize;          /*Application Window Size*/
} view;

/**
* @brief Application Inout Struct.
*/
layout(set = 0, binding = 1) uniform SpiecsInput {
    vec4 mousePos;            /*Mouse Postion and inverse position*/
    float gameTime;           /*Application Run time since start*/
    float frameTime;          /*Duration time since last frame*/
} spiecsInput;

/*****************************************************************************************/

/**********************************Shader Entry*******************************************/

void main()
{
    mat3 m3model = mat3(transpose(inverse(push.model)));
    vec3 normal = normalize(m3model * normal);

    vec4 pos = push.model * vec4(position, 1.0f);

    vertOut.position = pos.xyz;
    vertOut.normal = normal;
    vertOut.color = color;
    vertOut.texCoord = texCoord;
    
    gl_Position = view.projection * view.view * pos;
}

/*****************************************************************************************/