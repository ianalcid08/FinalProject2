/*****************************************************************
File name:			GamePlayState.cpp
Date:				7/17/13
Modify Date:		7/17/13
Modify Initials:	IA
Author:				Ian Alcid
Purpose:			Class for Game Play state.
******************************************************************/
#include "../Header Files/stdafx.h"
#include "../Header Files/GamePlayState.h"
#include "../Header Files/Game.h"
#include "../Header Files/Camera.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/CinematicSystem.h"


#include "../Header Files/CollisionEntity.h"
#include "../Header Files/CollisionLib.h"

//Sound includes...........................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
//#include "../../Includes/Sound/Source/Entity3D.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................


CGameplayState::CGameplayState(void)
{
	this->SetClosing(false);
	m_DisplayWaveNotice = true;
	m_DisplayWaveNoticeTimer = 5.0f;
	m_Transition = false;
	m_HideCursor = false;
}

CGameplayState* CGameplayState::GetInstance()
{
	static CGameplayState s_Instance;
	return &s_Instance;
}

void CGameplayState::Init(CGame* _game)
{	
	SetID( GAMEPLAY_STATE );
	m_Game			= _game;
	m_AssetManager	= CAssetManager::GetInstance();
	m_ObjFactory	= m_Game->GetOF();
	m_ObjManager	= m_Game->GetOM();
	m_Renderer		= m_Game->GetRenderer();
	m_VFXManager.Initialize(m_Renderer);
	m_pause = false;
	m_LowHPBeacon = false;
	//CJM
	m_bFadeOut = false;
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;

	m_DepotAlertFlashTimer = 0.0f;
	m_DepotAlertFlash = true;

	m_playThisOnlyOnce = false;
}

void CGameplayState::Enter()
{	
	m_HideCursor = false;
	// Initialize HUD
	m_HUD.Initialize(m_Renderer);

	//Reset all your geo emitters
	m_VFXManager.ResetGeoEmitters();

	// Initialize the Message System
	CMessageSystem::GetInstance()->Initialize(MessageProcessor);

	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_04);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_03);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_02);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_01);

	// Initialize AI
	m_AIManager.Initialize(nullptr);

	m_Transition = false;
	m_bFadeOut = false;
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;

	m_DepotAlertFlashTimer = 0.0f;
	m_DepotAlertFlash = true;
	DoIMakeCrates = false;

	// We don't have a real sky box yet.  It's still hacked into the
	// renderer at this point.
	// Create Skybox
	//m_ObjFactory->Create(ET_SKYBOX,&skybox);

	// Create World
	m_ObjFactory->Create(ET_WORLD,&m_World);

	// Create Player
	m_ObjFactory->Create( ET_PLAYER, &m_Player );
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

	// Load Camera
	m_Camera.ResetCam();
	m_Camera.SetTarget((CPlayer*)m_Player);
	//m_Camera.ResetCam();

	// Set player as the target to the AI
	m_AIManager.SetTarget((CPlayer*)m_Player);

	// Load the Depots
	for(int i = 0; i < 6; ++i)
		m_ObjFactory->Create( ET_DEPOT, &m_DepotRot[i]);
	// Initialize the depots.
	((CDepot*)m_DepotRot[0])->Initialize(60, 0);
	((CDepot*)m_DepotRot[1])->Initialize(60, 120);
	((CDepot*)m_DepotRot[2])->Initialize(60, 240);

	((CDepot*)m_DepotRot[3])->Initialize(120, 60);
	((CDepot*)m_DepotRot[4])->Initialize(120, 180);
	((CDepot*)m_DepotRot[5])->Initialize(120, 300);

	for(int i = 0; i < MAX_DEPOTS; ++i)
	{
		((CDepot*)m_DepotRot[i])->Reset();
	}

	for(int i = 0; i < MAX_LANDINGPADS; ++i)
		m_ObjFactory->Create(ET_LANDINGPAD,&m_LandingPads[i]);

	((CLandingPads*)m_LandingPads[0])->Initialize(0,0);
	((CLandingPads*)m_LandingPads[1])->Initialize(180,0);

	// Create RedBarrels
	for (int i = 0; i < MAX_BARRELS; ++i)
		m_ObjFactory->Create( ET_REDBARREL, &m_RedBarrel[i]);

	// Initialize RedBarrels
	((CRedBarrel*)m_RedBarrel[0])->Initialize(30, 30);
	((CRedBarrel*)m_RedBarrel[1])->Initialize(90, 45);
	((CRedBarrel*)m_RedBarrel[2])->Initialize(150, 90);
	((CRedBarrel*)m_RedBarrel[3])->Initialize(210, 120);
	((CRedBarrel*)m_RedBarrel[4])->Initialize(270, 150);

	((CRedBarrel*)m_RedBarrel[5])->Initialize(30, 180);
	((CRedBarrel*)m_RedBarrel[6])->Initialize(90, 210);
	((CRedBarrel*)m_RedBarrel[7])->Initialize(150, 240);
	((CRedBarrel*)m_RedBarrel[8])->Initialize(210, 270);
	((CRedBarrel*)m_RedBarrel[9])->Initialize(270, 300);

	//// Create Crates
	for(int i = 0; i < 6; ++i)
	{
		m_ObjFactory->Create( ET_CRATE, &m_TestCrates[i]);
		m_TestCrates[i]->SetIsActive(false);
		m_TestCrates[i]->SetIsAlive(false);
	}
	//// Initialize Crates
	//((CCrate*)m_TestCrates[0])->Init(5, 5);
	//((CCrate*)m_TestCrates[1])->Init(3, 9);
	//((CCrate*)m_TestCrates[2])->Init(10, 5);
	//((CCrate*)m_TestCrates[3])->Init(8, 5);
	//((CCrate*)m_TestCrates[4])->Init(4, 5);

	// Reset waves.
	m_Difficulty = m_Game->GetDifficulty();
	if(GetAsyncKeyState(VK_NUMPAD5))
	{
		m_WaveManager.LoadWaves("Assets/Scripts/Levels/TestBoss.txt");
	}
	else
	{
		switch(m_Difficulty)
		{
		case EASY_DIFF:
			{
				m_WaveManager.LoadWaves("Assets/Scripts/Levels/EasyMode.txt");
				MAX_HP_SMALL_ENEMY = 4;
				MAX_HP_MED_ENEMY = 50;
				MAX_HP_LARGE_ENEMY = 100;
				EXP_YELLOW_LVL1				= 75;
				EXP_YELLOW_LVL2				= 150;
				EXP_YELLOW_LVL3				= 225;
				EXP_BLUE_LVL1				= 75;
				EXP_BLUE_LVL2				= 150;
				EXP_BLUE_LVL3				= 225;
				EXP_RED_LVL1				= 75;
				EXP_RED_LVL2				= 150;
				EXP_RED_LVL3				= 225;
			}
			break;
		case NORMAL_DIFF:
			{
				m_WaveManager.LoadWaves("Assets/Scripts/Levels/MediumMode.txt");
				MAX_HP_SMALL_ENEMY = 4;
				MAX_HP_MED_ENEMY = 50;
				MAX_HP_LARGE_ENEMY = 100;
				EXP_YELLOW_LVL1				= 100;
				EXP_YELLOW_LVL2				= 200;
				EXP_YELLOW_LVL3				= 300;
				EXP_BLUE_LVL1				= 100;
				EXP_BLUE_LVL2				= 200;
				EXP_BLUE_LVL3				= 300;
				EXP_RED_LVL1				= 100;
				EXP_RED_LVL2				= 200;
				EXP_RED_LVL3				= 300;
			}
			break;
		case HARD_DIFF:
			{
				m_WaveManager.LoadWaves("Assets/Scripts/Levels/HardMode.txt");
				MAX_HP_SMALL_ENEMY = 15;
				MAX_HP_MED_ENEMY = 110;
				MAX_HP_LARGE_ENEMY = 225;
				EXP_YELLOW_LVL1				= 120;
				EXP_YELLOW_LVL2				= 250;
				EXP_YELLOW_LVL3				= 500;
				EXP_BLUE_LVL1				= 150;
				EXP_BLUE_LVL2				= 275;
				EXP_BLUE_LVL3				= 500;
				EXP_RED_LVL1				= 150;
				EXP_RED_LVL2				= 275;
				EXP_RED_LVL3				= 500;
			}
			break;
		};

	}
	m_Pull = nullptr;

	// Test VFX System.
	//Vec3f pos = {0.0f,0.0f,0.0f};
	//Vec3f pos2 = {15.0f,0.0f,0.0f};
	//m_VFXManager.StartGeoEmmiter(pos,50.0f,GEO_SCALING_BOX,GEO_SCALING_BOX_ANIM);
	//m_VFXManager.StartGeoEmmiter(pos2,50.0f,GEO_SCALING_BOX,GEO_SCALING_BOX_ANIM);

	m_AIManager.DestroySmall();

	// Create a temporary light pole.
	m_lightPoles[0].Initialize( 185.0, 0.0 ); // on a landing pad

	m_lightPoles[1].Initialize( 45.0, 60.0 );
	m_lightPoles[2].Initialize( 45.0, 180.0 );
	m_lightPoles[3].Initialize( 45.0, 300.0 );

	m_lightPoles[4].Initialize( 90.0, 0.0 );
	m_lightPoles[5].Initialize( 90.0, 72.0 );
	m_lightPoles[6].Initialize( 90.0, 144.0 );
	m_lightPoles[7].Initialize( 90.0, 216.0 );
	m_lightPoles[8].Initialize( 90.0, 288.0 );

	m_lightPoles[9].Initialize( 135.0, 0.0 );
	m_lightPoles[10].Initialize( 135.0, 120.0 );
	m_lightPoles[11].Initialize( 135.0, 240.0 );

	m_lightPoles[12].Initialize( 5.0, 0.0 ); // on the other landing pad

	m_WaveManager.Initialize(m_Renderer);
	((CPlayer*)m_Player)->GetGun().Reset();


	CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_QUAD_PLAYER,m_Player,0.0f,0.0f,0.0f));
	CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_HP_ARROW,m_Player,0.0f,0.0f,0.0f));

	Vec3f empty = {0.0f,0.0f,0.0f};
	Vec3f pos = {0.0f,0.0f,0.0f};
	Vec3f vel = {1.0f,0.0f,0.0f};

	m_testAsteroid.Initialize(ET_ASTEROID,pos,vel,pos);
	glsLoadVector3(pos,-500.0f,75.0f,0.0f);
	m_testPlanet.Initialize(ET_PLANET,pos,vel,empty);

	glsLoadVector3(pos,0.0f,-500.0f,-100.0f);
	m_testSun.Initialize(ET_SUN,pos,empty,empty);

	m_Cinema = new CCinematicSystem(&m_Camera);

	m_PlayerSpawning = true;
	m_OutroPlay = false;

	//CJM
	//m_BlackBackground.SetScreenHeight(m_Game->GetScreenHeight());
	//m_BlackBackground.SetScreenWidth(m_Game->GetScreenWidth());
	//m_BlackBackground.SetHeight((float)m_Game->GetScreenHeight());
	//m_BlackBackground.SetWidth((float)m_Game->GetScreenWidth());
	//m_BlackBackground.SetPosX(0.0f);
	//m_BlackBackground.BuildGeometry();
	//m_BlackBackground.SetTextureHandle(CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND));

	ResetUI();
}

