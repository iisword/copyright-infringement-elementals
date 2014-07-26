#include "Collision.h"
//#include "CollisionDefines.h"

#include <DirectXMath.h>
using namespace DirectX;

CCollision::CCollision(void)
{
}


CCollision::~CCollision(void)
{
}

bool CCollision::CheckCollision(CBaseColObj* obj1, CBaseColObj* obj2)
{
	//Early out
	if(obj1 == nullptr || obj2 == nullptr)
	{
		return false;
	}


	if( obj1->GetID() == _AABB)
	{
		CAABB *ptAB = ((CAABB*)obj1);
		if(obj2->GetID() == _AABB)
		{
			return AABBtoAABB(ptAB, (CAABB*)obj2);
		}
		else if(obj2->GetID() == _SPHERE)
		{
			return AABBtoSPHERE(ptAB, (CSphere*)obj2);
		}
		else if(obj2->GetID() == _CAPSULE)
		{
			return AABBtoCapsule(ptAB, (CCapsule*)obj2);
		}

	}
	else if(obj1->GetID() == _SPHERE)
	{
		CSphere *ptS = ((CSphere*)obj1);
		if(obj2->GetID() == _AABB)
		{
			return AABBtoSPHERE((CAABB*)obj2, ptS);
		}
		else if(obj2->GetID() == _SPHERE)
		{
			return SPHEREtoSPHERE(ptS, (CSphere*)obj2);
		}
		else if(obj2->GetID() == _CAPSULE)
		{
			return SPHEREtoCAPSULE(ptS, (CCapsule*)obj2);
		}
	}
	else if(obj1->GetID() == _CAPSULE)
	{
		CCapsule *ptC = ((CCapsule*)obj1);
		if(obj2->GetID() == _AABB)
		{
			return AABBtoCapsule((CAABB*)obj2, ptC);
		}
		else if(obj2->GetID() == _SPHERE)
		{
			return SPHEREtoCAPSULE((CSphere*)obj2, ptC);
		}
	}
	
	return false;
}


bool CCollision::AABBtoSPHERE( CAABB* const aabb, CSphere* const sphere )
{
	XMFLOAT3 closest; 

	if(sphere->GetPos().x < aabb->GetMin().x)
		closest.x = aabb->GetMin().x;
	else if(sphere->GetPos().x > aabb->GetMax().x)
		closest.x = aabb->GetMax().x;
	else
		closest.x = sphere->GetPos().x;

	if(sphere->GetPos().y < aabb->GetMin().y)
		closest.y = aabb->GetMin().y;
	else if(sphere->GetPos().y > aabb->GetMax().y)
		closest.y = aabb->GetMax().y;
	else
		closest.y = sphere->GetPos().y;

	if(sphere->GetPos().z < aabb->GetMin().z)
		closest.z = aabb->GetMin().z;
	else if(sphere->GetPos().z > aabb->GetMax().z)
		closest.z = aabb->GetMax().z;
	else
		closest.z = sphere->GetPos().z;

	XMFLOAT3 vec;
	vec.x = closest.x - sphere->GetPos().x;
	vec.y = closest.y - sphere->GetPos().y;
	vec.z = closest.z - sphere->GetPos().z;

	float sqdis = XMVector3Dot(XMLoadFloat3(&vec), XMLoadFloat3(&vec)).m128_f32[0];

	if(sqdis > (sphere->GetRad() * sphere->GetRad()))
		return false;
	else
		return true;
}

bool CCollision::AABBtoCapsule( CAABB* const aabb, CCapsule* const capsule )
{
	XMFLOAT3 abCenter;
	abCenter.x = (aabb->GetMin().x + aabb->GetMax().x) * 0.5f;
	abCenter.y = (aabb->GetMin().y + aabb->GetMax().y) * 0.5f;
	abCenter.z = (aabb->GetMin().z + aabb->GetMax().z) * 0.5f;

	XMFLOAT3 L;
	L.x = capsule->GetTop().x - capsule->GetBottom().x;
	L.y = capsule->GetTop().y - capsule->GetBottom().y;
	L.z = capsule->GetTop().z - capsule->GetBottom().z;
	XMFLOAT3 N;
	XMStoreFloat3(&N, XMVector3Normalize(XMLoadFloat3(&L)));

	XMFLOAT3 vec; 
	vec.x = abCenter.x - capsule->GetBottom().x;
	vec.y = abCenter.y - capsule->GetBottom().y;
	vec.z = abCenter.z - capsule->GetBottom().z;

	float d = XMVector3Dot(XMLoadFloat3(&N), XMLoadFloat3(&vec)).m128_f32[0];

	XMFLOAT3 closest;
	if(d < 0)
	{
		closest = capsule->GetBottom();
	}
	else if(d > XMVector3Length(XMLoadFloat3(&L)).m128_f32[0])
	{
		closest = capsule->GetTop();
	}
	else
	{
		XMFLOAT3 dis = N;
		dis.x *= d;
		dis.y *= d;
		dis.z *= d;

		closest.x = capsule->GetBottom().x + dis.x;
		closest.y = capsule->GetBottom().y + dis.y;
		closest.z = capsule->GetBottom().z + dis.z;
	}

	CSphere temp;
	temp.SetPos(closest);
	temp.SetRad(capsule->GetRad());

	return AABBtoSPHERE(aabb, &temp);
}

