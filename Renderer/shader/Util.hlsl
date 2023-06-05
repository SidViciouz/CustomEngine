
static const float PI = 3.14159265f;

void ConvertToTangentSpace(inout float3 pNormal, in float3 pT, in float3 pB, in float3 pN)
{
	pT = normalize(pT);
	pB = normalize(pB);
	pN = normalize(pN);

	float3x3 lTransform =
	{
		pB[0],pT[0],pN[0],
		pB[1],pT[1],pN[1],
		pB[2],pT[2],pN[2]
	};

	pNormal = mul(lTransform, pNormal);
}

float GetAttenuation(in float3 pPosition, in float3 pLightPosition)
{
	float lDistance = length(pLightPosition - pPosition);

	float lAttenuation = 1.0f / (lDistance * lDistance);

	return lAttenuation;
}

float3 FresnelSchlick(in float3 pH, in float3 pV, in float3 pF0)
{
	return pF0 + (1.0f - pF0) * pow(1.0f - max(dot(pH, pV),0.0f), 5.0f);
}

float DistributionGGX(in float3 pN, in float3 pH, in float pRoughness)
{
	float lA = pRoughness * pRoughness;
	float lA2 = lA * lA;
	float lNDotH = max(dot(pN, pH), 0.0f);
	float lNDotH2 = lNDotH * lNDotH;

	float lNumerator = lA2;
	float lDenominator = (lNDotH2 * (lA2 - 1.0f) + 1.0f);

	lDenominator = PI * lDenominator * lDenominator;

	return lNumerator / lDenominator;

}

float GeometrySchlickGGX(in float pNDotV, in float pRoughness)
{
	float lR = pRoughness + 1.0f;
	float lK = (lR * lR) / 8.0f;

	float lNumerator = pNDotV;
	float lDenominator = pNDotV * (1.0f - lK) + lK;

	return lNumerator / lDenominator;
}

float GeometrySmith(in float3 pN, in float3 pV, in float3 pL, in float pRoughness)
{
	float lNDotV = max(dot(pN, pV), 0.0f);
	float lNDotL = max(dot(pN, pL), 0.0f);
	float lGGX2 = GeometrySchlickGGX(lNDotV, pRoughness);
	float lGGX1 = GeometrySchlickGGX(lNDotL, pRoughness);

	return lGGX1 * lGGX2;
}