#ifndef _WIN_STATE_H_
#define _WIN_STATE_H_
/*///////////////////////////////////////////////////////////////////////////////////
/FileName:			WinState.h
/Author:			Thomas Stefl
/Last Modified:		7/30/2013
/Purpose:			This is the state that will handle input and rendering of necessary
/					items when the player wins the game.
*////////////////////////////////////////////////////////////////////////////////////

//TO DO - When added to main build, uncomment these things
#include "IState.h"

class CInput;
class CGame;
class CRenderer;
class CAssetManager;

enum eWinElements { WIN_BACKGROUND_ELEMENT, WIN_MAIN_ELEMENT, WIN_CREDITS_ELEMENT, WIN_EXIT_ELEMENT,
					WIN_MAIN_HIGHLIGHTED_ELEMENT, WIN_CREDITS_HIGHLIGHTED_ELEMENT,
					WIN_ALL3_ELEMENT, WIN_ALL3_CONTROLLER_ELEMENT,
					WIN_HIGHLIGHTED_ELEMENT, WIN_BLACK_BACKGROUND_ELEMENT, WIN_NUM_ELEMENTS};
enum eWinChoices { W_CREDITS, WIN_NUM_CHOICES};

class WinState : public IState
{
private:
	WinState(void);
	virtual ~WinState(void);

	//CUIElement winText;

	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;
	int				m_Choice;
	CUIElement		m_WinElements[WIN_NUM_ELEMENTS];
	bool			m_bMain;
	bool			m_bCredits;

	bool			m_bFadeOut;
	float			m_FadeTimer;
	float			m_Timer;

	bool			m_bMouseActive;
	int				m_PrevMousePosX;
	int				m_PrevMousePosY;

	bool			m_bSoundMain;
	bool			m_bSoundCredits;
	bool			m_bSoundCheck;
	bool			m_bEsc;
	CUIElement		m_reticle;
	CUIElement		m_BlackLetterbox;

	void ProcessStates(eWinChoices _choice);

public:

	static WinState* GetInstance();
	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render();
	void Enter();
	void Exit(); 

	//Accessors
	int GetChoice() {return m_Choice;}
	//int GetTimer() {return m_timer;}
	//bool IsClosing() {return m_closing;}

	//Mutators
	void SetChoice(int _newChoice) {m_Choice = _newChoice;}
	//void SetIsClosing (bool _isClosing){m_closing = _isClosing;}

};

#endif