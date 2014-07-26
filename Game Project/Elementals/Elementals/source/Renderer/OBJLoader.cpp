#include "OBJLoader.h"


OBJLoader::OBJLoader(void)
{
	size = 0;
}


OBJLoader::~OBJLoader(void)
{
//	bufferReturn.clear();
	delete[] bufferReturn;
//	delete bufferReturn; //Looks wrong here
	pDataObj.pSysMem = nullptr;
}

void OBJLoader::FileReader(const char * path)
{
	vector< float3 > temp_vertices;
	vector< float2 > temp_uvs;
	vector< float3 > temp_normals;	
	vector<commonObject> bufferReturnL;

	FILE * file = nullptr;
	fopen_s(&file, path, "r");
	int biggest = 0;
	if( file == NULL )
	{
		printf("Impossible to open the file !\n");
		return;
	}

	while( true )
	{
 
    char lineHeader[128];
    // read the first word of the line
	int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader) );// lineHeader);
    if (res == EOF)
        break; // EOF = End Of File. Quit the loop.
 
    // else : parse lineHeader
	if ( strcmp( lineHeader, "v" ) == 0 )
	{
		float3 vertex;
		fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
		temp_vertices.push_back(vertex);
	}
	else if ( strcmp( lineHeader, "vt" ) == 0 ){
		float2 uv;
		fscanf_s(file, "%f %f\n", &uv.x, &uv.y );
		uv.y *= -1;
		temp_uvs.push_back(uv);
	}
	else if ( strcmp( lineHeader, "vn" ) == 0 ){
		float3 normal;
		fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
		temp_normals.push_back(normal);
	}
	}
	fseek(file, 0, 0);
	while( true )
	{
 
    char lineHeader[128];
    // read the first word of the line
	int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader) );// lineHeader);
    if (res == EOF)
        break; // EOF = End Of File. Quit the loop.
	
	if ( strcmp( lineHeader, "f" ) == 0 )
	{
		//std::string vertex1, vertex2, vertex3;%
		unsigned int vertexIndex[4] = {0, 0, 0, 0}, uvIndex[4] = {0, 0, 0, 0}, normalIndex[4] = {0, 0, 0, 0};
		int matches = 0;
		if(temp_uvs.size() != 0)
			matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", 
			&vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
			&vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
		else if(temp_normals.size() == 0)
		{
			matches = fscanf_s(file, "%d %d %d %d\n", 
		    &vertexIndex[0],  
		    &vertexIndex[1],  
			&vertexIndex[2],  
			&vertexIndex[3] );
		}
		else//if there are no texture coords we need to skip the //
		{
			matches = fscanf_s(file, "%d%*c%*c%d %d%*c%*c%d %d%*c%*c%d %d%*c%*c%d\n", 
		    &vertexIndex[0], &normalIndex[0], 
		    &vertexIndex[1], &normalIndex[1], 
			&vertexIndex[2], &normalIndex[2], 
			&vertexIndex[3], &normalIndex[3] );
		}
		if(vertexIndex[0] == 0)//something went wrong and we dont have data
			continue;

		commonObject tempBuffer[4]; //some object to fill with data
		
		if(temp_normals.size() != 0)
		{
			tempBuffer[0].normal = temp_normals[ normalIndex[0]  -1];
			tempBuffer[1].normal = temp_normals[ normalIndex[1]  -1];
			tempBuffer[2].normal = temp_normals[ normalIndex[2]  -1];
		}

		if(temp_uvs.size() != 0) //if there are no uv's
		{
			tempBuffer[0].texture = temp_uvs[ uvIndex[0] - 1];
			tempBuffer[1].texture = temp_uvs[ uvIndex[1] - 1];
			tempBuffer[2].texture = temp_uvs[ uvIndex[2] - 1];
		}

		tempBuffer[0].vertex = temp_vertices[ vertexIndex[0] - 1];
		tempBuffer[1].vertex = temp_vertices[ vertexIndex[1] - 1];
		tempBuffer[2].vertex = temp_vertices[ vertexIndex[2] - 1];

		float3 normalUp;
		normalUp.x = normalUp.y = normalUp.z = 0;
		normalUp.y = 1;
		if(vertexIndex[3] != 0) //if this is a quad
		{
			if(temp_normals.size() != 0)
				tempBuffer[3].normal = temp_normals[ normalIndex[3]  -1];
			else 
				tempBuffer[3].normal = normalUp;
			if(temp_uvs.size() != 0)
				tempBuffer[3].texture = temp_uvs[ uvIndex[3] - 1];
			tempBuffer[3].vertex = temp_vertices[ vertexIndex[3] - 1];
		}

		if(vertexIndex[3] == 0 ) // it was just a triangle
			for(int i = 0; i < 3; i++)
				bufferReturnL.push_back(tempBuffer[i]);
		else
		{
			for(int i = 0; i < 3; i++)
				bufferReturnL.push_back(tempBuffer[i]);
			
				bufferReturnL.push_back(tempBuffer[0]);
				bufferReturnL.push_back(tempBuffer[2]);
				bufferReturnL.push_back(tempBuffer[3]);
		}
	
	}

}
	size = bufferReturnL.size();
	bufferReturn = new commonObject[size];
	memcpy(bufferReturn, &bufferReturnL[0], sizeof(commonObject) * size);

	bufferReturnL.clear();
	temp_normals.clear();
	temp_uvs.clear();
	temp_vertices.clear();
	objDesc.Usage = D3D11_USAGE_IMMUTABLE;
	objDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	objDesc.CPUAccessFlags = NULL;
	objDesc.ByteWidth = sizeof(commonObject) * size;
	objDesc.MiscFlags = 0;

	pDataObj.pSysMem = &bufferReturn[0];
}