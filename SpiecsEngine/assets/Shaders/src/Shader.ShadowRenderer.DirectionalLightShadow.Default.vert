#version 460

// vertex input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

layout(push_constant) uniform Push {
	mat4 model;                                    /*Model Matrix*/
	int entityID;                                  /*Entity ID*/
} push;

void main() 
{
	gl_Position = push.model * vec4(position, 1.0f);
}