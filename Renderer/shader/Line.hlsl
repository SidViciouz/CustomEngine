cbuffer dots : register(b0)
{
	float3 color;
	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
};

cbuffer world : register(b1)
{
	float4x4 view; //column major
	float4x4 projection; //column major
}

float4 VS(uint pIndex : SV_VertexID) : SV_POSITION
{
	float4 pos;

	if (pIndex == 0)
	{
		pos = float4(x1, y1, z1, 1);
	}
	else
	{
		pos = float4(x2, y2, z2, 1);
	}

	pos = mul(projection,mul(view, pos));

	return pos;
}

float4 PS() : SV_TARGET
{
	return float4(color, 1.0f);
}