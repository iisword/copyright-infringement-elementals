#ifndef WIN_H
#define WIN_H

//WHY DO I GET ERRORS FOR NOT HAVING THIS IN ANY NEW CLASS I MAKE?!?!?!?!?!?!?!?!?!
#include <Raknet\RakNetTypes.h>

#include "basestate.h"
class CWinState : public CBaseState
{
private:
	CRenderer* m_pRenderer;
	CText* m_pFont;

	bool m_bWon;

public:
	CWinState(void);
	~CWinState(void);

	void Init(CRenderer* r, CText* font);
	void Update(float dTime);
	void Render(void);

	//Accessor and Mutators
	void SetWon(bool won) { m_bWon = won; }


	/* Some fun functions for later sprints hopefully
	bool LoadStats(string sFile);
	void SaveStats(void);

	sets and accsesors for the updating stats.
	*/
};

#endif