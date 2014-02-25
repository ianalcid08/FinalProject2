#include "../Header Files/StdAfx.h"
#include "TutorialPause.h"
/*****************************************************************
File name:			TutorialPause.cpp
Date:				09/06/13
Modify Date:		08/06/13
Modify Initials:	CM
Author:				Corey Morehead
Purpose:			This Class will be the pause menu for the Tutorial
******************************************************************/
#include "../Header Files/Input.h"
#include "../Header Files/Game.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Entity3D.h"
//.........................................................
#include <string>
using namespace std;

CTutorialPause::CTutorialPause(void)
{
	this->SetClosing(false);
}

CTutorialPause::~CTutorialPause(void)
{
	Shutdown();
}

CTutorialPause* CTutorialPause::GetInstance()
{
	static CTutorialPause s_Instance;
	return &s_Instance;
}

void CTutorialPause::Init(CGame* _game)
{
	m_Game = _game;
	m_Renderer = _game->GetRenderer();
	m_Choice = 0;
	m_bSoundResume = true;
	m_bSoundPlay = true;
	m_bSoundMainMenu = true;
	m_bResume = false;
	m_bPlay = true;
	m_bMainMenu = true;
	m_bFadeOut = false;
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;
}

void CTutorialPause::Enter( void )
{
	//ShowCursor( TRUE );
	m_Choice = 0;
	m_bSoundResume = true;
	m_bSoundPlay = true;
	m_bSoundMainMenu = true;
	m_bResume = false;
	m_bPlay = true;
	m_bMainMenu = true;
	m_bFadeOut = false;
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;
	m_Esc = false;

	//BACKGROUNDs
	//m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetScreenHeight(m_Game->GetScreenHeight());
	//m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetScreenWidth(m_Game->GetScreenWidth());
	//m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetHeight((float)m_Game->GetScreenHeight());
	//m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetWidth((float)m_Game->GetScreenWidth());
	//m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetPosX(0.0f);				
	//m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetPosY(0.0f);
	//m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].BuildGeometry();
	//m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND));
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

	//m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].SetScreenHeight(m_Game->GetScreenHeight());
	//m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].SetScreenWidth(m_Game->GetScreenWidth());
	//m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].SetHeight(400);
	//m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].SetWidth(512);
	//m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].SetPosX((m_Game->GetScreenWidth() / 2.0f) - 256);			//280.0f
	//m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].SetPosY((m_Game->GetScreenHeight() / 2.0f) - 200);			//120.0f
	//m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].BuildGeometry();
	//m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(M_PAUSE_BG));
	m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_PAUSE_BG), 
		RECTANGLE(0.0f, 512.0f, 0.0f, 512.0f), (m_Game->GetScreenWidth() / 2.0f) - 256 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) - 200 * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);

	//NON-HIGHLIGHTED BUTTONS ------------------------------------------------------------------------------------------------
#pragma region Non-Highlighted Buttons

	m_TutorialPauseElements[TPAUSE_RESUME_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_RESUME), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f)-128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f)-64 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_TutorialPauseElements[TPAUSE_PLAY_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_START), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f)-128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 32 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_TutorialPauseElements[TPAUSE_MAINMENU_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MAINMENU), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f)-128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 128 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

#pragma endregion

	//HIGHLIGHTED BUTTONS ------------------------------------------------------------------------------------------------
#pragma region Highlighted Buttons

	m_TutorialPauseElements[TPAUSE_RESUME_HIGHLIGHTED_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_RESUME_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f)-128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f)-64 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_TutorialPauseElements[TPAUSE_PLAY_HIGHLIGHTED_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_START_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f)-128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 32 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_TutorialPauseElements[TPAUSE_MAINMENU_HIGHLIGHTED_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MAINMENU_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f)-128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 128 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

#pragma endregion

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);

	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RETREAT);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RISING);
}

