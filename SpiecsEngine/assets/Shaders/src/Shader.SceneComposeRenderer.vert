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
	vec4 gbufferSize;
	vec4 windowSize;
} push;

void main()
{
	vertOut.texCoord = (texCoord * push.windowSize.xy) / push.gbufferSize.xy;
	gl_Position = vec4(position * 2.0f, 1.0f);
}