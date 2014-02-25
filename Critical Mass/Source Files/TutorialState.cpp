/*****************************************************************
File name:			TutorialState.cpp
Date:				09/05/13
Modify Date:		09/05/13
Modify Initials:	CM
Author:				Corey Morehead
Purpose:			This class will give be the gameplay of the
tutorial
******************************************************************/
#pragma region #includes
#include "../Header Files/StdAfx.h"
#include "TutorialState.h"
#include "../Header Files/Objectfactory.h"
#include "../Header Files/Game.h"
#include "../Header Files/Input.h"
#include "../Header Files/Depot.h"
#include "../Header Files/Push.h"
#include "../Header Files/RedBarrel.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/LandingPads.h"
#include "../Header Files/Shield.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"

//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/Entity3D.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../Header Files/Emitter.h"
#include "../Header Files/MessageSystem.h"
#include <string>
using namespace std;
#pragma endregion

CTutorialState::CTutorialState(void)
{
	this->SetClosing(false);
}

CTutorialState::~CTutorialState(void)
{
	//this->Shutdown();
}

CTutorialState* CTutorialState::GetInstance()
{
	static CTutorialState s_Instance;
	return &s_Instance;
}

void CTutorialState::Init(CGame* _game)
{
	m_Game = _game;
	m_Renderer = _game->GetRenderer();
	m_VFXManager.Initialize(m_Renderer);
	m_Camera.UnAttach();
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	m_Camera.SetPosition(pos);
	m_OM = _game->GetOM();
	m_OF = _game->GetOF();
	m_SM = _game->GetStateManager();

	m_bRed				= true;
	m_bYellow			= false;
	m_bBlue				= false;
	m_bHealth			= true;
	m_bHealthFlash		= true;
	m_bPullFlash		= true;
	m_bCrates			= true;
	m_bHealthPickups	= false;
	m_bFlashPickUp		= true;
	m_bStep1			= false;
	m_bStep2			= false;
	m_bStep3			= false;
	m_bPull				= false;
	m_bPush				= false;
	m_bPushFlash		= false;
	m_bWeaponRed		= false;
	m_bWeaponYellow		= false;
	m_bWeaponBlue		= false;
	m_bFlashRed			= true;
	m_bFlashYellow		= true;
	m_bFlashBlue		= true;
	m_bShoot			= false;
	m_bFlashDepot		= true;
	m_bTab				= false;
	m_bDepotSwitch1		= true;
	m_bPurpleShield		= false;

	m_RedCounter		= 0;
	m_HealthFlashTimer	= 0.0f;
	m_FlashRedTimer		= 0.0f;
	m_FlashYellowTimer	= 0.0f;
	m_FlashBlueTimer	= 0.0f;
	m_DepotFlasher		= 0.0f;
	m_WeaponType		= 0;
	m_FlashTimer		= 0.0f;
	m_EnemyCounter		= 1;
	m_WeaponCounter		= 0;
	m_FlashTimer00		= 0.0f;
	m_Pull				= nullptr;
	m_Counter			= 0;
	m_LerpTimer			= 0.0f;
	m_LerpTimerHP		= 0.0f;
	m_LerpTimerMove		= 0.0f;
	m_LerpTimerPull		= 0.0f;
	m_LerpTimer00		= 0.0f;
	m_LerpTimer01		= 0.0f;
	m_LerpTimer02		= 0.0f;
	m_CounterOverall	= 0;
	m_LerpTimer03		= 0.0f;
	m_LerpTimer04		= 0.0f;
	m_LerpTimer05		= 0.0f;
	m_LerpTimer06		= 0.0f;
	m_LerpTimer07		= 0.0f;
	m_Timer00			= 0.0f;
	m_Timer01			= 0.0f;
	m_Timer02			= 0.0f;
	m_Timer03			= 0.0f;
	m_Timer04			= 0.0f;
	m_Timer05			= 0.0f;
	m_Timer06			= 0.0f;
	m_Timer07			= 0.0f;
	m_LerpTimer10		= 0.0f;
	m_FadeTimer11		= 1.0f;

	SetID( TUTORIAL_STATE );
}

void CTutorialState::Enter( void )
{
	//Reset all your geo emitters
	m_VFXManager.ResetGeoEmitters();

	// Initialize the Message System
	CMessageSystem::GetInstance()->Initialize(MessageProcessor);

	// Create RedBarrels
	for (int i = 0; i < 2; ++i)
		m_OF->Create( ET_REDBARREL, &m_RedBarrel[i]);

	// Initialize RedBarrels
	((CRedBarrel*)m_RedBarrel[0])->Initialize(30, 30);
	((CRedBarrel*)m_RedBarrel[1])->Initialize(90, 45);

	//LOAD CRATES
	MakeCrates();

	m_RedCounter		= 0;
	m_HealthFlashTimer	= 0.0f;
	m_FlashRedTimer		= 0.0f;
	m_FlashYellowTimer	= 0.0f;
	m_FlashBlueTimer	= 0.0f;
	m_WeaponType		= 0;
	m_EnemyCounter		= 1;
	m_Counter			= 0;
	m_WeaponCounter		= 0;
	m_DepotFlasher		= 0.0f;
	m_LerpTimer			= 0.0f;
	m_FlashTimer00		= 0.0f;
	m_LerpTimerHP		= 0.0f;
	m_LerpTimerMove		= 0.0f;
	m_LerpTimerPull		= 0.0f;
	m_LerpTimer00		= 0.0f;
	m_LerpTimer01		= 0.0f;
	m_LerpTimer02		= 0.0f;
	m_LerpTimer03		= 0.0f;
	m_LerpTimer04		= 0.0f;
	m_LerpTimer05		= 0.0f;
	m_LerpTimer06		= 0.0f;
	m_LerpTimer07		= 0.0f;
	m_Timer00			= 0.0f;
	m_Timer01			= 0.0f;
	m_Timer02			= 0.0f;
	m_Timer03			= 0.0f;
	m_Timer04			= 0.0f;
	m_Timer05			= 0.0f;
	m_Timer06			= 0.0f;
	m_Timer07			= 0.0f;
	m_FlashTimer		= 0.0f;
	m_LerpTimer10		= 0.0f;
	m_FadeTimer11		= 1.0f;

	m_Pull				= nullptr;
	m_bPushFlash		= false;
	m_bRed				= true;
	m_bYellow			= false;
	m_bBlue				= false;
	m_bShoot			= false;
	m_bPull				= false;
	m_bPush				= false;
	m_bHealth			= true;
	m_bHealthFlash		= true;
	m_bPullFlash		= true;
	m_bCrates			= true;
	m_bHealthPickups	= false;
	m_bFlashPickUp		= true;
	m_bStep1			= false;
	m_bStep2			= false;
	m_bStep3			= false;
	m_bWeaponRed		= false;
	m_bWeaponYellow		= false;
	m_bWeaponBlue		= false;
	m_bFlashRed			= true;
	m_bFlashYellow		= true;
	m_bFlashBlue		= true;
	m_bDepot			= false;
	m_bFlashDepot		= true;
	m_bDepotSwitch		= true;
	m_bTab				= false;
	m_bDepotSwitch1		= true;
	m_bPurpleShield		= false;

	m_CounterOverall	= 0;

	//LANDING PAD
	for(int i = 0; i < MAX_LANDINGPADS; ++i)
		m_OF->Create(ET_LANDINGPAD,&m_LandingPads[i]);

	((CLandingPads*)m_LandingPads[0])->Initialize(0,0);
	((CLandingPads*)m_LandingPads[1])->Initialize(180,0);

	//LOAD DEPOTS
	for(int i = 0; i < 2; ++i)
		m_OF->Create( ET_DEPOT, &m_DepotRot[i]);
	// Initialize the depots.
	((CDepot*)m_DepotRot[0])->Initialize(60, 0);
	((CDepot*)m_DepotRot[1])->Initialize(60, 120);
	//Setting depots
	for(int i = 0; i < 2; ++i)
		((CDepot*)m_DepotRot[i])->SetState(DEACTIVATED);

	//PLAYER
	m_OF->Create( ET_PLAYER, &m_Player );
	((CPlayer*)m_Player)->ResetMatrix();
	m_PlayerMove = 0.0f;
	Vec3f temp;
	glsLoadVector3(temp,-60.0f,-60.0f,0.0f);
	((CPlayer*)m_Player)->GetRenderNode().SetPosition(temp);
	((CPlayer*)m_Player)->SetPosition(temp);
	((CPlayer*)m_Player)->SetHealth(35);
	((CPlayer*)m_Player)->SetLives(MAX_LIVES_PLAYER);
	((CPlayer*)m_Player)->GetGun().Store(EMPTY);
	((CPlayer*)m_Player)->GetGun().SetConsumed(EMPTY);
	((CPlayer*)m_Player)->GetGun().Process();
	((CPlayer*)m_Player)->SetIsDying(false);
	((CPlayer*)m_Player)->SetResetWave(false);
	((CPlayer*)m_Player)->SetStopAnimSwitching(false);

	// Initialize AI
	m_AIManager.Initialize(nullptr);
	//ENEMY
	// Set player as the target to the AI
	m_AIManager.SetTarget((CPlayer*)m_Player);

	//WORLD
	m_OF->Create(ET_WORLD, &m_World);

	//Load Camera
	m_Camera.ResetCam();
	m_Camera.SetTarget((CPlayer*)m_Player);

	// Initialize HUD
	m_HUD.Initialize(m_Renderer);

	//m_WaveManager.Initialize(m_Renderer);
	((CPlayer*)m_Player)->GetGun().Reset();
	for(int i = 0; i < 2; i++)
		((CDepot*)m_DepotRot[i])->DestroyShield();

	//SET WEAPONS
	((CPlayer*)m_Player)->GetGun().SetExperience(RED, 25);
	((CPlayer*)m_Player)->GetGun().SetExperience(YELLOW, 75);
	((CPlayer*)m_Player)->GetGun().SetExperience(BLUE, 50);

	//HP ARROW
	//CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_QUAD_PLAYER, m_Player, 0.0f, 0.0f, 0.0f));
	CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_HP_ARROW, m_Player, 0.0f, 0.0f, 0.0f));

	ResetUI();

	Vec3f empty = {0.0f,0.0f,0.0f};
	Vec3f pos	= {0.0f,0.0f,0.0f};
	Vec3f vel	= {1.0f,0.0f,0.0f};

	m_testAsteroid.Initialize(ET_ASTEROID,pos,vel,pos);
	glsLoadVector3(pos,-500.0f,75.0f,0.0f);
	m_testPlanet.Initialize(ET_PLANET,pos,vel,empty);

	glsLoadVector3(pos,0.0f,-500.0f,-100.0f);
	m_testSun.Initialize(ET_SUN,pos,empty,empty);
}

