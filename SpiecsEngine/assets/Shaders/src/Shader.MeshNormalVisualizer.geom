/************************************Pre Compile*******************************************/

#version 460

/*****************************************************************************************/

/************************************Geometry Input***************************************/

/**
* @brief From Input Assembly.
*/
layout (triangles) in;

/**
* @brief Geometry Shader Input From Vertex Shader.
*/
layout(location = 0) in struct GeomInput {
    vec3 position;                         /*World Postion*/
    vec3 normal;                           /*World Normal*/
} geomInput[];

/*****************************************************************************************/

/************************************Geometry Output**************************************/

/**
* @brief Switch Input Assembly from triangles to line_strip.
* Set Max 2 points per line.
*/
layout (line_strip, max_vertices = 2) out;

/**
* @brief Geometry Shader Output to Fragment Shader.
*/
layout(location = 0) out struct GeomOut {
    vec3 color;                            /*Fragment Color*/
} geomOut;

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

void main(void)
{
    for(int i = 0; i < gl_in.length(); i++)
    {
        vec3 pos = gl_in[i].gl_Position.xyz;
        vec3 normal = geomInput[i].normal;
        gl_Position = view.projection * view.view * vec4(pos, 1.0f);
        geomOut.color = vec3(1.0f, 1.0f, 0.0f);
        EmitVertex();

        gl_Position = view.projection * view.view * vec4(pos + normal * 0.2f, 1.0f);
        geomOut.color = vec3(1.0f, 1.0f, 0.0f);
        EmitVertex();

        EndPrimitive();
    }
}

/*****************************************************************************************/