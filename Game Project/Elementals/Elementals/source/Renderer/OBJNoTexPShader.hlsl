
cbuffer LIGHT  : register( b0 )
{
	float4 lightColor;
	float3 lightPos;
	float intensity;
}


float4 main( float4 color : COLOR, float3 normal : NORMAL, float3 surfacePos : POSITION0) : SV_TARGET
{
	float4 finalColor = color; //float4(1.0f, 1.0f, 1.0f, 1.0f);
	float3 lightDir = float3(-.5, -1, .2); //normalize(lightPos - surfacePos);
	float lightRatio = saturate( dot(-lightDir, normal) );
	finalColor = lightRatio * finalColor;

	float3 lightDist = surfacePos - lightPos;
	lightDir = normalize(lightDist);
	lightRatio = saturate(dot(-lightDir, normal));
	float rat = intensity - clamp(length(lightDist), 0.0f, intensity);

	finalColor += lightColor * rat * lightRatio;
	return finalColor;
}

//#ifndef DEFERRED_POINT_LIGHT_HLSL
//#define DEFERRED_POINT_LIGHT_HLSL
//
//#include "VertexLayouts.hlsli"
//#include "TexturesAndSamplers.hlsli"
//#include "../EDRendererD3D/ConstantBuffers.h"
//#include "../EDRendererD3D/LightBuffers.h"

//float4 main( float4 color : COLOR, float3 normal : NORMAL, float3 surfacePos : POSITION0) : SV_TARGET
//{
//	float3 posWorld;
//	float4 depth, posScreen, finalAmbient, finalDiffuse, finalSpecular;
//	float3 toLight, texCubeCoords, reflectionVector, directionToCamera;
//	float2 texCoord;
//	float toLightLength, attenuation, nDotL, lightMapDepth, shadow, specMod, sDepth, alpha, damping_factor;
//
//	texCoord = float2(surfacePos.x / 1, - surfacePos.y) * 0.5f + 0.5f;
//
//	posScreen = float4(surfacePos, 1);
//	posWorld = surfacePos;
//
//	toLight = lightPos - posWorld;
//	toLightLength = length(toLight);
//	sDepth= max(max(abs(toLight.x), abs(toLight.y)), abs(toLight.z));
//	toLight /= toLightLength;
//
//	attenuation = saturate(1/(2.0 + 2.0 * toLightLength + 2.0 * toLightLength * toLightLength));
//	alpha = toLightLength / 2.0;
//	damping_factor = saturate(1.0 - alpha * alpha);
//	attenuation *= 1; //damping_factor;
//
//	nDotL = saturate(dot(normal.xyz, toLight));
//	finalDiffuse = nDotL * lightColor;
//
//	finalAmbient = float4(lightColor);
//
//	return attenuation * finalAmbient;
//}
//#endif //DEFERRED_POINT_LIGHT_HLSL
