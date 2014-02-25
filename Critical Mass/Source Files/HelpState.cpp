#include "../Header Files/StdAfx.h"
#include "../Header Files/HelpState.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/Game.h"
#include "../Header Files/StateManager.h"

CHelpState::CHelpState(void)
{
	m_Timer = 0.0f;
	m_FadeIn_Out = true;
}

CHelpState::~CHelpState(void)
{
}

CHelpState* CHelpState::GetInstance(void)
{
	static CHelpState s_Instance;

	return &s_Instance;
}

void CHelpState::Init(CGame* _game)
{
	m_Timer = 0.0f;

	m_Renderer = _game->GetRenderer();
	m_Game = _game;

	SetID( HELP_STATE );
}

void CHelpState::Shutdown()
{

}

void CHelpState::Enter()
{
	m_Timer = 1.0f;

	Vec4f white = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLuint temp;

	if(m_Game->GetUsingController())
		temp = CAssetManager::GetInstance()->GetTexture(CONTROLLER_HELP);
	else
		temp = CAssetManager::GetInstance()->GetTexture(KEYBOARD_HELP);

	m_BackGround.Setup(m_Renderer, temp, RECTANGLE(0.0f, 1280.0f, 0.0f, 720.0f),
		(float)m_Renderer->GetScreenWidth() / 2.0f - 640.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		(float)m_Renderer->GetScreenHeight() / 2.0f - 360.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		0.0f, (m_Renderer->GetScreenWidth() / 1280.0f) * (720.0f / m_Renderer->GetScreenHeight()), white);
	
	m_FadeIn_Out = true;
}

void CHelpState::Exit()
{

}

bool CHelpState::Input( CInput& _input )
{
	//m_Game->GetStateManager()->PopState();
	if( m_Timer <= 0.0f )
	{
		if( _input.KeyDown(DIK_RETURN) || _input.IsLeftClicked() ||
			_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A )
		{
			m_Timer = 1.0f;
			m_FadeIn_Out = false;
		}
	}

	return true;
}

void CHelpState::Update(float _fElapsedTime)
{
	m_Timer -= _fElapsedTime;
	if(m_Timer > 0.0f)
	{
		if( m_FadeIn_Out )
		{
			if( *m_BackGround.GetOpacity() < 1.0f )
				m_BackGround.SetOpacity(1.0f - m_Timer);
			else
				m_BackGround.SetOpacity(1.0f);
		}
		else
		{
			if( *m_BackGround.GetOpacity() > 0.0f )
				m_BackGround.SetOpacity(m_Timer);
			else
				m_BackGround.SetOpacity(0.0f);
		}

		m_BackGround.BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	}

	if( !m_FadeIn_Out && m_Timer <= 0.0f )
	{
		m_Game->GetStateManager()->PopState();
		return;
	}
}

void CHelpState::Render()
{
	m_Renderer->EnableDraw();

	m_Renderer->DrawTexture(&m_BackGround);

	m_Renderer->DisableDraw();
}