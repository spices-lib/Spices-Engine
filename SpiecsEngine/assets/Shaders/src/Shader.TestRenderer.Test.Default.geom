#version 460

#define CUBE_FACE 6

layout (triangles, invocations = CUBE_FACE) in;

layout (triangle_strip, max_vertices = 3) out;

void main()
{
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_Layer = gl_InvocationID;
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}