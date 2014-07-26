#pragma pack_matrix(row_major)


struct V_IN
{
	float3 posL : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	int4   BoneID : BONEIDS;
	float4 BoneWeight : BONEWEIGHTS;
};
struct V_OUT
{
	float2 uv : TEXCOORD;
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
cbuffer COLOR  : register( b2 )
{
float4 color;
}
cbuffer BoneMatrices : register(b3)
{
	float4x4 BoneMats[30];
}
V_OUT main( V_IN input ) 
{
	V_OUT output = (V_OUT)0;

	float4 localH = float4(input.posL,1); 
//	output.posH = localH;

    output.posH = (mul(float4(input.posL,1.0f),BoneMats[input.BoneID.x]) * input.BoneWeight.x); 
 	output.posH+= (mul(float4(input.posL,1.0f),BoneMats[input.BoneID.y]) * input.BoneWeight.y);
//  	output.posH+= (mul(float4(input.posL,1.0f),BoneMats[input.BoneID.z]) * input.BoneWeight.z);
//  	output.posH+= (mul(float4(input.posL,1.0f),BoneMats[input.BoneID.w]) * input.BoneWeight.w);

	output.posH = mul(output.posH, worldMatrix);
	output.posH = mul(output.posH, viewMatrix);
	output.posH = mul(output.posH, projectionMatrix);

		
	
	output.uv = input.uv;

	output.surfaceNorm = mul(float4(input.normal, 0), worldMatrix).xyz;
// 	localH = mul(localH, worldMatrix);
// 	output.surfacePos = localH.xyz;
// 	localH = mul(localH, viewMatrix);
// 	localH = mul(localH, projectionMatrix);
	return output; // send projected vertex to the rasterizer stage
}