bool CCollision::AABBtoAABB (CAABB* const aabb1, CAABB* const aabb2)
{
	if(aabb1->GetMax().x < aabb2->GetMin().x || aabb1->GetMin().x > aabb2->GetMax().x)
		return false;
	if(aabb1->GetMax().y < aabb2->GetMin().y || aabb1->GetMin().y > aabb2->GetMax().y)
		return false;
	if(aabb1->GetMax().z < aabb2->GetMin().z || aabb1->GetMin().z > aabb2->GetMax().z)
		return false;

	return false;
}

bool CCollision::SPHEREtoSPHERE(CSphere* const sphere1,  CSphere* const sphere2)
{
	float fR = (sphere1->GetRad() + sphere2->GetRad());
	fR *= fR;
	
	XMFLOAT3 vec;
	vec.x = sphere2->GetPos().x - sphere1->GetPos().x;
	vec.y = sphere2->GetPos().y - sphere1->GetPos().y;
	vec.z = sphere2->GetPos().z - sphere1->GetPos().z;
	
	float sqdis = XMVector3Dot(XMLoadFloat3(&vec), XMLoadFloat3(&vec)).m128_f32[0];

	if(sqdis > (fR))
		return false;
	else 
		return true;

}

bool CCollision::SPHEREtoCAPSULE (CSphere* const sphere, CCapsule* const capsule)
{
	XMFLOAT3 closest;

	if(sphere->GetPos().x < capsule->GetTop().x)
		closest.x = capsule->GetTop().x;
	else if(sphere->GetPos().x > capsule->GetBottom().x)
		closest.x = capsule->GetBottom().x;
	else
		closest.x = sphere->GetPos().x;

	if(sphere->GetPos().y > capsule->GetTop().y)
		closest.y = capsule->GetTop().y;
	else if(sphere->GetPos().y < capsule->GetBottom().y)
		closest.y = capsule->GetBottom().y;
	else
		closest.y = sphere->GetPos().y;

	
	if(sphere->GetPos().z < capsule->GetTop().z)
		closest.z = capsule->GetTop().z;
	else if(sphere->GetPos().z > capsule->GetBottom().z)
		closest.z = capsule->GetBottom().z;
	else
		closest.z = sphere->GetPos().z;

	CSphere temp;
	temp.SetPos(closest);
	temp.SetRad(capsule->GetRad());

	return SPHEREtoSPHERE(&temp, sphere);
}


bool CCollision::IntersectRaySphere(const XMFLOAT3 &sa, const XMFLOAT3 &n, const XMFLOAT3 &center, float radius, float &t, XMFLOAT3 &q)
{
	// TODO: Read the header file comments for this function!

	// TODO: Complete this function
	//		 BE SURE TO MODIFY THE ALGORITHM AS SPECIFIED IN THE HEADER FILE COMMENTS

	XMFLOAT3 m;
	m.x = sa.x - center.x;
	m.y = sa.y - center.y;
	m.z = sa.z - center.z;

	float b = XMVector3Dot(XMLoadFloat3(&m), XMLoadFloat3(&n)).m128_f32[0];
	float c = XMVector3Dot(XMLoadFloat3(&m), XMLoadFloat3(&m)).m128_f32[0] - (radius*radius);
	float discr = (b*b) - c;
	t = -b - sqrtf(discr);

	if(t < 0.0f || discr < 0.0f)
	{
		return false;
	}

	q.x = n.x + (sa.x * t);
	q.y = n.y + (sa.y * t);
	q.z = n.z + (sa.z * t);

	return true;
}

