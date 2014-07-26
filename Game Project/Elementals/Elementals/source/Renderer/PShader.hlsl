texture2D baseTexture : register(t0);

SamplerState samplerM : register(s0);

float4 main(float2 colorFromRasterizer : TEXCOORD ) : SV_TARGET
{
//	return baseTexture.Sample(samplerM, colorFromRasterizer);
	float4 finalColor = baseTexture.Sample(samplerM, colorFromRasterizer);
	if(finalColor[3] <= .2)
		discard;

//	finalColor.rgb = (finalColor.x + finalColor.y + finalColor.z) / 3;

	return finalColor;
}