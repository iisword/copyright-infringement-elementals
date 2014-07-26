#pragma pack_matrix(row_major)

struct V_IN
{
	float3 posL : POSITION;
	float2 tc : TEXCOORD;
	float3 normal : NORMAL;
};
struct V_OUT
{
	float2 tc : TEXCOORD;
	float3 surfacePos : POSITION0;
	float3 surfaceNorm : POSITION1;
	float4 posH : SV_POSITION;
};
cbuffer OBJECT : register( b0 )
{
	float4x4 worldMatrix;
}
cbuffer SCENE  : register( b1 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

V_OUT main( V_IN input ) 
{
	V_OUT output = (V_OUT)0;

	float4 localH = float4(input.posL,1); 

	localH = mul(localH, worldMatrix);
	output.surfacePos = localH.xyz;
	localH = mul(localH, viewMatrix);
	localH = mul(localH, projectionMatrix);

	output.posH = localH;
	output.tc = input.tc;

	output.surfaceNorm = mul(float4(input.normal, 0), worldMatrix).xyz;

	return output; // send projected vertex to the rasterizer stage
}
