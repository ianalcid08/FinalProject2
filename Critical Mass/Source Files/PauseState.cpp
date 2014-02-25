#include "../Header Files/StdAfx.h"
#include "..\Header Files\PauseState.h"
/*****************************************************************
File name:			PauseState.cpp
Date:				7/29/13
Modify Date:		08/13/13
Modify Initials:	CM
Author:				Ian Alcid
Purpose:			Class for pause state.
******************************************************************/
#include "../Header Files/Input.h"
#include "../Header Files/Game.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/UIElement.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/Entity3D.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"

#include <string>
using namespace std;
CPauseState::CPauseState(void)
{
	this->SetClosing(false);
}

CPauseState::~CPauseState(void)
{
	Shutdown();
}

CPauseState* CPauseState::GetInstance()
{
	static CPauseState s_Instance;
	return &s_Instance;
}

void CPauseState::Init(CGame* _game)
{
	m_Game = _game;
	m_Renderer = _game->GetRenderer();
	//ShowCursor( TRUE );
	m_Choice = 0;
	m_AM = CAssetManager::GetInstance();
	m_bExit = true;
	m_bPlay = false;
	m_bOptions = true;
	m_Esc = false;
	//static bool ifPressEsc = true;
	//m_Timer = 0.15f;		//??
//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PAUSE_ALL);

	m_bFadeOut = false;
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;

	m_bSoundPlay = true;;
	m_bSoundExit = true;
	m_bSoundOptions = true;

	SetID( PAUSE_STATE );
}

void CPauseState::Shutdown()
{
	for( unsigned int i = 0; i < PAUSE_NUM_ELEMENTS; ++i )
	{
		m_PauseElements[i].ReleaseGLBatch();
	}
	m_reticle.ReleaseGLBatch();
}

void CPauseState::Enter()
{
	//ShowCursor( TRUE );
	m_Choice = 0;
	m_AM = CAssetManager::GetInstance();
	m_bExit = true;
	m_bPlay = false;
	m_bOptions = true;
	m_Esc = false;
	//static bool ifPressEsc = true;
	//m_Timer = 0.15f;		//??
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_01);		
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_02);		
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_04);	
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_03);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RETREAT);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RISING);

	this->m_bFadeOut = false;
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;

	m_bSoundPlay = true;;
	m_bSoundExit = true;
	m_bSoundOptions = true;

	//NON-HIGHLIGHTED BUTTONS ------------------------------------------------------------------------------------------------
#pragma region Non-Highlighted Buttons

	//m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].SetScreenHeight(m_Game->GetScreenHeight());
	//m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].SetScreenWidth(m_Game->GetScreenWidth());
	//m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].SetHeight((float)m_Game->GetScreenHeight());
	//m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].SetWidth((float)m_Game->GetScreenWidth());
	//m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].SetPosX(0.0f);				
	//m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].SetPosY(0.0f);
	//m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].BuildGeometry();
	//m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND));
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

	m_PauseElements[PAUSE_BACKGROUND_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_PAUSE_BG), 
		RECTANGLE(0.0f, 512.0f, 0.0f, 512.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 256 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) - 200 * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);


	m_PauseElements[PAUSE_RESUME_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_PLAY), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) - 64 * (m_Renderer->GetScreenHeight() / 720.0f), 
		0.0f, 1.0f, white);

	m_PauseElements[PAUSE_OPTIONS_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_OPTIONS), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 32 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_PauseElements[PAUSE_EXIT_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_QUIT), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 128.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);


#pragma endregion

	//HIGHLIGHTED BUTTONS ----------------------------------------------------------------------------------------------------
#pragma region Highlighted Buttons

	m_PauseElements[PAUSE_PLAY_HIGHLIGHTED_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_PLAY_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) - 64 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_PauseElements[PAUSE_OPTIONS_HIGHLIGHTED_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_OPTIONS_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 32 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_PauseElements[PAUSE_EXIT_HIGHLIGHTED_ELEMENTS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_QUIT_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 128 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 128.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

#pragma endregion

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);

	for(int i = 0; i < PAUSE_NUM_ELEMENTS; ++i)
		m_PauseElements[i].SetOpacity(0.0f);
}

void CPauseState::Exit()
{
	while( ShowCursor( FALSE ) >= 0 );
	
	for( unsigned int i = 0; i < PAUSE_NUM_ELEMENTS; ++i )
	{
		m_PauseElements[i].ReleaseGLBatch();
	}
	m_reticle.ReleaseGLBatch();

	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_01);		
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_02);		
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_03);	

	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_04);	
}

