#include "Mesh.h"


CMesh::CMesh(void)
{
}


CMesh::~CMesh(void)
{
}
/**********************************
*  FunctionName:       LoadMeshFromXML()
*
*  In:                 const char* fileName -- file to load data from
*
*  Out:                Add parameters here
*
*  Return:             bool - Returns true if file loaded succesfuly
*
*  Mod. Date:          7/18/2014
*  Mod. Initials:      RT
**********************************/

bool CMesh::LoadMeshFromXML( const char* fileName )
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

	unsigned int Num_Verts, Num_Polys;

	Num_Verts = atoi(vert_count_element->GetText());
	Num_Polys = atoi(poly_count_element->GetText());

	m_pVerts.resize(Num_Verts); 
	m_vFaces.resize(Num_Polys);

	TiXmlElement* vert_list_element;
	vert_list_element = mesh_element->FirstChildElement("vertList");
	
	TiXmlElement* poly_list_element;
	poly_list_element = mesh_element->FirstChildElement("polyList");

	if(vert_list_element==0 || poly_list_element==0)
		return false;


	TiXmlElement* vert_element;
	unsigned int vertexID;
	
	
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
	}

	TiXmlElement* polygon_element;
	unsigned int triCounter = 0;

	for ( polygon_element = poly_list_element->FirstChildElement( "Polygon" ); polygon_element != 0; polygon_element = polygon_element->NextSiblingElement( ))
	{
		m_vFaces[triCounter].points[0] = atoi( polygon_element->FirstChildElement( "i1" )->GetText( ));
		m_vFaces[triCounter].points[1] = atoi( polygon_element->FirstChildElement( "i2" )->GetText( ));
		m_vFaces[triCounter].points[2] = atoi( polygon_element->FirstChildElement( "i3" )->GetText( ));

		triCounter++;
	}


	return true;//we loaded the data 
}