void CGameplayState::ResetUI( void )
{
	// Make sure we're not leaking memory.
	m_BlackBackground.ReleaseGLBatch();
	m_reticle.ReleaseGLBatch();

	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_BlackBackground.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	/*m_reticle.Setup(32, 32,_game->GetScreenWidth(), _game->GetScreenHeight(), retPos, 0.0f);
	m_reticle.BuildGeometry();
	m_reticle.SetTextureHandle(CAssetManager::GetInstance()->GetTexture(G_RETICULE));*/
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);
}

void CGameplayState::Exit()
{
	// Release all the VRAM for the HUD
	m_HUD.Release();
	m_BlackBackground.ReleaseGLBatch();
	m_reticle.ReleaseGLBatch();

	((CPlayer*)m_Player)->ResetMatrix();
	for(int i = 0; i < MAX_DEPOTS; i++)
		((CDepot*)m_DepotRot[i])->DestroyShield();
	m_ObjManager->RemoveAllEntities();
	m_ObjFactory->ProcessDestroy();
	m_AIManager.Reset();
	m_AIManager.Release();

	//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_RAIN_LOOP);	
	//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_WIND_LOOP);
	//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_KLAXON_LOOP);

	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_04);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_03);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_02);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_01);

	m_WaveManager.SetCurrWave(1);

	delete m_Cinema;
	m_Cinema = nullptr;

	CMessageSystem::GetInstance()->ClearMessages();
}

void CGameplayState::Shutdown()
{
	m_HUD.Release();
	m_BlackBackground.ReleaseGLBatch();
	m_reticle.ReleaseGLBatch();

	delete m_Cinema;
	m_Cinema = nullptr;
}

void CGameplayState::ResetGame()
{
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
	((CPlayer*)m_Player)->SetKillScreen(false);
	((CPlayer*)m_Player)->SetStopAnimSwitching(false);
	//

	// Set the players current rotation to the default orientation.
	((CPlayer*)m_Player)->GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	((CPlayer*)m_Player)->GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	((CPlayer*)m_Player)->GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// Reset Depots
	for(int i = 0; i < 6; ++i)
	{
		((CDepot*)m_DepotRot[i])->DestroyShield();
		m_ObjFactory->Destroy(m_DepotRot[i]);
	}

	this->m_AIManager.DestroySmall();
	m_WaveManager.Reset();
	m_Camera.ResetCam();
}

bool CGameplayState::Input( CInput& _input )
{	
	m_Camera.Input(_input);

	// Updates the reticle location based on the mouse pos.
	m_reticle.SetPosX(_input.GetMousePosX() - m_reticle.GetRect().GetWidth() * 0.5f);
	m_reticle.SetPosY(_input.GetMousePosY() - m_reticle.GetRect().GetHeight() * 0.5f);
	m_reticle.BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	static float foot_sound_timer = 5.0f;
	foot_sound_timer += 0.1f;

	if( !((CPlayer*)m_Player)->GetStopAnimSwitching() )
	{
		if(!m_Cinema->InProgress())
		{
			// Move direction dependent on left stick.

			if(m_Game->GetUsingController())
			{
				int sThumbLX = (int)_input.GetController().GetControllerState().Gamepad.sThumbLX;
				int sThumbLY = (int)_input.GetController().GetControllerState().Gamepad.sThumbLY;
				if( abs(sThumbLX) > 8000 || abs(sThumbLY) > 8000 )
				{
					Vec3f stick;
					glsLoadVector3(stick, (float)-sThumbLX, 0.0f, (float)sThumbLY);
					glsNormalizeVector3(stick);
					((CPlayer*)m_Player)->MoveForward(stick[2]);
					((CPlayer*)m_Player)->MoveRight(stick[0]);

					((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);

					if(foot_sound_timer >= 2.0f)
					{
						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_FOOTSTEP);
						foot_sound_timer = 0.0f;
					}
				}
				else
				{
					((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.3f);

				}
			}

			if( _input.KeyDown( DIK_W ))
			{
				((CPlayer*)m_Player)->MoveForward(1.0f);
				((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.2f);

				if(foot_sound_timer >= 2.0f)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_FOOTSTEP);
					foot_sound_timer = 0.0f;
				}
			}
			else if( _input.KeyDown( DIK_S ))
			{
				((CPlayer*)m_Player)->MoveForward(-1.0f);
				((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.2f);
				if(foot_sound_timer >= 2.0f)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_FOOTSTEP);
					foot_sound_timer = 0.0f;
				}
			}

			if( _input.KeyDown( DIK_A ))
			{
				((CPlayer*)m_Player)->MoveRight(1.0f);
				((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.2f);
				if(foot_sound_timer >= 2.0f)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_FOOTSTEP);
					foot_sound_timer = 0.0f;
				}
			}
			else if( _input.KeyDown( DIK_D ))
			{
				((CPlayer*)m_Player)->MoveRight(-1.0f);
				((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.2f);
				if(foot_sound_timer >= 2.0f)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_FOOTSTEP);
					foot_sound_timer = 0.0f;
				}
			}

			if( !_input.KeyDown( DIK_D ) &&  !_input.KeyDown( DIK_A ) && !_input.KeyDown( DIK_S ) && !_input.KeyDown( DIK_W ) )
				((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.3f);

			static bool buffer = false;
			if(_input.KeyDown(DIK_TAB) || _input.GetController().GetControllerState().Gamepad.wButtons & RIGHT_BUMPER)
			{
				if(!buffer)
				{
					m_HUD.ShowTab();
					buffer = true;
					if(m_HUD.GetShowTab())
						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_WEAPON_MENU_OPEN);
					else
						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_WEAPON_MENU_CLOSE);
				}
			}	
			else
				buffer = false;

			// Look and fire direction, controlled by right stick.
			int sThumbRX = (int)_input.GetController().GetControllerState().Gamepad.sThumbRX;
			int sThumbRY = (int)_input.GetController().GetControllerState().Gamepad.sThumbRY;
			if( abs(sThumbRY) > 8000 || abs(sThumbRX) > 8000 && m_Game->GetUsingController() )
			{
				Vec3f stick;
				//glsLoadVector3(stick, sThumbRX, 0.0f, -sThumbRY );
				glsLoadVector3(stick, (float)sThumbRX, (float)sThumbRY, 0.0f );
				glsNormalizeVector3(stick);

				((CPlayer*)m_Player)->RotateToPosition(stick, *m_Camera.GetOrientation());
			}
			else if( (abs(_input.GetDeltaMousePosX()) > 0 || abs(_input.GetDeltaMousePosY()) > 0))
			{
				Vec3f stick;
				glsLoadVector3(stick, (float)_input.GetDeltaMousePosX(), (float)_input.GetDeltaMousePosY(), 0.0f);
				glsNormalizeVector3(stick);

				((CPlayer*)m_Player)->RotateToPosition(stick, *m_Camera.GetOrientation());
				//m_UsingController = false;
			}


			static bool pressed = false;
			if( _input.IsLeftClicked() || _input.GetController().GetControllerState().Gamepad.bRightTrigger)
			{
				if(((CPlayer*)m_Player)->GetIsLoaded())
				{
					if(((CPlayer*)m_Player)->GetGun().GetConsumed() != EMPTY)
					{
						CMessageSystem::GetInstance()->SendMessage(new CCreateBulletMessage(m_Player,true,true));
					}
					else
					{
						CMessageSystem::GetInstance()->SendMessage(new CCreateBulletMessage(m_Player,true,true,true));
						CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_PUSH,m_Player,0.0f,0.0f,0.0f));
					}

					// switch the loaded back to empty
					((CPlayer*)m_Player)->SetIsLoaded(false);
					((CPlayer*)m_Player)->GetGun().Store(EMPTY);

				}
				else if(((CPlayer*)m_Player)->GetIsLoaded() == false )
				{
					dynamic_cast<CPlayer*>(m_Player)->GetGun().Shoot(m_ObjFactory, (CPlayer*)m_Player, 0 );

					/*			if (dynamic_cast<CPlayer*>(m_Player)->GetGun().GetConsumed() == BLUE)
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_MISSLE_LOOP);*/
				}
				pressed = true;
			}
			/*else if(((CPlayer*)m_Player)->GetGun().GetSuperLaserShot())
			{
			dynamic_cast<CPlayer*>(m_Player)->GetGun().Shoot(m_ObjFactory, (CPlayer*)m_Player, 0 );
			}*/
			else if(((CPlayer*)m_Player)->GetIsLoaded() == false && ((CPlayer*)m_Player)->GetHealth() > 0 )
			{
				//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_RAIN_LOOP);					
				dynamic_cast<CPlayer*>(m_Player)->GetGun().SetLaserChargeTimer(0.2f);
				pressed = false;
			}



			if( ( _input.GetController().GetControllerState().Gamepad.wButtons & LEFT_BUMPER || _input.KeyDown(DIK_SPACE) ) && ((CPlayer*)m_Player)->GetCanPush() )
			{
				if( m_Pull == nullptr )
				{
					((CPlayer*)m_Player)->HasPushed();
					IEntity* m_Push = nullptr;
					m_ObjFactory->Create( ET_PUSH, &m_Push);
					((CPush*)m_Push)->Initialize((CBaseEntity*)m_Player);

					CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_PUSH,m_Player,0.0f,0.0f,0.0f));
					//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_BODYFALL);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PUSH);

				}
			}


			if( /*(((CPlayer*)m_Player)->GetGun().GetConsumed() == EMPTY) && */(_input.GetController().GetControllerState().Gamepad.bLeftTrigger || _input.IsRightClicked())  && ((CPlayer*)m_Player)->GetCanPull() )
			{		
				if( m_Pull == nullptr )
				{
					m_Pull = nullptr;
					m_ObjFactory->Create( ET_PULL, &m_Pull);
					((CPull*)m_Pull)->Initialize((CBaseEntity*)m_Player,nullptr);
					//m_VFXManager.RequestEmitter(EMITTER_PULL,NULL,m_Player);
					CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_PULL,m_Pull,0.0f,0.0f,0.0f));
					//Play a short acvtivation sound
					//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::);
					//Start a sound loop for the vacuum
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_LOOP_PULL);


				}
			}
			else if( m_Pull != nullptr )
			{
				m_Pull->SetIsActive(false);
				m_Pull->SetIsAlive(false);
				m_ObjFactory->Destroy(m_Pull);
				m_Pull = nullptr;
				//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_WEAPONWHOOSH);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_PULL);
			}
		}
	}

	static bool ifPressEsc_GP = false;
	if(_input.IsEscapePressed() || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		if(!ifPressEsc_GP)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUPAUSE);
			m_Game->GetStateManager()->PushState(PAUSE_STATE);
			m_HideCursor = true;
			ifPressEsc_GP = true;
			return true;
		}
	}
	else
		ifPressEsc_GP = false;

	if(_input.KeyDown(DIK_LALT) || _input.KeyDown(DIK_RALT))
	{
		if(_input.KeyDown(DIK_TAB))
		{
			m_HideCursor = true;
			m_Game->GetStateManager()->PushState(PAUSE_STATE);
			return true;
		}
	}

	//if(_input.KeyUp(DIK_ESCAPE)/*_input.KeyDown(DIK_ESCAPE)*/ || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_START ) 
	//{
	//	m_Game->GetStateManager()->PushState(PAUSE_STATE);
	//}

	// GAIN WEAPON
	// RED
	static bool pressed_GainRed = false;
	if(_input.KeyDown(DIK_NUMPAD7))
	{
		if(!pressed_GainRed)
		{
			((CPlayer*)m_Player)->GetGun().SetConsumed(RED);
			((CPlayer*)m_Player)->GetGun().Store(RED);
			((CPlayer*)m_Player)->GetGun().Process();
		}
	}
	else
		pressed_GainRed = false;
	static bool pressed_RedLevelup = false;
	if(_input.KeyDown(DIK_NUMPAD4))
	{
		if(!pressed_RedLevelup)
		{
			((CPlayer*)m_Player)->GetGun().SetExperience(RED,1000);
		}
	}
	else
		pressed_RedLevelup = false;
	// BLUE
	static bool pressed_GainBlue = false;
	if(_input.KeyDown(DIK_NUMPAD8))
	{
		if(!pressed_GainBlue)
		{
			((CPlayer*)m_Player)->GetGun().SetConsumed(BLUE);
			((CPlayer*)m_Player)->GetGun().Store(BLUE);
			((CPlayer*)m_Player)->GetGun().Process();
		}
	}
	else
		pressed_GainBlue = false;
	static bool pressed_BlueLevelup = false;
	if(_input.KeyDown(DIK_NUMPAD5))
	{
		if(!pressed_BlueLevelup)
		{
			((CPlayer*)m_Player)->GetGun().SetExperience(BLUE,1000);
		}
	}
	else
		pressed_BlueLevelup = false;
	// YELLOW
	static bool pressed_GainYellow = false;
	if(_input.KeyDown(DIK_NUMPAD9))
	{
		if(!pressed_GainYellow)
		{
			((CPlayer*)m_Player)->GetGun().SetConsumed(YELLOW);
			((CPlayer*)m_Player)->GetGun().Store(YELLOW);
			((CPlayer*)m_Player)->GetGun().Process();
		}
	}
	else
		pressed_GainYellow = false;
	static bool pressed_YellowLevelup = false;
	if(_input.KeyDown(DIK_NUMPAD6))
	{
		if(!pressed_YellowLevelup)
		{
			((CPlayer*)m_Player)->GetGun().SetExperience(YELLOW,1000);
		}
	}
	else
		pressed_YellowLevelup = false;

