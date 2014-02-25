#include "../Header Files/stdafx.h"
#include "../Header Files/MainMenuState.h"
/*****************************************************************
File name:			MainMenuState.cpp
Date:				7/17/13
Modify Date:		7/17/13
Modify Initials:	IA
Author:				Ian Alcid
Purpose:			Class for main menu state.
******************************************************************/

#include "../Header Files/Input.h"
#include "../Header Files/Game.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/CinematicSystem.h"
#include "../Header Files/MessageSystem.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../../Includes/Sound/Source/Entity3D.h"


#include <string>
using namespace std;

CMainMenuState* CMainMenuState::GetInstance( void )
{
	static CMainMenuState s_Instance;
	return &s_Instance;
}

CMainMenuState::CMainMenuState(void)
{
	this->SetClosing(false);
}

CMainMenuState::~CMainMenuState(void)
{
	Shutdown();
}

void CMainMenuState::Init(CGame* _game)
{	 

	this->m_Game = _game;
	this->m_Renderer = m_Game->GetRenderer();
	this->m_Choice = 0;
	this->m_AM = CAssetManager::GetInstance();

	m_bExit = true;
	m_bPlay = false;
	m_bCredits = true;
	m_bOptions = true;
	m_bFadeOutC = false;

	m_bFadeOut = false;
	m_Timer = 0.0f;
	m_FadeTimer = 0.0f;
	m_bMouseActive = false;
	m_PrevMousePosX= 0;
	m_PrevMousePosY= 0;

	m_ObjFactory = m_Game->GetOF();
	m_ObjManager = m_Game->GetOM();

	//for(unsigned int i = 0; i < 4; ++i)
	//	m_LerpBools[i] = false;
	m_HideCursor = false;

	SetID( MAINMENU_STATE );
}

void CMainMenuState::Shutdown()
{
	for( unsigned int i = 0; i < MM_NUM_ELEMENTS; ++i )
	{
		m_MMElements[i].ReleaseGLBatch();
	}
	m_reticle.ReleaseGLBatch();
}

void CMainMenuState::Enter()
{
	//ShowCursor(TRUE);

	m_HideCursor = false;
	this->m_Renderer = m_Game->GetRenderer();
	this->m_Choice = 0;
	//m_LerpBools[MM_PLAY] = true;
	this->m_AM = CAssetManager::GetInstance();

	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_01);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_02);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_03);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_04);

	m_bFadeOut = false;
	m_bFadeOutC = false;
	m_Timer = 0.0f;
	m_FadeTimer = 1.0f;
	m_bMouseActive = false;
	m_PrevMousePosX= 0;
	m_PrevMousePosY= 0;

	m_bSoundExit = true;
	m_bSoundPlay = true;
	m_bSoundCredits = true;
	m_bSoundOptions = true;
	m_bSoundCheck = true;
	m_bEsc = true;
	//m_bController = false;

	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	//m_MMElements[MM_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_MAIN_BACKGROUND_1024x768), 
	//								RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 1.0f, white);

	m_MMElements[MM_CRITICALMAS_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_CRITICALMASS_MM_BACKGROUND_1280x720), 
		RECTANGLE(0.0f, 640.0f, 0.0f, 360.0f), 0.0f, 0.0f, 0.0f, 1.0f, white);

	// Creating the world
	m_ObjFactory->Create(ET_WORLD,&m_World);
	// Create the player
	/*m_ObjFactory->Create( ET_PLAYER, &m_Player );
	((CPlayer*)m_Player)->ResetMatrix();
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
	((CPlayer*)m_Player)->SetStopAnimSwitching(false);*/
	//
	

	// create the depots
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

	//m_MMElements[MM_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_MAIN_BACKGROUND_1280x720), 
	//	RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 1.0f, white);


	//
	//// create the landing pads
	for(int i = 0; i < MAX_LANDINGPADS; ++i)
		m_ObjFactory->Create(ET_LANDINGPAD,&m_LandingPads[i]);

	// init the landing pads
	((CLandingPads*)m_LandingPads[0])->Initialize(0,0);
	((CLandingPads*)m_LandingPads[1])->Initialize(180,0);

	// Creating the camera
	//m_Camera.ResetCam();
	m_Camera.SetTarget((CWorld*)m_World);
	m_Camera.SetStartMenu(true);

	m_MMElements[MM_BLACK_BACKGROUND_ELEMENT].Setup(
		m_Renderer,
		CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())),
		0.0f,
		0.0f,
		0.0f,
		720.0f / m_Game->GetScreenHeight(), // This is a hack to make it cover the whole screen.
		white);

	//BUTTONS ----------------------------------------------------------------------------------------------------------------
