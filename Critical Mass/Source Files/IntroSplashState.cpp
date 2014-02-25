/*****************************************************************
File name:			CIntroSplashState.cpp
Date:				08/12/13
Modify Date:		08/12/13
Modify Initials:	CM
Author:				Corey Morehead
Purpose:			This class will show the three images that are required 
from the GRC.
******************************************************************/

#include "../Header Files/StdAfx.h"
#include "IntroSplashState.h"
#include "../Header Files/Input.h"
#include "../Header Files/Game.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"

CIntroSplashState* CIntroSplashState::GetInstance( void )
{
	static CIntroSplashState s_Instance;
	return &s_Instance;
}

CIntroSplashState::CIntroSplashState(void)
{
	this->SetClosing(false);
}

void CIntroSplashState::Init(CGame* _game)
{
	this->m_Game = _game;
	this->m_Renderer = this->m_Game->GetRenderer();
	this->m_AM = CAssetManager::GetInstance();

	m_bGPIn = false;
	m_bGPOut = false;
	m_bStudioIn = false;
	m_bStudioOut = false;
	m_TeamIn = false;
	m_TeamOut = false;

	m_Timer = 0.0f;
	m_FadeTimer = 1.5f;

	m_FadeTimerRedo = 0.0f;
	m_bFadeOut = false;

	SetID( INTRO_SPLASH_STATE );
}

void CIntroSplashState::Enter( void )
{
	m_bGPIn = false;
	m_bGPOut = false;
	m_bStudioIn = false;
	m_bStudioOut = false;
	m_TeamIn = false;
	m_TeamOut = false;

	m_GPFadeTimerIn = 0.0f;
	m_GPFadeTimerOut = 2.0f;
	m_StudioFadeTimerIn = 3.0f;
	m_StudioFadeTimerOut = 5.0f;
	m_TeamFadeTimerIn = 6.0f;
	m_TeamFadeTimerOut = 8.0f;

	m_FadeTimerRedo = 0.0f;
	m_bFadeOut = false;

	//BACKGROUND
	//eISElements[IS_BLACK_BACKGROUND_ELEMENT].SetScreenHeight(m_Game->GetScreenHeight());
	//eISElements[IS_BLACK_BACKGROUND_ELEMENT].SetScreenWidth(m_Game->GetScreenWidth());
	//eISElements[IS_BLACK_BACKGROUND_ELEMENT].SetHeight((float)m_Game->GetScreenHeight());
	//eISElements[IS_BLACK_BACKGROUND_ELEMENT].SetWidth((float)m_Game->GetScreenWidth());
	//eISElements[IS_BLACK_BACKGROUND_ELEMENT].SetPosX(0.0f);
	//eISElements[IS_BLACK_BACKGROUND_ELEMENT].SetPosY(0.0f);
	//eISElements[IS_BLACK_BACKGROUND_ELEMENT].BuildGeometry();
	//eISElements[IS_BLACK_BACKGROUND_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND));
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	eISElements[IS_BLACK_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

	//Images
	//eISElements[IS_BACKGROUND_GP_ELEMENT].SetScreenHeight(m_Game->GetScreenHeight());
	//eISElements[IS_BACKGROUND_GP_ELEMENT].SetScreenWidth(m_Game->GetScreenWidth());
	//eISElements[IS_BACKGROUND_GP_ELEMENT].SetHeight((float)m_Game->GetScreenHeight());
	//eISElements[IS_BACKGROUND_GP_ELEMENT].SetWidth((float)m_Game->GetScreenWidth());
	//eISElements[IS_BACKGROUND_GP_ELEMENT].SetPosX(0.0f);
	//eISElements[IS_BACKGROUND_GP_ELEMENT].SetPosY(0.0f);
	//eISElements[IS_BACKGROUND_GP_ELEMENT].BuildGeometry();										
	//eISElements[IS_BACKGROUND_GP_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(M_GP_BACKGROUND));
	eISElements[IS_BACKGROUND_GP_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_GP_BACKGROUND), 
		RECTANGLE(0.0f, 1280.0f, 0.0f, 720.0f),
		(float)m_Renderer->GetScreenWidth() / 2.0f - 640.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		(float)m_Renderer->GetScreenHeight() / 2.0f - 360.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		0.0f, (m_Renderer->GetScreenWidth() / 1280.0f) * (720.0f / m_Renderer->GetScreenHeight()), white);

	//eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetScreenHeight(m_Game->GetScreenHeight());
	//eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetScreenWidth(m_Game->GetScreenWidth());
	//eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetHeight((float)m_Game->GetScreenHeight());
	//eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetWidth((float)m_Game->GetScreenWidth());
	//eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetPosX(0.0f);
	//eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetPosY(0.0f);
	//eISElements[IS_BACKGROUND_STUDIO_ELEMENT].BuildGeometry();										
	//eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(M_STUDIO_BACKGROUND));
	eISElements[IS_BACKGROUND_STUDIO_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_STUDIO_BACKGROUND), 
		RECTANGLE(0.0f, 1280.0f, 0.0f, 720.0f),
		(float)m_Renderer->GetScreenWidth() / 2.0f - 640.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		(float)m_Renderer->GetScreenHeight() / 2.0f - 360.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		0.0f, (m_Renderer->GetScreenWidth() / 1280.0f) * (720.0f / m_Renderer->GetScreenHeight()), white);

	//eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetScreenHeight(m_Game->GetScreenHeight());
	//eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetScreenWidth(m_Game->GetScreenWidth());
	//eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetHeight((float)m_Game->GetScreenHeight());
	//eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetWidth((float)m_Game->GetScreenWidth());
	//eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetPosX(0.0f);
	//eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetPosY(0.0f);
	//eISElements[IS_BACKGROUND_TEAM_ELEMENT].BuildGeometry();										
	//eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(M_TEAM_BACKGROUND));
	eISElements[IS_BACKGROUND_TEAM_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_TEAM_BACKGROUND), 
		RECTANGLE(0.0f, 1280.0f, 0.0f, 720.0f),
		(float)m_Renderer->GetScreenWidth() / 2.0f - 640.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		(float)m_Renderer->GetScreenHeight() / 2.0f - 360.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		0.0f, (m_Renderer->GetScreenWidth() / 1280.0f) * (720.0f / m_Renderer->GetScreenHeight()), white);
}

