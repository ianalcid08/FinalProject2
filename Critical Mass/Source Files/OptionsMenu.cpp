#include "../Header Files/stdafx.h"
#include "../Header Files/OptionsMenu.h"

/*****************************************************************
File name:			OptionsMenu.cpp
Date:				7/17/13
Modify Date:		08/13/13
Modify Initials:	CM
Author:				Ian Alcid
Purpose:			Class for Options state.
******************************************************************/

#include "../Header Files/Input.h"
#include "../Header Files/Game.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../Header Files/PauseState.h"
#include <fstream>
#include <string>
using namespace std;

COptionsState* COptionsState::GetInstance( void )
{
	static COptionsState s_Instance;
	return &s_Instance;
}

COptionsState::COptionsState( void )
{
	this->SetClosing(false);
	m_Switch = false;
}

COptionsState::~COptionsState( void )
{
	Shutdown();
}

// IState Functions
void COptionsState::Init( CGame* _game )
{	
	//LoadOptions( "volume.txt" );
	this->m_Game = _game;
	this->m_Renderer = m_Game->GetRenderer();
	this->m_Choice = 0;
	this->m_AM = CAssetManager::GetInstance();
	m_MusicVolume = 3;
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);
	m_SFXVolume = 3;
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::SFX_VOLUME, (float)m_SFXVolume * 10.0f);
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::DX_VOLUME, (float)m_SFXVolume * 10.0f);

	m_bMouseActive = false;
	m_PrevMousePosX= 0;
	m_PrevMousePosY= 0;
	//m_bExit = true;
	m_bGamma = true;
	m_bMusic = false;
	m_bSFX = true;
	m_bMouse = true;
	m_bDefault = true;
	m_bResolution = true;
	m_bLeftArrow = true;
	m_bRightArrow = true;
	m_bCancel = true;
	m_bAccept = true;
	m_bGamepad = true;
	//m_bEnable = false;

	m_Timer = 0.0f;
	m_FadeTimer = 0.0f;
	m_bFadeOut = false;
	m_HDC = m_Game->GetHDC();

	SetID( OPTIONS_STATE );
}