#pragma region Non-Highlighted Buttons

	

	m_MMElements[MM_BOARDER_ELEMENT].Setup(
		m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BOARDER),
		RECTANGLE(0.0f, 300.0f, 0.0f, 400.0f),
		float(m_Game->GetScreenWidth() / 2),
		float(m_Game->GetScreenHeight() / 2),
		0.0f, 1.0f, white );

	m_MMElements[MM_PLAY_ELEMENT].Setup(
		m_Renderer,
		CAssetManager::GetInstance()->GetTexture(BUTTON_PLAY),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() /2) + 195 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() / 2) -233 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f,
		1.0f,
		white 
		);

	m_MMElements[MM_OPTIONS_ELEMENT].Setup(
		m_Renderer,
		CAssetManager::GetInstance()->GetTexture(BUTTON_OPTIONS),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() /2) + 195 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() / 2) -105 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f,
		1.0f,
		white
		);

	m_MMElements[MM_CREDITS_ELEMENT].Setup(
		m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_CREDITS),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() /2) + 195 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() / 2) + 25 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f,
		1.0f,
		white
		);

	m_MMElements[MM_EXIT_ELEMENT].Setup(
		m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_QUIT),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() /2) + 195 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() / 2) +155 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f,
		1.0f,
		white
		);

	//--------------------------------------------------------------------------------------

	m_MMElements[MM_ALL3_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 180 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.75f, white );

	m_MMElements[MM_ALL3_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3_CONTROLLER),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 190 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.75f, white );

	//m_MMElements[MM_MOVE_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_MOVEMENT),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() / 2) - 400 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) - 96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_MMElements[MM_OK_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_OK),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() / 2) -96 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) - 96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_MMElements[MM_BACK_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_BACK),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() /2) + 224 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) -96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_MMElements[MM_MOVE_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_MOVEMENT_CONTROLLER),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() / 2) -400 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) - 96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_MMElements[MM_OK_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_OK_CONTROLLER),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() /2) -96 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) -96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_MMElements[MM_BACK_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_BACK_CONTROLLER),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() /2) + 224 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight() / 2) -96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );


#pragma endregion

	//HIGHLIGHTED BUTTONS ----------------------------------------------------------------------------------------------------
#pragma region Highlighted Buttons

	m_MMElements[MM_CREDITS_HIGHLIGHTED_ELEMENT].Setup(
		m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_CREDITS_HIGHLIGHTED),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() /2) + 195 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() / 2 + 25 * (m_Game->GetScreenHeight() / 720.0f)),
		0.0f,
		1.0f,
		white
		);

	m_MMElements[MM_PLAY_HIGHLIGHTED_ELEMENT].Setup(
		m_Renderer,
		CAssetManager::GetInstance()->GetTexture(BUTTON_PLAY_HIGHLIGHTED),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() /2) + 195 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() / 2) -233 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f,
		1.0f,
		white
		);

	m_MMElements[MM_OPTIONS_HIGHLIGHTED_ELEMENT].Setup(
		m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_OPTIONS_HIGHLIGHTED),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() /2) + 195 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() / 2) - 105 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f,
		1.0f,
		white
		);

	m_MMElements[MM_EXIT_HIGHLIGHTED_ELEMENT].Setup(
		m_Renderer,
		CAssetManager::GetInstance()->GetTexture(BUTTON_QUIT_HIGHLIGHTED),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() /2) + 195 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() / 2) +155 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f,
		1.0f,
		white
		);
