#ifndef _TUTORIALPAUSE_H_
#define _TUTORIALPAUSE_H_

/*****************************************************************
File name:			TutorialPause.h
Date:				09/06/13
Modify Date:		08/06/13
Modify Initials:	CM
Author:				Corey Morehead
Purpose:			This Class will be the pause menu for the Tutorial
******************************************************************/

#include "IState.h"

class CGame;
class CRenderer;

enum eTutorialPauseChoices { TP_RESUME = 0, TP_PLAY, TP_MAINMENU, TP_NUM_CHOICES };
enum eTutorialPauseElements { TPAUSE_BACKGROUND_ELEMENTS = 0, TPAUSE_BLACK_BACKGROUND_ELEMENT,
	TPAUSE_PLAY_ELEMENTS, TPAUSE_MAINMENU_ELEMENTS, TPAUSE_RESUME_ELEMENTS,
	TPAUSE_RESUME_HIGHLIGHTED_ELEMENTS, TPAUSE_PLAY_HIGHLIGHTED_ELEMENTS,
	TPAUSE_MAINMENU_HIGHLIGHTED_ELEMENTS, TPAUSE_NUM_ELEMENTS };

class CTutorialPause : public IState
{
private:
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	int				m_Choice;
	float			m_Timer;
	CUIElement		m_TutorialPauseElements[TPAUSE_NUM_ELEMENTS];
	float			m_FadeTimer;
	bool			m_bFadeOut;
	bool			m_bMouseActive;
	int				m_PrevMousePosX;
	int				m_PrevMousePosY;
	bool			m_Esc;
	bool			m_bSoundResume;
	bool			m_bSoundPlay;
	bool			m_bSoundMainMenu;
	bool			m_bResume;
	bool			m_bMainMenu;
	bool			m_bPlay;
	CUIElement		m_reticle;

	CTutorialPause( void );
	virtual ~CTutorialPause( void );

public:
	static CTutorialPause* GetInstance();
	void Init(CGame* _game);
	void Shutdown( void );
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render( void );
	void Enter( void );
	void Exit( void ); 
	void FadeIn( float _fTime );
	void FadeOut( float _fTime );
};

#endif