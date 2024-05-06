#version 450

// frag input
layout(location = 0) in struct FragInput {
	vec2 texCoord;
} fragInput;

layout(location = 0) out vec4 outColor;

// push constant
layout(push_constant) uniform Push {
	vec2 viewPortPos;
	vec2 viewPortSize;
	vec2 windowSize;
} push;

layout(input_attachment_index = 0, binding = 0) uniform subpassInput GBuffer[4];

layout(set = 1, binding = 0) uniform sampler2D selectBuffer;

float SampleWithOffest(vec2 uv_offest);
float Sobel(float MatColor[3][3]);

void main()
{
	vec3 color = subpassLoad(GBuffer[0]).rgb;
	outColor = vec4(color, 1.0f);

	float colors[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (SampleWithOffest(vec2(-1.0f + i, -1.0f + j)) / 1000000.0f < 1.0f)
			{
				colors[i][j] = 1;
			}
			else
			{
				colors[i][j] = 0;
			}
		}
	}

	//float outLine = Sobel(colors);
	//outColor.xyz = vec3(outLine);

	//outColor.xyz = vec3(SampleWithOffest(vec2(0.0f, 0.0f)));

	if (texture(selectBuffer, fragInput.texCoord).x > 0.5f)
	{
		outColor.xyz = vec3(1.0f);
	}
	//outColor = vec4(fragInput.texCoord, 0.0f, 1.0f);
}

float SampleWithOffest(vec2 uv_offest)
{
	vec2 viewPortLoca = fragInput.texCoord * push.viewPortSize + uv_offest;
	return texture(selectBuffer, viewPortLoca / push.viewPortSize).x;
}

float Sobel(float MatColor[3][3])
{
	float Gridx = -1.0f * MatColor[0][0] - 2.0f * MatColor[1][0] - 1.0f * MatColor[2][0] +
		1.0f * MatColor[0][2] + 2.0f * MatColor[1][2] + 1.0f * MatColor[2][2];

	float Gridy = -1.0f * MatColor[2][0] - 2.0f * MatColor[2][1] - 1.0f * MatColor[2][2] +
		1.0f * MatColor[0][0] + 2.0f * MatColor[0][1] + 1.0f * MatColor[0][2];

	return sqrt(Gridx * Gridx + Gridy * Gridy);
}