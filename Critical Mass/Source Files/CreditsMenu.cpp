#include "../Header Files/stdafx.h"
#include "../Header Files/CreditsMenu.h"
/*****************************************************************
File name:			CreditsMenu.cpp
Date:				7/17/13
Modify Date:		08/07/13
Modify Initials:	CM
Author:				Ian Alcid
Purpose:			Class for Credits state.
******************************************************************/
#include "../Header Files/Input.h"
#include "../Header Files/Game.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include <string>
using namespace std;


CCreditsState* CCreditsState::GetInstance( void )
{
	static CCreditsState s_Instance;
	return &s_Instance;
}

CCreditsState::CCreditsState(void)
{
	this->SetClosing(false);
	//m_Switch = false;
}

CCreditsState::~CCreditsState(void)
{
	//EMPTY
}

void CCreditsState::Init(CGame* _game)
{	
	this->m_Game = _game;
	this->m_Renderer = m_Game->GetRenderer();
	this->m_AM = CAssetManager::GetInstance();
	m_bFadeOut = false;
}

void CCreditsState::Enter()
{
	this->m_Renderer = m_Game->GetRenderer();
	this->m_AM = CAssetManager::GetInstance();

	m_Timer = 0.0f;
	m_FadeTimer =  1.0f;
	m_bFadeOut = false;
	m_bSoundCheck = true;

	//BLACK BACKGROUND
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_CMElements[CM_BLACK_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f,
		720.0f / m_Renderer->GetScreenHeight(), white);

	//	//BACKGROUND
//#if defined(_DEBUG) || defined(_BETA)
//	m_CMElements[CM_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_CREDITS_BACKGROUND_1024x768), 
//		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f,
//		720.0f / m_Renderer->GetScreenHeight(), white);
//#else
//	m_CMElements[CM_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_CREDITS_BACKGROUND_1280x720), 
//		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f,
//		720.0f / m_Renderer->GetScreenHeight(), white);
//#endif

	m_CMElements[CM_CREDITS_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_CREDITS_TEXT), 
		RECTANGLE(0.0f, 1280.0f, 0.0f, 10000.0f),
		0.0f, (float)m_Renderer->GetScreenHeight(), //m_Game->GetScreenWidth() / 2.0f - 300.0f * m_Renderer->GetScreenHeight() / 720.0f,
		//m_Game->GetScreenHeight() / 2.0f + 400.0f * m_Renderer->GetScreenHeight() / 720.0f,
		0.0f, (m_Renderer->GetScreenWidth() / 1280.0f) * (720.0f / m_Renderer->GetScreenHeight()), white);

	m_CMElements[CM_BUTTONS].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		10.0f * (m_Renderer->GetScreenHeight() / 720.0f), m_Renderer->GetScreenHeight() - 110.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		/*float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 180 * (m_Game->GetScreenHeight() / 720.0f),*/
		0.0f, 0.5f, white );

	m_CMElements[CM_CONTROLLER].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3_CONTROLLER),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		10.0f * (m_Renderer->GetScreenHeight() / 720.0f), m_Renderer->GetScreenHeight() - 110.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		/*float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 190 * (m_Game->GetScreenHeight() / 720.0f),*/
		0.0f, 0.5f, white );

	this->m_Movement = m_CMElements[CM_CREDITS_ELEMENTS].GetPosY();	//384

	for(int i = 0; i < CM_NUM_ELEMENTS; ++i)
		m_CMElements[i].SetOpacity(0.0f);
}

void CCreditsState::Exit()
{
	for( unsigned int i = 0; i < CM_NUM_ELEMENTS; ++i )
	{
		m_CMElements[i].ReleaseGLBatch();
	}
}

void CCreditsState::Shutdown()
{
	Exit();
}

bool CCreditsState::Input( CInput& _input )
{	
	static bool ifPressedEsc_Cred = false;
	if(!ifPressedEsc_Cred)
	{
		ifPressedEsc_Cred = true;
		if(_input.KeyDown(DIK_ESCAPE) || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			m_bFadeOut = true;
			if (m_bSoundCheck)
			{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
				m_bSoundCheck = false;
			}
			return true;
		}
	}
	else
		ifPressedEsc_Cred = false;
	return  true;
}

void CCreditsState::Update( float _fElapsedTime )
{	 
	this->m_Timer += _fElapsedTime;

	this->m_Movement = m_CMElements[CM_CREDITS_ELEMENTS].GetPosY() - (100.0f * _fElapsedTime);
	m_CMElements[CM_CREDITS_ELEMENTS].SetPosY(m_Movement);
	m_CMElements[CM_CREDITS_ELEMENTS].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	if (this->m_Movement <= -10000)
	{
		m_CMElements[CM_CREDITS_ELEMENTS].SetPosY(780.0f);
		this->m_Movement = m_CMElements[CM_CREDITS_ELEMENTS].GetPosY();
		m_CMElements[CM_CREDITS_ELEMENTS].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	}

	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
	{
		m_FadeTimer -= _fElapsedTime;											//count up to 1

		for( unsigned int i = 0; i < CM_NUM_ELEMENTS; ++i )
			m_CMElements[i].SetOpacity(1.0f - m_FadeTimer);     	// set the Opacity to that timer	

		if (m_FadeTimer <= 0.0f)
			m_FadeTimer = 0.0f;
	}

	if (m_bFadeOut)
	{
		m_FadeTimer += _fElapsedTime;											//count down to 0
		m_CMElements[CM_CREDITS_ELEMENTS].SetOpacity(1.0f - m_FadeTimer);     	// set the Opacity to that timer
	}

	if (m_bFadeOut && m_FadeTimer >= 1.0f)
	{
		m_bFadeOut = false;
		m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
		return;		
	}
}

void CCreditsState::Render()
{
	m_Renderer->EnableDraw();
	m_Renderer->DrawTexture(&m_CMElements[CM_BLACK_BACKGROUND_ELEMENT]);
	//m_Renderer->DrawTexture(&m_CMElements[CM_BACKGROUND_ELEMENT]);
	m_Renderer->DrawTexture(&m_CMElements[CM_CREDITS_ELEMENTS]);

	if(m_Game->GetUsingController() == false)
	{
		m_Renderer->DrawTexture(&m_CMElements[CM_BUTTONS]);
	}
	else if(m_Game->GetUsingController() == true)
	{
		m_Renderer->DrawTexture(&m_CMElements[CM_CONTROLLER]);
	}

	/*if(!m_bFadeOut && m_FadeTimer <= 1.0f)
		m_Renderer->DrawTexture(&m_CMElements[CM_BLACK_BACKGROUND_ELEMENT]);

	if(m_bFadeOut)
		m_Renderer->DrawTexture(&m_CMElements[CM_BLACK_BACKGROUND_ELEMENT]);	*/

	m_Renderer->DisableDraw();
}
