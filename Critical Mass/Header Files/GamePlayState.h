#ifndef _GAMEPLAYSTATE_H_
#define _GAMEPLAYSTATE_H_
/*****************************************************************
File name:			GameplayState.h
Date:				7/17/13
Modify Date:		8/12/13
Modify Initials:	CM
Author:				Ian Alcid
Purpose:			Class for main menu state.
******************************************************************/

#include "IState.h"
#include "AIManager.h"
#include "VFXManager.h"
#include "WaveManager.h"
#include "HUD.h"
#include "LightPole.h"
#include "SpaceObjects.h"
#include "Camera.h"

class CInput;
class CGame;
class CRenderer;
class CObjectFactory;
class CObjectManager;
class CAssetManager;
class IEntity;
class CDepot;
class CRedBarrel;
class CLandingPads;
class IMessage;
class CCinematicSystem;

class CGameplayState : public IState
{
	CCinematicSystem*	m_Cinema;
	Vec3f				m_CamPrevPos;

	CAssetManager	*m_AssetManager;
	CObjectFactory	*m_ObjFactory;
	CObjectManager	*m_ObjManager;
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	IEntity			*m_Player;
	CCamera			m_Camera;
	CHUD			m_HUD;
	CAIManager		m_AIManager;
	CVFXManager		m_VFXManager;
	//CUIElement		testElement;
	CUIElement		m_reticle;
	IEntity			*m_Pull;
	CWaveManager	m_WaveManager;

	// Temporary
	CLightPole		m_lightPoles[13];

	IEntity* skybox;

	float			m_Timer;
	float			m_PlayerMove;

	bool			m_HideCursor;
	//CJM
	bool			m_bFadeOut;
	bool			m_Transition;
	float			m_FadeTimer;
	CUIElement		m_BlackBackground;
	bool			m_OutroPlay;


	IEntity*		m_DepotRot[6];
	IEntity*		m_ShieldRot[6];
	IEntity*		m_RedBarrel[10];
	IEntity*		m_TestCrates[6];
	IEntity*		m_LandingPads[2];

	//Tom Stefl
	SpaceObjects	m_testAsteroid;      // could be an array later
	SpaceObjects	m_testPlanet;
	SpaceObjects    m_testSun;

	// SoundManager
	bool			m_pause;

	bool			m_DisplayWaveNotice;
	float			m_DisplayWaveNoticeTimer;
	void			MakeCrates();
	bool			DoIMakeCrates;
	bool			m_LowHPBeacon;
	int				m_Difficulty;

	int				m_ActiveEmitters[MAX_EMITTERS];

	std::vector<CRenderNode*>	m_DebugNodes;

	CGameplayState();
	virtual ~CGameplayState() { }

	float m_DepotAlertFlashTimer;
	bool m_DepotAlertFlash;

	bool m_PlayerSpawning;

	bool m_playThisOnlyOnce;

public:
	
	//Must be moved back to private
	//Made public for the AnimDebugger
	IEntity			*m_World;
	static CGameplayState* GetInstance();


	void ResetGame();
	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render();
		void Enter();
	void Exit(); 

	static void MessageProcessor( IMessage* _message );

	int GetNumActiveDepots();
	IEntity*	GetPlayer() { return m_Player;}
	CHUD*		GetHUD()	{ return &m_HUD; }
	void		ResetUI( void );
};


#endif
