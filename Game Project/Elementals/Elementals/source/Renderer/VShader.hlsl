#pragma pack_matrix(row_major)

struct V_IN
{
	float3 posL : POSITION;
	float2 tc : TEXCOORD;
};
struct V_OUT
{
	float2 tc : TEXCOORD;
	float4 posH : SV_POSITION;
};
cbuffer SCENE  : register( b1 )
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float2   screenDimensions;
	float2   padding2;
}

cbuffer DISPLAY_SIZE : register( b0 )
{
	float2 TOP_LEFT; //for the part of the image we want.
	float2 SIZE_WH;  //How much of the image we want to display
	float2 IMG_SIZE; 
	float2 padding;
	float4 SCREEN_COORDS;
};

V_OUT main( V_IN input ) 
{
	V_OUT output = (V_OUT)0;

	input.posL.x *= 1.0f / screenDimensions.x * ((SCREEN_COORDS.z - SCREEN_COORDS.x)); //Makes the image the size i want it to be
	input.posL.y *= 1.0f / screenDimensions.y * ((SCREEN_COORDS.w - SCREEN_COORDS.y)); 
	input.posL.x -= 1.0f; //Set the image to the upper left from the center
	input.posL.x += 2.0f / screenDimensions.x * (SCREEN_COORDS.x + ((SCREEN_COORDS.z-SCREEN_COORDS.x) / 2)); //Get the size of a pixel and mult it by the width and a half to offset the  image from the center.  images upper left is now where the coords are set
	input.posL.y += 1.0f;
	input.posL.y -= 2.0f / screenDimensions.y * (SCREEN_COORDS.y + ((SCREEN_COORDS.w-SCREEN_COORDS.y) / 2));

	float4 localH = float4(input.posL, 1); 

	output.posH = localH;


	input.tc.x /= IMG_SIZE.x / SIZE_WH.x;
	input.tc.y /= IMG_SIZE.y / SIZE_WH.y;

	if(TOP_LEFT.x != 0)
		input.tc.x += 1/(IMG_SIZE.x / TOP_LEFT.x);

	if(TOP_LEFT.y != 0)
		input.tc.y += 1/(IMG_SIZE.y / TOP_LEFT.y);

	output.tc = input.tc;

	return output; // send projected vertex to the rasterizer stage
}