void CTutorialState::ResetUI( void )
{
	// Make sure we're not leaking VRAM
	m_reticle.ReleaseGLBatch();
	m_BlackBackground.ReleaseGLBatch();
	for( unsigned int i = 0; i < TUTORIAL_HUD_NUM_ELEMENTS; ++i )
		m_TutorialHUDElements[i].ReleaseGLBatch();

	//For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), 
		RECTANGLE(0.0f, 32.0f, 0.0f, 32.0f), retPos[0], retPos[1], 0.0f, 1.0f, white);

	//Fade Background
	m_BlackBackground.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

	//SMALLENEMY
	//CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(m_Player, ET_SMALLENEMY));


#pragma region Tutorial HUD Items
	//PURPLE SHIELD
	m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_PURPLE_SHIELD), 
		RECTANGLE(0.0f, 400.0f, 0.0f, 140.0f),
		float(m_Game->GetScreenWidth() / 2.0f) - 200 * (m_Renderer->GetScreenHeight() / 720.0f),
		float(m_Game->GetScreenHeight() - 140 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//LANDING PAD
	m_TutorialHUDElements[TUTORIAL_LANDING_PAD_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_LANDING_PAD), 
		RECTANGLE(0.0f, 400.0f, 0.0f, 140.0f),
		float(m_Game->GetScreenWidth() / 2.0f) - 200 * (m_Renderer->GetScreenHeight() / 720.0f),
		float(m_Game->GetScreenHeight() - 140 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//TAB
	m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_TAB), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//ESC
	m_TutorialHUDElements[TUTORIAL_ESC_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_ESC), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		0.0f,
		float(m_Game->GetScreenHeight() - 70.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//WEAPONS
	m_TutorialHUDElements[TUTORIAL_RED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_RED_WEAPON), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -100.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_YELLOW_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_YELLOW_WEAPON), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -100.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_BLUE_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_BLUE_WEAPON), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -100.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//PUSH
	m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//HEALTH PICKUP
	m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(T_HEALTHPICKUP), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);		//HERE

	//PULL
	m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PULL), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//SHOOT
	m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_SHOOT_RED), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//DEPOT
	m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_DEPOT), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//HIGHLIGHT
	m_TutorialHUDElements[TUTORIAL_HIGHLIGHT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_LERP_PUSH_HIGHLIGHT), 
		RECTANGLE(0.0f, 210.0f, 0.0f, 74.0f),
		float(m_Game->GetScreenWidth() - 225.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -185.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_WEAPON_HIGHLIGHT_ELEMENT].Setup(	m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_LERP_PUSH_HIGHLIGHT), 
		RECTANGLE(0.0f, 210.0f, 0.0f, 74.0f),
		float(m_Game->GetScreenWidth() - 225.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		float((m_Game->GetScreenHeight() / 2) -105.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//COMPLETE
	m_TutorialHUDElements[TUTORIAL_MISSION_COMPLETE_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_COMPLETE), 
		RECTANGLE(0.0f, 512.0f, 0.0f, 64.0f),
		float(m_Game->GetScreenWidth() / 2) - 256.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		float(m_Game->GetScreenHeight() - 70 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);
	//HEALTH BAR
	m_TutorialHUDElements[TUTORIAL_HUD_HEALTH_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_HEALTH_BAR), 
		RECTANGLE(0.0f, 203.0f, 0.0f, 30.0f),
		105.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		16.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	//ARROW LEFT
	m_TutorialHUDElements[TUTORIAL_ARROW_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_ARROW), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 30.0f),
		230.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		22.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_ARROW2_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_ARROW), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 30.0f),
		230.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		76.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	//ARROW RIGHT
	m_TutorialHUDElements[TUTORIAL_ARROW3_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(T_ARROW_RIGHT), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 30.0f), 
		float(m_Game->GetScreenWidth() - 400.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		39.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_ARROW4_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(T_ARROW_RIGHT), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 30.0f),
		float(m_Game->GetScreenWidth() - 335.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		87.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_ARROW5_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(T_ARROW_RIGHT), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 30.0f),
		float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		120.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	//HEALTH / LIVES / AMMO TEXT
	m_TutorialHUDElements[TUTORIAL_HEALTH_TEXT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_HEALTH_TEXT), 
		RECTANGLE(0.0f, 230.0f, 0.0f, 36.0f),
		300.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		20.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_LIVES_TEXT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_LIVES_TEXT), 
		RECTANGLE(0.0f, 240.0f, 0.0f, 38.0f),
		300.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		69.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_TutorialHUDElements[TUTORIAL_AMMOAMOUNT_TEXT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_AMMOAMOUNT_TEXT), 
		RECTANGLE(0.0f, 240.0f, 0.0f, 38.0f),
		float(m_Game->GetScreenWidth() - 640.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		34.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_WEAPON_LEVEL_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_WEAPON_LEVEL), 
		RECTANGLE(0.0f, 200.0f, 0.0f, 38.0f),
		float(m_Game->GetScreenWidth() - 540.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		82.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	m_TutorialHUDElements[TUTORIAL_AMMOTYPE_TEXT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(TUTORIAL_AMMOTYPE_TEXT), 
		RECTANGLE(0.0f, 230.0f, 0.0f, 36.0f),
		float(m_Game->GetScreenWidth() - 450.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		125.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

#pragma endregion
}

bool CTutorialState::Input( CInput& _input )
{
	m_Camera.Input(_input);

	//Updates the reticle location based on the mouse pos.
	m_reticle.SetPosX(_input.GetMousePosX() - m_reticle.GetRect().GetWidth() * 0.5f);
	m_reticle.SetPosY(_input.GetMousePosY() - m_reticle.GetRect().GetHeight() * 0.5f);
	m_reticle.BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

#pragma region Esc - PAUSE MENU

	static bool ifPressedEnter_TM = false;
	//ESC TEMP: MAIN MENU, WILL BE PAUSE
	if (m_Timer > 0.5f)
	{
		static bool ifPressEsc = true;
		if(_input.IsEscapePressed() )
		{
			if(!ifPressEsc)
			{
				m_bFadeOut = true;
				ifPressEsc = true;
				//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUUNPAUSE);
				m_Game->GetStateManager()->PushState(TUTORIAL_PAUSE_STATE);
			}
		}
		else
			ifPressEsc = false;
	}

#pragma endregion

#pragma region Press Start

	if (m_bPush && m_bPull && m_bShoot && m_bDepot)
	{
		if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
		{
			if(!ifPressedEnter_TM)
			{		
				m_bFadeOut = true;				
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUUNPAUSE);
				m_Game->GetStateManager()->ChangeState(GAMEPLAY_STATE);
				ifPressedEnter_TM = true;
				return true;
			}
		}
		else
			ifPressedEnter_TM = false;
	}

#pragma endregion

#pragma region MOVE THE PLAYER

	//Move the player
	if( !((CPlayer*)m_Player)->GetStopAnimSwitching() )
	{
		// Move direction dependent on left stick.
		int sThumbLX = (int)_input.GetController().GetControllerState().Gamepad.sThumbLX;
		int sThumbLY = (int)_input.GetController().GetControllerState().Gamepad.sThumbLY;
		if( abs(sThumbLX) > 8000 || abs(sThumbLY) > 8000 )
		{
			Vec3f stick;
			glsLoadVector3(stick, (float)-sThumbLX, 0.0f, (float)sThumbLY);
			glsNormalizeVector3(stick);
			((CPlayer*)m_Player)->MoveForward(stick[2]);
			((CPlayer*)m_Player)->MoveRight(stick[0]);
		}

		if( _input.KeyDown( DIK_W ) )
		{
			((CPlayer*)m_Player)->MoveForward(1.0f);
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);
		}
		else if( _input.KeyDown( DIK_S ) )
		{
			((CPlayer*)m_Player)->MoveForward(-1.0f);
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);
		}

		if( _input.KeyDown( DIK_A ) )
		{
			((CPlayer*)m_Player)->MoveRight(1.0f);
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);
		}
		else if( _input.KeyDown( DIK_D ) )
		{
			((CPlayer*)m_Player)->MoveRight(-1.0f);
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);
		}

		if( !_input.KeyDown( DIK_D ) &&  !_input.KeyDown( DIK_A ) && !_input.KeyDown( DIK_S ) && !_input.KeyDown( DIK_W ) )
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.3f);

		// Look and fire direction, controlled by right stick.
		int sThumbRX = (int)_input.GetController().GetControllerState().Gamepad.sThumbRX;
		int sThumbRY = (int)_input.GetController().GetControllerState().Gamepad.sThumbRY;
		if( abs(sThumbRY) > 8000 || abs(sThumbRX) > 8000 )
		{
			Vec3f stick;
			//glsLoadVector3(stick, sThumbRX, 0.0f, -sThumbRY );
			glsLoadVector3(stick, (float)sThumbRX, (float)sThumbRY, 0.0f );
			glsNormalizeVector3(stick);

			((CPlayer*)m_Player)->RotateToPosition(stick, *m_Camera.GetOrientation());
		}

		if( abs(_input.GetDeltaMousePosX()) > 0 || abs(_input.GetDeltaMousePosY()) > 0 )
		{
			Vec3f stick;
			glsLoadVector3(stick, (float)_input.GetDeltaMousePosX(), (float)_input.GetDeltaMousePosY(), 0.0f);
			glsNormalizeVector3(stick);

			((CPlayer*)m_Player)->RotateToPosition(stick, *m_Camera.GetOrientation());
		}
	}