bool CPauseState::Input( CInput& _input )
{
	static bool ifPressedD = false;
	static bool ifPressedU = false;
	static bool ifPressedEnter_Paused = false;

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

	if (m_Timer > 0.5f)
	{
		static bool ifPressEsc = true;
		if(_input.IsEscapePressed() )
		{
			if(!ifPressEsc)
			{
				m_bFadeOut = true;
				m_Esc = true;
				m_Choice = P_PLAY;
				ifPressEsc = true;
///				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUUNPAUSE);
			}
		}
		else
			ifPressEsc = false;
	}

	if (m_bFadeOut && m_FadeTimer >= 1.0f )
	{
		switch(m_Choice)
		{
		case P_PLAY:
			{
//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUUNPAUSE);
				m_Game->GetStateManager()->PopState();
				return true;
			}
			break;
		case P_OPTIONS:
			{
//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				m_Game->GetStateManager()->PushState(OPTIONS_STATE);
				return true;
			}
			break;
		case P_EXIT:
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


	if((_input.KeyDown(DIK_S) || _input.KeyDown(DIK_DOWN) || _input.GetController().GetControllerState().Gamepad.sThumbLY < - 8000 ||
		_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  && !m_bFadeOut )
	{
		if(!ifPressedD)
		{
			m_Choice++;
			//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosX((m_Game->GetScreenWidth() / 2.0f) - 150);
			//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosY(m_Choice * 128.0f + 128.0f);
			//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].BuildGeometry();
			if(m_Choice == PAUSE_NUM_CHOICES)
			{
				m_Choice = P_PLAY;
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosY((m_Game->GetScreenHeight() / 2.0f) - 160);
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].BuildGeometry();
			}
			ifPressedD = true;
		}	
	}
	else
		ifPressedD = false;

	if((_input.KeyDown(DIK_W) || _input.KeyDown(DIK_UP) || abs(_input.GetController().GetControllerState().Gamepad.sThumbLY) > 8000 ||
		_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) && !m_bFadeOut)
	{
		if(!ifPressedU)
		{
			m_Choice--;
			//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosX((m_Game->GetScreenWidth() / 2.0f) - 150);
			//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosY(m_Choice * 128.0f + 128.0f);
			//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].BuildGeometry();
			if(m_Choice < 0)
			{
				m_Choice = P_EXIT;
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosY((m_Game->GetScreenHeight() / 2.0f) + 96.0f);
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].BuildGeometry();
			}
			ifPressedU = true;
		}	
	}
	else
		ifPressedU = false;

	if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
	{
		if(!ifPressedEnter_Paused)
		{	
			m_bFadeOut = true;
			ifPressedEnter_Paused = true;
		}
	}
	else
		ifPressedEnter_Paused = false;
	
	if (m_Choice == P_PLAY && !m_Esc)
	{
		if (m_bSoundPlay)
		{
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundPlay = false;
			m_bSoundExit = true;
			m_bSoundOptions = true;
		}
	}
	if (m_Choice == P_OPTIONS)
	{
		if (m_bSoundExit)
		{
///			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundPlay = true;
			m_bSoundExit = false;
			m_bSoundOptions = true;
		}
	}
	if (m_Choice == P_EXIT)
	{
		if (m_bSoundOptions)
		{
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundPlay = true;
			m_bSoundExit = true;
			m_bSoundOptions = false;
		}
	}

	if (m_Choice == P_PLAY && !m_Esc)
	{
		m_bPlay = false;
		m_bOptions = true;
		m_bExit = true;
	}
	if (m_Choice == P_OPTIONS)
	{
		m_bPlay = true;
		m_bOptions = false;
		m_bExit = true;

	}
	if (m_Choice == P_EXIT)
	{
		m_bPlay = true;
		m_bOptions = true;
		m_bExit = false;
	}

#pragma endregion

	//MOUSE INPUT FOR SELECTING BUTTONS-------------------------------------------------------------------------	
#pragma region Mouse Input for Selecting Buttons

	int _mouseX =			_input.GetMousePosX();
	int _mouseY =			_input.GetMousePosY();

	//PLAY BUTTON
	float _playWidth1 =			m_PauseElements[PAUSE_RESUME_ELEMENTS].GetRect().GetWidth();		//256
	float _playHeight1 =		m_PauseElements[PAUSE_RESUME_ELEMENTS].GetRect().GetHeight();		//64
	float _playPosX1 =			m_PauseElements[PAUSE_RESUME_ELEMENTS].GetPosX();		//280
	float _playPosY1 =			m_PauseElements[PAUSE_RESUME_ELEMENTS].GetPosY();		//612
	float _playCombinedX1 =		_playPosX1 + _playWidth1;						//536
	float _playCombinedY1 =		_playPosY1 + _playHeight1;						//676

	if (_mouseX >= _playPosX1 && _mouseX <= _playCombinedX1 && _mouseY <= _playCombinedY1 && _mouseY >= _playPosY1)
	{
		if (!m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = P_PLAY;
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosY(_playPosY1);
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].BuildGeometry();
				//if (m_bSoundPlay)
				//{
				//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				//	m_bSoundPlay = false;
				//	m_bSoundExit = true;
				//	m_bSoundOptions = true;
				//}
			}

			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
					m_bFadeOut = true;
			}
		}
	}

	//OPTIONS BUTTON
	float _optionsWidth1 =			m_PauseElements[PAUSE_OPTIONS_ELEMENTS].GetRect().GetWidth();		//256
	float _optionsHeight1 =			m_PauseElements[PAUSE_OPTIONS_ELEMENTS].GetRect().GetHeight();	//64
	float _optionsPosX1 =			m_PauseElements[PAUSE_OPTIONS_ELEMENTS].GetPosX();		//280
	float _optionsPosY1 =			m_PauseElements[PAUSE_OPTIONS_ELEMENTS].GetPosY();		//612
	float _optionsCombinedX1 =		_optionsPosX1 + _optionsWidth1;							//536
	float _optionsCombinedY1 =		_optionsPosY1 + _optionsHeight1;						//676

	if (_mouseX >= _optionsPosX1 && _mouseX <= _optionsCombinedX1 && _mouseY <= _optionsCombinedY1 && _mouseY >= _optionsPosY1)
	{
		if(!m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = P_OPTIONS;
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosY(_optionsPosY1);
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].BuildGeometry();
				//if (m_bSoundOptions)
				//{
				//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				//	m_bSoundPlay = true;
				//	m_bSoundExit = true;
				//	m_bSoundOptions = false;
				//}
			}
			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
					m_bFadeOut = true;
			}
		}
	}

	//EXIT BUTTON
	float _exitWidth1 =			m_PauseElements[PAUSE_EXIT_ELEMENTS].GetRect().GetWidth();		//256
	float _exitHeight1 =		m_PauseElements[PAUSE_EXIT_ELEMENTS].GetRect().GetHeight();		//64
	float _exitPosX1 =			m_PauseElements[PAUSE_EXIT_ELEMENTS].GetPosX();		//280
	float _exitPosY1 =			m_PauseElements[PAUSE_EXIT_ELEMENTS].GetPosY();		//612
	float _exitCombinedX1 =		_exitPosX1 + _exitWidth1;						//536
	float _exitCombinedY1 =		_exitPosY1 + _exitHeight1;						//676

	if (_mouseX >= _exitPosX1 && _mouseX <= _exitCombinedX1 && _mouseY <= _exitCombinedY1 && _mouseY >= _exitPosY1)
	{
		if(!m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_bExit = false;
				m_Choice = P_EXIT;
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].SetPosY(_exitPosY1);
				//m_PauseElements[PAUSE_HIGHLIGHTED_ELEMENTS].BuildGeometry();
				//if (m_bSoundExit)
				//{
				//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				//	m_bSoundPlay = true;
				//	m_bSoundExit = false;
				//	m_bSoundOptions = true;
				//}
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

void CPauseState::FadeIn(float _fTime)
{
	m_FadeTimer -= _fTime;											//count up to 1
	for(int i = 0; i < PAUSE_NUM_ELEMENTS; ++i)
		m_PauseElements[i].SetOpacity(1.0f - m_FadeTimer);    	// set the Opacity to that timer	
	if (m_FadeTimer <= 0.0f)
		m_FadeTimer = 0.0f;
}

void CPauseState::FadeOut(float _fTime)
{
	m_FadeTimer += _fTime;															//count down to 0
	for(int i = 0; i < PAUSE_NUM_ELEMENTS; ++i)
		m_PauseElements[i].SetOpacity(1.0f - m_FadeTimer);     	// set the Opacity to that timer
		
}

void CPauseState::Update(float _fElapsedTime)
{
	this->m_Timer += _fElapsedTime;
	
	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
	{
		FadeIn(_fElapsedTime);
	}
	if (m_bFadeOut)
	{
		FadeOut(_fElapsedTime);
		m_Esc = true;
	}

}

void CPauseState::Render()
{
	m_Renderer->EnableDraw();
	m_Renderer->DrawTexture(&m_PauseElements[PAUSE_BACKGROUND_ELEMENTS]);

	if (m_bPlay)
		m_Renderer->DrawTexture(&m_PauseElements[PAUSE_RESUME_ELEMENTS]);
	if (!m_bPlay)
		m_Renderer->DrawTexture(&m_PauseElements[PAUSE_PLAY_HIGHLIGHTED_ELEMENTS]);
	if(m_bOptions)
		m_Renderer->DrawTexture(&m_PauseElements[PAUSE_OPTIONS_ELEMENTS]);
	if(!m_bOptions)
		m_Renderer->DrawTexture(&m_PauseElements[PAUSE_OPTIONS_HIGHLIGHTED_ELEMENTS]);
	if(m_bExit)
		m_Renderer->DrawTexture(&m_PauseElements[PAUSE_EXIT_ELEMENTS]);
	if(!m_bExit)
		m_Renderer->DrawTexture(&m_PauseElements[PAUSE_EXIT_HIGHLIGHTED_ELEMENTS]);

	/*if(!m_bFadeOut && m_FadeTimer <= 1.0f)
		m_Renderer->DrawTexture(&m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT]);

	if (m_bFadeOut)
		m_Renderer->DrawTexture(&m_PauseElements[PAUSE_BLACK_BACKGROUND_ELEMENT]);*/

	//DRAW Reticle

	if (!m_bFadeOut)
		m_Renderer->DrawTexture( &m_reticle );

	m_Renderer->DisableDraw();
}

void CPauseState::ProcessStates(ePauseChoices _choice)
{

}