bool CTutorialPause::Input( CInput& _input )
{
	static bool ifPressedD_TP = false;
	static bool ifPressedU_TP = false;
	static bool ifPressedEnter_TPaused = false;
	static bool ifPressEsc_TP = true;

	// Updates the reticle location based on the mouse pos.
	m_reticle.SetPosX(_input.GetMousePosX() - m_reticle.GetRect().GetWidth() * 0.5f);
	m_reticle.SetPosY(_input.GetMousePosY() - m_reticle.GetRect().GetHeight() * 0.5f);
	m_reticle.BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	//Mouse Active over keybard
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

	//ESC
	if (m_Timer > 0.5f)
	{
		if(_input.IsEscapePressed() )
		{
			if(!ifPressEsc_TP)
			{
				m_bFadeOut = true;
				m_Esc = true;
				m_Choice = TP_RESUME;
				ifPressEsc_TP = true;
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUUNPAUSE);
			}
		}
		else
			ifPressEsc_TP = false;
	}

	//CHOICES MADE
	if (m_bFadeOut && m_FadeTimer >= 1.0f )
	{
		switch(m_Choice)
		{
		case TP_RESUME:
			{
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUUNPAUSE);
				//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::pa);

				m_Game->GetStateManager()->PopState();
				return true;
			}
			break;
		case TP_PLAY:
			{
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				m_Game->GetStateManager()->ChangeState(GAMEPLAY_STATE);
				return true;
			}
			break;
		case TP_MAINMENU:
			{
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
				return true;
			}
			break;
		};	
	}
	//KEYBOARD INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Keyboard Input for Selecting Buttons

	//SCROLL THRU
	if((_input.KeyDown(DIK_S) || _input.KeyDown(DIK_DOWN) || _input.GetController().GetControllerState().Gamepad.sThumbLY < - 8000 ||
		_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  && !m_bFadeOut )
	{
		if(!ifPressedD_TP)
		{
			m_Choice++;
			if(m_Choice == TP_NUM_CHOICES)
			{
				m_Choice = TP_RESUME;
			}
			ifPressedD_TP = true;
		}	
	}
	else
		ifPressedD_TP = false;

	if((_input.KeyDown(DIK_W) || _input.KeyDown(DIK_UP) || abs(_input.GetController().GetControllerState().Gamepad.sThumbLY) > 8000 ||
		_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) && !m_bFadeOut)
	{
		if(!ifPressedU_TP)
		{
			m_Choice--;
			if(m_Choice < 0)
			{
				m_Choice = TP_MAINMENU;
			}
			ifPressedU_TP = true;
		}	
	}
	else
		ifPressedU_TP = false;

	//SELECTED ITEM
	if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
	{
		if(!ifPressedEnter_TPaused)
		{	
			m_bFadeOut = true;
			ifPressedEnter_TPaused = true;
		}
	}
	else
		ifPressedEnter_TPaused = false;

	//SINGLE SOUND FOR HOVER
	if (m_Choice == TP_RESUME && !m_Esc)
	{
		if (m_bSoundResume)
		{
			///			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundResume = false;
			m_bSoundPlay = true;
			m_bSoundMainMenu = true;
		}
	}
	if (m_Choice == TP_PLAY)
	{
		if (m_bSoundPlay)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundResume = true;
			m_bSoundPlay = false;
			m_bSoundMainMenu = true;
		}
	}
	if (m_Choice == TP_MAINMENU)
	{
		if (m_bSoundMainMenu)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundResume = true;
			m_bSoundPlay = true;
			m_bSoundMainMenu = false;
		}
	}

	//DRAWN HIGHLIGHTED OR NOT
	if (m_Choice == TP_RESUME && !m_Esc)
	{
		m_bResume = false;
		m_bPlay = true;
		m_bMainMenu = true;
	}
	if (m_Choice == TP_PLAY)
	{
		m_bResume = true;
		m_bPlay = false;
		m_bMainMenu = true;

	}
	if (m_Choice == TP_MAINMENU)
	{
		m_bResume = true;
		m_bPlay = true;
		m_bMainMenu = false;
	}

#pragma endregion

	//MOUSE INPUT FOR SELECTING BUTTONS-------------------------------------------------------------------------	
