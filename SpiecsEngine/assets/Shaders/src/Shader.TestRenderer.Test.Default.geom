#version 460

#define CUBE_FACE 6

layout (triangles, invocations = CUBE_FACE) in;

layout (triangle_strip, max_vertices = 3) out;
layout(location = 0) out vec3 color;

void main()
{
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_Layer = gl_InvocationID;
		vec4 pos = gl_in[i].gl_Position;
		gl_Position = pos;
		color = gl_Layer < 1 ? vec3(1.0f, 0.0f, 0.0f) : vec3(0.0f, 1.0f, 0.0f);
		EmitVertex();
	}

	EndPrimitive();
}