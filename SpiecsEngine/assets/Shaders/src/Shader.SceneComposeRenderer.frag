#version 450

layout(input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput BaseColor;
layout(input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput NormalColor;

layout(set = 1, binding = 0) uniform Test {
	int e;
} test;

layout(location = 0) out vec4 outColor;

void main()
{
	// Read color from previous color input attachment
	vec3 color = subpassLoad(NormalColor).rgb;
	outColor = vec4(color, 1.0f);
	//outColor = vec4(0.8f);
}