#pragma region Mouse Input for Selecting Buttons

	int _mouseX =			_input.GetMousePosX();
	int _mouseY =			_input.GetMousePosY();

	//RESUME BUTTON
	float _resumeWidth1 =			m_TutorialPauseElements[TPAUSE_RESUME_ELEMENTS].GetRect().GetWidth();		//256
	float _resumeHeight1 =			m_TutorialPauseElements[TPAUSE_RESUME_ELEMENTS].GetRect().GetHeight();		//64
	float _resumePosX1 =			m_TutorialPauseElements[TPAUSE_RESUME_ELEMENTS].GetPosX();		//280
	float _resumePosY1 =			m_TutorialPauseElements[TPAUSE_RESUME_ELEMENTS].GetPosY();		//612
	float _resumeCombinedX1 =		_resumePosX1 + _resumeWidth1;						//536
	float _resumeCombinedY1 =		_resumePosY1 + _resumeHeight1;						//676

	if (_mouseX >= _resumePosX1 && _mouseX <= _resumeCombinedX1 && _mouseY <= _resumeCombinedY1 && _mouseY >= _resumePosY1)
	{
		if (!m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = TP_RESUME;
			}

			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
					m_bFadeOut = true;
			}
		}
	}

	//PLAY BUTTON
	float _playWidth1 =			m_TutorialPauseElements[TPAUSE_PLAY_ELEMENTS].GetRect().GetWidth();		//256
	float _playHeight1 =		m_TutorialPauseElements[TPAUSE_PLAY_ELEMENTS].GetRect().GetHeight();		//64
	float _playPosX1 =			m_TutorialPauseElements[TPAUSE_PLAY_ELEMENTS].GetPosX();		//280
	float _playPosY1 =			m_TutorialPauseElements[TPAUSE_PLAY_ELEMENTS].GetPosY();		//612
	float _playCombinedX1 =		_playPosX1 + _playWidth1;						//536
	float _playCombinedY1 =		_playPosY1 + _playHeight1;						//676

	if (_mouseX >= _playPosX1 && _mouseX <= _playCombinedX1 && _mouseY <= _playCombinedY1 && _mouseY >= _playPosY1)
	{
		if (!m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = TP_PLAY;
			}

			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
					m_bFadeOut = true;
			}
		}
	}

	//MAINMNEU BUTTON
	float _mainWidth1 =			m_TutorialPauseElements[TPAUSE_MAINMENU_ELEMENTS].GetRect().GetWidth();		//256
	float _mainHeight1 =		m_TutorialPauseElements[TPAUSE_MAINMENU_ELEMENTS].GetRect().GetHeight();		//64
	float _mainPosX1 =			m_TutorialPauseElements[TPAUSE_MAINMENU_ELEMENTS].GetPosX();		//280
	float _mainPosY1 =			m_TutorialPauseElements[TPAUSE_MAINMENU_ELEMENTS].GetPosY();		//612
	float _mainCombinedX1 =		_mainPosX1 + _mainWidth1;						//536
	float _mainCombinedY1 =		_mainPosY1 + _mainHeight1;						//676

	if (_mouseX >= _mainPosX1 && _mouseX <= _mainCombinedX1 && _mouseY <= _mainCombinedY1 && _mouseY >= _mainPosY1)
	{
		if (!m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = TP_MAINMENU;
			}

			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
					m_bFadeOut = true;
			}
		}
	}

#pragma endregion

	return true;
}

void CTutorialPause::Update(float _fElapsedTime)
{
	m_Timer += _fElapsedTime;

	//GTFO
	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
	{
		FadeIn(_fElapsedTime);
	}
	if(m_bFadeOut)
	{
		FadeOut(_fElapsedTime);
	}
}

void CTutorialPause::Render( void )
{
	m_Renderer->EnableDraw();		
	m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_BACKGROUND_ELEMENTS]);

	if (m_bResume)
		m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_RESUME_ELEMENTS]);
	if(!m_bResume)
		m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_RESUME_HIGHLIGHTED_ELEMENTS]);
	if (m_bPlay)
		m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_PLAY_ELEMENTS]);
	if(!m_bPlay)
		m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_PLAY_HIGHLIGHTED_ELEMENTS]);
	if (m_bMainMenu)
		m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_MAINMENU_ELEMENTS]);
	if(!m_bMainMenu)
		m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_MAINMENU_HIGHLIGHTED_ELEMENTS]);

	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
		m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT]);

	if(m_bFadeOut)
		m_Renderer->DrawTexture(&m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT]);
	//DRAW Reticle
	m_Renderer->DrawTexture( &m_reticle );
	m_Renderer->DisableDraw();
}

void CTutorialPause::Shutdown( void )
{

}

void CTutorialPause::Exit( void )
{
	while( ShowCursor( FALSE ) >= 0 );

	for( unsigned int i = 0; i < TPAUSE_NUM_ELEMENTS; ++i )
	{
		m_TutorialPauseElements[i].ReleaseGLBatch();
	}
	m_reticle.ReleaseGLBatch();
}

void CTutorialPause::FadeIn( float _fTime )
{
	m_FadeTimer -= _fTime;											//count up to 1
	m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetOpacity(m_FadeTimer);     	// set the Opacity to that timer	
	if (m_FadeTimer <= 0.0f)
		m_FadeTimer = 0.0f;
}

void CTutorialPause::FadeOut( float _fTime )
{
	m_FadeTimer += _fTime;															//count down to 0
	m_TutorialPauseElements[TPAUSE_BLACK_BACKGROUND_ELEMENT].SetOpacity(m_FadeTimer);     	// set the Opacity to that timer
}
