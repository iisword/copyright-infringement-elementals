/***********************************************
* Filename:  		Text.h
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Helps with writing text to the screen from a font.
************************************************/


#ifndef TEXT_FONT
#define TEXT_FONT

#include <string>
#include <map>
#include "..\Renderer\Renderer.h"

class CText
{
private:
	D2Dobject* fontObj;
	CRenderer* pR;

	struct fontInfo
	{
		float2 xy;
		float2 wh;
		float2 off;
	};

	map<char, fontInfo> m_mFontMap;


public:
	CText(void);
	~CText(void);

	bool Init(CRenderer* r, const char* fontDisc);
	//Writes Text not numbers or symbols yet. Scale doesn't Mean Anything either. 
	void WriteText(XMFLOAT2 pos, float scale, std::string text, XMFLOAT4 color);


};

#endif