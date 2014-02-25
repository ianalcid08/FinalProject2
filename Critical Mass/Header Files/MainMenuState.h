#ifndef _MAINMENUSTATE_H_
#define _MAINMENUSTATE_H_
/*****************************************************************
File name:			MainMenuState.h
Date:				7/17/13
Modify Date:		08/13/13
Modify Initials:	CM
Author:				Ian Alcid
Purpose:			Class for main menu state.
******************************************************************/

#include "IState.h"
#include "SpaceObjects.h"
#include "Camera.h"
enum eMMChoices { MM_PLAY = 0, MM_OPTIONS, MM_CREDITS, MM_EXIT, MM_NUM_CHOICES };
enum eMMElements {	MM_BACKGROUND_ELEMENT = 0, MM_PLAY_ELEMENT, MM_OPTIONS_ELEMENT,
					MM_CREDITS_ELEMENT, MM_EXIT_ELEMENT, MM_BLACK_BACKGROUND_ELEMENT, MM_HIGHLIGHTED_ELEMENT, MM_CRITICALMAS_ELEMENT,
					//MM_MOVE_ELEMENT, MM_OK_ELEMENT, MM_BACK_ELEMENT, MM_MOVE_CONTROLLER_ELEMENT, MM_OK_CONTROLLER_ELEMENT, MM_BACK_CONTROLLER_ELEMENT,
					MM_ALL3_ELEMENT, MM_ALL3_CONTROLLER_ELEMENT,
					MM_PLAY_HIGHLIGHTED_ELEMENT, MM_CREDITS_HIGHLIGHTED_ELEMENT,
					MM_OPTIONS_HIGHLIGHTED_ELEMENT, MM_EXIT_HIGHLIGHTED_ELEMENT, 
					MM_BOARDER_ELEMENT,
					MM_NUM_ELEMENTS};
//class CInput;w
class CGame;
class CRenderer;
class CAssetManager;
class CObjectFactory;
class CObjectManager;
class IEntity;
class CLandingPads;
class CCinematicSystem;

class CMainMenuState : public IState
{
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;
	CCinematicSystem *m_Cinema;
	int				m_Choice;
	
	// new variables for the 3d scene
	CAssetManager	*m_AssetManager;
	CObjectFactory	*m_ObjFactory;
	CObjectManager	*m_ObjManager;
	//IEntity			*m_Player;
	CCamera			m_Camera;
	//IEntity			*skybox;
	IEntity			*m_LandingPads[2];
	IEntity			*m_DepotRot[6];
	IEntity			*m_World;
	//float			m_PlayerMove;
	//bool			m_LerpBools[4];
	CUIElement		m_reticle;

	SpaceObjects	m_testAsteroid;      // could be an array later
	SpaceObjects	m_testPlanet;
	SpaceObjects    m_testSun;

	CUIElement		m_MMElements[MM_NUM_ELEMENTS];
	bool			m_bExit;
	bool			m_bPlay;
	bool			m_bCredits;
	bool			m_bOptions;

	float			m_Timer;
	float			m_FadeTimer;
	bool			m_bFadeOut;
	bool			m_bFadeOutC;

	bool			m_bMouseActive;
	int				m_PrevMousePosX;
	int				m_PrevMousePosY;

	bool			m_bSoundExit;
	bool			m_bSoundPlay;
	bool			m_bSoundCredits;
	bool			m_bSoundOptions;
	bool			m_bSoundCheck;
	bool			m_bEsc;

	int				m_ActiveDepot;
	bool			m_HideCursor;
	bool			m_bController;
	

	CMainMenuState(void);
	virtual ~CMainMenuState(void);
	void ProcessStates(eMMChoices _choice);

public:
	static CMainMenuState* GetInstance();
	
	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render();
	void Enter();
	void Exit(); 
};


#endif