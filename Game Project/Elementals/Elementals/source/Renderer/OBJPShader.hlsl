texture2D baseTexture : register(t0);

SamplerState samplerM : register(s0);

cbuffer TIMER  : register( b0 )
{
	float4 variant;
}

#define ITERATIONS 25
#define SPEED 1.0
#define DISPLACEMENT .28
#define TIGHTNESS 6.0
#define FLAMETONE float3(12.0, 3.0, 1.0)

float shape(float2 pos) // a blob shape to distort
{
	return clamp( sin(pos.x*3.1416) - pos.y+0.15, 0.0, 1.0 );
}

float noise( float3 x) // iq noise function
{
	float3 p = floor(x);
    float3 f = frac(x);
	f = f*f*(3.0-2.0*f);
	float2 uv = (p.xy+float2(37.0,17.0)*p.z) + f.xy;
	float2 rg = baseTexture.Sample(samplerM, (uv+ 0.2)/256.0 ).yx; 
	return lerp( rg.x, rg.y, f.z ) * 2.0 - 1.0;
}

#define time .2   //variant.x*.03
//#define fmod mod

#define width .005

float4 main( float2 colorFromRasterizer : TEXCOORD, float3 surfacePos : POSITION0, float3 surfaceNorm : POSITION1, float4 position : SV_POSITION) : SV_TARGET
{

	float4 finalColor = baseTexture.Sample(samplerM, colorFromRasterizer);
	return finalColor;
#if 0
	float zoom = time;
	float3 color=float3(0., 0, 0);
	float shape=0.;
	float2 res = float2(400, 300);

	float2 pos = surfacePos.xy; // / res - .5;
	pos.x -= .55;
	pos.x*=res.x/res.y;
	float2 uv=pos;
	uv.x *= time;
	uv.y *= time;
	uv.x -= time;
	uv.y -= time/2;
//	float sph = length(uv); sph = sqrt(1. - sph*sph)*1.5; // curve for spheric distortion
//	uv=normalize(float3(uv,sph)).xy;
	float a=time+fmod(time,1.)*.5;
	float2 luv=uv;
	float b=a*5.48535;
//	zoom*=1.+sin(1*3.758123)*.8;
//	uv= float4(cos(b),sin(b),-sin(b),cos(b)) * uv;
//	uv+=float2(sin(a),cos(a*.5))*8.;
	uv*=zoom;
	float pix=.5/res.x*zoom;
	float dof=1.;//max(1.,(2.-fmod(time,1.)/.01));
	float c=1.5+fmod(floor(time),6.)*.125;
	for (int aa=0; aa<3; aa++) {
		float2 aauv=floor(float2(float(aa)/6.,fmod(float(aa),6.)));
//		formula(uv+aauv*pix*dof,c);
		float2 z = uv+aauv*pix*dof;
		float minit=0.;
		float o,ot2,ot=ot2=1000.;
		for (int i=0; i<9; i++) {
			z=abs(z)/clamp(dot(z,z),.1,.5)-c;
			float l=length(z);
			o=min(max(abs(min(z.x,z.y)),-l+.25),abs(l-.25));
			ot=min(ot,o);
			ot2=min(l*.1,ot2);
			minit=max(minit,float(i)*(1.-abs(sign(ot-o))));
		}
		minit+=1.;
		float w=width*minit*2.;
		float circ=pow(max(0.,w-ot2)/w,6.);
		shape+=max(pow(max(0.,w-ot)/w,.25),circ);
		float3 col=normalize(.1+baseTexture.Sample(samplerM,float2(minit*.1,minit*.1)).rgb);
		color+=col*(.4+fmod(minit/9.*10.+ot2*2.,1.)*1.6);
		color+=float3(1.,.7,.3)*circ*(10.-minit)*3.*smoothstep(0.,.5,.15+baseTexture.Sample(samplerM,float2(.0,1.)).x-.5);
	}
	//shape/=36.; 
	color/=36.;
//	float3 colo=lerp(float3(.15, .15, .15),color,shape)*(1.-length(pos))*min(1.,abs(.5)*10.);	
//	colo*=float3(1.2,1.1,1.0);
	return float4(color,1.0);
#endif
#if 1
	float2 res = float2(600,600);

	float2 uv = surfacePos.xy;// / res;
	
	float bg = (cos(uv.x*3.14159*2.0) + sin((uv.y)*3.14159)) * 0.15;
	
	float2 p = uv*2.0 - 1.0;
	p *= 15.0;
	float2 sfunc = float2(p.x, p.y + 5.0*sin(uv.x*10.0-variant.x*2.0 + cos(variant.x*7.0) )+2.0*cos(uv.x*25.0+variant.x*4.0));
	sfunc.y *= uv.x*2.0+0.05;
	sfunc.y *= 2.0 - uv.x*2.0+0.05;
	sfunc.y /= 0.1; // Thickness fix
	
	float3 c = float3(abs(sfunc.y), abs(sfunc.y), abs(sfunc.y));
	c = pow(c, float3(-0.5, -0.5, -0.5));
	c *= float3(0.3,0.85,1.0);
	finalColor = float4(c, 1.0f);
	//float3 lightDir = float3(-1, -1, .2); //normalize(lightPos - surfacePos);
	//float lightRatio = saturate( dot(-lightDir, surfaceNorm ) );
	//finalColor = lightRatio * finalColor;
	return finalColor;
#endif
//	return finalColor;
	//float2 resolution = float2(600,600);
	//uv = surfacePos.xy;// / resolution;
	//uv.y = 1 - uv.y;
	//float nx = 0.0;
	//float ny = 0.0;
	//for (int i=0; i<ITERATIONS; i++)
	//{
	//	float ii = pow(float(i), 2.0);
	//	float ifrac = float(i)/float(ITERATIONS);
	//	float t = ifrac * variant.x * SPEED;
	//	float d = (1.0-ifrac) * DISPLACEMENT;
	//	nx += noise( float3(uv.x*ii+1.0, uv.y*0.35f*ii-t, 0.0)) * d * 2.0;
	//	ny += noise( float3(uv.x*ii-1.0, uv.y*0.35f*ii-t, 1.0)) * d;
	//}
	//float flame = shape( float2(uv.x+nx, uv.y+ny) );
	//float3 col = clamp(pow(flame, TIGHTNESS) * FLAMETONE, 0.0, 1.0);
	//if(col.x > 0.02f)
	//	col.x += .3;

	//finalColor = float4(col, uv.y);
	//return finalColor;
	/////Flash Shader	
	//if(/*finalColor.z > .9f &&*/ finalColor.y > .3f && finalColor.x > .3f)
	//	finalColor += variant;


//	return finalColor; 
	//// Flash shader end

	float3 lightDir = float3(-1, 0, .2); //normalize(lightPos - surfacePos);
	float lightRatio = saturate( dot(-lightDir, surfaceNorm ) );
	/*finalColor = lightRatio * finalColor;
	finalColor += baseTexture.Sample(samplerM, colorFromRasterizer) * float4(.3, .3, .3, 1);
*/
	//float3 normalized = float3(normalize(camPosition - surfacePos));
	//float3 halfVector = float3(normalize(-1 * float3(-5, 3, -1) + normalized));
	//float intensity  =  max(pow(saturate(dot(surfaceNorm, (halfVector))), 10) , 0); //changed power to 10 so sheen is a bit more apparent
	//float4 specularResult = specularTexture.Sample(samplerM, colorFromRasterizer) * intensity;
	//finalColor += specularResult ;

	return finalColor;
}


//float4 main( float2 colorFromRasterizer : TEXCOORD, float3 surfacePos : POSITION0, float3 surfaceNorm : POSITION1 ) : SV_TARGET
//{
////	float4 color = float4(0, 0, 1, 1);
////	return color;
//
//
//	float4 finalColor = float4(baseTexture.Sample(samplerM, colorFromRasterizer));
//
//	float3 lightDir = float3(-1, 0, .2); //normalize(lightPos - surfacePos);
//	float lightRatio = saturate( dot(-lightDir, surfaceNorm ) );
//	finalColor = lightRatio * finalColor;
//	finalColor += baseTexture.Sample(samplerM, colorFromRasterizer) * float4(.3, .3, .3, 1);
//	return finalColor;
//
//	//float3 normalized = float3(normalize(camPosition - surfacePos));
//	//float3 halfVector = float3(normalize(-1 * float3(-5, 3, -1) + normalized));
//	//float intensity  =  max(pow(saturate(dot(surfaceNorm, (halfVector))), 10) , 0); //changed power to 10 so sheen is a bit more apparent
//	//float4 specularResult = specularTexture.Sample(samplerM, colorFromRasterizer) * intensity;
//	//finalColor += specularResult ;
//
////	return finalColor;
//}

