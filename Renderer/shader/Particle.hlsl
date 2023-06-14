struct VertexIn
{
	float3 worldPosition : POSITION;
	float2 texCoord : TEX;
};

struct VertexOut
{
	float4 viewportPosition : SV_POSITION;
	float2 texCoord : TEX;
};

cbuffer worldConstant : register(b0)
{
	float4x4 view;
	float4x4 projection;
}

texture2D<float4> tSprites : register (t0);

SamplerState sSampler : register(s0);

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.viewportPosition = mul(projection, mul(view, float4(vin.worldPosition,1.0f)));
	vout.texCoord = vin.texCoord;

	return vout;
}

float4 PS(VertexOut pin) : SV_TARGET
{
	/*
	float3 lColor = tSprites.Sample(sSampler,pin.texCoord);
	float lAlpha = lColor.x;
	float4 lFinalColor = float4(lColor, lAlpha);
	*/

	float4 lFinalColor = tSprites.Sample(sSampler,pin.texCoord);

	return lFinalColor;
}