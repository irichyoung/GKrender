cbuffer percbuff : register(b0)
{
	matrix World:WORLD;
	matrix View:VIEW;
	matrix Projection:PROJECTION;
}
cbuffer inscbuffer : register(b1)
{
	float4 Diffuse : COLOR;
}
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
	float4 Diffuse : COLOR;
};

struct PS_INPUT
{
	float2 Tex : TEXCOOR;
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

Texture2D txDiff:register(t0);
SamplerState samLinear:register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Color = float4(input.Diffuse);
	output.Tex = input.Tex;
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	return Diffuse;
}