#pragma endregion

#pragma region PUSH

	//PUSH
	if (!m_bHealth)
	{
		if( ( _input.GetController().GetControllerState().Gamepad.wButtons & LEFT_BUMPER || _input.KeyDown(DIK_SPACE) ) && ((CPlayer*)m_Player)->GetCanPush() )
		{
			if( m_Pull == nullptr )
			{
				((CPlayer*)m_Player)->HasPushed();
				IEntity* m_Push = nullptr;
				m_OF->Create( ET_PUSH, &m_Push);
				((CPush*)m_Push)->Initialize((CBaseEntity*)m_Player);
				m_bPush = true;
				CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_PUSH,m_Player,0.0f,0.0f,0.0f));
			}
		}
	}

#pragma endregion

#pragma region PULL

	if (m_bTab)
	{
		//PULL
		if((_input.GetController().GetControllerState().Gamepad.bLeftTrigger || _input.IsRightClicked())  && ((CPlayer*)m_Player)->GetCanPull() )
		{		
			if( m_Pull == nullptr )
			{
				m_Pull = nullptr;
				m_OF->Create( ET_PULL, &m_Pull);
				((CPull*)m_Pull)->Initialize((CBaseEntity*)m_Player,nullptr);
				CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_PULL,m_Pull,0.0f,0.0f,0.0f));
				//Play a short acvtivation sound
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_WEAPONWHOOSH);
				//Start a sound loop for the vacuum
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_WIND_LOOP);
			}
		}
		else if( m_Pull != nullptr )
		{
			m_Pull->SetIsActive(false);
			m_Pull->SetIsAlive(false);
			m_OF->Destroy(m_Pull);
			m_Pull = nullptr;
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_WEAPONWHOOSH);
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_WIND_LOOP);
		}
	}

#pragma endregion

#pragma region SHOOTING

	//SHOOTING
	static bool pressed = false;
	if( _input.IsLeftClicked() || _input.GetController().GetControllerState().Gamepad.bRightTrigger)
	{
		if (m_bWeaponBlue)
			m_bShoot = true;
		dynamic_cast<CPlayer*>(m_Player)->GetGun().Shoot(m_OF, (CPlayer*)m_Player, 0 );
		pressed = true;
	}
	else
		pressed = false;

	if (((CPlayer*)m_Player)->GetGun().GetConsumed() != EMPTY)	
	{
		m_bPull = true;
	}

	if (((CPlayer*)m_Player)->GetGun().GetConsumed() == RED)
	{
		m_bStep1 = true;
		m_AIManager.DestroySmall();
	}
	if (((CPlayer*)m_Player)->GetGun().GetConsumed() == YELLOW)
	{
		m_bStep1 = true;
		m_AIManager.DestroySmall();
	}
	if (((CPlayer*)m_Player)->GetGun().GetConsumed() == BLUE)
	{
		m_bStep1 = true;
		m_AIManager.DestroySmall();
	}
	if (m_bStep1)
	{
		if (((CPlayer*)m_Player)->GetGun().GetConsumed() == EMPTY)
		{
			//check to see if any enemies are on screen.
			m_bStep2 = true;
			if (!m_bWeaponRed)
			{
				m_bWeaponRed = true;
				m_WeaponCounter++;
			}
			else if (!m_bWeaponYellow)
			{
				m_bWeaponYellow = true;
				m_WeaponCounter++;
			}
			else if (!m_bWeaponBlue)
			{
				m_bWeaponBlue = true;
				m_WeaponCounter++;
			}
		}
	}
	if (m_bStep2)
	{
		if (m_AIManager.GetNumSmallEnemies() == 0 && m_bTab)
		{
			CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(m_DepotRot[0], ET_SMALLENEMY));
		}
		m_bStep1 = false;
		m_bStep2 = false;
	}

	if (((CPlayer*)m_Player)->GetGun().GetConsumed() == EMPTY)
	{
		if (m_AIManager.GetNumSmallEnemies() == 0)
		{
			m_bStep2 = true;
		}
	}