#ifdef _DEBUG
#pragma region Cheats_and_hotkeys

	if(_input.KeyDown(DIK_H))
		((CPlayer*)m_Player)->SetHealth(0);

	static bool ifPress_Test = false;
	if(_input.KeyDown(DIK_T))
	{
		if(!ifPress_Test)
		{
			IEntity* test = nullptr;
			if( m_ObjFactory->Create(ET_BLUESTRIKE,&test) )
			{
				((CBlueStrike*)test)->Enter( ((CBaseEntity*)m_Player)->GetRenderNode().GetPosition(),false );
			}
			ifPress_Test = true;
			return true;
		}
	}
	else
		ifPress_Test = false;

	if(_input.KeyDown(DIK_T))
	{
		((CPlayer*)m_Player)->SetLives(1);
	}

	if(_input.KeyDown(DIK_P) )
	{
		((CPlayer*)m_Player)->SetHealth(10000000);
	}
	//if(_input.KeyDown(DIK_O) )
	//{
	//	m_Game->GetStateManager()->ChangeState(WIN_STATE);
	//}
	//if(_input.KeyDown(DIK_I) )
	//{
	//	m_Game->GetStateManager()->ChangeState(LOSE_STATE);
	//}

	static bool pressed_n6 = false;
	if(_input.KeyDown(DIK_NUMPAD3) )
	{
		if(!pressed_n6)
		{
			CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(m_DepotRot[0], ET_LARGEENEMY));
			pressed_n6 = true;
		}
	}
	else
		pressed_n6 = false;

	if( _input.KeyDown(DIK_9) )
	{
		m_Game->GetStateManager()->ChangeState(ANIM_STATE);
		return true;
	}

	if( _input.KeyDown(DIK_6) )
		m_Camera.SetStartWin(true);

	if( _input.KeyDown( DIK_B ) )
		m_Camera.UnAttach();

	static bool pressed_7  = false;
	if(_input.KeyDown(DIK_7) )
	{
		if(!pressed_7)
		{
			cout << "MouseX : " << _input.GetMousePosX() << endl;
			cout << "MouseY : " << _input.GetMousePosX() << endl;
			cout << "MouseDeltaX : " << _input.GetDeltaMousePosX() << endl;
			cout << "MouseDeltay : " << _input.GetDeltaMousePosX() << endl;
			pressed_7 = true;
		}
	}
	else
		pressed_7 = false;

	// DEBUG KEYS
	// STOP AI
	static bool pressed_StopAI = false;
	if(_input.KeyDown(DIK_NUMPAD0))
	{
		if(!pressed_StopAI)
		{
			m_AIManager.SetIsPaused( !m_AIManager.GetIsPaused() );
			pressed_StopAI = true;
		}
	}
	else
		pressed_StopAI = false;
	


	static bool pressed_2 = false;
	if (_input.KeyDown (DIK_2) )
	{
		if( !pressed_2 )
		{
			IEntity *_enemy = nullptr;
			m_ObjFactory->Create( ET_SMALLENEMY, &_enemy);
			Vec3f tempPos, tempForward, tempUp;
			glsCopyVector3(tempPos, ((CBaseEntity*)m_DepotRot[0])->GetRenderNode().GetPosition());
			glsCopyVector3(tempForward, ((CBaseEntity*)m_DepotRot[0])->GetRenderNode().GetForwardVector());
			glsCopyVector3(tempUp, ((CBaseEntity*)m_DepotRot[0])->GetRenderNode().GetUpVector());
			//set the enemy's orientation/position/up to the depots orientation/position/up
			((CBaseEntity*)_enemy)->GetRenderNode().SetUpVector(tempUp);
			((CBaseEntity*)_enemy)->GetRenderNode().SetForwardVector(tempForward);
			((CBaseEntity*)_enemy)->GetRenderNode().SetPosition(tempPos);
			((CBaseEntity*)_enemy)->SetPosition(tempPos);
			((CBaseEntity*)_enemy)->GetRenderNode().SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			m_AIManager.AddSmall((CLivingEntity*)_enemy);
			pressed_2 = true;
			_enemy = nullptr;
		}
	}
	else
		pressed_2 = false;

	if( _input.KeyDown( DIK_F12 ) )
		m_Game->GetStateManager()->ChangeState( WIN_STATE );
	if( _input.KeyDown( DIK_F11 ) )
		m_Game->GetStateManager()->ChangeState( LOSE_STATE );

#pragma endregion
#endif
	return  true;
}