bool COptionsState::Input( CInput& _input )
{	
	_input.ReadMouse();
	_input.ReadKeyboard();

	float _max, _min, _length, _unit;
	_min = m_OMElements[OM_BAR_ELEMENT1].GetPosX();

	static bool ifPressedD_Options = false;
	static bool ifPressedU_Options = false;
	static bool ifPressedEnter_Options = false;
	static bool ifLeftClickBack_Options = false;
	static bool	ifLeftClickDefault_Options = false;
	static bool ifLeftClickGamma_Options = false;
	static bool	ifLeftClickMusic_Options = false;
	static bool ifLeftClickSFX_Options = false;
	static bool ifLeftClickMouse_Options = false;
	static bool	ifLeftClickAccept_Options = false;
	static bool	ifLeftClickGamePad_Options = false;
	static bool	ifLeftClickEnable_Options = false;

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

	//Esc (EXIT THE PROGRAM)
	static bool ifPressedEsc_Cred = false;
	if(!ifPressedEsc_Cred)
	{
		ifPressedEsc_Cred = true;
		if(_input.KeyDown(DIK_ESCAPE))
		{
			m_bFadeOut = true;
			return true;
		}
	}
	else
		ifPressedEsc_Cred = false;

	//TODO: where you do the shit to move the bar around, just turn the bool back off for the buffering so as long as you are holding it down and are in the box or w.e. it will work

	//MOUSE INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Mouse Input for Selecting Buttons

	int _mouseX =			_input.GetMousePosX();
	int _mouseY =			_input.GetMousePosY();

	//ENABLE BUTTON
	float _enableWidth1 =			m_OMElements[OM_GAMEPAD_ELEMENT].GetRect().GetWidth();		//256
	float _enableHeight1 =			m_OMElements[OM_GAMEPAD_ELEMENT].GetRect().GetHeight();		//64
	float _enablePosX1 =			m_OMElements[OM_GAMEPAD_ELEMENT].GetPosX();		//280
	float _enablePosY1 =			m_OMElements[OM_GAMEPAD_ELEMENT].GetPosY();		//612
	float _enableCombinedX1 =		_enablePosX1 + _enableWidth1;						//536
	float _enableCombinedY1 =		_enablePosY1 + _enableHeight1;						//676

	if (_input.IsLeftClicked()) 
	{	
		if (!ifLeftClickEnable_Options)
		{
			ifLeftClickEnable_Options = true;
			if (_mouseX >= _enablePosX1 && _mouseX <= _enableCombinedX1 && _mouseY <= _enableCombinedY1 && _mouseY >= _enablePosY1)
			{
				if (m_Timer > 0.5f)
				{
					m_bEnable = !m_bEnable;
					m_Game->SetUsingController(m_bEnable);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}
	else
		ifLeftClickEnable_Options = false;

	//CANCEL BUTTON
	float _backWidth1 =			m_OMElements[OM_CANCEL_ELEMENT].GetRect().GetWidth();		//256
	float _backHeight1 =		m_OMElements[OM_CANCEL_ELEMENT].GetRect().GetHeight();		//64
	float _backPosX1 =			m_OMElements[OM_CANCEL_ELEMENT].GetPosX();		//280
	float _backPosY1 =			m_OMElements[OM_CANCEL_ELEMENT].GetPosY();		//612
	float _backCombinedX1 =		_backPosX1 + _backWidth1;						//536
	float _bacKCombinedY1 =		_backPosY1 + _backHeight1;						//676

	if (_input.IsLeftClicked()) 
	{	
		if (!ifLeftClickBack_Options)
		{
			ifLeftClickBack_Options = true;
			if (_mouseX >= _backPosX1 && _mouseX <= _backCombinedX1 && _mouseY <= _bacKCombinedY1 && _mouseY >= _backPosY1)
			{
				if (m_Timer > 0.5f)
				{
					m_bFadeOut = true;
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}
	else
		ifLeftClickBack_Options = false;

	if (_mouseX >= _backPosX1 && _mouseX <= _backCombinedX1 && _mouseY <= _bacKCombinedY1 && _mouseY >= _backPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
		{
			if (m_bMouseActive)
			{
				m_Choice = OM_CANCEL;
				if (m_bSoundCancel)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

				}
			}

		}
	}
	//GAMEPAD BUTTON
	float _gamepadWidth1 =			m_OMElements[OM_GAMEPAD_ELEMENT].GetRect().GetWidth();		//256
	float _gamepadHeight1 =			m_OMElements[OM_GAMEPAD_ELEMENT].GetRect().GetHeight();		//64
	float _gamepadPosX1 =			m_OMElements[OM_GAMEPAD_ELEMENT].GetPosX();		//280
	float _gamepadPosY1 =			m_OMElements[OM_GAMEPAD_ELEMENT].GetPosY() - 5.0f;		//612
	float _gamepadCombinedX1 =		_gamepadPosX1 + _gamepadWidth1;						//536
	float _gamepadCombinedY1 =		_gamepadPosY1 + _gamepadHeight1;						//676

	if (_input.IsLeftClicked()) 
	{	
		if (!ifLeftClickGamePad_Options)
		{
			ifLeftClickGamePad_Options = true;
			if (_mouseX >= _gamepadPosX1 && _mouseX <= _gamepadCombinedX1 && _mouseY <= _gamepadCombinedY1 && _mouseY >= _gamepadPosY1)
			{
				if (m_Timer > 0.5f)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}
	else
		ifLeftClickGamePad_Options = false;

	if (_mouseX >= _gamepadPosX1 && _mouseX <= _gamepadCombinedX1 && _mouseY <= _gamepadCombinedY1 && _mouseY >= _gamepadPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
		{
			if (m_bMouseActive)
			{
				m_Choice = OM_GAMEPAD;
				if (m_bSoundGamePad)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

				}
			}

		}
	}
	//ACCEPT BUTTON
	float _acceptWidth1 =			m_OMElements[OM_ACCEPT_ELEMENT].GetRect().GetWidth();		//256
	float _acceptHeight1 =			m_OMElements[OM_ACCEPT_ELEMENT].GetRect().GetHeight();		//64
	float _acceptPosX1 =			m_OMElements[OM_ACCEPT_ELEMENT].GetPosX();		//280
	float _acceptPosY1 =			m_OMElements[OM_ACCEPT_ELEMENT].GetPosY();		//612
	float _acceptCombinedX1 =		_acceptPosX1 + _acceptWidth1;						//536
	float _acceptCombinedY1 =		_acceptPosY1 + _acceptHeight1;						//676

	if (_input.IsLeftClicked()) 
	{	
		if (!ifLeftClickAccept_Options)
		{
			ifLeftClickAccept_Options = true;
			if (_mouseX >= _acceptPosX1 && _mouseX <= _acceptCombinedX1 && _mouseY <= _acceptCombinedY1 && _mouseY >= _acceptPosY1)
			{
				if ( m_Timer > 0.5f )
				{
					m_bFadeOut = true;
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

					if( m_Renderer->GetFullScreen() )
					{
						POINT xy = m_Renderer->GetAvailableResolutions()[m_nResolutionIndex];
						m_Game->ChangeResolution( xy.x, xy.y );
					}
				}
			}
		}
	}
	else
		ifLeftClickAccept_Options = false;

	if (_mouseX >= _acceptPosX1 && _mouseX <= _acceptCombinedX1 && _mouseY <= _acceptCombinedY1 && _mouseY >= _acceptPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
		{
			if (m_bMouseActive)
			{
				m_Choice = OM_ACCEPT;
				if (m_bSoundAccept)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

				}
			}

		}
	}
	//DEFAULT BUTTON
	float _defaultWidth1 =			m_OMElements[OM_DEFAULT_ELEMENT].GetRect().GetWidth();		
	float _defaultHeight1 =			m_OMElements[OM_DEFAULT_ELEMENT].GetRect().GetHeight();		
	float _defaultPosX1 =			m_OMElements[OM_DEFAULT_ELEMENT].GetPosX();		
	float _defaultPosY1 =			m_OMElements[OM_DEFAULT_ELEMENT].GetPosY();		
	float _defaultCombinedX1 =		_defaultPosX1 + _defaultWidth1;						
	float _defaultCombinedY1 =		_defaultPosY1 + _defaultHeight1;		

	if (_input.IsLeftClicked())
	{	
		if (!ifLeftClickDefault_Options)
		{
			ifLeftClickDefault_Options = true;
			if (_mouseX >= _defaultPosX1 && _mouseX <= _defaultCombinedX1 && _mouseY <= _defaultCombinedY1 && _mouseY >= _defaultPosY1)
			{
				if (m_Timer > 0.5f)
				{
					if (m_bSoundClick)
					{
						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

						m_bSoundClick = false;
					}
					m_bEnable = false;
					ResetDefault(m_Game);
				}
			}
		}
	}
	else
		ifLeftClickDefault_Options = false;

	if (_mouseX >= _defaultPosX1 && _mouseX <= _defaultCombinedX1 && _mouseY <= _defaultCombinedY1 && _mouseY >= _defaultPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
		{
			if (m_bMouseActive)
			{
				m_bDefault = false;
				m_Choice = OM_DEFAULT;
			}
		}
	}

	//GAMMA BUTTON
	float _gammaWidth1 =		m_OMElements[OM_GAMMA_HIGHLIGHTED_ELEMENT].GetRect().GetWidth();		//256
	float _gammaHeight1 =		m_OMElements[OM_GAMMA_HIGHLIGHTED_ELEMENT].GetRect().GetHeight();		//64
	float _gammaPosX1 =			m_OMElements[OM_GAMMA_HIGHLIGHTED_ELEMENT].GetPosX();		//280
	float _gammaPosY1 =			m_OMElements[OM_GAMMA_HIGHLIGHTED_ELEMENT].GetPosY();		//612
	float _gammaCombinedX1 =	_gammaPosX1 + _gammaWidth1 + m_OMElements[OM_BAR_ELEMENT3].GetRect().GetWidth();									//536
	float _gammaCombinedY1 =	_gammaPosY1 + _gammaHeight1;								//676

	if (_mouseX >= _gammaPosX1 && _mouseX <= _gammaCombinedX1 && _mouseY <= _gammaCombinedY1 && _mouseY >= _gammaPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				ifLeftClickGamma_Options = true;
				m_Choice = OM_GAMMA;
			}
		}
	}

	//MUSIC BUTTON
	float _musicWidth1 =		m_OMElements[OM_MUSIC_HIGHLIGHTED_ELEMENT].GetRect().GetWidth();		//256
	float _musicHeight1 =		m_OMElements[OM_MUSIC_HIGHLIGHTED_ELEMENT].GetRect().GetHeight();		//64
	float _musicPosX1 =			m_OMElements[OM_MUSIC_HIGHLIGHTED_ELEMENT].GetPosX();		//280
	float _musicPosY1 =			m_OMElements[OM_MUSIC_HIGHLIGHTED_ELEMENT].GetPosY();		//612
	float _musicCombinedX1 =	_musicPosX1 + _musicWidth1 + m_OMElements[OM_BAR_ELEMENT1].GetRect().GetWidth();	//the whole bar area
	float _musicCombinedY1 =	_musicPosY1 + _musicHeight1;								//676

	if (_mouseX >= _musicPosX1 && _mouseX <= _musicCombinedX1 && _mouseY <= _musicCombinedY1 && _mouseY >= _musicPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_bMusic = false;
				m_Choice = OM_MUSIC;
			}
		}
	}
	//SFX BUTTON
	float _SFXWidth1 =		m_OMElements[OM_SFX_HIGHLIGHTED_ELEMENT].GetRect().GetWidth();		//256
	float _SFXHeight1 =		m_OMElements[OM_SFX_HIGHLIGHTED_ELEMENT].GetRect().GetHeight();		//64
	float _SFXPosX1 =		m_OMElements[OM_SFX_HIGHLIGHTED_ELEMENT].GetPosX();		//280
	float _SFXPosY1 =		m_OMElements[OM_SFX_HIGHLIGHTED_ELEMENT].GetPosY();		//612
	float _SFXCombinedX1 =	_SFXPosX1 + _SFXWidth1 + m_OMElements[OM_BAR_ELEMENT2].GetRect().GetWidth();	//alot
	float _SFXCombinedY1 =	_SFXPosY1 + _SFXHeight1;								//676

	if (_mouseX >= _SFXPosX1 && _mouseX <= _SFXCombinedX1 && _mouseY <= _SFXCombinedY1 && _mouseY >= _SFXPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_bSFX = false;
				m_Choice = OM_SFX;
			}
		}
	}
	//RESOLUTION BUTTON
	float _ResolutionWidth1 =		m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetRect().GetWidth();		//256
	float _ResolutionHeight1 =		m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetRect().GetHeight();		//64
	float _ResolutionPosX1 =		m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetPosX();		//280
	float _ResolutionPosY1 =		m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetPosY();		//612
	float _ResolutionCombinedX1 =	_ResolutionPosX1 + _ResolutionWidth1;// + m_OMElements[OM_BAR_ELEMENT2].GetRect().GetWidth();	//alot
	float _ResolutionCombinedY1 =	_ResolutionPosY1 + _ResolutionHeight1;								//676

	if (_mouseX >= _ResolutionPosX1 && _mouseX <= _ResolutionCombinedX1 && _mouseY <= _ResolutionCombinedY1 && _mouseY >= _ResolutionPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_bResolution = false;
				m_Choice = OM_RESOLUTION;
			}
		}
	}

	if( m_Renderer->GetFullScreen() )
	{
		//RESOLUTION LEFT ARROW
		float _LeftArrowWidth1 =		m_OMElements[OM_LEFTARROW_ELEMENT].GetRect().GetWidth();		
		float _LeftArrowHeight1 =		m_OMElements[OM_LEFTARROW_ELEMENT].GetRect().GetHeight();	
		float _LeftArrowPosX1 =			m_OMElements[OM_LEFTARROW_ELEMENT].GetPosX();		
		float _LeftArrowPosY1 =			m_OMElements[OM_LEFTARROW_ELEMENT].GetPosY();		
		float _LeftArrowCombinedX1 =	_LeftArrowPosX1 + _LeftArrowWidth1;
		float _LeftArrowCombinedY1 =	_LeftArrowPosY1 + _LeftArrowHeight1;								

		if (_mouseX >= _LeftArrowPosX1 && _mouseX <= _LeftArrowCombinedX1 && _mouseY <= _LeftArrowCombinedY1 && _mouseY >= _LeftArrowPosY1)
		{
			if (m_FadeTimer <= 1.0f && !m_bFadeOut)
			{
				if (m_bMouseActive)
				{
					m_bLeftArrow = false;
					m_Choice = OM_LEFTARROW;
				}
			}

			if (_input.IsLeftClicked())
			{
				if (m_Timer > 0.5f && m_ResolutionChangeTimer < 0.0f)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
					--m_nResolutionIndex;
					if( m_nResolutionIndex < 0 )
						m_nResolutionIndex = m_Renderer->GetAvailableResolutions().size() - 1;

					m_ResolutionChangeTimer = 0.25f;
				}
			}
		}

		//RESOLUTION RIGHT ARROW
		float _RightArrowWidth1 =		m_OMElements[OM_RIGHTARROW_ELEMENT].GetRect().GetWidth();		
		float _RightArrowHeight1 =		m_OMElements[OM_RIGHTARROW_ELEMENT].GetRect().GetHeight();		
		float _RightArrowPosX1 =		m_OMElements[OM_RIGHTARROW_ELEMENT].GetPosX();		
		float _RightArrowPosY1 =		m_OMElements[OM_RIGHTARROW_ELEMENT].GetPosY();		
		float _RightArrowCombinedX1 =	_RightArrowPosX1 + _RightArrowWidth1;
		float _RightArrowCombinedY1 =	_RightArrowPosY1 + _RightArrowHeight1;		

		if (_mouseX >= _RightArrowPosX1 && _mouseX <= _RightArrowCombinedX1 && _mouseY <= _RightArrowCombinedY1 && _mouseY >= _RightArrowPosY1)
		{
			if (m_FadeTimer <= 1.0f && !m_bFadeOut)
			{
				if (m_bMouseActive)
				{
					m_bRightArrow = false;
					m_Choice = OM_RIGHTARROW;
				}
			}

			if (_input.IsLeftClicked())
			{
				if (m_Timer > 0.5f && m_ResolutionChangeTimer < 0.0f)
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
					++m_nResolutionIndex;
					if( m_nResolutionIndex >= (int)m_Renderer->GetAvailableResolutions().size() )
						m_nResolutionIndex = 0;

					m_ResolutionChangeTimer = 0.25f;
				}
			}
		}
	}

	//MOUSE SE BUTTON
	//float _MouseWidth1 =		m_OMElements[OM_MOUSE_HIGHLIGHTED_ELEMENT].GetRect().GetWidth();		//256
	//float _MouseHeight1 =		m_OMElements[OM_MOUSE_HIGHLIGHTED_ELEMENT].GetRect().GetHeight();		//64
	//float _MousePosX1 =			m_OMElements[OM_MOUSE_HIGHLIGHTED_ELEMENT].GetPosX();		//280
	//float _MousePosY1 =			m_OMElements[OM_MOUSE_HIGHLIGHTED_ELEMENT].GetPosY();		//612
	//float _MouseCombinedX1 =	_MousePosX1 + _MouseWidth1 + m_OMElements[OM_BAR_ELEMENT4].GetRect().GetWidth();
	//float _MouseCombinedY1 =	_MousePosY1 + _MouseHeight1;								//676

	//if (_mouseX >= _MousePosX1 && _mouseX <= _MouseCombinedX1 && _mouseY <= _MouseCombinedY1 && _mouseY >= _MousePosY1)
	//{
	//	if (m_FadeTimer <= 1.0f && !m_bFadeOut)
	//	{
	//		if (m_bMouseActive)
	//		{
	//			m_bMouse = false;
	//			m_Choice = OM_MOUSE;
	//		}
	//	}
	//}

	int CheckChoice = m_Choice;
	if (m_Choice == OM_CANCEL)
	{
		if (m_bSoundCancel)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = false;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = true;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}
	if (m_Choice == OM_GAMEPAD)
	{
		if (m_bSoundGamePad)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);
			
			m_bSoundGamePad = false;
			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = true;
			m_bSoundAccept = true;
		}
	}
	if (m_Choice == OM_ACCEPT)
	{
		if (m_bSoundAccept)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = true;
			m_bSoundAccept = false;
			m_bSoundGamePad = true;
		}
	}
	if (m_Choice == OM_MUSIC)
	{
		if (m_bSoundMusic)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = false;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = true;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}
	if (m_Choice == OM_GAMMA)
	{
		if (m_bSoundGamma)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = false;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = true;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}
	if (m_Choice == OM_SFX)
	{
		if (m_bSoundSFX)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = false;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = true;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}
	if (m_Choice == OM_DEFAULT)
	{
		if (m_bSoundDefault)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = false;
			m_bSoundClick = true;
			m_bSoundResolution = true;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}
	/*if (m_Choice == OM_MOUSE)
	{
		if (m_bSoundMouse)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = false;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = true;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}*/
	if (m_Choice == OM_RESOLUTION)
	{
		if (m_bSoundResolution)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = false;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}
	if (m_Choice == OM_LEFTARROW)
	{
		if (m_bSoundResolution)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = false;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}
	if (m_Choice == OM_RIGHTARROW)
	{
		if (m_bSoundResolution)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundCancel = true;
			m_bSoundMusic = true;
			m_bSoundSFX = true;
			m_bSoundMouse = true;
			m_bSoundGamma = true;
			m_bSoundDefault = true;
			m_bSoundClick = true;
			m_bSoundResolution = false;
			m_bSoundAccept = true;
			m_bSoundGamePad = true;
		}
	}

