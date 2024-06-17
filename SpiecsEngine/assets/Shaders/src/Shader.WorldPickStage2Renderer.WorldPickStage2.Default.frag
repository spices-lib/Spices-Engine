#version 460

// frag input
layout(location = 0) in struct FragInput {
	vec2 texCoord;
} fragInput;

layout(location = 0) out vec4 outColor;

// push constant

// uniform buffer
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

layout(set = 1, binding = 0) uniform sampler2D selectBuffer;

float SampleWithOffest(vec2 uv_offest);
float Sobel(float MatColor[3][3]);
float EdgeClear(float mask);

void main()
{
	float colors[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (SampleWithOffest(vec2(-1.0f + i, -1.0f + j)) > 0.5f)
			{
				colors[i][j] = 1;
			}
			else
			{
				colors[i][j] = 0;
			}
		}
	}

	float outLine = Sobel(colors);
	outLine = EdgeClear(outLine);

	if (outLine < 0.1f) discard;

	outColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}

float SampleWithOffest(vec2 uv_offest)
{
	float outLineWidth = min(fwidth(fragInput.texCoord.x * view.sceneTextureSize.x), fwidth(fragInput.texCoord.y * view.sceneTextureSize.y));
	vec2 uv = fragInput.texCoord + uv_offest * view.sceneTextureSize.zw * outLineWidth * 0.5f;
	return texture(selectBuffer, uv).x;
}

float Sobel(float MatColor[3][3])
{
	float Gridx = -1.0f * MatColor[0][0] - 2.0f * MatColor[1][0] - 1.0f * MatColor[2][0] +
		1.0f * MatColor[0][2] + 2.0f * MatColor[1][2] + 1.0f * MatColor[2][2];

	float Gridy = -1.0f * MatColor[2][0] - 2.0f * MatColor[2][1] - 1.0f * MatColor[2][2] +
		1.0f * MatColor[0][0] + 2.0f * MatColor[0][1] + 1.0f * MatColor[0][2];

	return smoothstep(0.0f, 1.0f, sqrt(Gridx * Gridx + Gridy * Gridy));
}

float EdgeClear(float mask)
{
	if (fragInput.texCoord.x < 0.001f || 
		fragInput.texCoord.x > 0.999f || 
		fragInput.texCoord.y < 0.001f || 
		fragInput.texCoord.y > 0.999f
		)
	{
		mask = 0.0f;
	}

	return mask;
}