#pragma endregion

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);

	//for(unsigned int i = 0; i < 6; ++i)
	//{
	//	((CDepot*)m_DepotRot[i])->Activate();
	//	CMessageSystem::GetInstance()->ClearMessages();
	//}

	for(int i = 0; i < MM_NUM_ELEMENTS; ++i)
		m_MMElements[i].SetOpacity(0.0f);

	// THE SPACE OBJECTS
	Vec3f empty = {0.0f,0.0f,0.0f};
	Vec3f pos = {0.0f,0.0f,0.0f};
	Vec3f vel = {1.0f,0.0f,0.0f};

	m_testAsteroid.Initialize(ET_ASTEROID,pos,vel,pos);
	glsLoadVector3(pos,-500.0f,75.0f,0.0f);
	m_testPlanet.Initialize(ET_PLANET,pos,vel,empty);

	glsLoadVector3(pos,0.0f,-500.0f,-100.0f);
	m_testSun.Initialize(ET_SUN,pos,empty,empty);
}

void CMainMenuState::Exit()
{
	while( ShowCursor( FALSE ) >= 0 );


	m_ObjManager->RemoveAllEntities();
	m_ObjFactory->ProcessDestroy();
	delete m_Cinema;
	m_Cinema = nullptr;

	// Reset Depots
	for(int i = 0; i < 6; ++i)
	{
		((CDepot*)m_DepotRot[i])->DestroyShield();
		((CDepot*)m_DepotRot[i])->Deactivate();
		m_ObjFactory->Destroy(m_DepotRot[i]);
	}

	for( unsigned int i = 0; i < MM_NUM_ELEMENTS; ++i )
	{
		m_MMElements[i].ReleaseGLBatch();
	}
	m_reticle.ReleaseGLBatch();
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RETREAT);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RISING);
}

