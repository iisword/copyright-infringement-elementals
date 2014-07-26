#include "Animation.h"
#include "..\Utilities\tinyxml.h"

CAnimation::CAnimation(void)
{
	m_fDuration = 0.0f;
	m_szName = "NAME NOT SET";
}


CAnimation::~CAnimation(void)
{
}
/**********************************
*  FunctionName:       LoadAnimationXML
*
*  In:                 const char* FilePath -- which .XML file to open
*
*  Out:                Add parameters here
*
*  Return:             return True is load was OK
*
*  Mod. Date:          7/21/2014
*  Mod. Initials:      RT
**********************************/
bool CAnimation::LoadAnimationXML( const char* FilePath )
{
	TiXmlDocument xml_document;
	TiXmlElement* animation_element;
	TiXmlElement* duration_element;
	TiXmlElement* bone_element;
	TiXmlElement* key_element;
	TiXmlElement* frame_element;
	TiXmlElement* key_time_element;
	TiXmlElement* transform_element;
	TiXmlElement* axis_element;

	tAnimatedJoint animated_joint;
	int num_keys, current_key;

	if ( xml_document.LoadFile( FilePath ) == false )
		return false;

	animation_element = xml_document.FirstChildElement( "Animation" );
	if ( animation_element == 0 )
		return false;

	// Get Animation Name
	m_szName = animation_element->Attribute( "name" );

	duration_element = animation_element->FirstChildElement( "Duration" );
	if ( duration_element == 0 )
		return false;

	// Get Animation Duration
	m_fDuration = ( float )atof( duration_element->GetText( ));

	bone_element = animation_element->FirstChildElement( "animBone" );

	for ( bone_element = animation_element->FirstChildElement( "animBone" ); bone_element != 0; bone_element = bone_element->NextSiblingElement( ))
	{
		animated_joint.m_vKeyFrames.clear();
		
		animated_joint.m_nIndex	 = atoi( bone_element->Attribute( "boneIndex" ));

		key_element = bone_element->FirstChildElement( "KeyFrames" );
		if ( key_element == 0 )
			continue;

		num_keys = atoi( key_element->Attribute( "numKeys" ));

		animated_joint.m_vKeyFrames.resize( num_keys - 1 );

		frame_element = key_element->FirstChildElement( "keyFrame" );

		for ( frame_element = key_element->FirstChildElement( "keyFrame" ); frame_element != 0; frame_element = frame_element->NextSiblingElement( ))
		{
			current_key = atoi( frame_element->Attribute( "num" )) - 1;
			if ( current_key < 0 )
				continue;

			key_time_element = frame_element->FirstChildElement( "keyTime" );
			if ( key_time_element == 0 )
				continue;

			animated_joint.m_vKeyFrames[current_key].m_fKeyTime = ( float )atof( key_time_element->GetText( ));

			transform_element = frame_element->FirstChildElement( "localTransform" );
			if ( transform_element == 0 )
				continue;

			// Get the local transform
			axis_element = transform_element->FirstChildElement( "xAxis" );
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._11 = ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._12 = ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._13 = -1.0f*( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._14 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			axis_element = transform_element->FirstChildElement( "yAxis" );
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._21 = ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._22 = ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._23 = -1.0f *( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._24 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			axis_element = transform_element->FirstChildElement( "zAxis" );
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._31 = -1.0f*( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._32 = -1.0f*( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._33 = ( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._34 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			axis_element = transform_element->FirstChildElement( "wAxis" );
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._41 = ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._42 = ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._43 = +1.0f*( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_LocalMatrix._44 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));

			transform_element = frame_element->FirstChildElement( "worldTransform" );
			if ( transform_element == 0 )
				continue;

			// Get the world transform
			axis_element = transform_element->FirstChildElement( "xAxis" );
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._11 = ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._12 = ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._13 = -1.0f*( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._14 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			axis_element = transform_element->FirstChildElement( "yAxis" );
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._21 = ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._22 = ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._23 = -1.0f*( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._24 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			axis_element = transform_element->FirstChildElement( "zAxis" );
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._31 = -1.0f*( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._32 = -1.0f*( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._33 = ( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._34 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			axis_element = transform_element->FirstChildElement( "wAxis" );
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._41 = ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._42 = ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._43 = -1.0f*( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			animated_joint.m_vKeyFrames[current_key].m_WorldMatrix._44 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
		}
		m_vAnimtedJoints.push_back( animated_joint );
		
	}

	return true;
}
/**********************************
*  FunctionName:       LoadAnimationBIN
*
*  In:                 const char* FilePath -- which binary file to open
*
*  Out:                Add parameters here
*
*  Return:             return True is load was OK
*
*  Mod. Date:          7/21/2014
*  Mod. Initials:      RT
**********************************/
bool CAnimation::LoadAnimationBIN( const char* FilePath )
{
	//TODO:  LOAD BINARY ANIM FILE
	return false;
}

// Returns the number of Joints/Bones this animation contains.
const unsigned int CAnimation::GetNumJoints() const
{
	{
		
		return(unsigned int)m_vAnimtedJoints.size();
	}
}





//////////////////////////////////////////////////////////////////////////////
//**********************IGNORE COMMENTED STUFF BELOW**************************


//BullshitFunctionTEST();
//m_pRenderer->Render3D(&Bullshit3Dobj,BullshitTexture);

//void CGameState::BullshitFunctionTEST()
//{
//	CMesh Test;
//	Test.LoadMeshFromXML("assets/YO.xml");
//
//	size_t numverts = Test.GetVerts().size();
//	size_t numtri = Test.GetTris().size();
//
//	commonObject* Buffer = new commonObject[numtri*3];
//	unsigned int V1,V2,V3;
//	for(int i =0; i< numtri; ++i)
//	{
//		V1 = Test.GetTris()[i].points[0];
//		V2 = Test.GetTris()[i].points[1];
//		V3 = Test.GetTris()[i].points[2];
//		
//		Buffer[i*3].vertex.x = Test.GetVerts()[V1].pos.x;
//		Buffer[i*3].vertex.y = Test.GetVerts()[V1].pos.y;
//		Buffer[i*3].vertex.z = Test.GetVerts()[V1].pos.z;
//		
//		Buffer[i*3].normal.x = Test.GetVerts()[V1].norm.x;
//		Buffer[i*3].normal.y = Test.GetVerts()[V1].norm.y;
//		Buffer[i*3].normal.z = Test.GetVerts()[V1].norm.z;
//
//		Buffer[i*3].texture.x = Test.GetVerts()[V1].UV.x;
//		Buffer[i*3].texture.y = Test.GetVerts()[V1].UV.y;
//		//********************************************
//		Buffer[i*3+1].vertex.x = Test.GetVerts()[V2].pos.x;
//		Buffer[i*3+1].vertex.y = Test.GetVerts()[V2].pos.y;
//		Buffer[i*3+1].vertex.z = Test.GetVerts()[V2].pos.z;
//				
//		Buffer[i*3+1].normal.x = Test.GetVerts()[V2].norm.x;
//		Buffer[i*3+1].normal.y = Test.GetVerts()[V2].norm.y;
//		Buffer[i*3+1].normal.z = Test.GetVerts()[V2].norm.z;
//				
//		Buffer[i*3+1].texture.x = Test.GetVerts()[V2].UV.x;
//		Buffer[i*3+1].texture.y = Test.GetVerts()[V2].UV.y;
//		//*************************************************
//		Buffer[i*3+2].vertex.x = Test.GetVerts()[V3].pos.x;
//		Buffer[i*3+2].vertex.y = Test.GetVerts()[V3].pos.y;
//		Buffer[i*3+2].vertex.z = Test.GetVerts()[V3].pos.z;
//				  
//		Buffer[i*3+2].normal.x = Test.GetVerts()[V3].norm.x;
//		Buffer[i*3+2].normal.y = Test.GetVerts()[V3].norm.y;
//		Buffer[i*3+2].normal.z = Test.GetVerts()[V3].norm.z;
//				
//		Buffer[i*3+2].texture.x = Test.GetVerts()[V3].UV.x;
//		Buffer[i*3+2].texture.y = Test.GetVerts()[V3].UV.y;
//		
//
//	}
//
//	D3D11_SUBRESOURCE_DATA pDataObj;
//	D3D11_BUFFER_DESC objDesc;
//	
//	objDesc.Usage = D3D11_USAGE_IMMUTABLE;
//	objDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	objDesc.CPUAccessFlags = NULL;
//	objDesc.ByteWidth = (sizeof(commonObject) * numtri *3);
//	objDesc.MiscFlags = 0;
//
//	pDataObj.pSysMem = &Buffer[0];
//
//	Bullshit3Dobj.GetObjLoader()->SetALLFields(pDataObj,objDesc,Buffer,numtri*3);
//	BullshitTexture = new D2Dobject(L"assets/PPG_3D_Player_Diffuse.dds",1024.0f,1024.0f);
//	m_pRenderer->AddD2D(BullshitTexture);
//
//	m_pRenderer->BullshitCreateBuffer(Bullshit3Dobj.GetObjLoader()->GetBufferDesc(), Bullshit3Dobj.GetObjLoader()->GetData(), Bullshit3Dobj.GetBuffer());
//		
//}


//This is from objLoader

// void SetALLFields(D3D11_SUBRESOURCE_DATA& DataObj, D3D11_BUFFER_DESC & OBJ_DESC, 
// 				  commonObject * pBuff, unsigned int BUFsize)
// {
// 	pDataObj = DataObj;
// 	objDesc = OBJ_DESC;
// 	bufferReturn = pBuff;
// 	size = BUFsize;
// }

//this is from the renderer

// void BullshitCreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* sub,ID3D11Buffer** buf)
// {
// 	device->CreateBuffer(desc,sub,buf);
// }