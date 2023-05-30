cbuffer objectConstant : register(b0)
{
	float4x4 world;
}

cbuffer worldConstant : register(b1)
{
	float4x4 view;
	float4x4 projection;
}

texture2D tAlbedo : register(t0);
texture2D tNormal : register(t1);
texture2D tMetallic : register(t2);
texture2D tRoughness : register(t3);
texture2D tAO : register(t4);

struct VertexIn
{
	float3 LocalPosition : POSITION;
};

float4 VS(VertexIn vin) : SV_POSITION
{
	float4 pos = float4(vin.LocalPosition,1.0f);

	pos = mul(projection, mul(view, mul(world, pos)));

	return pos;
}

float4 PS() : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}