bool CMainMenuState::Input( CInput& _input )
{	
	_input.ReadMouse();
	_input.ReadKeyboard();

	static bool ifPressedD = false;
	static bool ifPressedU = false;
	static bool ifPressedEnter = false;
	static bool ifPressedEsc_Cred = false;

	// Updates the reticle location based on the mouse pos.
	m_reticle.SetPosX(_input.GetMousePosX() - m_reticle.GetRect().GetWidth() * 0.5f);
	m_reticle.SetPosY(_input.GetMousePosY() - m_reticle.GetRect().GetHeight() * 0.5f);
	m_reticle.BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());


	if(_input.GetMousePosX() == m_PrevMousePosX && _input.GetMousePosY() == m_PrevMousePosY)
	{
		m_bMouseActive = false;
	}
	if(_input.GetMousePosX() != m_PrevMousePosX)
	{
		m_bMouseActive = true;
		m_PrevMousePosX = _input.GetMousePosX();
	}
	if(_input.GetMousePosY() != m_PrevMousePosY)
	{
		m_bMouseActive = true;
		m_PrevMousePosY = _input.GetMousePosY();
	}

	static bool ifPressedEsc_MM = false;
	if (m_Timer > 0.5f)
	{
		if(!ifPressedEsc_MM)
		{
			ifPressedEsc_MM = true;
			if(_input.KeyDown(DIK_ESCAPE))
			{
				//if (m_bEsc)
				//{
				//	m_bEsc = false;
				//}
				if (m_bSoundCheck)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
					m_bSoundCheck = false;
				}
				m_Choice  = MM_EXIT;
				m_bFadeOut = true;
				return true;
			}
		}
		else
		ifPressedEsc_MM = false;
	}


	//KEYBOARD INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Keyboard Input for Selecting Buttons

	if(_input.KeyDown(DIK_S) || _input.KeyDown(DIK_DOWN) || _input.GetController().GetControllerState().Gamepad.sThumbLY < - 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN )
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedD)
			{
				m_Choice++;
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosX(0.0f);
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosY(m_Choice * 128.0f + 128.0f);
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if(m_Choice == MM_NUM_CHOICES)
				{
					m_Choice = MM_PLAY;
					//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosY(128.0f);
					//m_MMElements[MM_HIGHLIGHTED_ELEMENT].BuildGeometry();
				}
				ifPressedD = true;
			}	
		}
	}
	else
		ifPressedD = false;

	if(_input.KeyDown(DIK_W) || _input.KeyDown(DIK_UP) || _input.GetController().GetControllerState().Gamepad.sThumbLY > 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedU)
			{
				m_Choice--;
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosX( 0.0f);
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosY( m_Choice * 128.0f + 128.0f);
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if(m_Choice < 0)
				{
					m_Choice = MM_EXIT;
					//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosY(512.0f);
					//m_MMElements[MM_HIGHLIGHTED_ELEMENT].BuildGeometry();
				}
				ifPressedU = true;
			}
		}
	}
	else
		ifPressedU = false;
	if (m_Choice == MM_EXIT)
	{
		if (m_bSoundExit)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundExit = false;
			m_bSoundPlay = true;
			m_bSoundOptions = true;
			m_bSoundCredits = true;
			//m_LerpBools[MM_EXIT] = true;
			//m_LerpBools[MM_PLAY] = false;
			//m_LerpBools[MM_CREDITS] = false;
			//m_LerpBools[MM_OPTIONS] = false;
			//m_Camera.SetTarget((CDepot*)m_DepotRot[3]);
		}
	}
	if (m_Choice == MM_PLAY)
	{
		if (m_bSoundPlay)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundExit = true;
			m_bSoundPlay = false;
			m_bSoundOptions = true;
			m_bSoundCredits = true;
			//m_LerpBools[MM_EXIT] = false;
			//m_LerpBools[MM_PLAY] = true;
			//m_LerpBools[MM_CREDITS] = false;
			//m_LerpBools[MM_OPTIONS] = false;
			//m_Camera.SetTarget((CLandingPads*)m_LandingPads[1]);
			//m_Camera.SetTarget((CPlayer*)m_Player);
		}
	}
	if (m_Choice == MM_CREDITS)
	{
		if (m_bSoundCredits)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundExit = true;
			m_bSoundPlay = true;
			m_bSoundOptions = true;
			m_bSoundCredits = false;
			//m_LerpBools[MM_EXIT] = false;
			//m_LerpBools[MM_PLAY] = false;
			//m_LerpBools[MM_CREDITS] = true;
			//m_LerpBools[MM_OPTIONS] = false;
			//m_Camera.SetTarget((CLandingPads*)m_LandingPads[1]);

		}
	}
	if (m_Choice == MM_OPTIONS)
	{
		if (m_bSoundOptions)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundExit = true;
			m_bSoundPlay = true;
			m_bSoundOptions = false;
			m_bSoundCredits = true;
			//m_LerpBools[MM_EXIT] = false;
			//m_LerpBools[MM_PLAY] = false;
			//m_LerpBools[MM_CREDITS] = false;
			//m_LerpBools[MM_OPTIONS] = true;
			//m_Camera.SetTarget((CDepot*)m_DepotRot[0]);
			//((CDepot*)m_DepotRot[0])->Activate();
		}
	}


	if (m_Timer > 0.5f)
	{
		if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
		{
			if(!ifPressedEnter)
			{	
				m_bFadeOut = true;
				///				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				ifPressedEnter = true;
			}
		}
		else
			ifPressedEnter = false;
	}
	/////////////////////////////////////////////////////////////////////////////
	// HOTKEYS //////////////////////////////////////////////////////////////////
	/*if(_input.KeyDown(DIK_T))
	{
		m_Camera.MainMenuLerpage(m_Timer,(CPlayer*)m_Player,((CDepot*)m_DepotRot[1]));	
	}
	if(_input.KeyDown(DIK_U))
	{
		m_Camera.MainMenuLerpage(m_Timer,(CPlayer*)m_Player,((CDepot*)m_DepotRot[0]));
	}
	if(_input.KeyDown(DIK_Y))
	{
		m_Camera.SetTarget((CDepot*)m_DepotRot[0]);
	}*/
	//if( _input.KeyDown( DIK_B ) )
	//	m_Camera.UnAttach();
	m_Camera.Input(_input);
	/////////////////////////////////////////////////////////////////////////////////
	if (m_Choice == MM_PLAY)
	{
		m_bPlay = false;
		m_bOptions = true;
		m_bCredits = true;
		m_bExit = true;
	}
	if (m_Choice == MM_OPTIONS)
	{
		m_bPlay = true;
		m_bOptions = false;
		m_bCredits = true;
		m_bExit = true;
	}
	if (m_Choice == MM_CREDITS)
	{
		m_bPlay = true;
		m_bOptions = true;
		m_bCredits = false;
		m_bExit = true;
	}
	if (m_Choice == MM_EXIT)
	{
		m_bPlay = true;
		m_bOptions = true;
		m_bCredits = true;
		m_bExit = false;
	}

