
texture2D baseTexture : register(t0);
SamplerState samplerM : register(s0);

cbuffer TIMER  : register( b0 )
{
	float4 variant;
}

float noise(float3 p) //Thx to Las^Mercury
{
	float3 i = floor(p);
	float4 a = dot(i, float3(1., 57., 21.)) + float4(0., 57., 21., 78.);
	float3 f = cos((p-i)*acos(-1.))*(-.5)+.5;
	a = lerp(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
	a.xy = lerp(a.xz, a.yw, f.y);
	return lerp(a.x, a.y, f.z);
}

#define time variant.x

//float hash21(float2 n){ return frac(sin(dot(n, float2(12.9898, 4.1414))) * 1.5453); }
float4 makem2(float theta){float c = cos(theta);float s = sin(theta);return float4(c,-s,s,c);}
float noise(float2 x ){return baseTexture.Sample(samplerM, x*.01).y;}

float2 gradn(float2 p)
{
	float ep = .09;
	float gradx = noise(float2(p.x+ep,p.y))-noise(float2(p.x-ep,p.y));
	float grady = noise(float2(p.x,p.y+ep))-noise(float2(p.x,p.y-ep));
	return float2(gradx,grady);
}

float flow(float2 p)
{
	float z=2.;
	float rz = 0.;
	float2 bp = p;
	for (float i= 1.;i < 7.;i++ )
	{
		//primary flow speed
		p += time*1.5;
		
		//secondary flow speed (speed of the perceived flow)
		bp += time*1.;
		
		//displacement field (try changing time multiplier)
		float2 gr = gradn(i*p*6.+time*1.);
		
		//rotation of the displacement field
		gr*= makem2(time*10.-(0.05*p.x+0.03*p.y)*40.);
		
		//displace the system
		p += gr*.5;
		
		//add noise octave
		rz+= (sin(noise(p)*7.)*0.5+0.5)/z;
		
		//blend factor (blending displaced system with base system)
		//you could call this advection factor (.5 being low, .95 being high)
		p = lerp(bp,p,.8);
		
		//intensity scaling
		z *= 1.4;
		//octave scaling
		p *= 2.;
		bp *= 1.9;
	}
	return rz;	
}

float4 main( float2 colorFromRasterizer : TEXCOORD, float3 surfacePos : POSITION0, float3 surfaceNorm : POSITION1, float4 pos : SV_POSITION) : SV_TARGET
{
	float temp = time;
	float2 p = pos.xy / float2(1024, 768).xy;
	p.x *= 1;
	p*= 3.;
	float rz = flow(p);
	
	float3 col = float3(.2,0.07,0.01)/rz;
	col=pow(col,float(1.4));
	return float4(col,1.0);
}