#pragma endregion

	//static bool ifPressedP_TM = false;
	//if(_input.KeyDown(DIK_P) )
	//{
	//	if(!ifPressedP_TM)
	//	{	
	//		//DEPOT
	//		((CDepot*)m_DepotRot[0])->Activate();
	//		((CDepot*)m_DepotRot[1])->Activate();

	//		ifPressedP_TM = true;
	//	}
	//}
	//else
	//	ifPressedP_TM = false;

	static bool ifPressedT_TM = false;
	if(_input.KeyDown(DIK_TAB) || _input.GetController().GetControllerState().Gamepad.wButtons & RIGHT_BUMPER)
	{
		if (!ifPressedT_TM)
		{
			m_HUD.ShowTab();
			ifPressedT_TM = true;
			if(m_HUD.GetShowTab())
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_WEAPON_MENU_OPEN);
			else
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_WEAPON_MENU_CLOSE);
			if (m_bHealthPickups)
				m_bTab = true;
		}
	}
	else
		ifPressedT_TM = false;

	return true;
}

void CTutorialState::Flash (short _counter)
{
	//DECIDED TO NOT 
}              

void CTutorialState::Update(float _fElapsedTime)
{
#pragma region Time Sets
	//TIME SETS
	m_Timer += _fElapsedTime;
	m_FlashTimer += _fElapsedTime;
	m_PlayerMove = _fElapsedTime * 5.0f;
	m_ElapsedTime = _fElapsedTime;
	m_HealthFlashTimer += _fElapsedTime;
	m_FlashRedTimer += _fElapsedTime;
	m_FlashYellowTimer += _fElapsedTime;
	m_FlashBlueTimer += _fElapsedTime;
	m_DepotFlasher += _fElapsedTime;
	m_EnemyTimer += _fElapsedTime;
	m_FlashTimer00 += _fElapsedTime;
#pragma endregion

#pragma region Updates & Depot
	//UPDATES
	if (m_bHealthPickups)
	{
		if (((CPlayer*)m_Player)->GetHealth() < 75)
			((CPlayer*)m_Player)->SetHealth(75);
	}
	if (!m_bHealthPickups)
	{
		if (((CPlayer*)m_Player)->GetHealth() < 30)
			((CPlayer*)m_Player)->SetHealth(35);
	}

	//if (m_bPush || m_bPull || m_bShoot)
	//((CDepot*)m_DepotRot[0])->SetHealth(100);

	m_testAsteroid.Update(_fElapsedTime);
	m_testPlanet.Update(_fElapsedTime);
	m_testSun.Update(_fElapsedTime);

	CMessageSystem::GetInstance()->ProcessMessages();
	m_OM->UpdateEntities(_fElapsedTime);
	m_OF->ProcessDestroy();
	m_OM->CheckCollisions();
	m_AIManager.Update(_fElapsedTime);
	m_Camera.Update(_fElapsedTime);
	m_Game->UpdateRenderer(*m_Camera.GetPosition(), *m_Camera.GetOrientation(), m_Game->GetDelta() );
	m_HUD.Update(_fElapsedTime,((CPlayer*)m_Player));

	//DEACTIVATE DEPOT
	if (m_bWeaponBlue)
	{
		if (m_bDepotSwitch)
		{
			((CDepot*)m_DepotRot[0])->Activate();
			m_bDepotSwitch = false;
		}
	}
	if (m_bDepot && m_bDepotSwitch1)
	{
		((CDepot*)m_DepotRot[1])->Activate();

		m_bPurpleShield = true;
		m_bDepotSwitch1 = false;
	}

	if (((CDepot*)m_DepotRot[0])->GetHealth() <= 0)
	{
		((CDepot*)m_DepotRot[0])->Deactivate();
	}
	if (((CDepot*)m_DepotRot[0])->GetState() == DEACTIVATED && m_bWeaponBlue)
	{
		m_bDepot = true;
	}
#pragma endregion

#pragma region Health / Life Indicator
	//HEALTH / LIFE INDICATORS
	if (m_bHealth)
	{
		if (m_HealthFlashTimer > 0.5f)
		{
			m_bHealthFlash = !m_bHealthFlash;
			m_HealthFlashTimer = 0.0f;
			m_Counter += 1;
		}
		if (m_Counter > 3 && m_Counter <= 9)
		{
			if (m_Counter >= 9)
			{
				m_bHealth = false;
				m_Counter = 0;
				m_HealthFlashTimer = 0.0f;
			}
		}
	}
#pragma endregion

#pragma region HIGHLIGHT
	//PUSH
	if (!m_bHealth && !m_bDepot)
	{
		if (m_FlashTimer > 0.5f)
		{
			m_bPushFlash = !m_bPushFlash;
			m_FlashTimer = 0.0f;
		}
	}

	//HEALTH PICK-UPS - PLAYERS HEALTH SET TO 100%
	if (!m_bHealthPickups)
	{
		if (m_bPush)
		{
			if (((CPlayer*)m_Player)->GetHealth() > 35)
			{
				m_bHealthPickups = true;
			}
		}
	}
#pragma endregion

	//AMMO TYPE / AMMO AMOUNT
	if (m_bPull)
	{
		if (m_FlashTimer00 > 0.5f)
		{
			m_bPullFlash = !m_bPullFlash;
			m_FlashTimer00 = 0.0f;
			m_Counter += 1;
		}
	}

	//TUTORIAL COMPLETED!
	if (m_bPull && m_bPush && m_bShoot && m_bDepot)
	{
		m_FadeTimer -= _fElapsedTime;
		m_TutorialHUDElements[TUTORIAL_MISSION_COMPLETE_ELEMENT].SetOpacity(*m_TutorialHUDElements[TUTORIAL_MISSION_COMPLETE_ELEMENT].GetOpacity() + 0.8f * _fElapsedTime);
		if (m_FadeTimer <= 0.0f)
			m_FadeTimer = 0.0f;
	}

	//LANDING PAD
	if (!m_bHealth)
	{
		if (m_bPull)
		{
			m_LandingPadTimer -= _fElapsedTime;
			m_TutorialHUDElements[TUTORIAL_LANDING_PAD_ELEMENT].SetOpacity(*m_TutorialHUDElements[TUTORIAL_LANDING_PAD_ELEMENT].GetOpacity() - 0.8f * _fElapsedTime);
		}
	}

	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region LERP Movement
	//PUSH SLIDE DOWN / HEALTH PICK UP SLIDE TO THE LEFT
	if (m_bHealthPickups)
	{
		m_LerpTimerHP += _fElapsedTime;
	}
	if (m_LerpTimerHP > 0.0f && m_LerpTimerHP <= 1.0f)
	{
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 180.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimerHP / 1.0f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH));

		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), 10.0f * (m_Renderer->GetScreenHeight() / 720.0f), m_LerpTimerHP / 1.0f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(T_HEALTHPICKUP));
	}
	//PUSH
	if (m_bPush)
	{
		m_LerpTimer += _fElapsedTime;
	}
	if (m_LerpTimer > 0.0f && m_LerpTimer <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth()), m_LerpTimer / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH));
	}
	if (m_LerpTimer > 0.5f && m_LerpTimer <= 1.0f)
	{
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosX(Lerp( -200 * (m_Renderer->GetScreenHeight() / 720.0f) , 10.0f * (m_Renderer->GetScreenHeight() / 720.0f), m_LerpTimer / 1.0f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH));
	}
	//HEALTH PICK UP
	if (m_bHealthPickups)
	{
		m_LerpTimerHP += _fElapsedTime;
		m_LerpTimerMove += _fElapsedTime;
	}

	if (m_LerpTimerMove > 0.0f && m_LerpTimerMove <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 180.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimerMove / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH));
	}	
	if (m_LerpTimerHP > 0.0f && m_LerpTimerHP <= 0.5f)
	{
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth()), m_LerpTimerHP / 0.5f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(T_HEALTHPICKUP));
	}
	if (m_LerpTimerHP > 0.5f && m_LerpTimerHP <= 1.0f)
	{
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosX(Lerp( -200 * (m_Renderer->GetScreenHeight() / 720.0f) , 10.0f * (m_Renderer->GetScreenHeight() / 720.0f), m_LerpTimerHP / 1.0f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(T_HEALTHPICKUP));
	}
	//TAB
	if (m_bTab)
	{
		m_LerpTimer08 += _fElapsedTime;
		m_LerpTimer09 += _fElapsedTime;
	}
	if (m_LerpTimer08 > 0.0f && m_LerpTimer08 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth()), m_LerpTimer08 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_TAB));

	}
	if (m_LerpTimer08 > 0.5f && m_LerpTimer08 <= 1.0f)
	{
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosX(Lerp( -200  * (m_Renderer->GetScreenHeight() / 720.0f), 10.0f * (m_Renderer->GetScreenHeight() / 720.0f), m_LerpTimer08 / 1.0f));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_TAB));
	}
	if (m_LerpTimer09 > 0.0f && m_LerpTimer09 <= 0.5f)
	{
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 180.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer09 / 0.5f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(T_HEALTHPICKUP));

		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 20.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer09 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH));
	}

	//PULL
	if (m_bPull)
	{
		m_LerpTimerPull += _fElapsedTime;
		m_LerpTimer00 += _fElapsedTime;
	}
	if (m_LerpTimer00 > 0.0f && m_LerpTimer00 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 180.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer00 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_TAB));

		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 20.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer00 / 0.5f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(T_HEALTHPICKUP));

		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 20.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) +60.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer00 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH));
	}	
	if (m_LerpTimerPull > 0.0f && m_LerpTimerPull <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth()), m_LerpTimerPull / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PULL));
	}
	if (m_LerpTimerPull > 0.5f && m_LerpTimerPull <= 1.0f)
	{
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetPosX(Lerp( -200  * (m_Renderer->GetScreenHeight() / 720.0f), 10.0f, m_LerpTimerPull / 1.0f));
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PULL));
	}
	//WEAPONS
	if (m_bWeaponRed)
	{
		m_LerpTimer01 += _fElapsedTime;
	}
	if (m_LerpTimer01 > 0.0f && m_LerpTimer01 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_RED_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth() + 5.0f * (m_Renderer->GetScreenHeight() / 720.0f) ), m_LerpTimer01 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_RED_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -100.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_RED_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_RED_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_RED_WEAPON));
	}
	if (m_bWeaponYellow)
	{
		m_LerpTimer02 += _fElapsedTime;
	}
	if (m_LerpTimer02 > 0.0f && m_LerpTimer02 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_YELLOW_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth() + 5.0f  * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer02 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_YELLOW_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -100.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_YELLOW_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_YELLOW_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_YELLOW_WEAPON));
	}
	if (m_bWeaponBlue)
	{
		m_LerpTimer03 += _fElapsedTime;
	}
	if (m_LerpTimer03 > 0.0f && m_LerpTimer03 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_BLUE_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth() + 5.0f * (m_Renderer->GetScreenHeight() / 720.0f) ), m_LerpTimer03 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_BLUE_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -100.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_BLUE_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_BLUE_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_BLUE_WEAPON));
	}
	//SHOOT
	if (m_bShoot)
	{
		m_LerpTimer04 += _fElapsedTime;
		m_LerpTimer05 += _fElapsedTime;
	}
	if (m_LerpTimer04 > 0.0f && m_LerpTimer04 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth()), m_LerpTimer04 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_SHOOT_RED));
	}
	if (m_LerpTimer04 > 0.5f && m_LerpTimer04 <= 1.0f)
	{
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetPosX(Lerp( -200 * (m_Renderer->GetScreenHeight() / 720.0f), 10.0f * (m_Renderer->GetScreenHeight() / 720.0f), m_LerpTimer04 / 1.0f));
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_SHOOT_RED));
	}
	if (m_LerpTimer05 > 0.0f && m_LerpTimer05 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 180.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer05 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PULL));

		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 20.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer05 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_TAB));

		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 20.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) +60.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer05 / 0.5f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(T_HEALTHPICKUP));

		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) +60.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) +140.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer05 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH));
	}
	//DEPOT
	if (m_bDepot)
	{
		m_LerpTimer06 += _fElapsedTime;
		m_LerpTimer07 += _fElapsedTime;
	}
	if (m_LerpTimer06 > 0.0f && m_LerpTimer06 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].SetPosX(Lerp(float(m_Game->GetScreenWidth() - 220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenWidth()), m_LerpTimer06 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_DEPOT));

	}
	if (m_LerpTimer06 > 0.5f && m_LerpTimer06 <= 1.0f)
	{
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].SetPosX(Lerp( -200  * (m_Renderer->GetScreenHeight() / 720.0f), 10.0f * (m_Renderer->GetScreenHeight() / 720.0f), m_LerpTimer06 / 1.0f));
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].SetPosY(float((m_Game->GetScreenHeight() / 2) -180.0f * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_DEPOT));
	}
	if (m_LerpTimer07 > 0.0f && m_LerpTimer07 <= 0.5f)
	{
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 180.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer07 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_SHOOT_RED));

		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 100.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) - 20.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer07 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PULL));

		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) - 20.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) +60.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer07 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_TAB));

		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) +60.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) +140.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer07 / 0.5f));
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(T_HEALTHPICKUP));

		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosX(10.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetPosY(Lerp(float((m_Game->GetScreenHeight() / 2) +140.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2) +220.0f * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer07 / 0.5f));
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TUTORIAL_PUSH));
	}
	//PURPLESHIELD
	if (m_bPurpleShield)
	{
		m_LerpTimer10 += _fElapsedTime;
	}
	if (m_LerpTimer10 > 0.0f && m_LerpTimer10 <= 1.0f)
	{
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].SetPosX((float(m_Game->GetScreenWidth() / 2.0f) - 200 * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].SetPosY(Lerp(float(m_Game->GetScreenHeight() - 140 * (m_Renderer->GetScreenHeight() / 720.0f)), float(m_Game->GetScreenHeight() - 280 * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer10 / 1.0f));
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_PURPLE_SHIELD));
	}
	if (m_LerpTimer10 > 4.0f && m_LerpTimer10 <= 5.0f)
	{
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].SetPosX((float(m_Game->GetScreenWidth() / 2.0f) - 200 * (m_Renderer->GetScreenHeight() / 720.0f)));
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].SetPosY(Lerp(-200 * (m_Renderer->GetScreenHeight() / 720.0f), float(m_Game->GetScreenHeight() - 140 * (m_Renderer->GetScreenHeight() / 720.0f)), m_LerpTimer10 / 5.0f));
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(TUTORIAL_PURPLE_SHIELD));

		m_FadeTimer11 -= _fElapsedTime;													
		m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT].SetOpacity(m_FadeTimer11);  
	}