#pragma endregion

	//MOUSE INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Mouse Input for Selecting Buttons

	int _mouseX =			_input.GetMousePosX();
	int _mouseY =			_input.GetMousePosY();

	//EXIT BUTTON
	float _exitWidth1 =			m_MMElements[MM_EXIT_ELEMENT].GetRect().GetWidth();		//256
	float _exitHeight1 =		m_MMElements[MM_EXIT_ELEMENT].GetRect().GetHeight();		//64
	float _exitPosX1 =			m_MMElements[MM_EXIT_ELEMENT].GetPosX();		//280
	float _exitPosY1 =			m_MMElements[MM_EXIT_ELEMENT].GetPosY();		//612
	float _exitCombinedX1 =		_exitPosX1 + _exitWidth1;						//536
	float _exitCombinedY1 =		_exitPosY1 + _exitHeight1;						//676

	if (_mouseX >= _exitPosX1 && _mouseX <= _exitCombinedX1 && _mouseY <= _exitCombinedY1 && _mouseY >= _exitPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = MM_EXIT;
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosY(_exitPosY1);
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if (m_bSoundExit)
				{
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);
					//m_Camera.SetTarget((CDepot*)m_DepotRot[3]);
					m_bSoundPlay = true;
					m_bSoundExit = false;
					m_bSoundCredits = true;
					m_bSoundOptions = true;
					//m_LerpBools[MM_EXIT] = true;
					//m_LerpBools[MM_PLAY] = false;
					//m_LerpBools[MM_CREDITS] = false;
					//m_LerpBools[MM_OPTIONS] = false;
				}
			}

			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked() )
				{
					m_bFadeOut = true;
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}

	//PLAY BUTTON
	float _playWidth1 =			m_MMElements[MM_PLAY_ELEMENT].GetRect().GetWidth();		//256
	float _playHeight1 =		m_MMElements[MM_PLAY_ELEMENT].GetRect().GetHeight();		//64
	float _playPosX1 =			m_MMElements[MM_PLAY_ELEMENT].GetPosX();		//280
	float _playPosY1 =			m_MMElements[MM_PLAY_ELEMENT].GetPosY();		//612
	float _playCombinedX1 =		_playPosX1 + _playWidth1;						//536
	float _playCombinedY1 =		_playPosY1 + _playHeight1;						//676

	if (_mouseX >= _playPosX1 && _mouseX <= _playCombinedX1 && _mouseY <= _playCombinedY1 && _mouseY >= _playPosY1 )
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{

				m_Choice = MM_PLAY;
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosY(_playPosY1);
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if (m_bSoundPlay)
				{
					///					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);
					//m_Camera.SetTarget((CPlayer*)m_Player);
					m_bSoundPlay = false;
					m_bSoundExit = true;
					m_bSoundCredits = true;
					m_bSoundOptions = true;
					//m_LerpBools[MM_EXIT] = false;
					//m_LerpBools[MM_PLAY] = true;
					//m_LerpBools[MM_CREDITS] = false;
					//m_LerpBools[MM_OPTIONS] = false;
				}

			}
			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
				{
					m_bFadeOut = true;
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

					//m_Game->GetStateManager()->ChangeState(INTRO_STATE);
					//return true;
				}
			}
		}
	}

	//CREDITS BUTTON
	float _creditsWidth1 =			m_MMElements[MM_CREDITS_ELEMENT].GetRect().GetWidth();		//256
	float _creditsHeight1 =			m_MMElements[MM_CREDITS_ELEMENT].GetRect().GetHeight();		//64
	float _creditsPosX1 =			m_MMElements[MM_CREDITS_ELEMENT].GetPosX();		//280
	float _creditsPosY1 =			m_MMElements[MM_CREDITS_ELEMENT].GetPosY();		//612
	float _creditsCombinedX1 =		_creditsPosX1 + _creditsWidth1;						//536
	float _creditsCombinedY1 =		_creditsPosY1 + _creditsHeight1;						//676
	static bool ifClicked =		true;

	if (_mouseX >= _creditsPosX1 && _mouseX <= _creditsCombinedX1 && _mouseY <= _creditsCombinedY1 && _mouseY >= _creditsPosY1 )
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = MM_CREDITS;
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosY(_creditsPosY1);
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if (m_bSoundCredits)
				{
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);
					//m_Camera.SetTarget((CLandingPads*)m_LandingPads[1]);
					m_bSoundPlay = true;
					m_bSoundExit = true;
					m_bSoundCredits = false;
					m_bSoundOptions = true;
					//m_LerpBools[MM_EXIT] = false;
					//m_LerpBools[MM_PLAY] = false;
					//m_LerpBools[MM_CREDITS] = true;
					//m_LerpBools[MM_OPTIONS] = false;
				}
			}
			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
				{
					m_bFadeOut = true;
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}

	//OPTIONS BUTTON
	float _optionsWidth1 =			m_MMElements[MM_OPTIONS_ELEMENT].GetRect().GetWidth();		
	float _optionsHeight1 =			m_MMElements[MM_OPTIONS_ELEMENT].GetRect().GetHeight();		
	float _optionsPosX1 =			m_MMElements[MM_OPTIONS_ELEMENT].GetPosX();		
	float _optionsPosY1 =			m_MMElements[MM_OPTIONS_ELEMENT].GetPosY();		
	float _optionsCombinedX1 =		_optionsPosX1 + _optionsWidth1;						
	float _optionsCombinedY1 =		_optionsPosY1 + _optionsHeight1;						
	static bool ifClicked_MM =		true;

	if (_mouseX >= _optionsPosX1 && _mouseX <= _optionsCombinedX1 && _mouseY <= _optionsCombinedY1 && _mouseY >= _optionsPosY1 )
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
		{
			if (m_bMouseActive)
			{
				m_Choice = MM_OPTIONS;
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].SetPosY(_optionsPosY1);
				//m_MMElements[MM_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if (m_bSoundOptions)
				{
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);
					//m_Camera.SetTarget((CDepot*)m_DepotRot[0]);
					m_bSoundPlay = true;
					m_bSoundExit = true;
					m_bSoundCredits = true;
					m_bSoundOptions = false;
					//m_LerpBools[MM_EXIT] = false;
					//m_LerpBools[MM_PLAY] = false;
					//m_LerpBools[MM_CREDITS] = false;
					//m_LerpBools[MM_OPTIONS] = true;
				}
			}
			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
				{
					m_bFadeOut = true;
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}