#pragma endregion

	//KEYBOARD INPUT ----------------------------------------------------------------------------------------
#pragma region Keyboard Input for Selecting Buttons

	if(_input.KeyDown(DIK_S) || _input.KeyDown(DIK_DOWNARROW) || _input.GetController().GetControllerState().Gamepad.sThumbLY < - 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN )
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedD_Options)
			{
				m_Choice++;

				if(!m_Game->GetRenderer()->GetFullScreen())
				{
					if( m_Choice == OM_RESOLUTION ) 
						m_Choice = OM_ACCEPT;
				}

				if(m_Choice == OM_NUM_CHOICES)
				{
					m_Choice = OM_MUSIC;
				}
				ifPressedD_Options = true;
			}
		}
	}
	else
		ifPressedD_Options = false;

	if(_input.KeyDown(DIK_W) || _input.KeyDown(DIK_UPARROW) ||_input.GetController().GetControllerState().Gamepad.sThumbLY > 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedU_Options)
			{
				m_Choice--;

				if(!m_Game->GetRenderer()->GetFullScreen())
				{	
					if( m_Choice == OM_RIGHTARROW ) 
						m_Choice = OM_GAMEPAD;
				}

				if(m_Choice < 0)
				{
					m_Choice = OM_CANCEL;
				}
				ifPressedU_Options = true;
			}
		}	
	}
	else
		ifPressedU_Options = false;

	//SELECTED (ENTER PRESSED)
	if (m_Timer > 0.5f)
	{
		if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
		{
			if (m_FadeTimer <= 1.0f && !m_bFadeOut)
			{
				if(!ifPressedEnter_Options)
				{	
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

					if (m_Choice == OM_CANCEL)
					{
						ResetDefault(m_Game);
						m_bFadeOut = true;
					}
					if (m_Choice == OM_DEFAULT)
						ResetDefault(m_Game);
					if (m_Choice == OM_ACCEPT)
					{
						m_bFadeOut = true;

						if( m_Renderer->GetFullScreen() )
						{
							POINT xy = m_Renderer->GetAvailableResolutions()[m_nResolutionIndex];
							m_Game->ChangeResolution( xy.x, xy.y );
						}
					}
					if (m_Choice == OM_GAMEPAD)
					{
						m_Game->SetUsingController(m_bEnable);
						m_bEnable = !m_bEnable;
					}
					if( m_Choice == OM_LEFTARROW )
					{
						--m_nResolutionIndex;
						if( m_nResolutionIndex < 0 )
							m_nResolutionIndex = m_Renderer->GetAvailableResolutions().size() - 1;
					}
					if( m_Choice == OM_RIGHTARROW )
					{
						++m_nResolutionIndex;
						if( m_nResolutionIndex >= (int)m_Renderer->GetAvailableResolutions().size() )
							m_nResolutionIndex = 0;
					}

					ifPressedEnter_Options = true;
				}
			}
		}
		else
			ifPressedEnter_Options = false;
	}

#pragma endregion

	//KEYBOARD INPUT LEFT / RIGHT ---------------------------------------------------------------------------
