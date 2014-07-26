#pragma once

//#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
using namespace std;
#include "commonObject.h"

#include <D3D11.h>
#pragma comment(lib,"d3d11.lib")
#include <DirectXMath.h>
#include <DirectXPackedVector.h> 
#include <DirectXColors.h> 
#include <DirectXCollision.h>
using namespace DirectX;

class OBJLoader
{
	D3D11_SUBRESOURCE_DATA pDataObj;
	D3D11_BUFFER_DESC objDesc;
	commonObject * bufferReturn;
	unsigned int size;

public:
	OBJLoader(void);
	~OBJLoader(void);

	void FileReader(const char * path);
	commonObject * dataPointer(){ return &bufferReturn[0]; };
	commonObject * GetCommonObject(unsigned int i){ return &bufferReturn[i]; };

	unsigned int Size(){ return size; };


	D3D11_SUBRESOURCE_DATA * GetData(){ return &pDataObj; };
	D3D11_BUFFER_DESC * GetBufferDesc(){ return &objDesc; };
	const void * GetpDataSysMem(){ return pDataObj.pSysMem; };

};