#pragma endregion

	return true;
}

void CMainMenuState::Update(float _fElapsedTime)
{
	//MM_BLACK_BACKGROUND_ELEMENT
	m_HideCursor = false;
	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
	{
		m_FadeTimer -= _fElapsedTime;											//count up to 1
		for(int i = 0; i < MM_NUM_ELEMENTS; ++i)
			m_MMElements[i].SetOpacity(1.0f - m_FadeTimer); // set the Opacity to that timer	
		if (m_FadeTimer <= 0.0f)
			m_FadeTimer = 0.0f;
	}
	if (m_bFadeOut)
	{
		m_FadeTimer += _fElapsedTime;											//count down to 0
		for(int i = 0; i < MM_NUM_ELEMENTS; ++i)
			m_MMElements[i].SetOpacity(1.0f - m_FadeTimer);     	// set the Opacity to that timer
	}

	if(m_bFadeOut && m_FadeTimer >= 1.0f && !m_bEsc)
	{
		m_Game->SetIsClosing(true);
	}

	if(m_bFadeOut && m_FadeTimer >= 1.0f && m_bEsc)
	{
		switch(m_Choice)
		{
		case MM_PLAY:
			{
				m_Game->GetStateManager()->PushState(DIFFICULTY_STATE);
				m_HideCursor = true;
				//m_Game->GetStateManager()->ChangeState(INTRO_STATE);
				return;
			}
			break;
		case MM_OPTIONS:
			{
				m_Game->GetStateManager()->PushState(OPTIONS_STATE);
				//m_Camera.MainMenuLerpage(_fElapsedTime,m_Player,m_DepotRot[1]);
				m_HideCursor = true;
				return;
			}
			break;
		case MM_CREDITS:
			{
				m_Game->GetStateManager()->PushState(CREDITS_STATE);
				m_HideCursor = true;
				return;
			}
			break;
		case MM_EXIT:
			{
				m_Game->GetStateManager()->PushState(QUITGAME_STATE);
				m_HideCursor = true;
				return;
				//m_Game->SetIsClosing(true);
			}
			break;

		};
	}

	this->m_Timer -= _fElapsedTime;
	
	if(m_Timer <= 0.0f)
	{
		((CDepot*)m_DepotRot[m_ActiveDepot])->Deactivate();
		m_ActiveDepot = rand() % 6;
		((CDepot*)m_DepotRot[m_ActiveDepot])->Activate();
		CMessageSystem::GetInstance()->ClearMessages();
		m_Timer = (rand() % 100) / 10.0f + 5.0f;
	}

	/*if(m_LerpBools[MM_PLAY])
	{
		if(m_Camera.MainMenuLerpage(_fElapsedTime,m_Player,m_LandingPads[0]))
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);
		else
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.3f);
	}
	if(m_LerpBools[MM_OPTIONS])
	{
		if(m_Camera.MainMenuLerpage(_fElapsedTime,m_Player,m_DepotRot[0]))
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);
		else
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.3f);

	}
	if(m_LerpBools[MM_CREDITS])
	{
		if(m_Camera.MainMenuLerpage(_fElapsedTime,m_Player,m_DepotRot[1]))
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);
		else
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.3f);

	}
	if(m_LerpBools[MM_EXIT])
	{
		if(m_Camera.MainMenuLerpage(_fElapsedTime,m_Player,m_DepotRot[2]))
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.3f);
		else
			((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.3f);

	}*/
	//this->m_FadeTimer -= _fElapsedTime;

	//if (m_Timer <= 1.0)
	//{
	//	float fadeInBackground = *(m_MMElements[MM_BACKGROUND_ELEMENT].GetOpacity());

	//	//BACKGROUND
	//	if (m_FadeTimer <= 0.0f)
	//	{
	//		if(fadeInBackground == 1.0f)
	//			fadeInBackground = 0.0f;
	//		fadeInBackground += _fElapsedTime; 
	//		m_MMElements[MM_BACKGROUND_ELEMENT].SetOpacity(fadeInBackground);
	//		m_Renderer->AddToUIList(&m_MMElements[MM_BACKGROUND_ELEMENT]);
	//	}
	//}
	//if (m_Timer >= 1.0 && m_bFadeOut == false)
	//{
	//	if (m_bPlay)
	//		m_Renderer->AddToUIList(&m_MMElements[MM_PLAY_ELEMENT]);
	//	if (!m_bPlay)
	//		m_Renderer->AddToUIList(&m_MMElements[MM_PLAY_HIGHLIGHTED_ELEMENT]);
	//	if(m_bOptions)
	//		m_Renderer->AddToUIList(&m_MMElements[MM_OPTIONS_ELEMENT]);
	//	if(!m_bOptions)
	//		m_Renderer->AddToUIList(&m_MMElements[MM_OPTIONS_HIGHLIGHTED_ELEMENT]);
	//	if(m_bCredits)
	//		m_Renderer->AddToUIList(&m_MMElements[MM_CREDITS_ELEMENT]);
	//	if(!m_bCredits)
	//		m_Renderer->AddToUIList(&m_MMElements[MM_CREDITS_HIGHLIGHTED_ELEMENT]);
	//	if(m_bExit)
	//		m_Renderer->AddToUIList(&m_MMElements[MM_EXIT_ELEMENT]);
	//	if(!m_bExit)
	//		m_Renderer->AddToUIList(&m_MMElements[MM_EXIT_HIGHLIGHTED_ELEMENT]);

	//	m_Renderer->AddToUIList(&m_MMElements[MM_HIGHLIGHTED_ELEMENT]);
	//	m_Renderer->AddToUIList(&m_MMElements[MM_BACKGROUND_ELEMENT]);
	//}
	//if (m_bFadeOut)
	//{
	//	float fadeInBackground = *(m_MMElements[MM_BACKGROUND_ELEMENT].GetOpacity());

	//	//BACKGROUND
	//	if (m_FadeTimer <= 0.0f)
	//	{
	//		fadeInBackground -= _fElapsedTime; 
	//		m_MMElements[MM_BACKGROUND_ELEMENT].SetOpacity(fadeInBackground);
	//		m_Renderer->AddToUIList(&m_MMElements[MM_BACKGROUND_ELEMENT]);
	//	}
	//	if (fadeInBackground <= 0.0f)
	//		ProcessStates((eMMChoices)m_Choice);
	//}

	m_testAsteroid.Update(_fElapsedTime);
	m_testPlanet.Update(_fElapsedTime);
	m_testSun.Update(_fElapsedTime);

	m_ObjManager->UpdateEntities(_fElapsedTime);
	m_ObjFactory->ProcessDestroy();
	m_ObjManager->CheckCollisions();

	m_Camera.Update(_fElapsedTime);
	m_Game->UpdateRenderer(*m_Camera.GetPosition(), *m_Camera.GetOrientation(), m_Game->GetDelta() );

}