#pragma endregion

#pragma region FADE IN/OUT
	//INSTRUCTIONS FADE IN/OUT!
	if (!m_bHealth && !m_bPush)
	{
		m_Timer00 += _fElapsedTime;													
		m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetOpacity(m_Timer00);     
	}
	if (m_bPush && !m_bHealthPickups)
	{
		m_Timer01 += _fElapsedTime;													
		m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetOpacity(m_Timer01);  
		if (*m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].GetOpacity() >= 0.5)
			m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].SetOpacity(0.5/**m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT].GetOpacity() - (0.8f * m_Timer00)*/);
	}
	if (m_bHealthPickups && !m_bTab)
	{
		m_Timer02 += _fElapsedTime;													
		m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT].SetOpacity(m_Timer02); 
		if (*m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].GetOpacity() >= 0.5)
			m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT].SetOpacity(0.5/**m_TutorialHUDElements[TUTORIAL_HEALTH_PICKUP_ELEMENT].GetOpacity() - (0.8f * m_Timer01)*/);
	}
	if (m_bTab && !m_bPull)
	{
		m_Timer02 += _fElapsedTime;													
		m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetOpacity(m_Timer02); 
		if (*m_TutorialHUDElements[	TUTORIAL_TAB_ELEMENT].GetOpacity() >= 0.5)
			m_TutorialHUDElements[	TUTORIAL_TAB_ELEMENT].SetOpacity(0.5/**m_TutorialHUDElements[TUTORIAL_HEALTH_PICKUP_ELEMENT].GetOpacity() - (0.8f * m_Timer01)*/);
	}
	if (m_bPull && !m_bShoot)
	{
		m_Timer03 += _fElapsedTime;													
		m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetOpacity(m_Timer03);
		m_TutorialHUDElements[TUTORIAL_RED_ELEMENT].SetOpacity(m_Timer03);
		m_TutorialHUDElements[TUTORIAL_YELLOW_ELEMENT].SetOpacity(m_Timer03);
		m_TutorialHUDElements[TUTORIAL_BLUE_ELEMENT].SetOpacity(m_Timer03);
		if (*m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].GetOpacity() >= 0.5)
			m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].SetOpacity(0.5/**m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT].GetOpacity() - (0.8f * m_Timer02)*/);
	}
	if (m_bShoot && !m_bDepot)
	{
		m_Timer04 += _fElapsedTime;													
		m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].SetOpacity(m_Timer04);
		if (*m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].GetOpacity() >= 0.5)
			m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT].SetOpacity(0.5/**m_TutorialHUDElements[TUTORIAL_SHOOT_YELLOW_ELEMENT].GetOpacity() - (0.8f * m_Timer03)*/);
	}
	if (m_bDepot)
	{
		if (*m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].GetOpacity() >= 0.5)
			m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].SetOpacity(0.5/**m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT].GetOpacity() - (0.8f * m_Timer04)*/);
	}

