#ifndef _CREDITSMENUSTATE_H_
#define _CREDITSMENUSTATE_H_
/*****************************************************************
File name:			CreditsMenu.h
Date:				7/17/13
Modify Date:		7/17/13
Modify Initials:	IA
Author:				Ian Alcid
Purpose:			Class for main menu state.
******************************************************************/

#include "IState.h"
class CInput;
class CGame;
class CRenderer;
class CAssetManager;

enum eCMElements { CM_BACKGROUND_ELEMENT = 0, CM_CREDITS_ELEMENTS, CM_BLACK_BACKGROUND_ELEMENT, CM_BUTTONS, CM_CONTROLLER, CM_NUM_ELEMENTS };

class CCreditsState : public IState
{
	CCreditsState();
	virtual ~CCreditsState();

public:
	// Singleton
	static CCreditsState* GetInstance();

	// IState
	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render();
	void Enter();
	void Exit();

private:
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;
	CUIElement		m_CMElements[CM_NUM_ELEMENTS];
	//int				m_Choice;
	float			m_Timer;
	bool			m_Switch;
	float			m_Movement;
	float			m_Speed;
	float			m_FadeTimer;
	bool			m_bFadeOut;

	bool			m_bSoundCheck;
};

#endif