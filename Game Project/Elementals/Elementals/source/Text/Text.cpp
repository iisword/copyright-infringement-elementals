/***********************************************
* Filename:  		Text.cpp
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	CP
* Author:    		Charles Peter
* Purpose:   		Helps with writing text to the screen from a font.
************************************************/
#include "Text.h"
#include <fstream>

CText::CText(void)
{
}


CText::~CText(void)
{
	(*fontObj->GetDataBuffer())->Release();
	delete fontObj;

	m_mFontMap.clear();
}


bool CText::Init(CRenderer* r, const char* fontDisc)
{
	pR = r;


	ifstream input;
	input.open(fontDisc);
	if(input.good())
	{
		std::string fileDDS;
		getline(input, fileDDS);
		std::wstring file = L"assets/Font/";
		file += std::wstring(fileDDS.begin(), fileDDS.end());
		
		fontObj = new D2DObject(file.c_str(), 256.0f, 256.0f);
		pR->AddD2D(fontObj);

		char buf[5];
		input>>buf;
		int numChar = atoi(buf);


		for(int i = 0; i < numChar; ++i)
		{
			fontInfo nChar;
			char id[10], x[10], y[10], w[10], h[10], ox[10], oy[10];
			input>>id;
			int _id = atoi(id);
			input>>x;
			nChar.xy.x = (float)atof(x);
			input>>y;
			nChar.xy.y = (float)atof(y);
			input>>w;
			nChar.wh.x = (float)atof(w);
			input>>h;
			nChar.wh.y = (float)atof(h);
			input>>ox;
			nChar.off.x = (float)atof(ox);
			input>>oy;
			nChar.off.y = (float)atof(oy);
			
			m_mFontMap.insert(std::pair<char, fontInfo>(_id, nChar));
		}

		input.close();
		return true;
	}
	else 
		return false;

}

//Writes Text not numbers or symbols yet.
void CText::WriteText(XMFLOAT2 pos, float scale, std::string text, XMFLOAT4 color)
{
	XMFLOAT2 mPos = pos;
	for(unsigned int i = 0; i < text.size(); ++i)
	{
		if(text[i] != ' ')
		{
			XMFLOAT4 draw;
			draw = XMFLOAT4(mPos.x + (m_mFontMap[text[i]].off.x*scale), mPos.y + (m_mFontMap[text[i]].off.y * scale), 
				mPos.x + ((m_mFontMap[text[i]].wh.x + m_mFontMap[text[i]].off.x) * scale), 
				mPos.y + ((m_mFontMap[text[i]].wh.y + m_mFontMap[text[i]].off.y) * scale));

			XMFLOAT4 img = XMFLOAT4(m_mFontMap[text[i]].xy.x, m_mFontMap[text[i]].xy.y, 
				m_mFontMap[text[i]].wh.x , m_mFontMap[text[i]].wh.y);

			pR->Render2D(fontObj, draw, img);
			mPos.x += ((m_mFontMap[text[i]].wh.x*scale) +1);
		}
		else
			mPos.x += 10*scale;
	}
}