#pragma endregion
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	if (!m_bFadeOut && m_Timer >= 0.0f)
	{
		m_FadeTimer -= _fElapsedTime;
		m_BlackBackground.SetOpacity(*m_BlackBackground.GetOpacity() - 0.8f * _fElapsedTime);

	}
	else
		m_bFadeOut = true;

	//HP ARROW

	for(int i = 0; i < 6; i++)
	{
		if(((CPlayer*)m_Player)->GetHealth() < 40 && ((CMovingEntity*)m_TestCrates[i])->GetIsActive() && ((CMovingEntity*)m_TestCrates[i])->GetIsAlive() )
		{
			//turn on the effect
			m_VFXManager.GetHPArrowFX()->GetGeoEffect()->SetIsActive(true);
			//turn on the emitter
			m_VFXManager.GetHPArrowFX()->SetIsActive(true);
			break;
		}
		else
		{
			m_VFXManager.GetHPArrowFX()->GetGeoEffect()->SetIsActive(false);
		}
	}

	m_VFXManager.Update(_fElapsedTime);


	for(int i = 0; i < 6; i ++)
	{
		if(m_TestCrates[i]->GetIsActive() && m_TestCrates[i]->GetIsAlive())
		{
			m_VFXManager.GetHPArrowFX()->GetGeoEffect()->FindClosestCrate(m_TestCrates[i]);
		}
	}
}

float CTutorialState::Lerp (float value1, float value2, float amount )
{
	return value1 + (value2 - value1) * amount;
}

void CTutorialState::Exit( void )
{
	//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_KLAXON_LOOP);
	//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_WIND_LOOP);

	((CPlayer*)m_Player)->ResetMatrix();
	// Reset the player.
	m_PlayerMove = 0.0f;
	Vec3f temp;
	glsLoadVector3(temp,0.0f,0.0f,0.0f);
	((CPlayer*)m_Player)->GetRenderNode().SetPosition( temp );
	((CPlayer*)m_Player)->SetPosition(temp);
	((CPlayer*)m_Player)->SetHealth(MAX_HP_PLAYER);
	((CPlayer*)m_Player)->SetLives(MAX_LIVES_PLAYER);
	((CPlayer*)m_Player)->GetGun().Store(EMPTY);
	((CPlayer*)m_Player)->GetGun().SetConsumed(EMPTY);
	((CPlayer*)m_Player)->GetGun().Process();
	((CPlayer*)m_Player)->SetIsDying(false);
	((CPlayer*)m_Player)->SetResetWave(false);
	((CPlayer*)m_Player)->SetStopAnimSwitching(false);

	m_OM->RemoveAllEntities();
	m_OF->ProcessDestroy();

	CMessageSystem::GetInstance()->ClearMessages();

	((CPlayer*)m_Player)->GetGun().Store(EMPTY);
	((CPlayer*)m_Player)->GetGun().SetConsumed(EMPTY);
	((CPlayer*)m_Player)->GetGun().Process();

	// Set the players current rotation to the default orientation.
	((CPlayer*)m_Player)->GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	((CPlayer*)m_Player)->GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	((CPlayer*)m_Player)->GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// Reset Depots
	for(int i = 0; i < 2; ++i)
	{
		((CDepot*)m_DepotRot[i])->DestroyShield();
		m_OF->Destroy(m_DepotRot[i]);
	}
	m_Camera.ResetCam();
	m_AIManager.Release();

	for( unsigned int i = 0; i < TUTORIAL_HUD_NUM_ELEMENTS; ++i )
	{
		m_TutorialHUDElements[i].ReleaseGLBatch();
	}

	m_HUD.Release();

	m_reticle.ReleaseGLBatch();
}

void CTutorialState::Shutdown()
{

}

void CTutorialState::FadeIn(float _fTime, CUIElement _UIE)
{
	m_FadeTimer -= _fTime;													
	_UIE.SetOpacity(m_FadeTimer);     
	//m_Renderer->AddToUIList(&_UIE);
	if (m_FadeTimer <= 0.0f)
		m_FadeTimer = 0.0f;
}

void CTutorialState::FadeOut(float _fTime, CUIElement _UIE)
{
	m_FadeTimer += _fTime;									
	_UIE.SetOpacity(m_FadeTimer);     
	//m_Renderer->AddToUIList(&_UIE);	
}

void CTutorialState::MakeCrates()
{
	// Create Crates
	for(int i = 0; i < 6; ++i)
	{
		m_OF->Create( ET_CRATE, &m_TestCrates[i]);
	}
	// Initialize Crates
	((CCrate*)m_TestCrates[0])->Init(-5, -5);
	((CCrate*)m_TestCrates[1])->Init(0, 0);
	((CCrate*)m_TestCrates[2])->Init(5, 5);
	((CCrate*)m_TestCrates[3])->Init(180, 180);
	((CCrate*)m_TestCrates[4])->Init(185, 185);
	((CCrate*)m_TestCrates[5])->Init(175, 175);
}

void CTutorialState::Render( void )
{
	//Tom Stefl
	m_Renderer->AddToRenderList(&m_testAsteroid.GetRenderNode());
	m_Renderer->AddToRenderList(&m_testPlanet.GetRenderNode());
	m_Renderer->AddToRenderList(&m_testSun.GetRenderNode());

	m_OM->RenderEntities();
	m_VFXManager.Render();

	m_Renderer->EnableDraw();


#pragma region GAME HUD
	//RETICLE
	m_Renderer->DrawTexture( &m_reticle );

	//HUD STATIC ELEMENTS
	//m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_HUD_HEALTH_ELEMENT]);
	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_HP_BG]); // bg
	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_HEALTH]); // fill
	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_HEALTH_LABEL]);

	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_WEAPON_TAB]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_BXP]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_RXP]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_YXP]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_BLVL]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_RLVL]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_YLVL]);

	////if (m_bPull)
	////{
	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_AMMO_BG]); // bg
	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_AMMO]); // fill
	////m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_AMMO_LABEL]);
	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_AMMOTYPE]);
	////}
	//bool test1 = false;
	//bool test2 = false;
	//if(((CPlayer*)m_Player)->GetGun().GetConsumed() != EMPTY)
	//	test1 = true;
	//if(((CPlayer*)m_Player)->GetGun().GetStored() != BLACK)
	//	test2 = true;
	//if(test1)
	//{
	//	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_WEAPON_LVL]);
	//	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_WEAPON_LVLBAR]);
	//}

	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_LABEL]);


	//Tom
	//Draw HUD frame(s)
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_AMMOTYPE]);

	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_MAINFRAME]);

	// health
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_HEALTH]); // fill
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_HP_BG]); // bg
	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_HEALTH_LABEL]);

	// ammo
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_AMMO]);		// fill
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_AMMO_BG]);	// bg

	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_WEAPON_CD]);
	//m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_AMMO_LABEL]);

	// ammo type
	bool test1 = false;
	bool test2 = false;
	if(((CPlayer*)m_Player)->GetGun().GetConsumed() != EMPTY)
		test1 = true;
	if(((CPlayer*)m_Player)->GetGun().GetStored() != BLACK)
		test2 = true;
	//if(((CPlayer*)m_Player)->GetGun().GetConsumed() != EMPTY || ((CPlayer*)m_Player)->GetGun().GetConsumed() != BLACK)
	if(test1)
	{
		m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_WEAPON_LVLBAR]);
		m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_EXP_FRAME]);
		m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_WEAPON_LVL]);
	}

	switch(((CPlayer*)m_Player)->GetLives())
	{
	case 1:
		{
			m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN1]);
		}
		break;
	case 2:
		{
			m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN1]);
			m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN2]);
		}
		break;
	case 3:
		{
			m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN1]);
			m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN2]);
			m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN3]);
		}
		break;
	};