void CMainMenuState::Render()
{
	m_Renderer->AddToRenderList(&m_testAsteroid.GetRenderNode());
	m_Renderer->AddToRenderList(&m_testPlanet.GetRenderNode());
	m_Renderer->AddToRenderList(&m_testSun.GetRenderNode());

	m_ObjManager->RenderEntities();
	m_Renderer->EnableDraw();

	m_Renderer->DrawTexture(&m_MMElements[MM_CRITICALMAS_ELEMENT]);
	//m_Renderer->DrawTexture(&m_MMElements[MM_BOARDER_ELEMENT]);

	if (m_bPlay)
		m_Renderer->DrawTexture(&m_MMElements[MM_PLAY_ELEMENT]);
	if (!m_bPlay)
		m_Renderer->DrawTexture(&m_MMElements[MM_PLAY_HIGHLIGHTED_ELEMENT]);
	if(m_bOptions)
		m_Renderer->DrawTexture(&m_MMElements[MM_OPTIONS_ELEMENT]);
	if(!m_bOptions) 
		m_Renderer->DrawTexture(&m_MMElements[MM_OPTIONS_HIGHLIGHTED_ELEMENT]);
	if(m_bCredits)
		m_Renderer->DrawTexture(&m_MMElements[MM_CREDITS_ELEMENT]);
	if(!m_bCredits)
		m_Renderer->DrawTexture(&m_MMElements[MM_CREDITS_HIGHLIGHTED_ELEMENT]);
	if(m_bExit)
		m_Renderer->DrawTexture(&m_MMElements[MM_EXIT_ELEMENT]);
	if(!m_bExit)
		m_Renderer->DrawTexture(&m_MMElements[MM_EXIT_HIGHLIGHTED_ELEMENT]);

	if(m_Game->GetUsingController() == false)
	{
		m_Renderer->DrawTexture(&m_MMElements[MM_ALL3_ELEMENT]);
	}
	else if(m_Game->GetUsingController() == true)
	{
		m_Renderer->DrawTexture(&m_MMElements[MM_ALL3_CONTROLLER_ELEMENT]);
	}


	//DRAW Reticle
	if( !m_HideCursor )
		m_Renderer->DrawTexture( &m_reticle );

	m_Renderer->DisableDraw();
}

void CMainMenuState::ProcessStates(eMMChoices _choice)
{
	//switch(_choice)
	//{
	//case MM_PLAY:
	//	{
	//		m_Game->GetStateManager()->ChangeState(INTRO_STATE);
	//		return;
	//	}
	//	break;
	//case MM_OPTIONS:
	//	{
	//		/*m_Game->GetStateManager()->PushState(OPTIONS_STATE);
	//		return;*/
	//	}
	//	break;
	//case MM_CREDITS:
	//	{
	//		/*m_Game->GetStateManager()->ChangeState(CREDITS_STATE);
	//		return;*/
	//	}
	//	break;
	//case MM_EXIT:
	//	{
	//		m_Game->SetIsClosing(true);
	//	}
	//	break;

	//};
}