void CGameplayState::Update( float fElapsedTime )
{
	//m_AIManager.Update(fElapsedTime);

	/*m_Camera.Update(fElapsedTime);
	m_Game->UpdateRenderer(*m_Camera.GetPosition(), *m_Camera.GetOrientation(), m_Game->GetDelta() );*/

	// Check to see if the game is completed!
	//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_GUN_LEVEL_UP);

	static bool hasCinema = false;
	if(!m_Cinema->Update(fElapsedTime))
	{
		if(!hasCinema)
			m_Camera.UnAttach();

		hasCinema = true;
		((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.1f);
	}
	else if(hasCinema)
	{
		m_Camera.ResetCam();
		((CPlayer*)m_Player)->ResetMatrix();
		m_Camera.SetTarget((CBaseEntity*)m_Player);
		m_Cinema->Clear();
		hasCinema = false;
		
		if(!m_OutroPlay)
		{
			m_FadeTimer = 0.00f;
			m_Transition = true;
			m_BlackBackground.SetOpacity(0.0f);
		}
	}

	if(!m_Cinema->InProgress())
		m_HideCursor = false;

	if(((CPlayer*)m_Player)->GetLives() == 0)
		m_Camera.SetStartLose(true);

	if(m_Transition)
	{
		m_FadeTimer -= fElapsedTime;
		if( !m_WaveManager.AllWavesCompleted() )
			m_BlackBackground.SetOpacity(*m_BlackBackground.GetOpacity() + 0.7f * fElapsedTime);
		
		if(m_OutroPlay && m_FadeTimer <= 8.0f)
			m_BlackBackground.SetOpacity(*m_BlackBackground.GetOpacity() + 0.3f * fElapsedTime);
		
		if(m_FadeTimer <= 0.0f)
		{
			if( m_WaveManager.AllWavesCompleted() )
			{
				m_Transition = false;
				((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.001f);
				((CPlayer*)m_Player)->SetStopAnimSwitching(false);
				m_Camera.ResetCam();
				((CPlayer*)m_Player)->ResetMatrix();
				m_Camera.SetStartWin(false);
				m_HUD.SetFadeHUD(false);
				m_BlackBackground.SetOpacity(1.0f);
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_04);
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_WIN_CHEERING);
				m_Game->GetStateManager()->ChangeState(WIN_STATE);

				return;
			}
			else if( ((CPlayer*)m_Player)->GetLives() == 0 )
			{
				m_Transition = false;
				((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.001f);
				((CPlayer*)m_Player)->SetStopAnimSwitching(false);
				m_Camera.ResetCam();
				((CPlayer*)m_Player)->ResetMatrix();
				m_Camera.SetStartWin(false);
				m_HUD.SetFadeHUD(false);
				ResetGame();

				m_BlackBackground.SetOpacity(1.0f);
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_04);
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_LOSE_SAD_MUSIC);
				m_Game->GetStateManager()->ChangeState(LOSE_STATE);

				return; 
			}
			else if( ((CPlayer*)m_Player)->GetIsDying() )
			{
				m_Transition = false;
				m_bFadeOut = false;
				m_Timer = 1.3f;
				m_BlackBackground.SetOpacity(1.0f);
				((CPlayer*)m_Player)->ResetMatrix();
				m_Camera.ResetCam();
				m_Camera.SetTarget((CBaseEntity*)m_Player);
				((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.001f);
				((CPlayer*)m_Player)->SetStopAnimSwitching(false);
				m_AIManager.Reset();
				m_AIManager.DestroySmall(true);
				m_ObjManager->ResetforWave();
				m_ObjFactory->ProcessDestroy();
				m_PlayerSpawning = true;
				//CMessageSystem::GetInstance()->ClearMessages();
			}
			else
			{
				m_Transition = false;
				m_bFadeOut = false;
				m_Timer = 1.3f;
				m_BlackBackground.SetOpacity(1.0f);
			}
		}

		if( m_WaveManager.AllWavesCompleted() )// || ((CPlayer*)m_Player)->GetLives() == 0 )
		{
			m_ObjManager->UpdateEntities(fElapsedTime);
			//m_ObjManager->CheckCollisions();
			m_ObjFactory->ProcessDestroy();

			m_AIManager.Update(fElapsedTime);

			m_Camera.Update(fElapsedTime);
			m_Game->UpdateRenderer(*m_Camera.GetPosition(), *m_Camera.GetOrientation(), m_Game->GetDelta() );
		}

		//m_VFXManager.Update(fElapsedTime);
		//CMessageSystem::GetInstance()->ProcessMessages();

		if(!m_Cinema->InProgress())
			return;
	}

	//CJM
	if (!m_bFadeOut && m_Timer >= 0.0f)
	{
		m_FadeTimer -= fElapsedTime;
		m_BlackBackground.SetOpacity(*m_BlackBackground.GetOpacity() - 0.8f * fElapsedTime);
	}
	else 
		m_bFadeOut = true;

	if( m_PlayerSpawning )
	{
		CMessageSystem::GetInstance()->SendMessageW(new CCreateBlueStrikeMessage(0.0f, 0.0f, 0.0f, true));

		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_BOSS_ENERGY_FADE);

		m_Camera.Update(fElapsedTime);
		((CPlayer*)m_Player)->SetStopAnimSwitching(true);
		m_Camera.Rotate((float)glsDegToRad(90.0f), 1.0f, 0.0f, 0.0f);
		m_Camera.SetStartWin(true);
		m_PlayerSpawning = false;
		m_Timer = 1.6f;
	}

	if( !m_PlayerSpawning )
	{	
		m_Timer -= fElapsedTime;
		if( m_Timer <= 0.0f )
		{
			((CPlayer*)m_Player)->SetStopAnimSwitching(false);
			m_Camera.SetStartWin(false);
		}
	}

	CMessageSystem::GetInstance()->ProcessMessages();
	///////////////////////////////////////////////////////////////////////
	if(!m_WaveManager.AllWavesCompleted())
	{
		m_ObjManager->UpdateEntities(fElapsedTime);
		m_ObjFactory->ProcessDestroy();
		if(!hasCinema)
			m_ObjManager->CheckCollisions();
		
		if(!m_Cinema->InProgress())
			m_AIManager.Update(fElapsedTime);

		m_Camera.Update(fElapsedTime);
		m_Game->UpdateRenderer(*m_Camera.GetPosition(), *m_Camera.GetOrientation(), m_Game->GetDelta() );
	}
	
	for(int i = 0; i < 6; ++i)
	{
		if(((CDepot*)m_DepotRot[i])->GetShield() != nullptr)
		{
			m_ObjManager->AddinBrokenShieldShit(((CDepot*)m_DepotRot[i])->GetShield());
			if(((CShield*)((CDepot*)m_DepotRot[i])->GetShield())->GetHealth() > 0.0f)
			{
				((CDepot*)m_DepotRot[i])->GetShield()->SetIsActive(true);
				((CDepot*)m_DepotRot[i])->GetShield()->SetIsAlive(true);
			}
		}
	}

	if(m_AIManager.GetBoss())
	{
		if(((CBossEnemy*)m_AIManager.GetBoss())->GetShield())
		{
			m_ObjManager->AddinBrokenShieldShit(((CBossEnemy*)m_AIManager.GetBoss())->GetShield());
			if(((CBossEnemy*)m_AIManager.GetBoss())->GetShield()->GetHealth() > 0.0f)
			{
				((CBossEnemy*)m_AIManager.GetBoss())->GetShield()->SetIsActive(true);
				((CBossEnemy*)m_AIManager.GetBoss())->GetShield()->SetIsAlive(true);
			}
		}
	}

	// Used for cinematics, specifically boss phase change.
	glsCopyVector3(m_CamPrevPos,*m_Camera.GetPosition());

	//Toms HP and Crate checks
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

	//Test state change
	static bool once1 = false;
	if( ((CPlayer*)m_Player)->GetIsDying() && ((CPlayer*)m_Player)->GetLives() > 0 )
	{
		if(((CPlayer*)m_Player)->GetResetWave())
		{
			// Reset the wave.
			m_WaveManager.Reset();

			//m_AIManager.DestroySmall();

			//Turn off the health compass
			//m_VFXManager.GetHPArrowFX()->GetGeoEffect()->SetIsActive(false);

			// Reset the player.
			m_PlayerMove = 0.0f;
			((CPlayer*)m_Player)->SetLives(((CPlayer*)m_Player)->GetLives()-1);
			((CPlayer*)m_Player)->SetHealth(MAX_HP_PLAYER);
			((CPlayer*)m_Player)->GetGun().SetConsumed(EMPTY);
			((CPlayer*)m_Player)->GetGun().Store(EMPTY);
			((CPlayer*)m_Player)->SetIsLoaded(false);

			((CPlayer*)m_Player)->GetGun().Process();
			// Set the players current rotation to the default orientation.
			((CPlayer*)m_Player)->GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
			((CPlayer*)m_Player)->GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
			((CPlayer*)m_Player)->GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

			for(int i = 0; i < MAX_DEPOTS; ++i)
			{
				((CDepot*)m_DepotRot[i])->Reset();
				((CDepot*)m_DepotRot[i])->DestroyShield();
			}

			((CPlayer*)m_Player)->SetResetWave(false);
			m_Transition = true;
			m_FadeTimer = 1.8f;
			m_BlackBackground.SetOpacity(0.0f);

			//m_DisplayWaveNotice = true;
		}		

		if(!once1)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_EXPLOSION);
			once1 = true;
		}
	}

	if(m_WaveManager.IsWaveComplete(m_DepotRot))
	{
		if(m_WaveManager.WaveCompleted() && !m_WaveManager.AllWavesCompleted())
		{
			m_DisplayWaveNotice = true;
			m_HUD.ChangeWaveNotice(m_WaveManager.GetCurrentWave() + 1);
			m_playThisOnlyOnce = false; 

			switch(m_WaveManager.GetCurrentWave() + 1)
			{
			case 2:
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_WAVE_02);
				}
				break;
			case 3:
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_WAVE_03);
				}
				break;
			case 4:
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_WAVE_04);
				}
				break;
			case 5:
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_WAVE_05);
				}
				break;
			case 6:
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_WAVE_06);
				}
				break;
			case 7:
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_WAVE_07);
				}
				break;

			};
		}
		else if(m_WaveManager.GetNextWaveTimer() == 5.0f)
		{
			this->m_AIManager.DestroySmall();
			MakeCrates();
		}
	}

	if(m_DisplayWaveNotice)
	{
		m_DisplayWaveNoticeTimer -= fElapsedTime;
		//Tom - here is where you will add the things to the render list for the wave notice - Past Tom
		//for( std::vector<CUIElement*>::iterator iter = m_HUD.m_WaveDisplayVec.begin(); iter != m_HUD.m_WaveDisplayVec.end(); iter++)
		//{
		//	m_Renderer->AddToUIList((*iter));
		//}

		if(m_DisplayWaveNoticeTimer < 0.0f)
		{
			m_DisplayWaveNotice = false;
			m_DisplayWaveNoticeTimer = 5.0f;
		}
	}

	// UI
	this->m_HUD.Update(fElapsedTime,((CPlayer*)m_Player));

	if(GetNumActiveDepots() == 1 && ((CPlayer*)m_Player)->GetLives() != 0)
	{
		if(!m_playThisOnlyOnce)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_RADIO_DEPOT);
			m_playThisOnlyOnce = true;
		}

		if(m_DepotAlertFlash)
		{
			m_DepotAlertFlashTimer += fElapsedTime;

			if(m_DepotAlertFlashTimer >= 1.0f)
				m_DepotAlertFlash = false;
		}
		else
		{
			m_DepotAlertFlashTimer -= fElapsedTime;

			if(m_DepotAlertFlashTimer <= 0.0f)
				m_DepotAlertFlash = true;
		}
		m_HUD.GetHUDElements()[HUD_ONE_DEPOT_LEFT].SetOpacity(m_DepotAlertFlashTimer);
	}

	if(!m_WaveManager.AllWavesCompleted())
	{
		m_WaveManager.Update( fElapsedTime, m_DepotRot, &m_AIManager );
	}



	//THIS SECTION IS FOR TOM'S COMPASS/////////////////////////////////////////////////////////

	for(int i = 0; i < 6; i ++)
	{
		if(m_TestCrates[i]->GetIsActive() && m_TestCrates[i]->GetIsAlive())
		{
			m_VFXManager.GetHPArrowFX()->GetGeoEffect()->FindClosestCrate(m_TestCrates[i]);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	m_VFXManager.Update(fElapsedTime);


	//Tom Stefl
	m_testAsteroid.Update(fElapsedTime);
	m_testPlanet.Update(fElapsedTime);
	m_testSun.Update(fElapsedTime);


	/*if(((CPlayer*)m_Player)->GetHealth() < 30 && !m_LowHPBeacon)
	{
	m_LowHPBeacon = true;
	if(m_LowHPBeacon)
	{
	for(unsigned int i = 0; i < 6; ++i)
	{
	if(m_TestCrates[i]->GetIsAlive() && m_TestCrates[i]->GetIsActive())
	{
	CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_BEACON,m_TestCrates[i],0.0f,0.0f,0.0f));

	}
	}
	}
	}
	else
	{
	m_LowHPBeacon = false;
	}*/

	//CHECK FOR PLAYER DEATH
	if(((CPlayer*)m_Player)->GetLives() <= 0 && ((CPlayer*)m_Player)->GetIsDying())
	{
		m_Transition = true;
		m_FadeTimer = 1.6f;
		m_HUD.SetFadeHUD(true);
		m_BlackBackground.SetOpacity(0.0f);
		//GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_DEATH), 0.1f);
	}
}

void CGameplayState::Render()
{
	Vec3f a, b;
	glsSubtractVectors3( a, ((CBaseEntity*)m_Player)->GetRenderNode().GetPosition(), ((CBaseEntity*)m_World)->GetRenderNode().GetPosition() );
	glsNormalizeVector3( a );

	for( unsigned int i = 0; i < 13; ++i )
	{
		m_Renderer->AddToRenderList( &(m_lightPoles[i].GetRenderNode()) );

		glsSubtractVectors3( b, m_lightPoles[i].GetRenderNode().GetPosition(), ((CBaseEntity*)m_World)->GetRenderNode().GetPosition() );
		glsNormalizeVector3( b );

		if( glsDotProduct3( a, b ) > 0.225f )
			m_Renderer->AddToPointLightList( &(m_lightPoles[i].GetPointLight()) );
	}

	//Tom Stefl
	m_Renderer->AddToRenderList(&m_testAsteroid.GetRenderNode());
	m_Renderer->AddToRenderList(&m_testPlanet.GetRenderNode());
	m_Renderer->AddToRenderList(&m_testSun.GetRenderNode());

	m_ObjManager->RenderEntities();
	m_VFXManager.Render();

	// Delete the debug render nodes.
	for(unsigned int i = 0; i < m_DebugNodes.size(); ++i)
	{
		SAFE_DELETE(m_DebugNodes[i]);
		m_DebugNodes.erase(m_DebugNodes.begin()+i);
		i--;
	}

	m_Renderer->EnableDraw();
	if (!m_bFadeOut && m_Timer >= 0.0f)
		m_Renderer->DrawTexture(&m_BlackBackground);

	if(m_WaveManager.IsWaveComplete(m_DepotRot))
	{
		if(m_WaveManager.GetNextWaveTimer() <= 5.0f)
		{
			m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_PICKUPS]);
			m_Renderer->DrawTexture(&m_WaveManager.m_WaveCompletedText);
		}
	}

	if(m_DisplayWaveNotice)
	{
		for(int i = 0; i < MAX_WAVE_DISPLAY_NUM; i++)
		{
			m_Renderer->DrawTexture(m_HUD.GetWaveDisplayArr()[i]);
		}
	}

	// Tab
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_WEAPON_TAB]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_BXP]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_RXP]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_YXP]);

	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_BLVL]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_RLVL]);
	m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_YLVL]);


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

	//m_Renderer->AddToUIList( &testElement );
	if(!m_Game->GetUsingController() && !m_HideCursor)
		m_Renderer->DrawTexture( &m_reticle );
	//m_Renderer->AddToRenderList(();
	//->DrawTexture(&m_HUD.GetHUDElements()[HUD_LIVES_LABEL]);
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

	if(m_HUD.GetPlayLevelUpSeq())
	{
		m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_LEVELUP_TEXT]);
	}


	if(GetNumActiveDepots() == 1 && ((CPlayer*)m_Player)->GetLives() != 0)
		m_Renderer->DrawTexture(&m_HUD.GetHUDElements()[HUD_ONE_DEPOT_LEFT]);

	if(m_Transition)
		m_Renderer->DrawTexture(&m_BlackBackground);

	m_WaveManager.DrawElements();

	m_Renderer->DisableDraw();
}

