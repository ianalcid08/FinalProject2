#ifndef _TUTORIALMENU_H_
#define _TUTORIALMENU_H_

/*****************************************************************
File name:			TutorialMenu.h
Date:				09/04/13
Modify Date:		09/04/13
Modify Initials:	CM
Author:				Corey Morehead
Purpose:			This class will give a option to the player, 
					they would care to play thru the tutorial or not
******************************************************************/

#include "IState.h"

class CGame;
class CRenderer;
enum eTutorialChoices {		TUTORIAL_YES, TUTORIAL_NO, TUTORIAL_ESC, TUTORIAL_NUM_CHOICES };
enum eTutorialElements {	TUTORIAL_BACKGROUND_ELEMENT = 0, TUTORIAL_BLACK_BACKGROUND_ELEMENT,
							TUTORIAL_YES_ELEMENT, TUTORIAL_NO_ELEMENT, TUTORIAL_YES_HIGHLIGHTED_ELEMENT, TUTORIAL_TEXT_ELEMENT,
							TUTORIAL_NO_HIGHLIGHTED_ELEMENT, TUTORIAL_NUM_ELEMENTS };

class CTutorialMenu : public IState
{
private:
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	float			m_Timer;
	float			m_FadeTimer;
	bool			m_bFadeOut;
	bool			m_bMouseActive;
	int				m_PrevMousePosX;
	int				m_PrevMousePosY;
	int				m_Choice;
	bool			m_bSoundYes;
	bool			m_bSoundNo;
	bool			m_bNo;
	bool			m_bYes;
	CUIElement		m_TutorialElements[TUTORIAL_NUM_ELEMENTS];
	CUIElement		m_reticle;

	CTutorialMenu(void);
	virtual ~CTutorialMenu(void);
public:
	static CTutorialMenu* GetInstance();
	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render( void );
	void Enter( void );
	void Exit( void ); 
	void FadeIn(float _fTime);
	void FadeOut(float _fTime);
};

#endif