#pragma region Keyboard Input for Control Slider

	static bool pressed = false;
	if(_input.KeyDown(DIK_D) || _input.KeyDown(DIK_RIGHTARROW) ||_input.GetController().GetControllerState().Gamepad.sThumbLY > 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if( !pressed )
		{
			if (m_Choice == OM_MUSIC)
			{
				pressed = true;
				m_MusicVolume++;

				//set sound
				AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);
				//CSoundManager::GetInstance()->SetVolume(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);

				if (this->m_MusicVolume <= 0)
				{
					m_OMElements[OM_SLIDER_ELEMENT1].SetPosX(_min - 15 * (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT1].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
				if (this->m_MusicVolume > 9)
				{
					this->m_MusicVolume = 9;
					m_OMElements[OM_SLIDER_ELEMENT1].SetPosX(550* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT1].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
					AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);
					//CSoundManager::GetInstance()->SetVolume(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);
				}
			}
			if (m_Choice == OM_SFX)
			{
				m_SFXVolume++;
				pressed = true;

				//set sound
				AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::SFX_VOLUME, (float)m_SFXVolume * 10.0f);
				AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::DX_VOLUME, (float)m_SFXVolume * 10.0f);			

				if (this->m_SFXVolume <= 0)
				{
					m_OMElements[OM_SLIDER_ELEMENT2].SetPosX(_min - 15* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT2].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
				if (this->m_SFXVolume > 9)
				{
					this->m_SFXVolume = 9;
					m_OMElements[OM_SLIDER_ELEMENT2].SetPosX(550* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT2].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
			}
			if (m_Choice == OM_GAMMA)
			{
				m_Gamma++;
				pressed = true;

				//set sound
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUBACKUP);				

				if (this->m_Gamma <= 0)
				{
					m_OMElements[OM_SLIDER_ELEMENT3].SetPosX(_min - 15* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT3].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
				if (this->m_Gamma > 9)
				{
					this->m_Gamma = 9;
					m_OMElements[OM_SLIDER_ELEMENT3].SetPosX(550* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT3].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
			}
			//if (m_Choice == OM_MOUSE)
			//{
			//	m_MouseSen++;
			//	pressed = true;

			//	//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUBACKUP);				

			//	if (this->m_MouseSen <= 0)
			//	{
			//		m_OMElements[OM_SLIDER_ELEMENT4].SetPosX(_min - 15* (m_Renderer->GetScreenHeight() / 720.0f));
			//		m_OMElements[OM_SLIDER_ELEMENT4].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
			//	}
			//	if (this->m_MouseSen > 9)
			//	{
			//		this->m_MouseSen = 9;
			//		m_OMElements[OM_SLIDER_ELEMENT4].SetPosX(550* (m_Renderer->GetScreenHeight() / 720.0f));
			//		m_OMElements[OM_SLIDER_ELEMENT4].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
			//	}
			//}
		}
	}
	else
		pressed = false;

	static bool pressed1 = false;
	if(_input.KeyDown(DIK_A) || _input.KeyDown(DIK_LEFTARROW) ||_input.GetController().GetControllerState().Gamepad.sThumbLY > 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		if( !pressed1 )
		{
			if (m_Choice == OM_MUSIC)
			{
				pressed1 = true;
				m_MusicVolume--;

				//set sound
				AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);				

				if (this->m_MusicVolume <= 0)
				{
					this->m_MusicVolume = 0;
					m_OMElements[OM_SLIDER_ELEMENT1].SetPosX(_min - 15* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT1].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
				if (this->m_MusicVolume > 9)
				{
					this->m_MusicVolume = 9;
					m_OMElements[OM_SLIDER_ELEMENT1].SetPosX(550* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT1].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
					AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);				
				}
			}
			if (m_Choice == OM_SFX)
			{
				pressed1 = true;
				m_SFXVolume--;

				//set sound
				AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::SFX_VOLUME, (float)m_SFXVolume * 10.0f);	
				AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::DX_VOLUME, (float)m_SFXVolume * 10.0f);
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUBACKUP);				

				if (this->m_SFXVolume <= 0)
				{
					this->m_SFXVolume = 0;
					m_OMElements[OM_SLIDER_ELEMENT2].SetPosX(_min - 15* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT2].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
				if (this->m_SFXVolume > 9)
				{
					this->m_SFXVolume = 9;
					m_OMElements[OM_SLIDER_ELEMENT2].SetPosX(550* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT2].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
			}
			if (m_Choice == OM_GAMMA)
			{
				pressed1 = true;
				m_Gamma--;

				//set sound
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUBACKUP);				

				if (this->m_Gamma <= 0)
				{
					this->m_Gamma = 0;
					m_OMElements[OM_SLIDER_ELEMENT3].SetPosX(_min - 15* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT3].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
				if (this->m_Gamma > 9)
				{
					this->m_Gamma = 9;
					m_OMElements[OM_SLIDER_ELEMENT3].SetPosX(550* (m_Renderer->GetScreenHeight() / 720.0f));
					m_OMElements[OM_SLIDER_ELEMENT3].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
				}
			}
			//if (m_Choice == OM_MOUSE)
			//{
			//	pressed1 = true;
			//	m_MouseSen--;

			//	//set sound
			//	///				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUBACKUP);				

			//	if (this->m_MouseSen <= 0)
			//	{
			//		this->m_MouseSen = 0;
			//		m_OMElements[OM_SLIDER_ELEMENT4].SetPosX(_min - 15* (m_Renderer->GetScreenHeight() / 720.0f));
			//		m_OMElements[OM_SLIDER_ELEMENT4].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
			//	}
			//	if (this->m_MouseSen > 9)
			//	{
			//		this->m_MouseSen = 9;
			//		m_OMElements[OM_SLIDER_ELEMENT4].SetPosX(550* (m_Renderer->GetScreenHeight() / 720.0f));
			//		m_OMElements[OM_SLIDER_ELEMENT4].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
			//	}
			//}
		}
	}
	else
		pressed1 = false;

#pragma endregion

	if (m_Choice == OM_SFX)
	{
		m_bSFX = false;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}
	if (m_Choice == OM_GAMEPAD)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = false;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}
	if (m_Choice == OM_ACCEPT)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = false;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}
	if (m_Choice == OM_MUSIC)
	{
		m_bSFX = true;
		m_bMusic = false;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}
	if (m_Choice == OM_GAMMA)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = false;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}
	/*if (m_Choice == OM_MOUSE)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = false;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}*/
	if (m_Choice == OM_CANCEL)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = false;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}
	if (m_Choice == OM_DEFAULT)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = false;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}
	if (m_Choice == OM_RESOLUTION)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = false;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = true;
	}
	if (m_Choice == OM_LEFTARROW)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = false;
		m_bRightArrow = true;
	}
	if (m_Choice == OM_RIGHTARROW)
	{
		m_bSFX = true;
		m_bMusic = true;
		m_bGamma = true;
		m_bMouse = true;
		m_bCancel = true;
		m_bDefault = true;
		m_bResolution = true;
		m_bAccept = true;
		m_bGamepad = true;
		m_bLeftArrow = true;
		m_bRightArrow = false;
	}

	//BUTTONS ----------------------------------------------------------------------------------------------