#pragma endregion

	if (m_bHealth)
	{
		if (m_bHealthFlash)
		{
			if (m_Counter <= 3)
			{
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW_ELEMENT]);
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW2_ELEMENT]);
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_HEALTH_TEXT_ELEMENT]);
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_LIVES_TEXT_ELEMENT]);
			}
		}

		if (m_Counter > 3 && m_Counter <= 9)
		{
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW_ELEMENT]);
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_HEALTH_TEXT_ELEMENT]);
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW2_ELEMENT]);
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_LIVES_TEXT_ELEMENT]);
		}
	}
	if (m_bPushFlash && !m_bHealth && !m_bPull)
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_HIGHLIGHT_ELEMENT]);
	if (m_bPushFlash && m_bPull && !m_bShoot)
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_WEAPON_HIGHLIGHT_ELEMENT]);
	if (m_bPushFlash && m_bShoot && !m_bDepot)
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_HIGHLIGHT_ELEMENT]);

	if (m_bPull)
	{
		if(m_bPullFlash)
		{
			if (m_Counter <= 3)
			{
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_AMMOTYPE_TEXT_ELEMENT]);
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW3_ELEMENT]);
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_AMMOAMOUNT_TEXT_ELEMENT]);
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW4_ELEMENT]);
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_WEAPON_LEVEL_ELEMENT]);
				m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW5_ELEMENT]);
			}
		}

		if (m_Counter > 3 && m_Counter <= 9)
		{
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_AMMOTYPE_TEXT_ELEMENT]);
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW3_ELEMENT]);
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_AMMOAMOUNT_TEXT_ELEMENT]);
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW4_ELEMENT]);
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_WEAPON_LEVEL_ELEMENT]);
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW5_ELEMENT]);
		}
	}

	//LIVES
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN1]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN2]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_TOKEN3]);

	//COMPLETE
	if (m_bPull && m_bPush && m_bShoot && m_bDepot)
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_MISSION_COMPLETE_ELEMENT]);	

	//INSTRUCTIONS FADE IN!
	m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ESC_ELEMENT]);
	if (!m_bHealth)  
	{
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_PUSH_ELEMENT]);
	}

	if (m_bPush)   
		m_Renderer->DrawTexture(&m_TutorialHUDElements[T_HEALTHPICKUPS_ELEMENT]);

	if (m_bHealthPickups)
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_TAB_ELEMENT]);
	if (m_bTab)
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_PULL_ELEMENT]);

	if (m_bPull)
	{
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_SHOOT_ELEMENT]);
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_RED_ELEMENT]);
		if (m_bWeaponRed)
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_YELLOW_ELEMENT]);
		if (m_bWeaponYellow)
			m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_BLUE_ELEMENT]);
	}

	//LANDING PAD
	if (!m_bHealth)
	{
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_LANDING_PAD_ELEMENT]);
	}

	if (m_bShoot)
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_DEPOT_ELEMENT]);

	if (!m_bFadeOut && m_Timer >= 0.0f)
		m_Renderer->DrawTexture(&m_BlackBackground);

	//SHOW WEAPON LEVEL
	if (((CPlayer*)m_Player)->GetGun().GetConsumed() != EMPTY && m_WeaponCounter > 0 &&  m_WeaponCounter < 3)
	{
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_AMMOTYPE_TEXT_ELEMENT]);
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW3_ELEMENT]);
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_AMMOAMOUNT_TEXT_ELEMENT]);	
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW4_ELEMENT]);
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_WEAPON_LEVEL_ELEMENT]);
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_ARROW5_ELEMENT]);
	}

	//PURPLE SHIELDa
	if (m_bPurpleShield)
		m_Renderer->DrawTexture(&m_TutorialHUDElements[TUTORIAL_PURPLE_SHIELD_ELEMENT]);

	m_Renderer->DisableDraw();
}

