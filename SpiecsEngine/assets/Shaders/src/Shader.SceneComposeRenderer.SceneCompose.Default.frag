#version 450

// frag input
layout(location = 0) in struct FragInput {
	vec2 texCoord;
} fragInput;

layout(location = 0) out vec4 outColor;


layout(set = 0, binding = 0) uniform View {
	mat4 projection;
	mat4 view;
	mat4 inView;
	vec4 sceneTextureSize;
	vec4 windowSize;
} view;

layout(set = 0, binding = 1) uniform SpiecsInput {
	vec4 mousePos;
	float gameTime;
	float frameTime;
} spiecsInput;

layout(input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput GBuffer[4];

void main()
{
	vec3 color = subpassLoad(GBuffer[0]).rgb;
	outColor = vec4(color, 1.0f);
}

