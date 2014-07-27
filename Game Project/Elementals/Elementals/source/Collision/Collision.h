/***********************************************
* Filename:  		Collision.h
* Date:      		7/16/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		Collision Class. This System will handle collisions.
************************************************/
#ifndef  _COLLISION_H
#define _COLLISION_H

//class CAABB;
//class CSphere;
//class CCapsule;
#include "CollisionDefines.h"


class CCollision
{
public:
	CCollision(void);
	~CCollision(void);

	/**********************************
	*	CheckCollision:		    Used to load a server configuration file. The base loads all stats
	*							required for normal play.
	*
	*	In:						CBaseColObj* obj1, obj2 - The Objects beign checked for collision.
	*
	*	Return:					bool - true == colliding false == not
	*
	*	Mod. Date:				7/21/2014
	*	Mod. Initials:			CP
	**********************************/
	bool CheckCollision(CBaseColObj* obj1, CBaseColObj* obj2);


	bool AABBtoSPHERE (CAABB* const aabb, CSphere* const sphere);
	bool AABBtoCapsule (CAABB* const aabb, CCapsule* const capsule);
	bool AABBtoAABB (CAABB* const aabb1, CAABB* const aabb2);

	bool SPHEREtoSPHERE (CSphere* const sphere1, CSphere* const sphere2);
	bool SPHEREtoCAPSULE (CSphere* const sphere, CCapsule* const capsule);
	bool SPHEREtoTRIANGLE(CSphere* const sphere, CTriangle* const Tri);
	//bool CAPSULEtoCAPSULE (CCapsule* const capsule1, CCapsule* const capsule2);


	/**********************************
	*	IntersectRaySphere:		Check if a ray intersects a sphere
	*							
	*	In:						XMFLOAT3 sa - Start of the ray.
	*							XMFLOAT3 n - Direction of the ray.
	*							XMFLOAT3 center - center of the sphere.
	*							float radius - Direction of the ray.
	*							float t - Returns time of intersection.
	*							XMFLOAT3 q - Returns Position of the intersection.
	*
	*	Return:					bool - true == colliding false == not
	*
	*	Mod. Date:				7/21/2014
	*	Mod. Initials:			CP
	**********************************/
	bool IntersectRaySphere(const XMFLOAT3 &sa, const XMFLOAT3 &n, const XMFLOAT3 &center, float radius, float &t, XMFLOAT3 &q);

	/**********************************
	*	IntersectRaySphere:		Check if a ray intersects a sphere
	*							
	*	In:						XMFLOAT3 sa - Start of the ray.
	*							XMFLOAT3 n - Direction of the ray.
	*							CCapsule cap - Holds info of the Cylinder
	*							float t - Returns time of intersection.
	*
	*	Return:					bool - true == colliding false == not
	*
	*	Mod. Date:				7/21/2014
	*	Mod. Initials:			CP
	**********************************/
	bool IntersectRayCylinder(const XMFLOAT3 &sa, const XMFLOAT3 &n, CCapsule* const cap, float &t);

	/**********************************
	*	IntersectRaySphere:		Check if a ray intersects a sphere
	*							
	*	In:						XMFLOAT3 sa - Start of the ray.
	*							XMFLOAT3 n - Direction of the ray.
	*							CCapsule cap - Holds capsule info.
	*							float t - Returns time of intersection.
	*							XMFLOAT3 colPos - Returns Position of the intersection.
	*
	*	Return:					bool - true == colliding false == not
	*
	*	Mod. Date:				7/21/2014
	*	Mod. Initials:			CP
	**********************************/
	bool IntersectRayCapsule(const XMFLOAT3 &sa, const XMFLOAT3 &n, CCapsule* const cap, float &t, XMFLOAT3 &colPos);

};
#endif 
