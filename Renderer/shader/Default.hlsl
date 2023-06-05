cbuffer objectConstant : register(b0)
{
	float4x4 world;
}

cbuffer worldConstant : register(b1)
{
	float4x4 view;
	float4x4 projection;
}

struct VertexIn
{
	float3 localPosition : POSITION;
};

float4 VS(VertexIn vin) : SV_POSITION
{
	float4 pos = float4(vin.localPosition,1.0f);

	pos = mul(projection, mul(view, mul(world, pos)));

	return pos;
}

float4 PS() : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}