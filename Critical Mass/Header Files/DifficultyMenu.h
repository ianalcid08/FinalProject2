#ifndef _DIFFICULTYMENUSTATE_H_
#define _DIFFICULTYMENUSTATE_H_
/*****************************************************************
File name:			DifficultyMenu.h
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

enum eDMChoices { CHOICE_EASY,CHOICE_NORMAL, CHOICE_HARD,CHOICE_BACK, CHOICE_TOTAL };
enum eDMElements { DM_BACKGROUND,DM_BLACK_BG,
					DM_EASY,DM_NORMAL, DM_HARD ,DM_BACK,
					DM_ALL3_ELEMENT, DM_ALL3_CONTROLLER_ELEMENT,
					DM_EASY_HL,DM_NORMAL_HL, DM_HARD_HL ,DM_BACK_HL, GAME_LOGO,
					DM_MAX_ELEMENTS};

class CDifficultyMenu : public IState
{
	CDifficultyMenu();
	virtual ~CDifficultyMenu();
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;

	float m_Timer;
	float m_FadeTimer;
	bool  m_FadeOut;

	int m_Choice;
	CUIElement		m_DMElements[DM_MAX_ELEMENTS];

	bool m_MouseActive;
	int m_PrevMousePosX;
	int m_PrevMousePosY;

	bool	m_SoundEasy;
	bool	m_SoundNormal;
	bool	m_SoundHard;
	bool	m_SoundBack;
	CUIElement		m_reticle;

	bool	m_isHighlighted;
public:
	static CDifficultyMenu* GetInstance();
	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render();
	void Enter();
	void Exit(); 
};


#endif