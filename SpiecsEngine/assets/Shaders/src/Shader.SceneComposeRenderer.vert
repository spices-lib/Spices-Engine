#version 450

// vertex input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

// vertex output
layout(location = 0) out struct FragInput{
    vec2 texCoord;
} vertOut;

// push constant
layout(push_constant) uniform Push {
	vec2 viewPortPos;
	vec2 viewPortSize;
	vec2 windowSize;
} push;

void main()
{
	vertOut.texCoord = (texCoord * push.windowSize) / push.viewPortSize;
	gl_Position = vec4(position * 2.0f, 1.0f);
}