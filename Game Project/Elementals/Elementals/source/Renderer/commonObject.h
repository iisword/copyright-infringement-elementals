#pragma once
struct float2{ float x; float y;};
struct float3{ float x; float y; float z; };
struct float4{ float x; float y; float z; float w;};

struct DISPLAY_SIZE {
	float2 TOP_LEFT;
	float2 SIZE_WH;
	float2 IMG_SIZE;
	float2 padding;
	float4 SCREEN_COORDS;
};

class commonObject
{
public:
	float3 vertex; float2 texture; float3 normal;

	commonObject(void);
	~commonObject(void);
};