#pragma region Mouse Input Sliders

	//MOUSE INPUT SLIDER MUSIC-------------------------------------------------------------------------------------------------------------------------------------------------------
	float _sliderWidth1 =	m_OMElements[OM_SLIDER_ELEMENT1].GetRect().GetWidth();	//44
	float _sliderHeight1 =	m_OMElements[OM_SLIDER_ELEMENT1].GetRect().GetHeight();	//44
	float _barWidth1 =		m_OMElements[OM_BAR_ELEMENT1].GetRect().GetWidth();		//408
	float _barHeight1 =		m_OMElements[OM_BAR_ELEMENT1].GetRect().GetHeight();		//44
	float _barPosX1 =		m_OMElements[OM_BAR_ELEMENT1].GetPosX();		//550
	float _barPosY1 =		m_OMElements[OM_BAR_ELEMENT1].GetPosY();		//108
	float _units1 =			_barWidth1 / 10.0f;								//40.799999
	float _barCombinedX1 =	_barPosX1 + _barWidth1;							//958
	float _barCombinedY1 =	_barPosY1 + _barHeight1;						//152



	if (_input.IsLeftClicked() && _mouseX > _barPosX1 && _mouseX < _barCombinedX1 && _mouseY > _barPosY1 && _mouseY < _barCombinedY1 && !ifLeftClickSFX_Options && !ifLeftClickGamma_Options )// && !ifLeftClickSFX_Options && !ifLeftClickGamma_Options)		
	{
		//if (!ifLeftClickMusic_Options)
		//{
		ifLeftClickMusic_Options = true;
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)																				//BUFFER TO MAKE SURE IT DOESNT JUMP INTO THIS FROM ANOTHER MENU
		{
			//if (_mouseX >= _barPosX1 && _mouseX <= _barCombinedX1 && _mouseY <= _barCombinedY1 && _mouseY >= _barPosY1)		//INSIDE THE BUTTON LOCATION
			//{
			for (int i = 0; i < 10; i++)																				//THE 10 SLOTS
			{
				if (_mouseX >= _barPosX1 + _units1 * i)																	//Pos Mouse + the Unit (length on the bar) * the number (1 - 10)			
				{
					testCounter++;
					m_OMElements[OM_SLIDER_ELEMENT1].SetPosX((float)_input.GetMousePosX());								//Draw the slider on screen					
					m_OMElements[OM_SLIDER_ELEMENT1].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());	
					this->m_MusicVolume = i;																			//MusicVol = the number (1-10)
					AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);	
					if(!playThisFrickinSoundOnce)
					{
						//								AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUBACKUP);	
						playThisFrickinSoundOnce = true;
					}
				}
			}
			//}
		}
		//}
	}
	else if (!_input.IsLeftClicked())
	{
		ifLeftClickMusic_Options = false;
		playThisFrickinSoundOnce = false;

	}

	//MOUSE INPUT SLIDER 2-------------------------------------------------------------------------------------------------------------------------------------------------------

	float _sliderWidth2 =	m_OMElements[OM_SLIDER_ELEMENT2].GetRect().GetWidth();	//44
	float _sliderHeight2 =	m_OMElements[OM_SLIDER_ELEMENT2].GetRect().GetHeight();	//44
	float _barWidth2 =		m_OMElements[OM_BAR_ELEMENT2].GetRect().GetWidth();		//408
	float _barHeight2 =		m_OMElements[OM_BAR_ELEMENT2].GetRect().GetHeight();		//44
	float _barPosX2 =		m_OMElements[OM_BAR_ELEMENT2].GetPosX();		//550
	float _barPosY2 =		m_OMElements[OM_BAR_ELEMENT2].GetPosY();		//108
	float _units2 =			_barWidth2 / 10.0f;								//40.799999
	float _barCombinedX2 =	_barPosX2 + _barWidth1;							//958
	float _barCombinedY2 =	_barPosY2 + _barHeight1;						//152

	if (_input.IsLeftClicked() && _mouseX > _barPosX2 && _mouseX < _barCombinedX2 && _mouseY > _barPosY2 && _mouseY < _barCombinedY2 && !ifLeftClickMusic_Options && !ifLeftClickGamma_Options)
	{
		//if (!ifLeftClickSFX_Options ) //&& !ifLeftClickMusic_Options && !ifLeftClickGamma_Options)
		//{
		ifLeftClickSFX_Options = true;
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (_mouseX >= _barPosX2 && _mouseX <= _barCombinedX2 && _mouseY <= _barCombinedY2 && _mouseY >= _barPosY2)
			{
				for (int i = 0; i < 10; i++)
				{
					if (_mouseX >= _barPosX2 + _units2 * i)
					{
						m_OMElements[OM_SLIDER_ELEMENT2].SetPosX((float)_input.GetMousePosX());
						m_OMElements[OM_SLIDER_ELEMENT2].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
						this->m_SFXVolume = i;
						AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::SFX_VOLUME, (float)m_SFXVolume * 10.0f);		
						AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::DX_VOLUME, (float)m_SFXVolume * 10.0f);		

						if(!playThisFrickinSoundOnce)
						{
							//							AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_KLAXON_LOOP);	
							playThisFrickinSoundOnce = true;
						}	
					}
				}
			}
		}
		//}
	}
	else if(!_input.IsLeftClicked())
	{
		ifLeftClickSFX_Options = false;
		playThisFrickinSoundOnce = false;
		//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_KLAXON_LOOP);	
	}

	//MOUSE INPUT SLIDER 3-------------------------------------------------------------------------------------------------------------------------------------------------------

	float _sliderWidth3 =	m_OMElements[OM_SLIDER_ELEMENT3].GetRect().GetWidth();	//44
	float _sliderHeight3 =	m_OMElements[OM_SLIDER_ELEMENT3].GetRect().GetHeight();	//44
	float _barWidth3 =		m_OMElements[OM_BAR_ELEMENT3].GetRect().GetWidth();		//408
	float _barHeight3 =		m_OMElements[OM_BAR_ELEMENT3].GetRect().GetHeight();		//44
	float _barPosX3 =		m_OMElements[OM_BAR_ELEMENT3].GetPosX();		//550
	float _barPosY3 =		m_OMElements[OM_BAR_ELEMENT3].GetPosY();		//108
	float _units3 =			_barWidth3 / 10.0f;								//40.799999
	float _barCombinedX3 =	_barPosX3 + _barWidth3;							//958
	float _barCombinedY3 =	_barPosY3 + _barHeight3;						//152

	if (_input.IsLeftClicked() && _mouseX >= _barPosX3 && _mouseX <= _barCombinedX3 && _mouseY <= _barCombinedY3 && _mouseY >= _barPosY3 && !ifLeftClickMusic_Options && !ifLeftClickSFX_Options )
	{
		//if (!ifLeftClickGamma_Options)
		//{
		ifLeftClickGamma_Options = true;
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (_mouseX >= _barPosX3 && _mouseX <= _barCombinedX3 && _mouseY <= _barCombinedY3 && _mouseY >= _barPosY3)
			{
				for (int i = 0; i < 10; i++)
				{
					if (_mouseX >= _barPosX3 + _units3 * i)
					{
						m_OMElements[OM_SLIDER_ELEMENT3].SetPosX((float)_input.GetMousePosX());
						m_OMElements[OM_SLIDER_ELEMENT3].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
						this->m_Gamma = i;

						if(!playThisFrickinSoundOnce)
						{
							///								AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUBACKUP);	
							playThisFrickinSoundOnce = true;
						}				
					}
				}
			}
		}
		//}
	}
	else if (!_input.IsLeftClicked())
	{
		ifLeftClickGamma_Options = false;
		playThisFrickinSoundOnce = false;
	}

	//MOUSE INPUT SLIDER 4-------------------------------------------------------------------------------------------------------------------------------------------------------

	//float _sliderWidth4 =	m_OMElements[OM_SLIDER_ELEMENT4].GetRect().GetWidth();	//44
	//float _sliderHeight4 =	m_OMElements[OM_SLIDER_ELEMENT4].GetRect().GetHeight();	//44
	//float _barWidth4 =		m_OMElements[OM_BAR_ELEMENT4].GetRect().GetWidth();		//408
	//float _barHeight4 =		m_OMElements[OM_BAR_ELEMENT4].GetRect().GetHeight();		//44
	//float _barPosX4 =		m_OMElements[OM_BAR_ELEMENT4].GetPosX();		//550
	//float _barPosY4 =		m_OMElements[OM_BAR_ELEMENT4].GetPosY();		//108
	//float _units4 =			_barWidth4 / 10.0f;								//40.799999
	//float _barCombinedX4 =	_barPosX4 + _barWidth4;							//958
	//float _barCombinedY4 =	_barPosY4 + _barHeight4;						//152

	//if (_input.IsLeftClicked() && _mouseX >= _barPosX4 && _mouseX <= _barCombinedX4 && _mouseY <= _barCombinedY4 && _mouseY >= _barPosY4)
	//{
	//	//if (!ifLeftClickMouse_Options)
	//	//{
	//	ifLeftClickMouse_Options = true;
	//	if (m_FadeTimer <= 1.0f && !m_bFadeOut)
	//	{
	//		if (_mouseX >= _barPosX4 && _mouseX <= _barCombinedX4 && _mouseY <= _barCombinedY4 && _mouseY >= _barPosY4)
	//		{
	//			for (int i = 0; i < 10; i++)
	//			{
	//				if (_mouseX >= _barPosX4 + _units4 * i)
	//				{
	//					m_OMElements[OM_SLIDER_ELEMENT4].SetPosX((float)_input.GetMousePosX());
	//					m_OMElements[OM_SLIDER_ELEMENT4].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	//					this->m_MouseSen = i;
	//					if(!playThisFrickinSoundOnce)
	//					{
	//						//								AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUBACKUP);	
	//						playThisFrickinSoundOnce = true;
	//					}						
	//				}
	//			}
	//		}
	//	}
	//	//}
	//}
	else if (!_input.IsLeftClicked())
	{
		ifLeftClickMouse_Options = false;
		playThisFrickinSoundOnce = false;
	}

#pragma endregion

	//SETTING SLIDER ON X -----------------------------------------------------------------------------------
#pragma region Setting Slider on X

	//start:
	_min = m_OMElements[OM_BAR_ELEMENT1].GetPosX();
	_max = m_OMElements[OM_BAR_ELEMENT1].GetPosX() + 408;
	_length = _max - _min;
	_unit = _length / 10.0f;	//40.7999999

	if (this->m_MusicVolume > 9)
		this->m_MusicVolume = 9;
	else
		this->m_SliderX1 = _min;
	this->m_SliderY1 = m_OMElements[OM_SLIDER_ELEMENT1].GetPosY();
	m_OMElements[OM_SLIDER_ELEMENT1].SetPosX(this->m_SliderX1 + _unit * m_MusicVolume * (m_Renderer->GetScreenHeight() / 720.0f));
	m_OMElements[OM_SLIDER_ELEMENT1].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	this->m_SliderX2 = _min;
	this->m_SliderY2 = m_OMElements[OM_SLIDER_ELEMENT2].GetPosY();
	m_OMElements[OM_SLIDER_ELEMENT2].SetPosX(this->m_SliderX2 + _unit * m_SFXVolume * (m_Renderer->GetScreenHeight() / 720.0f));
	m_OMElements[OM_SLIDER_ELEMENT2].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	this->m_SliderX3 = _min;
	this->m_SliderY3 = m_OMElements[OM_SLIDER_ELEMENT3].GetPosY();
	m_OMElements[OM_SLIDER_ELEMENT3].SetPosX(this->m_SliderX3 + _unit * m_Gamma * (m_Renderer->GetScreenHeight() / 720.0f));
	m_OMElements[OM_SLIDER_ELEMENT3].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	//this->m_SliderX4 = _min;
	//this->m_SliderY4 = m_OMElements[OM_SLIDER_ELEMENT4].GetPosY();
	//m_OMElements[OM_SLIDER_ELEMENT4].SetPosX(this->m_SliderX4 + _unit * m_MouseSen);
	//m_OMElements[OM_SLIDER_ELEMENT4].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

#pragma endregion

	return true;
}

void COptionsState::FadeIn (float _fTime)
{		
	m_FadeTimer -= _fTime;//count up to 1
	for(int i = 0; i < OM_NUM_ELEMENTS; ++i)
		m_OMElements[i].SetOpacity(1.0f - m_FadeTimer);     	// set the Opacity to that timer

	for( unsigned int i = 0, n = m_UIResolution.size(); i < n; ++i )
		m_UIResolution[i]->SetOpacity( 1.0f - m_FadeTimer );

	if (m_FadeTimer <= 0.0f)
		m_FadeTimer = 0.0f;
}

void COptionsState::FadeOut (float _fTime)
{
	m_FadeTimer += _fTime;												//count down to 0
	for(int i = 0; i < OM_NUM_ELEMENTS; ++i)
		m_OMElements[i].SetOpacity(1.0f - m_FadeTimer);     	// set the Opacity to that timer	

	for( unsigned int i = 0, n = m_UIResolution.size(); i < n; ++i )
		m_UIResolution[i]->SetOpacity( 1.0f - m_FadeTimer );
}

void COptionsState::ResetDefault ( CGame* _game )
{
	m_Game = _game;
	m_MusicVolume = 3;
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);
	m_SFXVolume = 3;
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::SFX_VOLUME, (float)m_SFXVolume * 10.0f);
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::DX_VOLUME, (float)m_SFXVolume * 10.0f);

	m_Gamma = 3;
	m_MouseSen = 4;
	m_HDC = m_Game->GetHDC();

	m_OMElements[OM_SLIDER_ELEMENT4].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	m_OMElements[OM_SLIDER_ELEMENT3].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	m_OMElements[OM_SLIDER_ELEMENT2].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	m_OMElements[OM_SLIDER_ELEMENT1].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
}

