#include "Util.hlsl"

cbuffer objectConstant : register(b0)
{
	float4x4 world;
}

cbuffer worldConstant : register(b1)
{
	float4x4 view;
	float4x4 projection;
}

texture2D<float3> tAlbedo : register(t0);
texture2D<float> tMetallic : register(t1);
texture2D<float3> tNormal : register(t2);
texture2D<float> tRoughness : register(t3);
texture2D<float> tAO : register(t4);

SamplerState sSampler: register(s0);

struct Bone
{
	float4x4 transform;
};

StructuredBuffer<Bone> Skeleton : register(t5);

cbuffer SkeletonData : register(b2)
{
	int hasSkeleton;
}

struct VertexIn
{
	float3 localPosition : POSITION;
	float2 texCoord : TEX;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	uint4 boneIndices : BONEINDICES;
	float3 boneWeights : BONEWEIGHTS;
};

struct VertexOut
{
	float3 worldPosition : POSITION;
	float4 viewportPosition : SV_POSITION;
	float2 texCoord : TEX;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

static float3 LightPosition = {0,0,0};

static float3 LightColor = {1,1,1};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	float4 lPos = float4(vin.localPosition, 1.0f);

	if (hasSkeleton == 1)
	{
		float4x4 transform = Skeleton[vin.boneIndices[0]].transform;

		lPos = mul(transform, lPos);
	}

	float4 lWorldPosition = mul(world, lPos);
	vout.worldPosition = lWorldPosition.xyz;
	vout.viewportPosition = mul(projection, mul(view, lWorldPosition));
	vout.texCoord = vin.texCoord;
	vout.normal = mul(world,float4(vin.normal,0.0f)).xyz;
	vout.tangent = mul(world,float4(vin.tangent,0.0f)).xyz;
	vout.binormal = mul(world,float4(vin.binormal,0.0f)).xyz;

	return vout;
}


float4 PS(VertexOut pin) : SV_TARGET
{
	float3 lLo = 0.0f;

	float lMetallic = tMetallic.Sample(sSampler, pin.texCoord);
	float lRoughness = tRoughness.Sample(sSampler, pin.texCoord);
	float3 lAlbedo = tAlbedo.Sample(sSampler, pin.texCoord);

	float2 lUV = float2(pin.texCoord[0], pin.texCoord[1]);

	float3 lCamPosition = float3(view[0][3], view[1][3], view[2][3]);

	float3 lV = normalize(lCamPosition - pin.worldPosition);

	//vector pointing at light position
	float3 lL = normalize(LightPosition - pin.worldPosition);

	//normal vector in tangent space
	float3 lN = tNormal.Sample(sSampler, lUV);
	ConvertToTangentSpace(lN, pin.tangent, pin.binormal, pin.normal);

	float lCosine = max(dot(lN, lL), 0.0f);
	
	float lAttenuation = GetAttenuation(pin.worldPosition, LightPosition);

	float3 lRadiance = LightColor;// *lAttenuation;

	float3 lF0 = 0.04f;
	lF0 = lerp(lF0, tAlbedo.Sample(sSampler, pin.texCoord), lMetallic);

	float3 lH = normalize(lV + lL);


	float lNDF = DistributionGGX(lN, lH, lRoughness);
	float lG = GeometrySmith(lN, lV, lL, lRoughness);
	float3 lF = FresnelSchlick(lH, lV, lF0);

	float3 lKS = lF;
	float3 lKD = (1.0f - lKS)*(1.0f - lMetallic);

	float3 lNumerator = lNDF * lG * lF;
	float lDenominator = 4.0f * max(dot(lN, lV), 0.0f) * max(dot(lN , lL),0.0f) + 0.00001;
	float3 lSpecular = lNumerator / lDenominator;

	lLo += ((lKD * lAlbedo / PI )+ lSpecular) * lRadiance * lCosine;

	float3 lAmbient = lAlbedo * 0.03f;

	float3 lFinalColor = lAmbient + lLo;

	//tone mapping
	lFinalColor = lFinalColor / (lFinalColor + 1.0f);
	
	//gamma correction
	//lFinalColor = pow(lFinalColor, 1.0f / 2.2f);

	return float4(lFinalColor,1.0f);
}