#ifndef _LOSE_STATE_H_
#define _LOSE_STATE_H_
/*///////////////////////////////////////////////////////////////////////////////////
/FileName:		LoseState.h
/Author:		Thomas Stefl
				Corey Morehead
/Last Modified: 7/30/2013
				08/10/13
/Purpose:		This is the state that will handle input and rendering of necessary
/				items when the player loses the game.
*////////////////////////////////////////////////////////////////////////////////////
#include "IState.h"

class CInput;
class CGame;
class CGame;
class CRenderer;
class CUIElement;
class CAssetManager;

enum eLoseElements {	LOSE_BACKGROUND_ELEMENT = 0,
						LOSE_MAIN_ELEMENT, LOSE_PLAY_ELEMENT,
						LOSE_MAIN_HIGHLIGHTED_ELEMENT, LOSE_PLAY_HIGHLIGHTED_ELEMENT,
						LOSE_ALL3_ELEMENT, LOSE_ALL3_CONTROLLER_ELEMENT,
						LOSE_HIGHLIGHTED_ELEMENT, LOSE_BLACK_BACKGROUND_ELEMENT, LOSE_NUM_ELEMENTS};
enum eLoseChoices { L_MAIN_MENU, L_PLAY, L_NUM_CHOICES};

class LoseState : public IState
{
private:
	LoseState( void );
	virtual ~LoseState( void );

	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;
	int				m_Choice;
	CUIElement		m_LoseElements[LOSE_NUM_ELEMENTS];
	bool			m_bMain;
	bool			m_bPlay;

	float			m_FadeTimer;
	float			m_Timer;
	bool			m_bFadeOut;

	bool			m_bMouseActive;
	int				m_PrevMousePosX;
	int				m_PrevMousePosY;

	bool			m_bSoundMain;
	bool			m_bSoundPlay;
	bool			m_bSoundCheck;
	bool			m_bEsc;
	CUIElement		m_reticle;
	CUIElement		m_BlackLetterbox;

	void ProcessStates(eLoseChoices _choice);

public:
	static LoseState* GetInstance();
	void Init(CGame* _game);
	void Shutdown( void );
	bool Input(CInput& _input );
	void Update(float _fElapsedTime);
	void Render( void );
	void Enter( void );
	void Exit( void ); 
};

#endif