void COptionsState::Update( float _fElapsedTime )
{	 
	this->m_Timer += _fElapsedTime;
	m_ResolutionChangeTimer -= _fElapsedTime;

	CGame::Gamma( m_Gamma, m_GammaArray, m_HDC);

	int tempSens;
	tempSens = (m_MouseSen+1)*2;
	SystemParametersInfo(SPI_SETMOUSESPEED, 0,(void*)&tempSens, SPIF_SENDCHANGE);

	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
	{
		this->FadeIn(_fElapsedTime);
	}
	if (m_bFadeOut)
	{
		this->FadeOut(_fElapsedTime);
	}

	//Time to leave options IF you are fading out and it has faded all the way to 0.0f
	if(m_bFadeOut && m_FadeTimer >= 1.0f)
	{
		this->SetClosing(true);
		return;
	}

	// Yay all hacky resolution stuff
	if( m_Renderer->GetFullScreen() )
	{
		if( m_bCheckResolution )
		{
			for( unsigned int i = 0, n = m_Renderer->GetAvailableResolutions().size(); i < n; ++i )
			{
				if( m_Renderer->GetAvailableResolutions()[i].x == m_Renderer->GetScreenWidth() &&
					m_Renderer->GetAvailableResolutions()[i].y == m_Renderer->GetScreenHeight() )
				{
					m_nResolutionIndex = i;
					m_bCheckResolution = false;
					break;
				}
			}
		}

		for( unsigned int i = 0, n = m_UIResolution.size(); i < n; ++i )
		{
			m_UIResolution[i]->ReleaseGLBatch();
			delete m_UIResolution[i];
		}
		m_UIResolution.clear();

		int screenWidth = (int)m_Renderer->GetAvailableResolutions()[m_nResolutionIndex].x;
		int screenHeight = (int)m_Renderer->GetAvailableResolutions()[m_nResolutionIndex].y;

		Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
		CUIElement* temp = nullptr;
		float x = m_OMElements[OM_BAR_ELEMENT1].GetPosX();
		float y = m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetPosY();
		float offset = 24.0f * (m_Renderer->GetScreenHeight() / 720.0f);
		float scale = 0.5f;
		unsigned int texture = OM_NUMBER_0;

		x += 64.0f * (m_Renderer->GetScreenHeight() / 720.0f);

		if( screenWidth / 1000 )
		{
			temp = new CUIElement();

			texture = OM_NUMBER_0 + screenWidth / 1000;
			if( texture > OM_NUMBER_0 + 9 ) // only 0 - 9
				texture = OM_NUMBER_0 + 9;

			// Chop off the leftmost number
			screenWidth -= (screenWidth / 1000) * 1000;

			temp->Setup( m_Renderer, m_AM->GetTexture( texture ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
				x,
				y,
				0.0f, scale, white );
			temp->SetOpacity( 1.0f - m_FadeTimer );
			m_UIResolution.push_back( temp );

			x += offset;
		}

		// 100s place - width
		temp = new CUIElement();

		texture = OM_NUMBER_0 + screenWidth / 100;
		if( texture > OM_NUMBER_0 + 9 ) // only 0 - 9
			texture = OM_NUMBER_0 + 9;

		// Chop off the leftmost number
		screenWidth -= (screenWidth / 100) * 100;

		temp->Setup( m_Renderer, m_AM->GetTexture( texture ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
			x,
			y,
			0.0f, scale, white );
		temp->SetOpacity( 1.0f - m_FadeTimer );
		m_UIResolution.push_back( temp );

		x += offset;


		// 10s place - width
		temp = new CUIElement();

		texture = OM_NUMBER_0 + screenWidth / 10;
		if( texture > OM_NUMBER_0 + 9 ) // only 0 - 9
			texture = OM_NUMBER_0 + 9;

		// Chop off the leftmost number
		screenWidth -= (screenWidth / 10) * 10;

		temp->Setup( m_Renderer, m_AM->GetTexture( texture ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
			x,
			y,
			0.0f, scale, white );
		temp->SetOpacity( 1.0f - m_FadeTimer );
		m_UIResolution.push_back( temp );

		x += offset;

		// 1s place - width
		temp = new CUIElement();

		texture = OM_NUMBER_0 + screenWidth;
		if( texture > OM_NUMBER_0 + 9 ) // only 0 - 9
			texture = OM_NUMBER_0 + 9;

		temp->Setup( m_Renderer, m_AM->GetTexture( texture ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
			x,
			y,
			0.0f, scale, white );
		temp->SetOpacity( 1.0f - m_FadeTimer );
		m_UIResolution.push_back( temp );

		x += offset;

		// SPACE
		x += offset;

		// The 'X' in the middle
		temp = new CUIElement();
		temp->Setup( m_Renderer, m_AM->GetTexture( OM_NUMBER_X ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
			x,
			y,
			0.0f, scale, white );
		temp->SetOpacity( 1.0f - m_FadeTimer );
		m_UIResolution.push_back( temp );
		x += offset;

		// SPACE
		x += offset;

		if( screenHeight / 1000 )
		{
			temp = new CUIElement();

			texture = OM_NUMBER_0 + screenHeight / 1000;
			if( texture > OM_NUMBER_0 + 9 ) // only 0 - 9
				texture = OM_NUMBER_0 + 9;

			// Chop off the leftmost number
			screenHeight -= (screenHeight / 1000) * 1000;

			temp->Setup( m_Renderer, m_AM->GetTexture( texture ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
				x,
				y,
				0.0f, scale, white );
			temp->SetOpacity( 1.0f - m_FadeTimer );
			m_UIResolution.push_back( temp );

			x += offset;
		}

		// 100s place - height
		temp = new CUIElement();

		texture = OM_NUMBER_0 + screenHeight / 100;
		if( texture > OM_NUMBER_0 + 9 ) // only 0 - 9
			texture = OM_NUMBER_0 + 9;

		// Chop off the leftmost number
		screenHeight -= (screenHeight / 100) * 100;

		temp->Setup( m_Renderer, m_AM->GetTexture( texture ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
			x,
			y,
			0.0f, scale, white );
		temp->SetOpacity( 1.0f - m_FadeTimer );
		m_UIResolution.push_back( temp );

		x += offset;


		// 10s place - height
		temp = new CUIElement();

		texture = OM_NUMBER_0 + screenHeight / 10;
		if( texture > OM_NUMBER_0 + 9 ) // only 0 - 9
			texture = OM_NUMBER_0 + 9;

		// Chop off the leftmost number
		screenHeight -= (screenHeight / 10) * 10;

		temp->Setup( m_Renderer, m_AM->GetTexture( texture ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
			x,
			y,
			0.0f, scale, white );
		temp->SetOpacity( 1.0f - m_FadeTimer );
		m_UIResolution.push_back( temp );

		x += offset;

		// 1s place - height
		temp = new CUIElement();

		texture = OM_NUMBER_0 + screenHeight;
		if( texture > OM_NUMBER_0 + 9 ) // only 0 - 9
			texture = OM_NUMBER_0 + 9;

		temp->Setup( m_Renderer, m_AM->GetTexture( texture ), RECTANGLE( 0.0f, 48.0f, 0.0f, 64.0f ),
			x,
			y,
			0.0f, scale, white );
		temp->SetOpacity( 1.0f - m_FadeTimer );
		m_UIResolution.push_back( temp );
	}
}

void COptionsState::Enter()
{
	m_nResolutionIndex = m_Renderer->GetAvailableResolutions().size() - 1;
	m_bCheckResolution = true;
	m_ResolutionChangeTimer = 0.0f;

	testCounter = 0;
	playThisFrickinSoundOnce = false;

	//ShowCursor(TRUE);
	SetClosing(false);
	m_bMouseActive = false;
	m_PrevMousePosX= 0;
	m_PrevMousePosY= 0;

	this->m_Renderer = m_Game->GetRenderer();
	this->m_Choice = 0;
	this->m_AM = CAssetManager::GetInstance();
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;
	m_bFadeOut = false;
	m_Choice = 0;

	//m_bSoundExit = true;
	m_bSoundMusic = true;
	m_bSoundSFX = true;
	m_bSoundMouse = true;
	m_bSoundDefault = true;
	m_bSoundGamma = true;
	m_bSoundClick = true;
	m_bSoundResolution = true;
	m_bSoundCancel = true;
	m_bSoundAccept = true;
	m_bSoundGamePad = true;
	m_bSoundEnable = true;

	ResetDefault( m_Game );
	LoadOptions( L"\\volume.bin" );


	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::SFX_VOLUME, (float)m_SFXVolume * 10.0f);
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::DX_VOLUME, (float)m_SFXVolume * 10.0f);

	if(m_Enable == 0)
		m_bEnable = false;
	else
		m_bEnable = true;

	m_Game->SetUsingController(m_bEnable);

	//BLACK BACKGROUND
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_OMElements[OM_BLACK_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

	//BACKGROUND
#if defined(_DEBUG) || defined(_BETA)
	m_OMElements[OM_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_OPTIONS_BACKGROUND_1024x768), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);
#else
	m_OMElements[OM_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_OPTIONS_BACKGROUND_1280x720), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);
#endif

	//BUTTONS ----------------------------------------------------------------------------------------------------
#pragma region Non-Highlighted Buttons

	//--------------------------------------------------------------------------------------

	m_OMElements[OM_ALL3_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 230 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.5f, white );

	m_OMElements[OM_ALL3_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3_CONTROLLER),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 240 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.5f, white );

	//m_OMElements[OM_MOVE_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_MOVEMENT),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() / 2) - 400 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) - 96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_OMElements[OM_OK_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_OK),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() / 2) -96 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) - 96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_OMElements[OM_BACK_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_BACK),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() /2) + 224 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) -96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_OMElements[OM_MOVE_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_MOVEMENT_CONTROLLER),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() / 2) -400 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) - 96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_OMElements[OM_OK_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_OK_CONTROLLER),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() /2) -96 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight()) -96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//m_OMElements[OM_BACK_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_BACK_CONTROLLER),
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
	//	float((m_Game->GetScreenWidth() /2) + 224 * (m_Game->GetScreenHeight() / 720.0f)),
	//	((float)m_Game->GetScreenHeight() / 2) -96 * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 0.75f, white );

	//--------------------------------------------------------------------------------------
	
	m_OMElements[OM_MUSIC_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MUSIC), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 320* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_SFX_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_SFX), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 256* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_GAMMA_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_GAMMA), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 192* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_MOUSE_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MOUSE), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 128* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_RESOLUTION_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_RESOLUTION), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384 * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_ACCEPT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_ACCEPT), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		 float(m_Game->GetScreenWidth() / 2.0f) - 416.0f * (m_Renderer->GetScreenHeight() / 720.0f), float((m_Game->GetScreenHeight() / 2.0f) + 256 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_DEFAULT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_DEFAULT), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		 float(m_Game->GetScreenWidth() / 2.0f) - 128.0f * (m_Renderer->GetScreenHeight() / 720.0f), float((m_Game->GetScreenHeight() / 2.0f) + 256 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_CANCEL_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_CANCEL), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		 float(m_Game->GetScreenWidth() / 2.0f) + 160.0f * (m_Renderer->GetScreenHeight() / 720.0f), float((m_Game->GetScreenHeight() / 2.0f) + 256 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);


	m_OMElements[OM_GAMEPAD_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_GAMEPAD), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 100 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_ENABLE_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_ENABLED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) + 64 * (m_Renderer->GetScreenHeight() / 720.0f)), m_OMElements[OM_GAMEPAD_ELEMENT].GetPosY(),
		0.0f, 1.0f, white);