void CTutorialState::MessageProcessor( IMessage* _message )
{
	CTutorialState* game = CTutorialState::GetInstance();
	switch(_message->GetMessageID())
	{
	case MSG_CREATE_EXPLOSION:
		{
			CCreateExplosionMessage* message = ((CCreateExplosionMessage*)_message);
			CExplosion* explosion = nullptr;
			if(game->m_OF->Create(ET_EXPLOSION, (IEntity**)&explosion))
			{
				explosion->GetRenderNode().SetPosition( ((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition());
				if(message->GetIsEMP())
				{
					explosion->SetIsEMP(true);
					explosion->GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_EMP).m_DiffuseHandle);
				}
				else
					explosion->SetIsEMP(false);

				Vec3f temp;
				glsCopyVector3(temp,((CBaseEntity*)explosion)->GetRenderNode().GetPosition());

				explosion->SetSize(message->GetSize());
				CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_SMOKE,explosion,0.0f,0.0f,0.0f));
			}
		}
		break;
	case MSG_CREATE_ENEMY:
		{
			CCreateEnemyMessage* message = ((CCreateEnemyMessage*)_message);

			if( message->GetType() == ET_SMALLENEMY )
			{
				CSmallEnemy* _enemy = nullptr;
				if( game->m_OF->Create(ET_SMALLENEMY, (IEntity**)&_enemy) )
				{
					// Set the enemies position/orientation to this depot's position/orientation.
					((CBaseEntity*)_enemy)->GetRenderNode().SetUpVector(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetUpVector());
					((CBaseEntity*)_enemy)->GetRenderNode().SetForwardVector(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetForwardVector());
					((CBaseEntity*)_enemy)->GetRenderNode().SetPosition( ((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition());
					Vec3f temp;
					glsLoadVector3(temp, 90.0f, 0.0f, 0.0f);
					((CBaseEntity*)_enemy)->SetPosition(temp);


					// Reset the small enemies data.
					((CSmallEnemy*)_enemy)->Reset();
					// Set small enemies HP
					((CSmallEnemy*)_enemy)->SetHealth(MAX_HP_SMALL_ENEMY);
					// Set enemy color type.
					// for black enemy. Make them rarer than the rest.

					int type ;//= rand()%3;

					if( game->m_bWeaponRed && !game->m_bWeaponYellow)
						type = YELLOW;
					else if( game->m_bWeaponYellow && !game->m_bWeaponBlue)
						type = BLUE;
					else if( game->m_bWeaponBlue )
						type = rand()%3;
					else
						type = RED;

					((CSmallEnemy*)_enemy)->SetPowerupType(type);
					// Set texture handle based on enemy color type.
					switch(type)
					{
					case RED:
						{
							((CSmallEnemy*)_enemy)->GetRenderNode().SetColor(0.8f, 0.0f, 0.0f, 1.0f);
							((CSmallEnemy*)_enemy)->GetRenderNode().SetEmissiveTextureHandle(CAssetManager::GetInstance()->GetSmallEnemyData(RED).m_hTextureEmissive);
							break;
						}

					case YELLOW:
						{
							((CSmallEnemy*)_enemy)->GetRenderNode().SetColor(0.8f, 0.8f, 0.0f, 1.0f);
							((CSmallEnemy*)_enemy)->GetRenderNode().SetEmissiveTextureHandle(CAssetManager::GetInstance()->GetSmallEnemyData(YELLOW).m_hTextureEmissive);
							break;
						}

					case BLUE:
						{
							((CSmallEnemy*)_enemy)->GetRenderNode().SetColor(0.0f, 0.0f, 0.8f, 1.0f);
							((CSmallEnemy*)_enemy)->GetRenderNode().SetEmissiveTextureHandle(CAssetManager::GetInstance()->GetSmallEnemyData(BLUE).m_hTextureEmissive);
							break;
						}

					case BLACK:
						((CSmallEnemy*)_enemy)->GetRenderNode().SetColor(0.4f, 0.4f, 0.4f, 1.0f);
						((CSmallEnemy*)_enemy)->GetRenderNode().SetEmissiveTextureHandle(CAssetManager::GetInstance()->GetSmallEnemyData(BLACK).m_hTextureEmissive);
						break;
					};

					// Add enemy to the AI Manager so that it can obtain and use a Behavior Tree.
					game->m_AIManager.AddSmall((CLivingEntity*)_enemy);
				}
			}
		}
		break;
	case MSG_CREATE_BULLET:
		{
			CProjectile* bullet = nullptr;
			CBaseEntity* target = (CBaseEntity*)((CCreateBulletMessage*)_message)->GetTarget();
			float angleY = ((CCreateBulletMessage*)_message)->GetAngleY();
			float angleX = ((CCreateBulletMessage*)_message)->GetAngleX();

			if( (CBaseEntity*)((CCreateBulletMessage*)_message)->GetTarget()->GetType() == (char)ET_PLAYER )
				game->m_OF->Create(ET_BULLET_PLAYER, (IEntity**)&bullet);
			else
				game->m_OF->Create(ET_BULLET_ENEMY, (IEntity**)&bullet);

			// Adjust bullets starting position:
			bullet->GetRenderNode().SetPosition(target->GetRenderNode().GetPosition());

			Vec3f scaledUp;
			glsCopyVector3(scaledUp, bullet->GetRenderNode().GetUpVector());
			glsScaleVector3(scaledUp, 3.25f);
			glsSubtractVectors3(*bullet->GetPosition(), bullet->GetRenderNode().GetPosition(), scaledUp);

			// For exploding bullet:
			Vec3f velocity;
			glsCopyVector3( velocity, target->GetRenderNode().GetForwardVector() );
			glsScaleVector3( velocity, -40.0f );
			bullet->SetVelocity(velocity);

			// Rotation.
			if( fabs(angleX) > 0.0f )
			{
				Vec3f rotationBullet;
				Matrix3x3f rotBulletMat;
				glsRotationMatrix3x3(rotBulletMat, (float)glsDegToRad(angleX),  target->GetRenderNode().GetPosition()[0], target->GetRenderNode().GetPosition()[1],  target->GetRenderNode().GetPosition()[2]); 
				glsRotateVector(rotationBullet, *(bullet->GetVelocity()), rotBulletMat);
				bullet->SetVelocity(rotationBullet);
			}

			if(((CCreateBulletMessage*)_message)->GetIsPlasma())
				bullet->GetRenderNode().SetColor(1.0f,0.0f,0.0f,1.0f);
			else
				bullet->GetRenderNode().SetColor(1.0f,1.0f,0.0f,1.0f);


			if(((CCreateBulletMessage*)_message)->GetIsRandVelocities())
			{
				float randSpeed = (float)(rand()%30) + 70.0f;
				bullet->SetSpeed(randSpeed);
			}
			bullet->m_Explode = ((CCreateBulletMessage*)_message)->GetExplode();
		}
		break;
	case MSG_CREATE_SHIELD:
		{
			CCreateShieldMessage* msg = ((CCreateShieldMessage*)_message);
			IEntity* shield = nullptr;
			if(game->m_OF->Create(ET_SHIELD, &shield ))
			{
				if (game->m_bPurpleShield)
				{
					((CShield*)shield)->Initialize(msg->GetRotX(),msg->GetRotY(),true);
					((CDepot*)msg->GetShield())->SetShield(shield);
					((CShield*)shield)->SetHealth(MAX_HP_SHIELD);
				}
				else 
				{
					((CShield*)shield)->Initialize(msg->GetRotX(),msg->GetRotY(),false);
					((CDepot*)msg->GetShield())->SetShield(shield);
					((CShield*)shield)->SetHealth(MAX_HP_SHIELD*2);
				}
			}
		}
		break;
	case MSG_CREATE_HEALTH_PICKUP:
		{
			CCreateHealthPickupMessage* msg = ((CCreateHealthPickupMessage*)_message);

			CHealthPickup* health = nullptr;
			if(game->m_OF->Create(ET_HEALTH_PICKUP_LARGE, (IEntity**)&health ))
			{
				((CBaseEntity*)health)->GetRenderNode().SetPosition( msg->GetX(), msg->GetY(), msg->GetZ());
				glsCopyVector3( *((CBaseEntity*)health)->GetPosition(), ((CBaseEntity*)health)->GetRenderNode().GetPosition() );
			}
		}
		break;
	case MSG_CREATE_EMP:
		{
			CCreateEMPMessage* msg = ((CCreateEMPMessage*)_message);

			CEMP* emp = nullptr;
			if( game->m_OF->Create(ET_EMP, (IEntity**)&emp ) )
			{
				Vec3f pos = { msg->GetX(), msg->GetY(), msg->GetZ() };
				emp->Enter(msg->GetImpactTimer(),pos);
			}
		}
		break;
	case MSG_CREATE_MORTAR:
		{
			CCreateMortarMessage* msg = ((CCreateMortarMessage*)_message);

			CMortar* mortar = nullptr;
			if( game->m_OF->Create(ET_MORTAR, (IEntity**)&mortar) )
			{
				Vec3f position;
				glsLoadVector3(position, msg->GetX(),msg->GetY(), msg->GetZ());
				mortar->Enter(position,msg->GetFinalBlast());
			}
		}
		break;
	case MSG_CREATE_RED_STRIKE:
		{
			CCreateRedStrikeMessage* msg = ((CCreateRedStrikeMessage*)_message);

			IEntity* strike;
			if( game->m_OF->Create(ET_REDSTRIKE,&strike) )
			{
				Vec3f position, offset;
				glsCopyVector3(position, ((CBaseEntity*)msg->GetPlayer())->GetRenderNode().GetPosition());
				glsCopyVector3(offset, ((CBaseEntity*)msg->GetPlayer())->GetRenderNode().GetForwardVector());
				if(msg->GetSize() == 1.0f)
					glsScaleVector3(offset,-(msg->GetSize()*20));
				else if(msg->GetSize() == 2.5f)
					glsScaleVector3(offset,-(msg->GetSize()*13));
				else
					glsScaleVector3(offset,-(msg->GetSize()*12));

				glsAddVectors3(position,position,offset);
				((CRedStrike*)strike)->Enter(position,msg->GetFinalBlast());
				((CRedStrike*)strike)->Reset(msg->GetDuration(),msg->GetRate(),msg->GetTimerStart(),msg->GetTimerEnd(),msg->GetSize());
			}
		}
		break;
	case MSG_CREATE_BLUE_STRIKE:
		{
			CCreateBlueStrikeMessage* msg = ((CCreateBlueStrikeMessage*)_message);

			IEntity* strike;
			if( game->m_OF->Create(ET_BLUESTRIKE,&strike) )
			{
				Vec3f position;
				glsLoadVector3(position,msg->GetX(),msg->GetY(),msg->GetZ());
				((CBlueStrike*)strike)->Enter(position,msg->GetIsCrate());
			}
		}
		break;
	case MSG_SLOW_TIME:
		{
			CSlowTimeMessage* msg = ((CSlowTimeMessage*)_message);
			game->m_Game->SetSlowTimer(msg->GetSlowTimer());
		}
		break;
	case MSG_CREATE_VFX:
		{
			CCreateVFXMessage* msg = ((CCreateVFXMessage*)_message);

			Vec3f position;
			glsLoadVector3(position, msg->GetX(),msg->GetY(), msg->GetZ());

			game->m_VFXManager.RequestEmitter(msg->GetVFXType(),position,msg->GetTarget());

		}
		break;
	};
}
