
texture2D baseTexture : register(t0);
SamplerState samplerM : register(s0);

cbuffer TIMER  : register( b0 )
{
	float4 variant;
}

#define PI					 3.141592
#define speed				 0.8
#define speed_x				 0.9
#define speed_y				 0.3
#define emboss				 0.50
#define intensity			 2.4
#define steps				 2
#define frequency			 6.0
#define	angle				 7
#define delta				 60.
#define intence				 700.
#define reflectionCutOff	 0.012
#define reflectionIntence	 200000.


#define time variant.x*1.3

float col(float2 coord)
  {
    float delta_theta = 2.0 * PI / float(angle);
    float col = 0.0;
    float theta = 0.0;
    for (int i = 0; i < steps; i++)
    {
      float2 adjc = coord;
      theta = delta_theta*float(i);
      adjc.x += cos(theta)*time*speed + time * speed_x;
      adjc.y -= sin(theta)*time*speed - time * speed_y;
      col = col + cos( (adjc.x*cos(theta) - adjc.y*sin(theta))*frequency)*intensity;
    }

    return cos(col);
  }

float4 main( float2 colorFromRasterizer : TEXCOORD, float3 surfacePos : POSITION0, float3 surfaceNorm : POSITION1, float4 pos : SV_POSITION) : SV_TARGET
{
	float2 p = (pos.xy) / float2(1280, 760);
		p.y = 1-p.y;
	float2	c1 = p, c2 = p;
	float cc1 = col(c1);

	c2.x += 1280.0/delta;
	float dx = emboss*(cc1-col(c2))/delta;

	c2.x = p.x;
	c2.y += 760.0/delta;
	float dy = emboss*(cc1-col(c2))/delta;

	c1.x += dx*2.;
	c1.y = -(c1.y+dy*2.);

	float alpha = 1.+dot(dx,dy)*intence;
	
	float ddx = dx - reflectionCutOff;
	float ddy = dy - reflectionCutOff;
	if (ddx > 0. && ddy > 0.)
		alpha = pow(alpha, ddx*ddy*reflectionIntence);
	
	return baseTexture.Sample(samplerM,c1)*(alpha);
}