#pragma endregion

	//HIGHLIGHTED BUTTONS----------------------------------------------------------------------------------------------------
#pragma region Highlighted buttons

	//m_OMElements[OM_BACK_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_BACK_HIGHLIGHTED), 
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
	//	280.0f * (m_Game->GetScreenHeight() / 720.0f),
	//	632.0f * (m_Game->GetScreenHeight() / 720.0f),
	//	0.0f, 1.0f, white);

	m_OMElements[OM_MUSIC_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MUSIC_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 320* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_SFX_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_SFX_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f), 
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 256* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_GAMMA_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_GAMMA_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 192* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_MOUSE_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MOUSE_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 128* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_RESOLUTION_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 384* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f)),
		0.0f, 1.0f, white);


	m_OMElements[OM_ACCEPT_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_ACCEPT_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		 float(m_Game->GetScreenWidth() / 2.0f) - 416.0f * (m_Renderer->GetScreenHeight() / 720.0f), float((m_Game->GetScreenHeight() / 2.0f) + 256 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_DEFAULT_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_DEFAULT_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		 float(m_Game->GetScreenWidth() / 2.0f) - 128.0f * (m_Renderer->GetScreenHeight() / 720.0f), float((m_Game->GetScreenHeight() / 2.0f) + 256 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_CANCEL_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_CANCEL_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		 float(m_Game->GetScreenWidth() / 2.0f) + 160.0f * (m_Renderer->GetScreenHeight() / 720.0f), float((m_Game->GetScreenHeight() / 2.0f) + 256 * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_GAMEPAD_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_GAMEPAD_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		m_OMElements[OM_GAMEPAD_ELEMENT].GetPosX(),  m_OMElements[OM_GAMEPAD_ELEMENT].GetPosY(),
		0.0f, 1.0f, white);

	m_OMElements[OM_DISABLE_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_DISABLED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		 float((m_Game->GetScreenWidth() / 2.0f) + 64 * (m_Renderer->GetScreenHeight() / 720.0f)), m_OMElements[OM_GAMEPAD_ELEMENT].GetPosY(),
		0.0f, 1.0f, white);

#pragma endregion

	//SLIDERS & BARS----------------------------------------------------------------------------------------------------
#pragma region Sliders and Bars

	m_OMElements[OM_BAR_ELEMENT1].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_SLIDE_BAR), 
		RECTANGLE(0.0f, 408.0f, 0.0f, 44.0f),
		float((m_Game->GetScreenWidth() / 2.0f) + 32* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 304* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);	//RECTANGLE(0.0f, 408.0f, 0.0f, 44.0f), 550.0f, 133.0f, 0.0f, 1.0f, white);

	m_OMElements[OM_BAR_ELEMENT2].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_SLIDE_BAR), 
		RECTANGLE(0.0f, 408.0f, 0.0f, 44.0f),
		float((m_Game->GetScreenWidth() / 2.0f) + 32* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 240* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_BAR_ELEMENT3].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_SLIDE_BAR), 
		RECTANGLE(0.0f, 408.0f, 0.0f, 44.0f),
		float((m_Game->GetScreenWidth() / 2.0f) + 32* (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 176* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_BAR_ELEMENT4].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_SLIDE_BAR), 
		RECTANGLE(0.0f, 408.0f, 0.0f, 44.0f),
		float((m_Game->GetScreenWidth() / 2.0f) - 425.0f * (m_Renderer->GetScreenHeight() / 720.0f)), float((m_Game->GetScreenHeight() / 2.0f) - 72.0f* (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	m_OMElements[OM_SLIDER_ELEMENT1].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_SLIDE_POINTER), 
		RECTANGLE(0.0f, 44.0f, 0.0f, 44.0f),
		float((m_Game->GetScreenWidth() / 2.0f) + 32* (m_Renderer->GetScreenHeight() / 720.0f)), m_OMElements[OM_BAR_ELEMENT1].GetPosY() - 12.0f* (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_OMElements[OM_SLIDER_ELEMENT2].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_SLIDE_POINTER), 
		RECTANGLE(0.0f, 44.0f, 0.0f, 44.0f),
		float((m_Game->GetScreenWidth() / 2.0f) + 32* (m_Renderer->GetScreenHeight() / 720.0f)),  m_OMElements[OM_BAR_ELEMENT2].GetPosY() - 12.0f* (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_OMElements[OM_SLIDER_ELEMENT3].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_SLIDE_POINTER), 
		RECTANGLE(0.0f, 44.0f, 0.0f, 44.0f),
		float((m_Game->GetScreenWidth() / 2.0f) + 32* (m_Renderer->GetScreenHeight() / 720.0f)), m_OMElements[OM_BAR_ELEMENT3].GetPosY() - 12.0f* (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_OMElements[OM_SLIDER_ELEMENT4].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_SLIDE_BAR), 
		RECTANGLE(0.0f, 408.0f, 0.0f, 44.0f),
		float((m_Game->GetScreenWidth() / 2.0f)) + 15.0f, float((m_Game->GetScreenHeight() / 2.0f) - 72.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		0.0f, 1.0f, white);

	for(int i = 0; i < OM_NUM_ELEMENTS; ++i)
		m_OMElements[i].SetOpacity(0.0f);

#pragma endregion

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);

	// Left Arrow
	m_OMElements[OM_LEFTARROW_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_LEFTARROW), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 64.0f),
		m_OMElements[OM_BAR_ELEMENT1].GetPosX(),
		m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetPosY(),
		0.0f, 0.5f, white);

	// Right Arrow
	m_OMElements[OM_RIGHTARROW_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_RIGHTARROW), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 64.0f),
		m_OMElements[OM_BAR_ELEMENT1].GetPosX() + (m_OMElements[OM_BAR_ELEMENT1].GetRect().right - 32.0f) * (m_Renderer->GetScreenHeight() / 720.0f),
		m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetPosY(),
		0.0f, 0.5f, white);  // the 32.0f in X takes into account the 0.5f scale.

	// Left Arrow Highlighted
	m_OMElements[OM_LEFTARROW_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_LEFTARROW_HIGHLIGHTED), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 64.0f),
		m_OMElements[OM_BAR_ELEMENT1].GetPosX(),
		m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetPosY(),
		0.0f, 0.5f, white);

	// Right Arrow Highlighted
	m_OMElements[OM_RIGHTARROW_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_RIGHTARROW_HIGHLIGHTED), 
		RECTANGLE(0.0f, 64.0f, 0.0f, 64.0f),
		m_OMElements[OM_BAR_ELEMENT1].GetPosX() + (m_OMElements[OM_BAR_ELEMENT1].GetRect().right - 32.0f) * (m_Renderer->GetScreenHeight() / 720.0f),
		m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT].GetPosY(),
		0.0f, 0.5f, white);  // the 32.0f in X takes into account the 0.5f scale.
}

