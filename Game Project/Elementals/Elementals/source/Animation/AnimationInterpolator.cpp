#include "AnimationInterpolator.h"
#include "../Renderer/Renderer.h"
#include "../Object/ObjectManager.h"

CAnimationInterpolator::CAnimationInterpolator(void)
{
	m_Animation = nullptr;
	m_fCurrentTime = 0.0f;
	m_BindPoses = nullptr;
#if _DEBUG
	m_nKeyFrame = 0;
#endif
}


CAnimationInterpolator::~CAnimationInterpolator(void)
{
}
/**********************************
*  FunctionName:       AddTime()
*
*  In:                 float Time
*
*  Out:                Add parameters here
*
*  Return:             void
*
*  Mod. Date:          7/21/2014
*  Mod. Initials:      RT
**********************************/

void CAnimationInterpolator::AddTime( float Time )
{
	if ( m_Animation == nullptr )
	{
		m_fCurrentTime = 0.0f;
	}
 	else
 	{
 	
 		m_fCurrentTime = fmod( m_fCurrentTime + Time, m_Animation->GetDuration( ));
 	}

	if(m_fCurrentTime < 0.0f)
	{
		m_fCurrentTime+= m_Animation->GetDuration();
	}
	else if(m_fCurrentTime> m_Animation->GetDuration())
	{
		m_fCurrentTime -= m_Animation->GetDuration();
	}
}
/**********************************
*  FunctionName:       Add details here
*
*  In:                 Add parameters here
*
*  Out:                Add parameters here
*
*  Return:             void
*
*  Mod. Date:          -/-/2014
*  Mod. Initials:      RT
**********************************/
void CAnimationInterpolator::SetTime( float Time )
{
	if ( m_Animation == nullptr )
	{
		m_fCurrentTime = 0.0f;
	}
	else
	{
		m_fCurrentTime = fmod( Time, m_Animation->GetDuration( ));
	}
}
/**********************************
*  FunctionName:       Add details here
*
*  In:                 Add parameters here
*
*  Out:                Add parameters here
*
*  Return:             void
*
*  Mod. Date:          -/-/2014
*  Mod. Initials:      RT
**********************************/
void CAnimationInterpolator::Process()
{

	if(m_BindPoses==nullptr)
	{
		OutputDebugString(L"Interpolator does not have valid Bind POSES!");
		return;
	}
	// 	float TweenTime, Lambda; //UNREFERENCED LOCAL VARIABLES
	// 	unsigned int nCurrFrame, nNextFrame;
	unsigned int nNumFrames = m_Animation->GetAnimatedJoints()[0].m_vKeyFrames.size();

	for(unsigned int i =0; i < nNumFrames;)
	{
		if(m_fCurrentTime>m_Animation->GetAnimatedJoints()[0].m_vKeyFrames[i].m_fKeyTime)
			++i;
		else
		{
			m_nKeyFrame= i;
			break;
		}
	}
//  	for(unsigned int i= 0; i <m_Animation->GetAnimatedJoints().size();++i)
//  	{
//  		m_vInterpolatedJoints[i] = m_BindPoses[i];
//  	}
	

  	for(unsigned int i= 0; i <m_Animation->GetAnimatedJoints().size();++i)
  	{
 		XMMATRIX matBind = XMLoadFloat4x4(&m_BindPoses[i]);
 		XMMATRIX matPose =  XMLoadFloat4x4(&m_Animation->GetAnimatedJoints()[i].m_vKeyFrames[m_nKeyFrame].m_WorldMatrix);
 		XMMATRIX matResult = XMMatrixMultiply(matBind, matPose);
   		XMStoreFloat4x4(&(m_vInterpolatedJoints[i]), matResult);
  
  		 //m_vInterpolatedJoints[i]= m_Animation->GetAnimatedJoints()[i].m_vKeyFrames[m_nKeyFrame].m_WorldMatrix;
  
  	}
}


// this function set the animation
void CAnimationInterpolator::SetAnimation( const CAnimation* anim )
{
	m_fCurrentTime = 0.0f;
	m_Animation = anim;

	if(anim)
	{
		m_vInterpolatedJoints.resize(m_Animation->GetAnimatedJoints().size());
	}
		
}

#if _DEBUG
void CAnimationInterpolator::GoToNextKeyFrame()
{
	m_nKeyFrame++;

	if( (unsigned)m_nKeyFrame >m_Animation->GetAnimatedJoints()[0].m_vKeyFrames.size()-1)
	{
		m_nKeyFrame = 0;
	}
}

void CAnimationInterpolator::GoToPrevKeyFrame()
{
	m_nKeyFrame--;

	if( m_nKeyFrame < 0)
	{
		m_nKeyFrame = (unsigned int)m_Animation->GetAnimatedJoints()[0].m_vKeyFrames.size()-1;
	}
}

void CAnimationInterpolator::CreateBones(CRenderer * renderer )
{
	if(m_Animation!= nullptr)
	{
		m_vBones.clear();
		m_vBones.resize(m_Animation->GetAnimatedJoints().size());

		m_vBones[0] = new CObject();
		m_vBones[0]->SetMesh(renderer->AddModel(m_vBones[0]->GetMesh(),
			"assets/sphere.obj"));
		
		for(unsigned int i = 1; i < m_vBones.size();++i)
		{
			m_vBones[i] = new CObject();
			m_vBones[i]->CopyMesh(m_vBones[0]->GetMesh());
		}
	}
	
}
#endif

XMFLOAT4X4* CAnimationInterpolator::GetBones()
{
	return &m_vInterpolatedJoints[0];
	
}

const unsigned int CAnimationInterpolator::GetNumBones()
{
		return (unsigned int) m_vInterpolatedJoints.size();
}

void CAnimationInterpolator::DebugProcess(CMatrix mat)
{

#if _DEBUG	
		XMFLOAT4X4 temp;
		 	temp = *mat.GetMatrix();
		// 	//TODO:: interpolate the bone matrices



	// 	float TweenTime, Lambda; //UNREFERENCED LOCAL VARIABLES
	// 	unsigned int nCurrFrame, nNextFrame;
	unsigned int nNumFrames = m_Animation->GetAnimatedJoints()[0].m_vKeyFrames.size();

	for(unsigned int i =0; i < nNumFrames;)
	{
		if(m_fCurrentTime>m_Animation->GetAnimatedJoints()[0].m_vKeyFrames[i].m_fKeyTime)
			++i;
		else
		{
			m_nKeyFrame= i;
			break;
		}
	}

	for(unsigned int i= 0; i <m_Animation->GetAnimatedJoints().size();++i)
	{
		m_vInterpolatedJoints[i] = m_Animation->GetAnimatedJoints()[i].m_vKeyFrames[m_nKeyFrame].m_WorldMatrix;
		m_vBones[i]->GetMesh()->SetMatrix(m_vInterpolatedJoints[i]);


		 
		 		XMMATRIX matScal = XMLoadFloat4x4(m_vBones[i]->GetMesh()->GetMatrix());
		 		XMMATRIX matTemp =  XMLoadFloat4x4(&temp);
		 		matScal = XMMatrixMultiply(matScal, matTemp);
		 		XMStoreFloat4x4(&(*m_vBones[i]->GetMesh()->GetCMatrix()->GetMatrix()), matScal);
		 
		 		m_vBones[i]->Scale(XMFLOAT3(0.25f,0.25f,0.25f));

	}
	return;
#endif
}