void CIntroSplashState::Update(float _fElapsedTime)
{
	m_Timer += _fElapsedTime;	

	//GP
	if (m_Timer < 1.0f)
	{
		m_FadeTimerRedo += _fElapsedTime;
		eISElements[IS_BACKGROUND_GP_ELEMENT].SetOpacity(m_FadeTimerRedo);
	}
	if (m_Timer >= 1.0f && m_Timer < 2.0f)
	{
	}
	if (m_Timer >= 2.0f && m_Timer < 3.0f)
	{
		m_FadeTimerRedo -= _fElapsedTime;
		eISElements[IS_BACKGROUND_GP_ELEMENT].SetOpacity(m_FadeTimerRedo);
	}
	//STUDIO
	if (m_Timer >= 3.0f && m_Timer < 4.0f)
	{
		m_FadeTimerRedo += _fElapsedTime;
		eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetOpacity(m_FadeTimerRedo);
	}
	if (m_Timer >= 5.0f && m_Timer < 6.0f)
	{
		m_FadeTimerRedo -= _fElapsedTime;
		eISElements[IS_BACKGROUND_STUDIO_ELEMENT].SetOpacity(m_FadeTimerRedo);
	}
	//TEAM
	if (m_Timer >= 6.0f && m_Timer < 7.0f)
	{
		m_FadeTimerRedo += _fElapsedTime;
		eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetOpacity(m_FadeTimerRedo);
	}
	if (m_Timer >= 8.0f && m_Timer < 9.0f)
	{
		m_FadeTimerRedo -= _fElapsedTime;
		eISElements[IS_BACKGROUND_TEAM_ELEMENT].SetOpacity(m_FadeTimerRedo);
	}

	if (m_Timer >= 9.0f)
	{
		m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
		return;
	}
}

bool CIntroSplashState::Input( CInput& _input )
{
	return true;
}

void CIntroSplashState::Render( void )
{
	m_Renderer->EnableDraw();
	m_Renderer->DrawTexture(&eISElements[IS_BLACK_BACKGROUND_ELEMENT]);
	//GP
	if (m_Timer < 1.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_GP_ELEMENT]);
	else if (m_Timer >= 1.0f && m_Timer < 2.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_GP_ELEMENT]);
	else if (m_Timer >= 2.0f && m_Timer < 3.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_GP_ELEMENT]);
	//STUDIO
	if (m_Timer >= 3.0f && m_Timer < 4.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_STUDIO_ELEMENT]);
	else if (m_Timer >= 4.0f && m_Timer < 5.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_STUDIO_ELEMENT]);
	else if (m_Timer >= 5.0f && m_Timer < 6.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_STUDIO_ELEMENT]);
	//TEAM
	if (m_Timer >= 6.0f && m_Timer < 7.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_TEAM_ELEMENT]);
	else if (m_Timer >= 7.0f && m_Timer < 8.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_TEAM_ELEMENT]);
	else if (m_Timer >= 8.0f && m_Timer < 9.0f)
		m_Renderer->DrawTexture(&eISElements[IS_BACKGROUND_TEAM_ELEMENT]);
	m_Renderer->DisableDraw();
}

void CIntroSplashState::Shutdown( void )
{
	Exit();
}

void CIntroSplashState::Exit( void )
{
	for( unsigned int i = 0; i < IS_NUMBER_ELEMENTS; ++i )
	{
		eISElements[i].ReleaseGLBatch();
	}
}