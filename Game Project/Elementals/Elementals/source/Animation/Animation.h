/***********************************************
* Filename:  		Animation.h
* Date:      		7/16/2014
* Mod. Date: 		7/16/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		This Class contains animation data.
************************************************/



#ifndef _ANIMATION_H
#define _ANIMATION_H
#include "AnimationDefines.h"

class CAnimation
{
	float m_fDuration;
	std::string m_szName;

	vector<tAnimatedJoint> m_vAnimtedJoints;

public:
	CAnimation(void);
	~CAnimation(void);

	inline float GetDuration()const {return m_fDuration;}
	inline const string& GetName() const {return m_szName;}
	inline const vector<tAnimatedJoint>& GetAnimatedJoints()const {return m_vAnimtedJoints;}
	const unsigned int GetNumJoints() const;
	

	bool LoadAnimationXML(const char* FilePath);
	bool LoadAnimationBIN(const char* FilePath);

	//D3DObject Bullshit3Dobj;
	//D2Dobject* BullshitTexture;
	//void BullshitFunctionTEST();



};

#endif
