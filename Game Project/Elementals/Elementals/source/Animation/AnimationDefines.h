/***********************************************
* Filename:  		AnimationDefines.h
* Date:      		7/16/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		Common Defines used by animation system.
************************************************/

#ifndef _ANIMATION_DEFINES_H_
#define _ANIMATION_DEFINES_H_
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;

// Key Frame structure.
struct tKeyFrame
{
	float m_fKeyTime;
	XMFLOAT4X4 m_LocalMatrix;
	XMFLOAT4X4 m_WorldMatrix;
};
// Animated Joint / Bone
struct tAnimatedJoint
{
	unsigned int m_nIndex;
	vector<tKeyFrame> m_vKeyFrames;
};

//Vertex 
struct tVERTEX
{
	XMFLOAT3 pos;
	XMFLOAT3 norm;
	XMFLOAT2 UV;
};
typedef tVERTEX tDXVERT;

//Animated Vertex
struct tAnimVert
{
	XMFLOAT3 pos;
	XMFLOAT3 norm;
	XMFLOAT2 UV;

	//These two values will be based of animation. 
	// EXAMPLE:
	// 2 bone influences will Equate to XMINT2.x = 1st bone inf
	//									XMINT2.y = 2nd bone inf
	// The Bone weights MUST match the num of bone indices
	//									XMFLOAT2.x = weight for 1st bone
	//									XMFLOAT2.y = weight for 2nd bone

	//Assumes 4 bones max = 4 bone weights
	XMINT4 BoneIndex;
	XMFLOAT4 BoneWeights;
};

struct tTRIANGLE
{
	unsigned int points[3];
};


#endif