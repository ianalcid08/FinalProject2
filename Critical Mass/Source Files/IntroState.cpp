#include "../Header Files/stdafx.h"
#include "../Header Files/IntroState.h"
/*****************************************************************
File name:			IntroState.cpp
Date:				7/17/13
Modify Date:		08/23/13
Modify Initials:	CM
Author:				Corey Morehead
Purpose:			Class for Intro state.
******************************************************************/
#include "../Header Files/Input.h"
#include "../Header Files/Game.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/GamePlayState.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Wwise_IDs.h"

CIntroState::CIntroState(void)
{
	this->SetClosing(false);
	m_Switch = false;
}

CIntroState* CIntroState::GetInstance( void )
{
	static CIntroState s_Instance;
	return &s_Instance;
}

CIntroState::~CIntroState(void)
{

}

void CIntroState::Init(CGame* _game)
{	
	this->m_Game = _game;
	this->m_Renderer = m_Game->GetRenderer();
	this->m_choice = 0;
	this->m_AM = CAssetManager::GetInstance();
	m_bControlOut = false;
	m_bMouseOut = false;
	m_bEnter = false;
	m_bMouseIn = false;
	m_bControlIn = false;
	m_timer = 0.0f;
	m_FadeTimerIn1 = 0.0f;
	m_FadeTimerOut = 2.0f;
	m_FadeTimerIn = 3.0f;
	m_bControl = true;
	m_bGTFO = false;
	m_bMouse = false;

	SetID( INTRO_STATE );
}

void CIntroState::Shutdown()
{
	for( unsigned int i = 0; i < IN_NUMBER_ELEMENTS; ++i )
	{
		eINElements[i].ReleaseGLBatch();
	}
	m_BackGround.ReleaseGLBatch();
	m_BlackBackground.ReleaseGLBatch();
}

void CIntroState::Enter()
{
	ShowCursor( TRUE );
	m_timer = 0.0f;
	m_FadeTimerIn1 = 0.0f;
	m_FadeTimerOut = 2.0f;
	m_FadeTimerIn = 3.0f;

	m_bControlOut = false;
	m_bMouseIn = false;
	m_bEnter = false;
	m_bMouseOut = false;
	m_bControlIn = false;
	m_bControl = true;
	m_bGTFO = false;
	m_bMouse = false;

	m_bSoundMouse = true;
	m_bSoundControl = true;

	this->m_Renderer = m_Game->GetRenderer();
	this->m_choice = 0;
	this->m_AM = CAssetManager::GetInstance();

	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLuint temp;

	if(m_Game->GetUsingController())
		temp = CAssetManager::GetInstance()->GetTexture(CONTROLLER_HELP);
	else
		temp = CAssetManager::GetInstance()->GetTexture(KEYBOARD_HELP);

	m_BackGround.Setup(m_Renderer, temp, RECTANGLE(0.0f, 1280.0f, 0.0f, 720.0f),
		(float)m_Renderer->GetScreenWidth() / 2.0f - 640.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		(float)m_Renderer->GetScreenHeight() / 2.0f - 360.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		0.0f, (m_Renderer->GetScreenWidth() / 1280.0f) * (720.0f / m_Renderer->GetScreenHeight()), white);

	m_BlackBackground.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture( M_BLACK_BACKGROUND ),
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 1.0f, white);
	
	m_FadeIn_Out = true;
}

void CIntroState::Exit()
{
	while( ShowCursor( FALSE ) >= 0 );

	for( unsigned int i = 0; i < IN_NUMBER_ELEMENTS; ++i )
	{
		eINElements[i].ReleaseGLBatch();
	}
	m_BackGround.ReleaseGLBatch();
	m_BlackBackground.ReleaseGLBatch();
}

bool CIntroState::Input( CInput& _input )
{

	if( m_timer <= 0.0f )
	{
		if( _input.KeyDown(DIK_RETURN) || _input.IsLeftClicked() ||
			_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A )
		{
			m_timer = 1.0f;
			m_FadeIn_Out = false;
		}
	}

	return true;
}

void CIntroState::Update( float _fElapsedTime )
{	

	m_timer -= _fElapsedTime;
	if(m_timer > 0.0f)
	{
		if( m_FadeIn_Out )
		{
			if( *m_BackGround.GetOpacity() < 1.0f )
				m_BackGround.SetOpacity(1.0f - m_timer);
			else
				m_BackGround.SetOpacity(1.0f);
		}
		else
		{
			if( *m_BackGround.GetOpacity() > 0.0f )
				m_BackGround.SetOpacity(m_timer);
			else
				m_BackGround.SetOpacity(0.0f);
		}

		m_BackGround.BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	}

	if( !m_FadeIn_Out && m_timer <= 0.0f )
	{
		m_Game->GetStateManager()->ChangeState(TUTORIAL_GAMEPLAY_STATE);
		return;
	}
}

void CIntroState::Render()
{

	m_Renderer->EnableDraw();

	m_Renderer->DrawTexture(&m_BlackBackground);
	m_Renderer->DrawTexture(&m_BackGround);

	m_Renderer->DisableDraw();
}
