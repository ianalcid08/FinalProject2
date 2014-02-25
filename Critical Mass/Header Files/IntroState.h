#ifndef _INTROSTATE_H_
#define _INTROSTATE_H_
/*****************************************************************
File name:			IntroState.h
Date:				7/17/13
Modify Date:		7/17/13
Modify Initials:	IA
Author:				Ian Alcid
Purpose:			Class for main menu state.
******************************************************************/

#include "IState.h"

class CGame;
class CRenderer;
class CAssetManager;

enum eINElements { IN_MOUSE_ELEMENT = 0, IN_CONTROLLER_ELEMENT, IN_NUMBER_ELEMENTS };

class CIntroState : public IState
{
	
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;
	int				m_choice;
	float			m_timer;
	float			m_FadeTimerOut;
	//float			m_FadeTimerOut1;
	float			m_FadeTimerIn1;
	float			m_FadeTimerIn;
	bool			m_Switch;
	CUIElement		eINElements[IN_NUMBER_ELEMENTS];
	bool			m_bControlOut;
	bool			m_bControlIn;
	bool			m_bMouseIn;
	bool			m_bMouseOut;
	bool			m_bEnter;

	float			m_FadeTimer;
	bool			m_bControlFadeOut;
	bool			m_bMouseFadeOut;
	bool			m_bControl;
	bool			m_bGTFO;
	bool			m_bMouse;
	float			GTFOTimer;
	CUIElement		m_BackGround;
	CUIElement		m_BlackBackground;
	bool			m_bSoundMouse;
	bool			m_bSoundControl;
		bool			m_FadeIn_Out;

	//CUIElement		controllerScreen;

	CIntroState();
	~CIntroState();

public:
	static CIntroState* GetInstance( void );

	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render();
	void Enter();
	void Exit(); 
};


#endif