void CGameplayState::MakeCrates()
{
	for(int i = 0; i < 6; ++i)
	{
		m_TestCrates[i]->SetIsActive(false);
		m_TestCrates[i]->SetIsAlive(false);
		//m_ObjFactory->Destroy(m_TestCrates[i]);
	}
	m_ObjManager->RemoveHealthPickups();
	// Create Crates
	for(int i = 0; i < 6; ++i)
	{
		m_ObjFactory->Create( ET_CRATE, &m_TestCrates[i]);
		//CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_CRATE_BEACON,m_TestCrates[i],0.0f,0.0f,0.0f));
	}
	// Initialize Crates
	((CCrate*)m_TestCrates[0])->Init(-5, -5);
	((CCrate*)m_TestCrates[1])->Init(0, 0);
	((CCrate*)m_TestCrates[2])->Init(5, 5);
	((CCrate*)m_TestCrates[3])->Init(180, 180);
	((CCrate*)m_TestCrates[4])->Init(185, 185);
	((CCrate*)m_TestCrates[5])->Init(175, 175);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: The message process for the MessageSystem. Takes the sent in message
and does a specific action to it based on it's type.
Parameters:
[in] _message - the message to process.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CGameplayState::MessageProcessor( IMessage* _message )
{
	CGameplayState* game = CGameplayState::GetInstance();

	switch(_message->GetMessageID())
	{
	case MSG_CREATE_EXPLOSION:
		{
			CCreateExplosionMessage* message = ((CCreateExplosionMessage*)_message);
			CExplosion* explosion = nullptr;
			if(game->m_ObjFactory->Create(ET_EXPLOSION, (IEntity**)&explosion))
			{
				explosion->GetRenderNode().SetPosition( ((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition() );
				explosion->SetOwner(message->GetOwner());
				if(message->GetIsEMP())
				{
					explosion->SetIsEMP(true);
					explosion->GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_EMP).m_DiffuseHandle );
				}
				else
					explosion->SetIsEMP(false);

				if(message->GetOwner() == ET_BOSSENEMY)
				{
					CBaseEntity* boss = ((CBaseEntity*)message->GetTarget());

					float dist = (float(rand()%100+100)-100.0f) / 10.0f;
					float dist_y = -(float(rand()%100+100)) / 20.0f;

					float randAngle = (float)glsDegToRad(float(rand()%360));

					Vec3f newPos, offset_x, offset_y, offset_z;
					glsCopyVector3(offset_x,boss->GetRenderNode().GetRightVector());
					glsCopyVector3(offset_y,boss->GetRenderNode().GetUpVector());
					glsCopyVector3(offset_z,boss->GetRenderNode().GetForwardVector());

					newPos[0] = boss->GetRenderNode().GetPosition()[0] + (-dist) * cos(randAngle) * boss->GetRenderNode().GetForwardVector()[0] + (-dist) * sin(randAngle) * boss->GetRenderNode().GetUpVector()[0];
					newPos[1] = boss->GetRenderNode().GetPosition()[1] + (-dist) * cos(randAngle) * boss->GetRenderNode().GetForwardVector()[1] + (-dist) * sin(randAngle) * boss->GetRenderNode().GetUpVector()[1];
					newPos[2] = boss->GetRenderNode().GetPosition()[2] + (-dist) * cos(randAngle) * boss->GetRenderNode().GetForwardVector()[2] + (-dist) * sin(randAngle) * boss->GetRenderNode().GetUpVector()[2];

					glsScaleVector3(offset_y,dist_y);

					glsAddVectors3(newPos,newPos,offset_y);

					explosion->GetRenderNode().SetPosition(newPos);
				}

				explosion->SetSize(message->GetSize());
				CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_SMOKE,explosion,0.0f,0.0f,0.0f));
				CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_EXPLOSION,explosion,0.0f,0.0f,0.0f));
				//Beacuse an explosion happens for only a few frames, the entity passed over may or may not survive long enough for the smoke message to be 
				// processed.  If the case where is did not survive happens, the smoke emitter below will process the effect based on a null entity, getting weird results.
				// To fix this I have manually passed the explosions position over
			}
		}
		break;
	case MSG_CREATE_ENEMY:
		{
			CCreateEnemyMessage* message = ((CCreateEnemyMessage*)_message);
			if( message->GetType() == ET_SMALLENEMY )
			{
				CSmallEnemy* _enemy = nullptr;
				if( game->m_ObjFactory->Create(ET_SMALLENEMY, (IEntity**)&_enemy) )
				{
					// Set the enemies position/orientation to this depot's position/orientation.
					((CBaseEntity*)_enemy)->GetRenderNode().SetUpVector(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetUpVector());
					((CBaseEntity*)_enemy)->GetRenderNode().SetForwardVector(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetForwardVector());
					((CBaseEntity*)_enemy)->GetRenderNode().SetPosition(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition());
					((CBaseEntity*)_enemy)->SetPosition( ((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition());


					// Reset the small enemies data.
					((CSmallEnemy*)_enemy)->Reset();
					// Set small enemies HP
					((CSmallEnemy*)_enemy)->SetHealth(MAX_HP_SMALL_ENEMY);
					// Set enemy color type.
					// for black enemy. Make them rarer than the rest.
					int type = rand()%3;
					if(rand()% (SMALL_ENEMY_BLACK_CHANCE * 2) == 0)
						type = BLACK;
					((CSmallEnemy*)_enemy)->SetPowerupType(type);
					// Set texture handle based on enemy color type.
					//e = CAssetManager::GetInstance()->GetSmallEnemyData(type).m_DiffuseHandle;
					switch(type)
					{
					case RED:
						((CSmallEnemy*)_enemy)->GetRenderNode().SetColor(0.8f, 0.0f, 0.0f, 1.0f);
						((CSmallEnemy*)_enemy)->GetRenderNode().SetEmissiveTextureHandle( CAssetManager::GetInstance()->GetSmallEnemyData(RED).m_hTextureEmissive );
						break;

					case YELLOW:
						((CSmallEnemy*)_enemy)->GetRenderNode().SetColor(0.8f, 0.8f, 0.0f, 1.0f);
						((CSmallEnemy*)_enemy)->GetRenderNode().SetEmissiveTextureHandle( CAssetManager::GetInstance()->GetSmallEnemyData(YELLOW).m_hTextureEmissive );
						break;

					case BLUE:
						((CSmallEnemy*)_enemy)->GetRenderNode().SetColor(0.0f, 0.0f, 0.8f, 1.0f);
						((CSmallEnemy*)_enemy)->GetRenderNode().SetEmissiveTextureHandle( CAssetManager::GetInstance()->GetSmallEnemyData(BLUE).m_hTextureEmissive );
						break;

					case BLACK:
						((CSmallEnemy*)_enemy)->GetRenderNode().SetColor(0.4f, 0.4f, 0.4f, 1.0f);
						((CSmallEnemy*)_enemy)->GetRenderNode().SetEmissiveTextureHandle( CAssetManager::GetInstance()->GetSmallEnemyData(BLACK).m_hTextureEmissive );
						break;
					};

					// Add enemy to the AI Manager so that it can obtain and use a Behavior Tree.
					game->m_AIManager.AddSmall((CLivingEntity*)_enemy);
				}
			}
			else if( message->GetType() == ET_MEDIUMENEMY )
			{
				CMediumEnemy* _enemy = nullptr;
				if( game->m_ObjFactory->Create(ET_MEDIUMENEMY, (IEntity**)&_enemy) )
				{
					// Set the enemies position/orientation to this depot's position/orientation.
					((CBaseEntity*)_enemy)->GetRenderNode().SetUpVector(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetUpVector());
					((CBaseEntity*)_enemy)->GetRenderNode().SetForwardVector(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetForwardVector());
					((CBaseEntity*)_enemy)->GetRenderNode().SetPosition(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition());
					((CBaseEntity*)_enemy)->SetPosition( ((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition());

					// Reset the small enemies data.
					((CMediumEnemy*)_enemy)->Reset();
					// Set texture handle based on enemy color type.
					//((CMediumEnemy*)_enemy)->GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetSmallEnemyData(BLACK).m_DiffuseHandle );
					// Add enemy to the AI Manager so that it can obtain and use a Behavior Tree.
					game->m_AIManager.AddMedium((CLivingEntity*)_enemy);
				}
			}
			else if( message->GetType() == ET_LARGEENEMY )
			{
				CLargeEnemy* _enemy = nullptr;
				if( game->m_ObjFactory->Create(ET_LARGEENEMY, (IEntity**)&_enemy) )
				{
					// Set the enemies position/orientation to this depot's position/orientation.
					((CBaseEntity*)_enemy)->GetRenderNode().SetUpVector(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetUpVector());
					((CBaseEntity*)_enemy)->GetRenderNode().SetForwardVector(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetForwardVector());
					((CBaseEntity*)_enemy)->GetRenderNode().SetPosition(((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition());
					((CBaseEntity*)_enemy)->SetPosition( ((CBaseEntity*)message->GetTarget())->GetRenderNode().GetPosition());

					// Reset the small enemies data.
					((CLargeEnemy*)_enemy)->Reset();

					((CLargeEnemy*)_enemy)->Init();
					// Give him a shield!
					IEntity* shield = nullptr;
					if( game->m_ObjFactory->Create(ET_SHIELD, &shield) )
					{
						((CShield*)shield)->Initialize(0.0f,0.0f,false);
						if(game->m_Difficulty != HARD_DIFF)
							((CShield*)shield)->SetHealth(MAX_HP_SHIELD*2);
						else
							((CShield*)shield)->SetHealth(MAX_HP_SHIELD*3);
						((CShield*)shield)->GetRenderNode().SetPosition(_enemy->GetRenderNode().GetPosition());
						((CLargeEnemy*)_enemy)->SetShield((CShield*)shield);
					}
					// Add enemy to the AI Manager so that it can obtain and use a Behavior Tree.
					game->m_AIManager.AddLarge((CLivingEntity*)_enemy);
				}
			}
			else if( message->GetType() == ET_BOSSENEMY )
			{
				CBossEnemy* _enemy = nullptr;
				if( game->m_ObjFactory->Create(ET_BOSSENEMY, (IEntity**)&_enemy) )
				{
					// Set the enemies position/orientation to this depot's position/orientation.
					Vec3f right = {1.0f,0.0f,0.0f};
					Vec3f up	= {0.0f,1.0f,0.0f};
					Vec3f fwd	= {0.0f,0.0f,1.0f};
					((CBaseEntity*)_enemy)->GetRenderNode().SetUpVector(up);
					((CBaseEntity*)_enemy)->GetRenderNode().SetForwardVector(fwd);
					((CBaseEntity*)_enemy)->GetRenderNode().SetRightVector(right);

					Vec3f pos = {0.0f,0.0f,0.0f};
					((CBaseEntity*)_enemy)->SetPosition(pos);

					// Reset the small enemies data.
					((CBossEnemy*)_enemy)->Reset();
					// Set texture handle based on enemy color type.
					((CBossEnemy*)_enemy)->Init();
					// Add enemy to the AI Manager so that it can obtain and use a Behavior Tree.
					game->m_AIManager.AddBoss((CLivingEntity*)_enemy);
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

			if(target == nullptr)
				return;

			if( (CBaseEntity*)((CCreateBulletMessage*)_message)->GetTarget()->GetType() == (char)ET_PLAYER )
			{
				game->m_ObjFactory->Create(ET_BULLET_PLAYER, (IEntity**)&bullet);
				game->m_VFXManager.GetMuzzleFlashFX()->GetGeoEffect()->SetOpacity(1.0f);
			}
			else
				game->m_ObjFactory->Create(ET_BULLET_ENEMY, (IEntity**)&bullet);

			bullet->SetStrike(((CCreateBulletMessage*)_message)->GetStrike());



			// Adjust bullets starting position:
			Vec3f temp;
			glsCopyVector3(temp,((CBaseEntity*)target)->GetRenderNode().GetForwardVector());
			glsScaleVector3(temp,-3.0f);
			glsAddVectors3(temp,*target->GetPosition(),temp);

			bullet->GetRenderNode().SetPosition( temp );

			Vec3f scaledUp;
			glsCopyVector3(scaledUp,bullet->GetRenderNode().GetUpVector());
			glsScaleVector3(scaledUp, 3.25f);
			glsSubtractVectors3(*bullet->GetPosition(),bullet->GetRenderNode().GetPosition(),scaledUp);

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
				glsRotationMatrix3x3(rotBulletMat, (float)glsDegToRad(angleX),  target->GetRenderNode().GetUpVector()[0], target->GetRenderNode().GetUpVector()[1],  target->GetRenderNode().GetUpVector()[2]); 
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

			//CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_BULLET_TRAIL,bullet,0.0f,0.0f,0.0f));
		}
		break;
	case MSG_CREATE_SHIELD:
		{
			CCreateShieldMessage* msg = ((CCreateShieldMessage*)_message);
			IEntity* shield = nullptr;
			if(game->m_ObjFactory->Create(ET_SHIELD, &shield ))
			{
				((CShield*)shield)->Initialize(msg->GetRotX(),msg->GetRotY(),msg->GetInvulnerable());
				((CShield*)shield)->SetOwnership(msg->GetOwner());
				((CDepot*)msg->GetShield())->SetShield(shield);
				if(!msg->GetInvulnerable())
				{
					CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_BEACON,shield,0.0f,0.0f,0.0f));
					CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_BEACON,shield,0.0f,0.0f,0.0f));
				}
				if(game->m_Difficulty != HARD_DIFF)
					((CShield*)shield)->SetHealth(MAX_HP_SHIELD*2);
				else
					((CShield*)shield)->SetHealth(MAX_HP_SHIELD*3);

				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_SHEILDPICKUP);

				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_SHIELD_ACTIVATE);

			}
		}
		break;
	case MSG_CREATE_HEALTH_PICKUP:
		{
			CCreateHealthPickupMessage* msg = ((CCreateHealthPickupMessage*)_message);

			CHealthPickup* health = nullptr;
			if(game->m_ObjFactory->Create(ET_HEALTH_PICKUP_LARGE, (IEntity**)&health ))
			{
				((CBaseEntity*)health)->GetRenderNode().SetPosition(msg->GetX(), msg->GetY(), msg->GetZ());
				glsCopyVector3( *((CBaseEntity*)health)->GetPosition(), ((CBaseEntity*)health)->GetRenderNode().GetPosition() );
				Vec3f origin = {0.0f,75.0f,0.0f};
				//pos - origin = vec;
				Vec3f newVec;
				glsSubtractVectors3(newVec,origin,*((CBaseEntity*)health)->GetPosition());
				glsNormalizeVector3(newVec);
				//vec = up;
				((CBaseEntity*)health)->GetRenderNode().SetUpVector(newVec);

			}
		}
		break;
	case MSG_CREATE_EMP:
		{
			CCreateEMPMessage* msg = ((CCreateEMPMessage*)_message);

			CEMP* emp = nullptr;
			if( game->m_ObjFactory->Create(ET_EMP, (IEntity**)&emp ) )
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
			if( game->m_ObjFactory->Create(ET_MORTAR, (IEntity**)&mortar) )
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
			if( game->m_ObjFactory->Create(ET_REDSTRIKE,&strike) )
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
			if( game->m_ObjFactory->Create(ET_BLUESTRIKE,&strike) )
			{
				Vec3f position;
				glsLoadVector3(position,msg->GetX(),msg->GetY(),msg->GetZ());
				((CBlueStrike*)strike)->Enter(position,msg->GetIsCrate());
			}
		}
		break;
	case MSG_CREATE_DEBUG_RENDERNODE:
		{
			CCreateDebugRenderNodeMessage* msg = ((CCreateDebugRenderNodeMessage*)_message);
			CRenderNode* debugNode = new CRenderNode();

			Vec3f up, right, fwd;
			glsCopyVector3(up,msg->GetUp());
			glsCopyVector3(fwd,msg->GetForward());
			glsCopyVector3(right,msg->GetRight());

			glsScaleVector3(up,10.0f);
			glsScaleVector3(fwd,3.0f);
			glsScaleVector3(right,3.0f);

			debugNode->SetPosition(msg->GetPos());
			debugNode->SetUpVector(up);
			debugNode->SetForwardVector(fwd);
			debugNode->SetRightVector(right);
			debugNode->SetColor(msg->GetColor());

			debugNode->SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[BLUE_STRIKE_TB] );
			debugNode->SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_LASER).m_DiffuseHandle );
			debugNode->SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
			debugNode->SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
			debugNode->SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
			debugNode->SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
			debugNode->SetEnableBlending( true );

			game->m_Renderer->AddToRenderList(debugNode);
			game->m_DebugNodes.push_back(debugNode);
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

			if(msg->GetVFXType() != GEO_EMITTER_ENERGY_RING || (msg->GetVFXType() == GEO_EMITTER_ENERGY_RING && game->GetNumActiveDepots() == 0))
			{
				game->m_VFXManager.RequestEmitter(msg->GetVFXType(),position,msg->GetTarget());
			}

		}
		break;
	case MSG_FINISH_VFX:
		{

		}
		break;
	case MSG_GIVE_EXP:
		{
			CGiveExperienceMessage* msg = ((CGiveExperienceMessage*)_message);
			int gainedExp			= msg->GetExp();
			unsigned int tempGun	= msg->GetGun();
			CPlayer* tPlayer		= (CPlayer*)game->GetPlayer();
			int currExp				= tPlayer->GetGun().GetExperience(tempGun);

			currExp = gainedExp + currExp;
			//tempPlayer->GetGun().SetExperience(tempPlayer->GetGun().GetConsumed(),tempExp);
			tPlayer->GetGun().SetExperience(tempGun,currExp);
			cout << tPlayer->GetGun().GetConsumed() << " is getting ";
			cout << gainedExp << " exp for a total of " << currExp << endl;

		}
		break;
	case MSG_BOSS_PHASE_CHANGE:
		{
			CBossPhaseChangeMessage* msg = ((CBossPhaseChangeMessage*)_message);
			CBaseEntity* boss = ((CBaseEntity*)msg->GetBoss());

			game->m_FadeTimer = 0.00f;
			game->m_Transition = true;
			game->m_HideCursor = true;

			game->m_AIManager.DestroySmall(false);

			//if(msg->GetPhase() < 3)
			//{
			//	IEntity* shield = nullptr;
			//	if( game->m_ObjFactory->Create(ET_SHIELD, &shield) )
			//	{
			//		((CShield*)shield)->Initialize(0.0f,0.0f,false);
			//		if(game->m_Difficulty != HARD_DIFF)
			//			((CShield*)shield)->SetHealth(MAX_HP_BOSS_ENEMY);
			//		else
			//			((CShield*)shield)->SetHealth(MAX_HP_BOSS_ENEMY*2);
			//		((CShield*)shield)->GetRenderNode().SetPosition(boss->GetRenderNode().GetPosition());
			//		((CBossEnemy*)boss)->SetShield((CShield*)shield);
			//	}
			//}
			//else
			{
				if(game->m_Difficulty != HARD_DIFF)
					((CBossEnemy*)boss)->SetHealth(MAX_HP_BOSS_ENEMY);
				else
					((CBossEnemy*)boss)->SetHealth(MAX_HP_BOSS_ENEMY*2);
			}

			if(msg->GetPhase() == 1)
			{
				game->m_FadeTimer = 0.00f;
				game->m_Transition = true;
				game->m_HideCursor = true;

				boss->GetRenderNode().SetRightVector(-1.0f,0.0f,0.0f);
				boss->GetRenderNode().SetUpVector(0.0f,1.0f,0.0f);
				boss->GetRenderNode().SetForwardVector(0.0f,0.0f,-1.0f);

				// Set the player to a position away from the start pos
				Vec3f playerStartPos = {0.0f,0.0f,-40.0f};
				((CBaseEntity*)game->m_Player)->GetRenderNode().SetPosition(playerStartPos);
				((CBaseEntity*)game->m_Player)->SetPosition(playerStartPos);
				((CBaseEntity*)game->m_Player)->GetRenderNode().SetRightVector(1.0f,0.0f,0.0f);
				((CBaseEntity*)game->m_Player)->GetRenderNode().SetForwardVector(0.0f,0.0f,1.0f);
				((CBaseEntity*)game->m_Player)->GetRenderNode().SetUpVector(0.0f,1.0f,0.0f);
				((CBaseEntity*)game->m_Player)->Update(0.0f);

				/////////////////////////////////////////////////////////////////////////
				// ORIENT THE CAMERA
				/////////////////////////////////////////////////////////////////////////
				Vec3f boss_pos;
				glsCopyVector3(boss_pos,boss->GetRenderNode().GetPosition());
				
				Vec3f up = {0.0f,-1.0f,0.0f}, right = {1.0f,0.0f,0.0f}, forward = {0.0f,0.0f,1.0f}, camStartPos = {0.0f,-50.0f,-50.0f};

				glsCrossProduct3( forward, right, up );
				glsNormalizeVector3( forward );
				
				glsCrossProduct3( right, up, forward );
				glsNormalizeVector3( right );
				
				Matrix3x3f cam_orient;
				for(int i = 0; i < 3; ++i)
					cam_orient[i] = right[i];
				for(int i = 0; i < 3; ++i)
					cam_orient[i+3] = up[i];
				for(int i = 0; i < 3; ++i)
					cam_orient[i+6] = forward[i];
				game->m_Camera.SetOrientation(cam_orient);
				game->m_Camera.SetPosition(boss_pos);
				game->m_Camera.UnAttach();
				/////////////////////////////////////////////////////////////////////////

				// Scene that follows boss falling to the earth
				game->m_Cinema->AddNode(camStartPos,4.0f,camStartPos,boss);
				game->m_Cinema->AddNode(camStartPos,2.0f,camStartPos,boss);
				
				// Scene that is behind the player, facing the boss as his shield phases into existance!
				Vec3f moveUp = {0.0f,-10.0f,0.0f};
				Vec3f camBehindPlayerFar;
				glsCopyVector3(camBehindPlayerFar,((CBaseEntity*)game->m_Player)->GetRenderNode().GetPosition());
				Vec3f offsetFar;
				glsCopyVector3(offsetFar,((CBaseEntity*)game->m_Player)->GetRenderNode().GetForwardVector());
				glsScaleVector3(offsetFar,-15.0f);
				glsAddVectors3(camBehindPlayerFar,camBehindPlayerFar,offsetFar);
				glsAddVectors3(camBehindPlayerFar,camBehindPlayerFar,moveUp);
				game->m_Cinema->AddNode(camBehindPlayerFar,4.0f,camBehindPlayerFar,boss);

				Vec3f camBehindPlayerNear;
				glsCopyVector3(camBehindPlayerNear,((CBaseEntity*)game->m_Player)->GetRenderNode().GetPosition());
				Vec3f offsetNear;
				glsCopyVector3(offsetNear,((CBaseEntity*)game->m_Player)->GetRenderNode().GetForwardVector());
				glsScaleVector3(offsetNear,-6.0f);
				glsAddVectors3(camBehindPlayerNear,camBehindPlayerNear,offsetNear);
				glsAddVectors3(camBehindPlayerNear,camBehindPlayerNear,moveUp);
				game->m_Cinema->AddNode(camBehindPlayerNear,0.0f,camBehindPlayerNear,boss);
			}
			else
			{
				Vec3f boss_pos;
				glsCopyVector3(boss_pos,boss->GetRenderNode().GetPosition());


				Vec3f up, right, forward, moon = {0.0f,75.0f,0.0f};
				glsSubtractVectors3( up, moon, boss->GetRenderNode().GetPosition() );
				glsNormalizeVector3( up );

				glsCrossProduct3( forward, boss->GetRenderNode().GetRightVector(), up );
				glsNormalizeVector3( forward );

				glsCrossProduct3( right, up, forward );
				glsNormalizeVector3( right );


				Matrix3x3f cam_orient;
				for(int i = 0; i < 3; ++i)
					cam_orient[i] = right[i];
				for(int i = 0; i < 3; ++i)
					cam_orient[i+3] = up[i];
				for(int i = 0; i < 3; ++i)
					cam_orient[i+6] = forward[i];
				game->m_Camera.SetOrientation(cam_orient);
				game->m_Camera.SetPosition(boss_pos);
				game->m_Camera.UnAttach();
							
				float dist = 60.0f;
				float total_time = 6.0f;
				unsigned int num_nodes = 32;
				float time_per_node = total_time / float(num_nodes);
				float angle_per_node = 360.0f / float(num_nodes);

				boss = ((CBaseEntity*)msg->GetBoss());
				for(unsigned int i = 0; i < num_nodes; ++i)
				{
					float angle = (float)glsDegToRad(angle_per_node*i);

					Vec3f pos;
					pos[0] = boss->GetRenderNode().GetPosition()[0] + (-dist) * cos(angle) * forward[0] + (-dist) * sin(angle) * right[0];
					pos[1] = boss->GetRenderNode().GetPosition()[1] + (-dist) * cos(angle) * forward[1] + (-dist) * sin(angle) * right[1];
					pos[2] = boss->GetRenderNode().GetPosition()[2] + (-dist) * cos(angle) * forward[2] + (-dist) * sin(angle) * right[2];

					Vec3f look;
					glsSubtractVectors3(look,boss_pos,pos);
					glsNormalizeVector3(look);

					game->m_Cinema->AddNode(pos,time_per_node,look);
				}
			}
		}
		break;
	case MSG_BOSS_OUTRO:
		{
			CBossOutroMessage* msg = ((CBossOutroMessage*)_message);
			CBaseEntity* boss = ((CBaseEntity*)msg->GetBoss());

			game->m_AIManager.DestroySmall(false);
			game->m_WaveManager.SetCurrWave(100);
			game->m_HideCursor = true;
			game->m_Transition = true;
			game->m_BlackBackground.SetOpacity(0.0f);
			game->m_FadeTimer = 16.0f;
			game->m_HUD.SetFadeHUD(true);
			game->m_OutroPlay = true;

			boss->GetRenderNode().SetRightVector(-1.0f,0.0f,0.0f);
			boss->GetRenderNode().SetUpVector(0.0f,1.0f,0.0f);
			boss->GetRenderNode().SetForwardVector(0.0f,0.0f,-1.0f);
			boss->GetRenderNode().SetPosition(0.0f,0.0f,0.0f);

			// Set the player to a position away from the start pos
			Vec3f playerStartPos = {0.0f,0.0f,40.0f};
			((CBaseEntity*)game->m_Player)->GetRenderNode().SetPosition(playerStartPos);
			((CBaseEntity*)game->m_Player)->SetPosition(playerStartPos);
			((CBaseEntity*)game->m_Player)->GetRenderNode().SetRightVector(-1.0f,0.0f,0.0f);
			((CBaseEntity*)game->m_Player)->GetRenderNode().SetForwardVector(0.0f,0.0f,-1.0f);
			((CBaseEntity*)game->m_Player)->GetRenderNode().SetUpVector(0.0f,1.0f,0.0f);
			((CBaseEntity*)game->m_Player)->Update(0.0f);

			/////////////////////////////////////////////////////////////////////////
			// ORIENT THE CAMERA
			/////////////////////////////////////////////////////////////////////////
			Vec3f boss_pos;
			glsCopyVector3(boss_pos,boss->GetRenderNode().GetPosition());
			
			Vec3f up, right, forward, moon = {0.0f,75.0f,0.0f};
			glsSubtractVectors3( up, moon, boss->GetRenderNode().GetPosition() );
			glsNormalizeVector3( up );
			
			glsCrossProduct3( forward, boss->GetRenderNode().GetRightVector(), up );
			glsNormalizeVector3( forward );
			
			glsCrossProduct3( right, up, forward );
			glsNormalizeVector3( right );
			
			Matrix3x3f cam_orient;
			for(int i = 0; i < 3; ++i)
				cam_orient[i] = right[i];
			for(int i = 0; i < 3; ++i)
				cam_orient[i+3] = up[i];
			for(int i = 0; i < 3; ++i)
				cam_orient[i+6] = forward[i];
			game->m_Camera.SetOrientation(cam_orient);
			game->m_Camera.SetPosition(boss_pos);
			game->m_Camera.UnAttach();

			boss->GetRenderNode().SetRightVector(1.0f,0.0f,0.0f);
			boss->GetRenderNode().SetUpVector(0.0f,1.0f,0.0f);
			boss->GetRenderNode().SetForwardVector(0.0f,0.0f,1.0f);
			boss->GetRenderNode().SetPosition(0.0f,0.0f,0.0f);
			/////////////////////////////////////////////////////////////////////////

			// Scene that follows boss falling to the earth
			Vec3f camStartPos = {0.0f,-20.0f,20.0f};
			game->m_Cinema->AddNode(camStartPos,4.0f,camStartPos,boss);
			game->m_Cinema->AddNode(camStartPos,2.0f,camStartPos,boss);
			
			// Scene that is behind the player, facing the boss as his shield phases into existance!
			Vec3f moveUp = {0.0f,-15.0f,0.0f};
			Vec3f camBehindPlayerFar;
			glsCopyVector3(camBehindPlayerFar,((CBaseEntity*)game->m_Player)->GetRenderNode().GetPosition());
			Vec3f offsetFar;
			glsCopyVector3(offsetFar,((CBaseEntity*)game->m_Player)->GetRenderNode().GetForwardVector());
			glsScaleVector3(offsetFar,-15.0f);
			glsAddVectors3(camBehindPlayerFar,camBehindPlayerFar,offsetFar);
			glsAddVectors3(camBehindPlayerFar,camBehindPlayerFar,moveUp);
			game->m_Cinema->AddNode(camBehindPlayerFar,4.0f,camBehindPlayerFar,boss);

			Vec3f camBehindPlayerFurther;
			glsCopyVector3(camBehindPlayerFurther,((CBaseEntity*)game->m_Player)->GetRenderNode().GetPosition());
			Vec3f offsetNear;
			glsCopyVector3(offsetNear,((CBaseEntity*)game->m_Player)->GetRenderNode().GetForwardVector());
			glsScaleVector3(offsetNear,-20.0f);
			glsAddVectors3(camBehindPlayerFurther,camBehindPlayerFurther,offsetNear);
			glsAddVectors3(camBehindPlayerFurther,camBehindPlayerFurther,moveUp);
			game->m_Cinema->AddNode(camBehindPlayerFurther,5.0f,camBehindPlayerFurther,boss);

			// Make it look up to the sky
			glsAddVectors3(camBehindPlayerFurther,camBehindPlayerFurther,moveUp);
			glsAddVectors3(camBehindPlayerFurther,camBehindPlayerFurther,moveUp);
			glsAddVectors3(camBehindPlayerFurther,camBehindPlayerFurther,moveUp);
			Vec3f lookUp = {0.0f,-1.0f,0.0f};
			game->m_Cinema->AddNode(camBehindPlayerFurther,0.0f,lookUp);
		}
		break;
	case MSG_BOSS_INTRO:
		{
			CBossIntroMessage* msg = ((CBossIntroMessage*)_message);
			CBaseEntity* boss = ((CBaseEntity*)msg->GetBoss());
			game->m_FadeTimer = 0.00f;
			game->m_Transition = true;
			game->m_HideCursor = true;

			boss->GetRenderNode().SetRightVector(1.0f,0.0f,0.0f);
			boss->GetRenderNode().SetUpVector(0.0f,1.0f,0.0f);
			boss->GetRenderNode().SetForwardVector(0.0f,0.0f,1.0f);

			// 0.0f, -100.0f, 30.0f;
			//IEntity* shield = nullptr;
			//if( game->m_ObjFactory->Create(ET_SHIELD, &shield) )
			//{
			//	((CShield*)shield)->Initialize(0.0f,0.0f,false);
			//	if(game->m_Difficulty != HARD_DIFF)
			//		((CShield*)shield)->SetHealth(MAX_HP_BOSS_ENEMY);
			//	else
			//		((CShield*)shield)->SetHealth(MAX_HP_BOSS_ENEMY*2);
			//	((CShield*)shield)->GetRenderNode().SetPosition(boss->GetRenderNode().GetPosition());
			//	((CBossEnemy*)boss)->SetShield((CShield*)shield);
			//}

			// Set the player to a position away from the start pos
			Vec3f playerStartPos = {0.0f,0.0f,40.0f};
			((CBaseEntity*)game->m_Player)->GetRenderNode().SetPosition(playerStartPos);
			((CBaseEntity*)game->m_Player)->SetPosition(playerStartPos);
			((CBaseEntity*)game->m_Player)->GetRenderNode().SetRightVector(-1.0f,0.0f,0.0f);
			((CBaseEntity*)game->m_Player)->GetRenderNode().SetForwardVector(0.0f,0.0f,-1.0f);
			((CBaseEntity*)game->m_Player)->GetRenderNode().SetUpVector(0.0f,1.0f,0.0f);
			((CBaseEntity*)game->m_Player)->Update(0.0f);

			/////////////////////////////////////////////////////////////////////////
			// ORIENT THE CAMERA
			/////////////////////////////////////////////////////////////////////////
			Vec3f boss_pos;
			glsCopyVector3(boss_pos,boss->GetRenderNode().GetPosition());
			
			Vec3f up, right, forward, moon = {0.0f,75.0f,0.0f};
			glsSubtractVectors3( up, moon, boss->GetRenderNode().GetPosition() );
			glsNormalizeVector3( up );
			
			glsCrossProduct3( forward, boss->GetRenderNode().GetRightVector(), up );
			glsNormalizeVector3( forward );
			
			glsCrossProduct3( right, up, forward );
			glsNormalizeVector3( right );
			
			Matrix3x3f cam_orient;
			for(int i = 0; i < 3; ++i)
				cam_orient[i] = right[i];
			for(int i = 0; i < 3; ++i)
				cam_orient[i+3] = up[i];
			for(int i = 0; i < 3; ++i)
				cam_orient[i+6] = forward[i];
			game->m_Camera.SetOrientation(cam_orient);
			game->m_Camera.SetPosition(boss_pos);
			game->m_Camera.UnAttach();
			/////////////////////////////////////////////////////////////////////////

			// Scene that follows boss falling to the earth
			Vec3f camStartPos = {0.0f,-50.0f,50.0f};
			game->m_Cinema->AddNode(camStartPos,4.0f,camStartPos,boss);
			game->m_Cinema->AddNode(camStartPos,2.0f,camStartPos,boss);
			
			// Scene that is behind the player, facing the boss as his shield phases into existance!
			Vec3f moveUp = {0.0f,-10.0f,0.0f};
			Vec3f camBehindPlayerFar;
			glsCopyVector3(camBehindPlayerFar,((CBaseEntity*)game->m_Player)->GetRenderNode().GetPosition());
			Vec3f offsetFar;
			glsCopyVector3(offsetFar,((CBaseEntity*)game->m_Player)->GetRenderNode().GetForwardVector());
			glsScaleVector3(offsetFar,-15.0f);
			glsAddVectors3(camBehindPlayerFar,camBehindPlayerFar,offsetFar);
			glsAddVectors3(camBehindPlayerFar,camBehindPlayerFar,moveUp);
			game->m_Cinema->AddNode(camBehindPlayerFar,4.0f,camBehindPlayerFar,boss);

			Vec3f camBehindPlayerNear;
			glsCopyVector3(camBehindPlayerNear,((CBaseEntity*)game->m_Player)->GetRenderNode().GetPosition());
			Vec3f offsetNear;
			glsCopyVector3(offsetNear,((CBaseEntity*)game->m_Player)->GetRenderNode().GetForwardVector());
			glsScaleVector3(offsetNear,-6.0f);
			glsAddVectors3(camBehindPlayerNear,camBehindPlayerNear,offsetNear);
			glsAddVectors3(camBehindPlayerNear,camBehindPlayerNear,moveUp);
			game->m_Cinema->AddNode(camBehindPlayerNear,0.0f,camBehindPlayerNear,boss);

		}
		break;
	case MSG_HUD_LEVEL_UP:
		{
			CMessageSystem::GetInstance()->SendMessage(new CSlowTimeMessage(1.5f));
			CHUDLevelUpMessage* msg = ((CHUDLevelUpMessage*)_message);
			game->m_HUD.ResetLevelUpSequence();
			game->m_HUD.SetWeaponLevel(msg->GetWeaponLevel());
			game->m_HUD.SetWhichWeapon(msg->GetWhichWeapon());
			game->m_HUD.SetPlayLevelUpSeq(true);
		}
		break;
	}
}

int CGameplayState::GetNumActiveDepots()
{
	int active = 0;
	for(int i = 0; i < MAX_DEPOTS; i++)
	{
		if(((CDepot*)m_DepotRot[i])->GetState() == ACTIVATED)
		{
			active++;
		}
	}

	return active;
}