void COptionsState::Render( void )
{
	m_Renderer->EnableDraw();
	//m_Renderer->DrawTexture(&m_OMElements[OM_BACKGROUND_ELEMENT]);
	m_OMElements[OM_BLACK_BACKGROUND_ELEMENT].SetOpacity(0.4f);
	m_Renderer->DrawTexture(&m_OMElements[OM_BLACK_BACKGROUND_ELEMENT]);

	if( m_Renderer->GetFullScreen() )
	{
		if (m_bResolution)
			m_Renderer->DrawTexture(&m_OMElements[OM_RESOLUTION_ELEMENT]);
		if (!m_bResolution)
			m_Renderer->DrawTexture(&m_OMElements[OM_RESOLUTION_HIGHLIGHT_ELEMENT]);

		if( m_bLeftArrow )
			m_Renderer->DrawTexture(&m_OMElements[OM_LEFTARROW_ELEMENT]);
		else
			m_Renderer->DrawTexture(&m_OMElements[OM_LEFTARROW_HIGHLIGHTED_ELEMENT]);

		if( m_bRightArrow )
			m_Renderer->DrawTexture(&m_OMElements[OM_RIGHTARROW_ELEMENT]);
		else
			m_Renderer->DrawTexture(&m_OMElements[OM_RIGHTARROW_HIGHLIGHTED_ELEMENT]);
	}

	if (m_bMusic)
		m_Renderer->DrawTexture(&m_OMElements[OM_MUSIC_ELEMENT]);
	if (!m_bMusic)
		m_Renderer->DrawTexture(&m_OMElements[OM_MUSIC_HIGHLIGHTED_ELEMENT]);

	if (m_bSFX)
		m_Renderer->DrawTexture(&m_OMElements[OM_SFX_ELEMENT]);
	if (!m_bSFX)
		m_Renderer->DrawTexture(&m_OMElements[OM_SFX_HIGHLIGHTED_ELEMENT]);

	if (m_bGamma)
		m_Renderer->DrawTexture(&m_OMElements[OM_GAMMA_ELEMENT]);
	if (!m_bGamma)
		m_Renderer->DrawTexture(&m_OMElements[OM_GAMMA_HIGHLIGHTED_ELEMENT]);

	/*if (m_bMouse)
		m_Renderer->DrawTexture(&m_OMElements[OM_MOUSE_ELEMENT]);
	if (!m_bMouse)
		m_Renderer->DrawTexture(&m_OMElements[OM_MOUSE_HIGHLIGHTED_ELEMENT]);*/

	if (m_bAccept)
		m_Renderer->DrawTexture(&m_OMElements[OM_ACCEPT_ELEMENT]);
	if (!m_bAccept)
		m_Renderer->DrawTexture(&m_OMElements[OM_ACCEPT_HIGHLIGHTED_ELEMENT]);

	if (m_bDefault)
		m_Renderer->DrawTexture(&m_OMElements[OM_DEFAULT_ELEMENT]);
	if (!m_bDefault)
		m_Renderer->DrawTexture(&m_OMElements[OM_DEFAULT_HIGHLIGHTED_ELEMENT]);

	if (m_bCancel)
		m_Renderer->DrawTexture(&m_OMElements[OM_CANCEL_ELEMENT]);
	if (!m_bCancel)
		m_Renderer->DrawTexture(&m_OMElements[OM_CANCEL_HIGHLIGHTED_ELEMENT]);

	if (m_bGamepad)
		m_Renderer->DrawTexture(&m_OMElements[OM_GAMEPAD_ELEMENT]);
	if (!m_bGamepad)
		m_Renderer->DrawTexture(&m_OMElements[OM_GAMEPAD_HIGHLIGHTED_ELEMENT]);

	if (m_bEnable)
	{
		m_Renderer->DrawTexture(&m_OMElements[OM_ENABLE_ELEMENT]);
		m_Renderer->DrawTexture(&m_OMElements[OM_ALL3_CONTROLLER_ELEMENT]);
	}
	if (!m_bEnable)
	{
		m_Renderer->DrawTexture(&m_OMElements[OM_DISABLE_ELEMENT]);
		m_Renderer->DrawTexture(&m_OMElements[OM_ALL3_ELEMENT]);
	}


	m_Renderer->DrawTexture(&m_OMElements[OM_BAR_ELEMENT1]);			//BG_BAR
	m_Renderer->DrawTexture(&m_OMElements[OM_SLIDER_ELEMENT1]);			//SLIDER

	m_Renderer->DrawTexture(&m_OMElements[OM_BAR_ELEMENT2]);			//BG_BAR
	m_Renderer->DrawTexture(&m_OMElements[OM_SLIDER_ELEMENT2]);			//SLIDER

	m_Renderer->DrawTexture(&m_OMElements[OM_BAR_ELEMENT3]);			//BG_BAR
	m_Renderer->DrawTexture(&m_OMElements[OM_SLIDER_ELEMENT3]);			//SLIDER

	//m_Renderer->DrawTexture(&m_OMElements[OM_BAR_ELEMENT4]);			//BG_BAR
	//m_Renderer->DrawTexture(&m_OMElements[OM_SLIDER_ELEMENT4]);		//SLIDER

	//if(!m_bFadeOut && m_FadeTimer <= 1.0f && m_FadeTimer > 0.0f)
	//	

	//if (m_bFadeOut)
	//	//FadeOut();	
	//		m_Renderer->DrawTexture(&m_OMElements[OM_BLACK_BACKGROUND_ELEMENT]);

	// Selected Resolution
	for( unsigned int i = 0, n = m_UIResolution.size(); i < n; ++i )
	{
		m_Renderer->DrawTexture( m_UIResolution[i] );
	}

	//DRAW Reticle
	m_Renderer->DrawTexture( &m_reticle );
	m_Renderer->DisableDraw();
}

void COptionsState::Exit()
{
	while( ShowCursor( FALSE ) >= 0 );
	if (!m_bCancel)							//might need to swap these.
	{
		LoadOptions( L"\\volume.bin" );
		
		AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::SFX_VOLUME, (float)m_SFXVolume * 10.0f);
		AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);
		AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::DX_VOLUME, (float)m_SFXVolume * 10.0f);

		if(m_Enable == 0)
			m_bEnable = false;
		else
			m_bEnable = true;

		m_Game->SetUsingController(m_bEnable);
	}
	if (!m_bAccept)
		SaveOptions( L"\\volume.bin" );
	m_Timer = 0.0f;

	for( unsigned int i = 0; i < OM_NUM_ELEMENTS; ++i )
	{
		m_OMElements[i].ReleaseGLBatch();
	}

	m_reticle.ReleaseGLBatch();

	for( unsigned int i = 0, n = m_UIResolution.size(); i < n; ++i )
	{
		m_UIResolution[i]->ReleaseGLBatch();
		delete m_UIResolution[i];
	}
	m_UIResolution.clear();
}

void COptionsState::Shutdown( void )
{
	for( unsigned int i = 0; i < OM_NUM_ELEMENTS; ++i )
	{
		m_OMElements[i].ReleaseGLBatch();
	}

	m_reticle.ReleaseGLBatch();

	for( unsigned int i = 0, n = m_UIResolution.size(); i < n; ++i )
	{
		m_UIResolution[i]->ReleaseGLBatch();
		delete m_UIResolution[i];
	}
	m_UIResolution.clear();
}

void COptionsState::ProcessStates( eOMChoices _choice )
{

}

bool COptionsState::LoadOptions( const WCHAR* szFilename)
{	
	WCHAR	_szFilePath [4096];
	LPWSTR _returnFilePath = nullptr;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &_returnFilePath );
	wcscpy_s(_szFilePath, _returnFilePath);
	CoTaskMemFree(_returnFilePath);
	wcsncat_s(_szFilePath, szFilename, wcslen(szFilename));

	std::fstream volFile;
	volFile.open(_szFilePath, ios_base::in | ios_base::binary);
	volFile.seekg(0, ios_base::beg);

	if (volFile.is_open())
	{
		volFile.read((char*)&m_MusicVolume, sizeof(int));
		volFile.read((char*)&m_SFXVolume, sizeof(int));
		volFile.read((char*)&m_Gamma, sizeof(int));
		volFile.read((char*)&m_MouseSen, sizeof(int));
		volFile.read((char*)&m_Enable, sizeof(int));
	}
	volFile.close();
	return true;
}

void COptionsState::SaveOptions(const WCHAR* szFilename)
{
	WCHAR	_szFilePath [4096];
	LPWSTR _returnFilePath = nullptr;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &_returnFilePath );
	wcscpy_s(_szFilePath, _returnFilePath);
	CoTaskMemFree(_returnFilePath);
	wcsncat_s(_szFilePath, szFilename, wcslen(szFilename));

	std::fstream volFile;
	volFile.open(_szFilePath, ios_base::out | ios_base::binary | ios_base::trunc);
	volFile.seekg(0, ios_base::beg);

	if(!volFile.is_open())
	{
		// Sends error message if broken
		MessageBoxA(0, "Volume File not found.", 0, 0);
		return;
	}

	if (volFile.is_open())
	{
		volFile.write((char*)&m_MusicVolume, sizeof(int));
		volFile.write((char*)&m_SFXVolume, sizeof(int));
		volFile.write((char*)&m_Gamma, sizeof(int));
		volFile.write((char*)&m_MouseSen, sizeof(int));
		volFile.write((char*)&m_Enable, sizeof(int));
	}
	volFile.close();
}
