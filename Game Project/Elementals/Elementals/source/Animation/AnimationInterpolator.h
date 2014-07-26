/***********************************************
* Filename:  		AnimationInterpolator.h
* Date:      		7/21/2014
* Mod. Date: 		7/21/2014
* Mod. Initials:	RT
* Author:    		Roman Taran
* Purpose:   		This is animation iterpolator class, will be responsible for interpolation of key frames
************************************************/

#ifndef _CANIMATION_INTERPOLATOR_H_
#define _CANIMATION_INTERPOLATOR_H_
#include "AnimationDefines.h"
#include "Animation.h"
#include "../Object/Object.h"

class CRenderer;
class CMatrix;

class CAnimationInterpolator
{
	const CAnimation* m_Animation;
	float m_fCurrentTime;
	vector<XMFLOAT4X4> m_vInterpolatedJoints;

	const XMFLOAT4X4 * m_BindPoses;

	
#if _DEBUG
private:
	int m_nKeyFrame;
	
	
public:
	vector<CObject*> m_vBones;
	void GoToNextKeyFrame();
	void GoToPrevKeyFrame();

	void CreateBones(CRenderer * renderer);

#endif

public:

	void AddTime(float Time);
	void SetTime(float Time);
	
	inline float GetTime() const {return m_fCurrentTime;}
	inline const CAnimation* GetAnimation(void) const {return m_Animation;}
	void SetBindPoses(const XMFLOAT4X4 * bindposes) {m_BindPoses = bindposes;}
	
	XMFLOAT4X4* GetBones();     
	const unsigned int GetNumBones(); 
	const vector<XMFLOAT4X4>& InterpolatedJoints() const { return m_vInterpolatedJoints; }
	void SetAnimation(const CAnimation* anim);
	
	void DebugProcess(CMatrix mat);
	void Process();
	
	CAnimationInterpolator(void);
	~CAnimationInterpolator(void);




};
#endif
