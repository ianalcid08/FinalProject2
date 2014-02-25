#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_
/*///////////////////////////////////////////////////////////////////////////////////
/FileName:			StateManager.h
/Author:			Thomas Stefl
/Last Modified:		7/22/2013
/Purpose:			This class is to house and keep track of
/					the current state the game is in.
/					This manager will render,update, and take input
/					for the BACK most state on the stack
/					This will give it a FIFO funtionality.
*////////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include "../Header Files/Definitions.h"

// For an ugly resolution hack
#include "../Header Files/MainMenuState.h"

//enum eGameStates{ INTRO_STATE, MAINMENU_STATE,GAMEPLAY_STATE,DIFFICULTY_STATE, OPTIONS_STATE, CREDITS_STATE,PAUSE_STATE,RETRY_STATE, ANIM_STATE, NUM_STATES};
class IState;
class CGame;
class CInput;
//class CRender

//TO DO - evewhere an Int is being used
//		  substitute with a gamestate.


// TO DO - forward declare or #include all the states that youll need here

class StateManager
{
private:
	std::vector<IState*> m_currStates;
	CGame* m_Game;

public:
	//TO DO - Temp public , change later

	StateManager(void);
	~StateManager(void);
	void Initialize(CGame * _game);
	void Shutdown();
	void Input(CInput & _input);
	void Update(float _fElapsedTime);
	void Render();

	void ChangeState(eGameStates _eNewState);
	void PushState(eGameStates _eNewState);
	void PopState();
	IState* ProcessEnum(eGameStates _eIn);
	

	//Accessor(s)
	int GetNumCurrStates() {return m_currStates.size(); }
	int CetCurrStateID() { return ((CMainMenuState*)m_currStates.back())->GetID(); }

};

#endif