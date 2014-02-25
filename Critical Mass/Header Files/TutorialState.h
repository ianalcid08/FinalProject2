#ifndef _TUTORIALSTATE_H_
#define _TUTORIALSTATE_H_

/*****************************************************************
File name:			TutorialState.h
Date:				09/05/13
Modify Date:		09/05/13
Modify Initials:	CM
Author:				Corey Morehead
Purpose:			This class will give be the gameplay of the
					tutorial
******************************************************************/

#include "IState.h"
#include "AssetManager.h"
#include "Renderer.h"
#include "RenderNode.h"
#include "AIManager.h"
#include "VFXManager.h"
#include "WaveManager.h"
#include "HUD.h"
#include "Light.h"
#include "LightPole.h"
#include "Camera.h"
#include "SpaceObjects.h"

class CInput;
class CGame;
class CObjectFactory;
class CObjectManager;
class StateManager;
class IEntity;
class CDepot;
class CRedBarrel;
class CLandingPads;
class IMessage;

enum eTutorialHUDElements {	/*INSTRUCTION TAGS*/
							TUTORIAL_ESC_ELEMENT = 0,
							TUTORIAL_PUSH_ELEMENT,
							TUTORIAL_PULL_ELEMENT,
							TUTORIAL_SHOOT_ELEMENT,
							/*TUTORIAL_HEALTH_PICKUP_ELEMENT,*/
							TUTORIAL_RED_ELEMENT,
							TUTORIAL_YELLOW_ELEMENT,
							TUTORIAL_BLUE_ELEMENT,
							TUTORIAL_DEPOT_ELEMENT, 
							TUTORIAL_TAB_ELEMENT,
							TUTORIAL_LANDING_PAD_ELEMENT,
							TUTORIAL_MISSION_COMPLETE_ELEMENT,
							TUTORIAL_HUD_HEALTH_ELEMENT,
							TUTORIAL_PURPLE_SHIELD_ELEMENT,
							/*INDICATORS*/
							TUTORIAL_ARROW_ELEMENT,
							TUTORIAL_HEALTH_TEXT_ELEMENT,
							TUTORIAL_ARROW2_ELEMENT,
							TUTORIAL_ARROW3_ELEMENT,
							TUTORIAL_ARROW4_ELEMENT,
							TUTORIAL_ARROW5_ELEMENT,
							TUTORIAL_LIVES_TEXT_ELEMENT,
							TUTORIAL_AMMOTYPE_TEXT_ELEMENT,
							TUTORIAL_AMMOAMOUNT_TEXT_ELEMENT,
							TUTORIAL_WEAPON_LEVEL_ELEMENT,
							/*HIGHLIGHTS*/
							TUTORIAL_HIGHLIGHT_ELEMENT,
							TUTORIAL_WEAPON_HIGHLIGHT_ELEMENT,
							/*NEW ASSETS*/
							T_HEALTHPICKUPS_ELEMENT,

							TUTORIAL_HUD_NUM_ELEMENTS };

class CTutorialState : public IState
{
private:
	CGame						*m_Game;
	CRenderer					*m_Renderer;
	CVFXManager					m_VFXManager;
	CObjectFactory				*m_OF;
	CObjectManager				*m_OM;
	StateManager				*m_SM;
	CCamera						m_Camera;
	IEntity						*m_Player;
	IEntity						*m_World;
	IEntity						*m_Pull;
	IEntity						*m_Enemy;
	IEntity						*m_RedBarrel[2];
	IEntity						*m_LandingPads[2];
	IEntity						*m_DepotRot[2];
	IEntity						*m_ShieldRot[2];
	IEntity						*m_TestCrates[6];
	CHUD						m_HUD;
	CUIElement					m_reticle;
	CUIElement					m_TutorialHUDElements[TUTORIAL_HUD_NUM_ELEMENTS];
	CUIElement					m_BlackBackground;
	CAIManager					m_AIManager;
	CWaveManager				m_WaveManager;

	SpaceObjects	m_testAsteroid;      // could be an array later
	SpaceObjects	m_testPlanet;
	SpaceObjects    m_testSun;

	float						m_ElapsedTime, m_PlayerMove, m_Timer, m_FlashTimer, m_HealthFlashTimer, m_FlashRedTimer, m_FlashYellowTimer, m_FlashBlueTimer, m_SpawnTimer, m_FadeTimer, m_DepotFlasher,
								m_EnemyTimer, m_LandingPadTimer, m_FlashTimer00;
	float						m_LerpTimer, m_LerpTimerHP, m_LerpTimerMove, m_LerpTimerPull, m_LerpTimer00, m_LerpTimer01, m_LerpTimer02, m_LerpTimer03, m_LerpTimer04, m_LerpTimer05, m_LerpTimer06, m_LerpTimer07;
	float						m_Timer00, m_Timer01, m_Timer02, m_Timer03, m_Timer04, m_Timer05, m_Timer06, m_Timer07, m_LerpTimer08, m_LerpTimer09, m_LerpTimer10, m_FadeTimer11;
	bool						m_bPush, m_bPushFlash, m_bPullFlash, m_bPull, m_bChecked, m_bHealth, m_bBlue, m_bRed, m_bYellow, m_bCrates, m_bHealthFlash, m_bHealthPickups, m_bFlashPickUp, m_bStep1,
								m_bStep2, m_bStep3, m_bWeaponRed, m_bWeaponYellow, m_bWeaponBlue, m_bFlashRed, m_bFlashYellow, m_bFlashBlue, m_bDepot, m_bFadeOut, m_Transition, m_bShoot, m_bFlashDepot,
								m_bDepotSwitch, m_bTab, m_bDepotSwitch1, m_bPurpleShield;
	short						m_Counter, m_RedCounter, m_WeaponType, m_EnemyCounter, m_WeaponCounter;
	short						m_CounterOverall;

	CTutorialState(void);
	virtual ~CTutorialState(void);

public:
	static CTutorialState* GetInstance();
	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render( void );
	void Enter( void );
	void Exit( void ); 
	void FadeIn(float _fTime, CUIElement _UIE);
	void FadeOut(float _fTime, CUIElement _UIE);
	void MakeCrates ( void );
	void ResetGame ( void );
	static void MessageProcessor( IMessage* _message );
	void Flash (short _counter);
	float Lerp (float value1, float value2, float amount );
	CHUD*		GetHUD()	{ return &m_HUD; }
	void		ResetUI( void );
};

#endif
