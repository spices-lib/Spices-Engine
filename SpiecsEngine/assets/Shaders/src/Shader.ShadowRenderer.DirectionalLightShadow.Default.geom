#version 460

#define MAX_DIRECTIONALLIGHT_NUM 2

layout(triangles, invocations = MAX_DIRECTIONALLIGHT_NUM) in;

layout(triangle_strip, max_vertices = 3) out;
layout(location = 0) out vec3 color;

void main()
{
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_Layer = gl_InvocationID;
		vec4 pos = gl_in[i].gl_Position;
		pos.z = 1.0f;
		gl_Position = pos;
		color = vec3(sin(gl_InvocationID), cos(gl_InvocationID), sin(45 + gl_InvocationID));
		EmitVertex();
	}

	EndPrimitive();
}