#include "AnimMesh.h"


CAnimMesh::CAnimMesh(void)
{
	m_pFaces = nullptr;
	m_pVerts = nullptr;
	m_pBindPoseMats = nullptr;
	m_nNumVerts = 0;
	m_NumTriangles = 0;
}


CAnimMesh::~CAnimMesh(void)
{
	if(m_pVerts)
		{
			delete [] m_pVerts;
			m_pVerts = nullptr;
		}
	if(m_pFaces)
		{
			delete [] m_pFaces;
			m_pFaces = nullptr;
		}
	if(m_pBindPoseMats)
	{
		delete[] m_pBindPoseMats;
		m_pBindPoseMats=nullptr;
	}
}


/**********************************
*  FunctionName:       LoadAnimMeshFromXML
*
*  In:                 const char* fileName -- file path to open
*
*  Out:                Add parameters here
*
*  Return:             bool == returns true is load was OK
*
*  Mod. Date:          7/23/2014
*  Mod. Initials:      RT
*  Loads animated mesh from an .XML file format
**********************************/
struct tBindPose
{
	XMFLOAT4X4 BoneBindPose;
	int BonesParent;
};
bool CAnimMesh::LoadAnimMeshFromXML( const char* fileName )
{
	TiXmlDocument xml_Document;

	if(xml_Document.LoadFile(fileName)== false)
		return false;

	TiXmlElement* model_element;
	
	model_element = xml_Document.FirstChildElement("Model");

	if(model_element == 0)
		return false;

	

	TiXmlElement* mesh_element;

	mesh_element = model_element->FirstChildElement("Mesh");
	if(mesh_element == 0)
		return false;

	TiXmlElement* vert_count_element;
	TiXmlElement* poly_count_element;

	vert_count_element = mesh_element->FirstChildElement("VertCount");
	poly_count_element = mesh_element->FirstChildElement("PolyCount");

	if(vert_count_element == 0 || poly_count_element==0)
		return false;

	

	m_nNumVerts = atoi(vert_count_element->GetText());
	m_NumTriangles = atoi(poly_count_element->GetText());

	m_pVerts = new tAnimVert[m_nNumVerts];//.resize(Num_Verts); 
	m_pFaces = new tTRIANGLE[m_NumTriangles];//.resize(Num_Polys);

	TiXmlElement* vert_list_element;
	vert_list_element = mesh_element->FirstChildElement("vertList");

	TiXmlElement* poly_list_element;
	poly_list_element = mesh_element->FirstChildElement("polyList");

	if(vert_list_element==0 || poly_list_element==0)
		return false;


	TiXmlElement* vert_element;
	unsigned int vertexID;

	TiXmlElement* influences_element;
	TiXmlElement* BoneInfluence_element;
	unsigned int BoneIndices[4] = {0,0,0,0};
	float BoneWeights[4] = {0,0,0,0};
	int InfCount;

	for ( vert_element = vert_list_element->FirstChildElement( "Vert" ); vert_element != 0; vert_element = vert_element->NextSiblingElement( ))
	{
		// Vertex Id
		vertexID = atoi( vert_element->Attribute( "id" ));

		// Vertex Pos
		m_pVerts[vertexID].pos.x = ( float )atof( vert_element->FirstChildElement( "x" )->GetText( ));
		m_pVerts[vertexID].pos.y = ( float )atof( vert_element->FirstChildElement( "y" )->GetText( ));
		m_pVerts[vertexID].pos.z = ( float )atof( vert_element->FirstChildElement( "z" )->GetText( ));

		// Vertex Normal
		m_pVerts[vertexID].norm.x = ( float )atof( vert_element->FirstChildElement( "Nx" )->GetText( ));
		m_pVerts[vertexID].norm.y = ( float )atof( vert_element->FirstChildElement( "Ny" )->GetText( ));
		m_pVerts[vertexID].norm.z = ( float )atof( vert_element->FirstChildElement( "Nz" )->GetText( ));

		// Vertex Texture Coordinate
		m_pVerts[vertexID].UV.x = ( float )atof( vert_element->FirstChildElement( "Tu" )->GetText( ));
		m_pVerts[vertexID].UV.y = ( float )atof( vert_element->FirstChildElement( "Tv" )->GetText( ));
	
		//Bone Influences
		influences_element = vert_element->FirstChildElement("influences");

		if(influences_element == nullptr)
				continue;
		
		InfCount = 0; 
		//This should not be more than 4 bone influences / 4 bone weights
		for(BoneInfluence_element = influences_element->FirstChildElement("influence"); 
			BoneInfluence_element !=0;BoneInfluence_element= BoneInfluence_element->NextSiblingElement())
		{
			BoneIndices[InfCount] = atoi(BoneInfluence_element->FirstChildElement("index")->GetText());
			BoneWeights[InfCount] = atof(BoneInfluence_element->FirstChildElement("weight")->GetText());
			InfCount++;

		}
		//Now that we have our weights....we need to copy the info into XMFLOAT% data type

		
		m_pVerts[vertexID].BoneIndex = XMINT4(BoneIndices[0],BoneIndices[1],BoneIndices[2],BoneIndices[3]);
		m_pVerts[vertexID].BoneWeights = XMFLOAT4(BoneWeights[0],BoneWeights[1],BoneWeights[2],BoneWeights[3]);


	}

	TiXmlElement* polygon_element;
	unsigned int triCounter = 0;

	for ( polygon_element = poly_list_element->FirstChildElement( "Polygon" ); polygon_element != 0; polygon_element = polygon_element->NextSiblingElement( ))
	{
		m_pFaces[triCounter].points[2] = atoi( polygon_element->FirstChildElement( "i1" )->GetText( ));
		m_pFaces[triCounter].points[1] = atoi( polygon_element->FirstChildElement( "i2" )->GetText( ));
		m_pFaces[triCounter].points[0] = atoi( polygon_element->FirstChildElement( "i3" )->GetText( ));

		triCounter++;
	}

	TiXmlElement* skeleton_element;
	skeleton_element = model_element->FirstChildElement("Skeleton");
	m_nNumBones = atoi(skeleton_element->Attribute( "numBones" ));
	m_pBindPoseMats = new XMFLOAT4X4[m_nNumBones];

	TiXmlElement* joint_element;
	TiXmlElement* joint_transform_element;
	TiXmlElement* axis_element;

	vector<tBindPose> Skeleton; 
	//We'll use this vector to flatten the hierarchy
	Skeleton.resize(m_nNumBones);

	for ( joint_element = skeleton_element->FirstChildElement( "bone" ); joint_element != 0; joint_element = joint_element->NextSiblingElement( ))
	{
		int joint_index = atoi( joint_element->FirstChildElement( "name" )->Attribute( "index" ));
			
		Skeleton[joint_index].BonesParent = atoi(joint_element->FirstChildElement("parentIndex")->GetText());
		
		// Get the transform
		joint_transform_element = joint_element->FirstChildElement( "localTransform" );
		if ( joint_transform_element )
		{
			axis_element = joint_transform_element->FirstChildElement( "xAxis" );
			Skeleton[joint_index].BoneBindPose._11= ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._12= ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._13=-1.0f * ( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._14= ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			
			axis_element = joint_transform_element->FirstChildElement( "yAxis" );
			Skeleton[joint_index].BoneBindPose._21= ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._22= ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._23= -1.0f *( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._24= ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			
			axis_element = joint_transform_element->FirstChildElement( "zAxis" );
			Skeleton[joint_index].BoneBindPose._31 = -1.0f *( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._32 = -1.0f *( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._33 = ( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._34 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
			
			axis_element = joint_transform_element->FirstChildElement( "wAxis" );
			Skeleton[joint_index].BoneBindPose._41 = ( float )atof( axis_element->FirstChildElement( "x" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._42 = ( float )atof( axis_element->FirstChildElement( "y" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._43 = -1.0f *( float )atof( axis_element->FirstChildElement( "z" )->GetText( ));
			Skeleton[joint_index].BoneBindPose._44 = ( float )atof( axis_element->FirstChildElement( "w" )->GetText( ));
		}
	}
	

	//this loop flattens the hierarchy
	unsigned int size = Skeleton.size( );
	unsigned int parent;
	for (unsigned int i = 0; i < size; ++i )
	{
		XMMATRIX mResult = XMMatrixIdentity();

		for ( parent = i; parent < size; parent = Skeleton[parent].BonesParent )
		{
			XMMATRIX mLocal = XMLoadFloat4x4(&Skeleton[parent].BoneBindPose);
			
			//m = m * Skeleton[parent].BoneBindPose;
			mResult = XMMatrixMultiply(mResult,mLocal);
		}

				XMStoreFloat4x4(&m_pBindPoseMats[i], mResult);
	}


  	for(int i = 0 ; i< m_nNumBones; i++)
  	{
  		XMMATRIX mIdent = XMMatrixIdentity();
  		XMMATRIX mLocal = XMLoadFloat4x4(&m_pBindPoseMats[i]);
  		
  		XMMATRIX mWorld;
  		mWorld = XMMatrixMultiply(mIdent, mLocal);
  		XMVECTOR determinat;
  		XMMATRIX BindInverse=XMMatrixInverse(&determinat,mWorld);
  		XMStoreFloat4x4(&m_pBindPoseMats[i], BindInverse);
  	}


	return true;//we loaded the data 
}

/**********************************
*  FunctionName:       LoadAnimMeshFromBIN
*
*  In:                 const char* fileName -- file path to open
*
*  Out:                Add parameters here
*
*  Return:             bool == returns true is load was OK
*
*  Mod. Date:          7/23/2014
*  Mod. Initials:      RT
*  Loads animated mesh from an binary file format
**********************************/
bool CAnimMesh::LoadAnimMeshFromBIN( const char* fileName )
{
	return false;
}