bool CCollision::IntersectRayCylinder(const XMFLOAT3 &sa, const XMFLOAT3 &n, CCapsule* const cap, float &t)
{	
	// TODO: Read the header file comments for this function!
	//p - First (base) point on the cylinder segment
	//q - Second (top) point on the cylinder segment

	XMFLOAT3 d; // vector from first point on cylinder segment to the end point on cylinder segment
	d.x = cap->GetTop().x - cap->GetBottom().x;
	d.y = cap->GetTop().y - cap->GetBottom().y;
	d.z = cap->GetTop().z - cap->GetBottom().z;

	XMFLOAT3 m; // vector from first point on cylinder segment to start point of ray
	m.x = sa.x - cap->GetBottom().x;
	m.y = sa.y - cap->GetBottom().y;
	m.z = sa.z - cap->GetBottom().z;
	// Values used to calculate coefficients of quadratic formula.
	// You do not necessarily have to use any of these directly for the rest of the algorithm.
	float dd = XMVector3Dot(XMLoadFloat3(&d), XMLoadFloat3(&d)).m128_f32[0]; // dot product of d with d (squared magnitude of d)
	float nd = XMVector3Dot(XMLoadFloat3(&n), XMLoadFloat3(&d)).m128_f32[0]; // dot product of ray normal (n) with d
	float mn = XMVector3Dot(XMLoadFloat3(&m), XMLoadFloat3(&n)).m128_f32[0]; 
	float md = XMVector3Dot(XMLoadFloat3(&m), XMLoadFloat3(&d)).m128_f32[0]; 
	float mm = XMVector3Dot(XMLoadFloat3(&m), XMLoadFloat3(&m)).m128_f32[0]; 


	// TODO: Optimization by early out
	//If the ray starts outside the top or bottom planes and points away, there can be no intersection.

	if(mm < 0.0f && nd < 0.0f)
		return false;
	if(mm > dd && nd > dd)
		return false;

	// Coefficients for the quadratic formula
	float a = dd - nd * nd;
	float b = dd*mn - nd*md;
	float c = dd*(mm - cap->GetRad()*cap->GetRad()) - md*md;

	// If a is approximately 0.0 then the ray is parallel to the cylinder and can't intersect
	if( abs(a) < FLT_EPSILON )
		return false;

	// TODO: Find time of intersection, if any
	//		 Use the quadratic formula to solve for t. Reference "Advanced Ray to Sphere.ppt" for an example.
	//		 As with "Advanced Ray to Sphere", the 2s and 4 in the formula ( x = (-b - sqrt(b*b - 4ac)) / 2a )
	//		 are cancelled out, resulting in a simplified form.
	float discr = (b*b) - (a*c);
	if(discr < 0.0f)
		return false;

	t = (-b - sqrtf(discr))/a;
	if(t < 0.0f)
		return false;

	XMFLOAT3 in;
	in.x = sa.x + (n.x*t);
	in.y = sa.y + (n.y*t);
	in.z = sa.z + (n.z*t);

	XMFLOAT3 pn;
	pn.x = cap->GetTop().x - cap->GetBottom().x;
	pn.y = cap->GetTop().y - cap->GetBottom().y;
	pn.z = cap->GetTop().z - cap->GetBottom().z;

	XMFLOAT3 qn;
	qn.x = cap->GetBottom().x - cap->GetTop().x;
	qn.y = cap->GetBottom().y - cap->GetTop().y;
	qn.z = cap->GetBottom().z - cap->GetTop().z;

	XMStoreFloat3(&pn, XMVector3Normalize(XMLoadFloat3(&pn)));
	XMStoreFloat3(&qn, XMVector3Normalize(XMLoadFloat3(&qn)));
	//float pOff =  dot_product(pn, p);
	//float qOff =  dot_product(qn, q);

	XMFLOAT3 pin;
	pin.x = in.x - cap->GetBottom().x;
	pin.y = in.y - cap->GetBottom().y;
	pin.z = in.z - cap->GetBottom().z;

	XMFLOAT3 qin;
	qin.x = in.x - cap->GetTop().x;
	qin.y = in.y - cap->GetTop().y;
	qin.z = in.z - cap->GetTop().z;


	if(XMVector3Dot(XMLoadFloat3(&pin), XMLoadFloat3(&pn)).m128_f32[0] < 0.0f)
		return false;

	if(XMVector3Dot(XMLoadFloat3(&qin), XMLoadFloat3(&qn)).m128_f32[0] < 0.0f)
		return false;

	return true;
}

bool CCollision::IntersectRayCapsule(const XMFLOAT3 &sa, const XMFLOAT3 &n, CCapsule* const cap, float &t, XMFLOAT3 &colPos)
{
	// TODO: Read the header file comments for this function!

	float fTime = FLT_MAX;
	t = FLT_MAX;
	bool bReturn = false;

	// TODO: Complete this function
	XMFLOAT3 inter = XMFLOAT3(0.0f, 0.0f, 0.0f);

	if(IntersectRayCylinder(sa, n, cap, t) == true)
	{
		bReturn = true;
	}
	else
	{
		float s1 = FLT_MAX;
		float s2 = FLT_MAX;
		bool both[2] = { false, false };

		if(IntersectRaySphere(sa, n, cap->GetBottom(), cap->GetRad(), s1, inter) == true)
		{
			bReturn = true;
			both[0] = true;
		}

		if(IntersectRaySphere(sa, n, cap->GetTop(), cap->GetRad(), s2, inter) == true)
		{
			bReturn = true;
			both[1] = true;
		}

		if(bReturn == true && both[0] == true && both[1] == true)
		{
			if(s1 < s2)
				t = s1;
			else if(s2 < s1)
				t = s2;
		}
		else if(bReturn == true && both[0] == true)
			t = s1;
		else if(bReturn == true && both[1] == true)
			t = s2;
	}


	colPos.x = n.x + (sa.x * t);
	colPos.y = n.y + (sa.y * t);
	colPos.z = n.z + (sa.z * t);

	return bReturn;
}

