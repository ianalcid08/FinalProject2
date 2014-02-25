/*///////////////////////////////////////////////////////////////////////////////////
/FileName:			WinState.cpp
/Author:			Thomas Stefl
/Last Modified:		7/30/2013
/Purpose:			This is the state that will handle input and rendering of necessary
/					items when the player wins the game.
*////////////////////////////////////////////////////////////////////////////////////
#include "../Header Files/stdafx.h"
#include "WinState.h"
#include "../Header Files/Game.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../Header Files/AssetManager.h"


WinState::WinState(void) 
{
	this->SetClosing(false);
	m_bFadeOut = false;
	//this->SetID(WIN_STATE);
}

WinState::~WinState(void)
{

}

WinState* WinState::GetInstance()
{
	static WinState s_Instance;
	return &s_Instance;
}

void WinState::Init(CGame* _game)
{
	this->m_Game = _game;
	this->m_Renderer = m_Game->GetRenderer();
	this->m_Choice = 0;
	this->m_AM = CAssetManager::GetInstance();

	m_bMain = true;
	m_bCredits = false;
	m_bFadeOut = false;

	m_Timer = 0.0f;

	SetID( WIN_STATE );
}

void WinState::Shutdown()
{
	while( ShowCursor( FALSE ) >= 0 );

	for( unsigned int i = 0; i < WIN_NUM_ELEMENTS; ++i )
	{
		m_WinElements[i].ReleaseGLBatch();
	}
	m_BlackLetterbox.ReleaseGLBatch();
	m_reticle.ReleaseGLBatch();
}

void WinState::Enter()
{
	//ShowCursor(TRUE);

	this->m_Renderer = m_Game->GetRenderer();
	this->m_Choice = 0;
	this->m_AM = CAssetManager::GetInstance();

	m_bMain = true;
	m_bCredits = false;
	m_bFadeOut = false;

	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;

	m_bSoundMain = true;
	m_bSoundCredits = true;
	m_bSoundCheck = true;
	m_bEsc = false;

	//BLACK BACKGROUND
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLint temp;
	temp = CAssetManager::GetInstance()->GetTexture(M_WIN_BACKGROUND);

	m_WinElements[WIN_BLACK_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);
			
	m_WinElements[WIN_BACKGROUND_ELEMENT].Setup(m_Renderer, temp, RECTANGLE(0.0f, 1280.0f, 0.0f, 720.0f),
		(float)m_Renderer->GetScreenWidth() / 2.0f - 640.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		(float)m_Renderer->GetScreenHeight() / 2.0f - 360.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		0.0f, (m_Renderer->GetScreenWidth() / 1280.0f) * (720.0f / m_Renderer->GetScreenHeight()), white);

	m_BlackLetterbox.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);
	
//#if defined(_DEBUG) || defined(_BETA)
//	m_WinElements[WIN_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_WIN_BACKGROUND_1024x768), 
//		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);
//#else
//	m_WinElements[WIN_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_WIN_BACKGROUND_1280x720), 
//		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, float(m_Game->GetScreenHeight())), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);
//#endif

	//NON-HIGHLIGHTED BUTTONS --------------------------------------------------------------------------------------------
#pragma region Non-Highlighted Buttons

	//m_WinElements[WIN_MAIN_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MAINMENU), 
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
	//	(float)(m_Game->GetScreenWidth() /2) - 384 * (m_Renderer->GetScreenHeight() / 720.0f),
	//	((float)m_Game->GetScreenHeight() / 2) +96 * (m_Renderer->GetScreenHeight() / 720.0f),
	//	0.0f, 1.0f, white);

	m_WinElements[WIN_CREDITS_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_CREDITS), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(float)(m_Game->GetScreenWidth() /2) - (256.0f * 0.5f) * (m_Renderer->GetScreenHeight() / 720.0f),
		((float)m_Game->GetScreenHeight() / 2) +88 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	
	m_WinElements[WIN_ALL3_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 180 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.75f, white );

	m_WinElements[WIN_ALL3_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3_CONTROLLER),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 190 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.75f, white );

#pragma endregion

	//HIGHLIGHTED BUTTONS --------------------------------------------------------------------------------------------
#pragma region Highlighted Buttons

	//m_WinElements[WIN_MAIN_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MAINMENU_HIGHLIGHTED), 
	//	RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
	//	(float)(m_Game->GetScreenWidth() /2) - 384 * (m_Renderer->GetScreenHeight() / 720.0f),
	//	((float)m_Game->GetScreenHeight() / 2) +96 * (m_Renderer->GetScreenHeight() / 720.0f),
	//	0.0f, 1.0f, white);

	m_WinElements[WIN_CREDITS_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_CREDITS_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		m_WinElements[WIN_CREDITS_ELEMENT].GetPosX(),
		m_WinElements[WIN_CREDITS_ELEMENT].GetPosY(),
		0.0f, 1.0f, white);

#pragma endregion

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);

}

void WinState::Exit()
{
	while( ShowCursor( FALSE ) >= 0 );

	for( unsigned int i = 0; i < WIN_NUM_ELEMENTS; ++i )
	{
		m_WinElements[i].ReleaseGLBatch();
	}
	m_BlackLetterbox.ReleaseGLBatch();
	m_reticle.ReleaseGLBatch();
}

bool WinState::Input(CInput& _input )
{
	_input.ReadMouse();
	_input.ReadKeyboard();

	static bool ifPressedD_WIN = false;
	static bool ifPressedU_WIN = false;
	static bool ifPressedEnter_WIN = false;

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

	static bool ifPressedEsc_WIN = false;
	if(!ifPressedEsc_WIN)
	{
		ifPressedEsc_WIN = true;
		if(_input.KeyDown(DIK_ESCAPE))
		{
			m_bFadeOut = true;
			if (m_bEsc)
			{
				m_bEsc = false;
			}
			if (m_bSoundCheck)
			{
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				m_bSoundCheck = false;
			}
			return true;
		}
	}
	else
		ifPressedEsc_WIN = false;

	//MOUSE INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Mouse Input for Selecting Buttons

	int _mouseX =			_input.GetMousePosX();
	int _mouseY =			_input.GetMousePosY();

	//MAIN MENU BUTTON
	//float _mainWidth1 =			m_WinElements[WIN_MAIN_ELEMENT].GetRect().GetWidth();		//256
	//float _mainHeight1 =		m_WinElements[WIN_MAIN_ELEMENT].GetRect().GetHeight();		//64
	//float _mainPosX1 =			m_WinElements[WIN_MAIN_ELEMENT].GetPosX();		//280
	//float _mainPosY1 =			m_WinElements[WIN_MAIN_ELEMENT].GetPosY();		//612
	//float _mainCombinedX1 =		_mainPosX1 + _mainWidth1;						//536
	//float _mainCombinedY1 =		_mainPosY1 + _mainHeight1;						//676

	//if (_mouseX >= _mainPosX1 && _mouseX <= _mainCombinedX1 && _mouseY <= _mainCombinedY1 && _mouseY >= _mainPosY1)
	//{
	//	if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
	//	{
	//		if (m_bMouseActive)
	//		{
	//			m_Choice = W_MAIN_MENU;
	//			if (m_bSoundMain)
	//			{
	//				//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
	//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

	//				m_bSoundMain = false;
	//				m_bSoundCredits = true;
	//			}
	//		}
	//		if (m_Timer > 0.5f)
	//		{
	//			if (_input.IsLeftClicked())
	//			{
	//				//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
	//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

	//				m_bFadeOut = true;
	//			}
	//			/*ProcessStates((eWinChoices)m_Choice);*/
	//		}
	//	}
	//}

	//CREDITS BUTTON
	float _creditsWidth1 =		m_WinElements[WIN_CREDITS_ELEMENT].GetRect().GetWidth();		//256
	float _creditsHeight1 =		m_WinElements[WIN_CREDITS_ELEMENT].GetRect().GetHeight();		//64
	float _creditsPosX1 =		m_WinElements[WIN_CREDITS_ELEMENT].GetPosX();		//280
	float _creditsPosY1 =		m_WinElements[WIN_CREDITS_ELEMENT].GetPosY();		//612
	float _creditsCombinedX1 =	_creditsPosX1 + _creditsWidth1;						//536
	float _creditsCombinedY1 =	_creditsPosY1 + _creditsHeight1;						//676

	if (_mouseX >= _creditsPosX1 && _mouseX <= _creditsCombinedX1 && _mouseY <= _creditsCombinedY1 && _mouseY >= _creditsPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
		{
			if (m_bMouseActive)
			{
				m_Choice = W_CREDITS;
				if (m_bSoundCredits)
				{
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_bSoundMain = true;
					m_bSoundCredits = false;
				}
			}
			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
				{
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

					m_bFadeOut = true;
				}
			}
		}
	}

#pragma endregion

	//KEYBOARD INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Keyboard Input for Selecting Buttons

	//float _winPosY1 = m_WinElements[WIN_HIGHLIGHTED_ELEMENT].GetPosY();

	/*if(_input.KeyDown(DIK_A) || _input.KeyDown(DIK_LEFT) || _input.GetController().GetControllerState().Gamepad.sThumbLY < - 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedD_WIN)
			{
				m_Choice++;
				if(m_Choice == WIN_NUM_CHOICES)
				{
					m_Choice = W_MAIN_MENU;
				}
				ifPressedD_WIN = true;
			}	
		}
	}
	else
		ifPressedD_WIN = false;

	if(_input.KeyDown(DIK_D) || _input.KeyDown(DIK_RIGHT) || _input.GetController().GetControllerState().Gamepad.sThumbLY > 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedU_WIN)
			{
				m_Choice--;
				if(m_Choice < 0)
				{
					m_Choice = W_CREDITS;
				}
				ifPressedU_WIN = true;
			}
		}
	}
	else
		ifPressedU_WIN = false;*/

	if (m_Timer > 0.5f)
	{
		if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
		{
			if(!ifPressedEnter_WIN)
			{
				m_bFadeOut = true;
				ifPressedEnter_WIN = true;
			}
		}
		else
			ifPressedEnter_WIN = false;
	}



	//if (m_Choice == W_MAIN_MENU)
	//{
	//	if (m_bSoundMain)
	//	{
	//		//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
	//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

	//		m_bSoundMain = false;
	//		m_bSoundCredits = true;
	//	}
	//}
	if (m_Choice == W_CREDITS)
	{
		if (m_bSoundCredits)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

			m_bSoundMain = true;
			m_bSoundCredits = false;
		}
	}

	//if (m_Choice == W_MAIN_MENU)
	//{
	//	m_bMain = false;
	//	m_bCredits = true;
	//}
	if (m_Choice == W_CREDITS)
	{
		m_bMain = true;
		m_bCredits = false;
	}

#pragma endregion

	return true;
}

void WinState::Update(float _fElapsedTime)
{		
	m_Timer += _fElapsedTime;

	if(!m_bFadeOut && m_FadeTimer >= 0.0f && !m_bEsc)
	{
		m_FadeTimer -= _fElapsedTime;											//count up to 1
		m_WinElements[WIN_BLACK_BACKGROUND_ELEMENT].SetOpacity(m_FadeTimer);     	// set the Opacity to that timer	
		if (m_FadeTimer <= 0.0f)
			m_FadeTimer = 0.0f;
	}
	if (m_bFadeOut)
	{
		m_FadeTimer += _fElapsedTime;											//count down to 0
		m_WinElements[WIN_BLACK_BACKGROUND_ELEMENT].SetOpacity(m_FadeTimer);     	// set the Opacity to that timer
	}

	//if (m_bFadeOut && m_FadeTimer >= 1.0f && !m_bEsc)
	//{
	//	m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
	//	return;
	//}

	if (m_bFadeOut && m_FadeTimer >= 1.0f && !m_bEsc)
	{
		switch(m_Choice)
		{
		//case W_MAIN_MENU:99
		//	{
		//		//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_LOSE_SAD_MUSIC);
		//		//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_WIN_CHEERING);
		//		//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_04);
		//		m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
		//		return;
		//	}	
		//	break;
		case W_CREDITS:
			{
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_WIN_CHEERING);
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_04);
				m_Game->GetStateManager()->PushState(CREDITS_STATE);
				return;
			}
			break;
		};

	}

}

void WinState::Render()
{
	m_Renderer->EnableDraw();
	m_Renderer->DrawTexture(&m_BlackLetterbox);
	m_Renderer->DrawTexture(&m_WinElements[WIN_BACKGROUND_ELEMENT]);

	/*if(m_bMain)
		m_Renderer->DrawTexture(&m_WinElements[WIN_MAIN_ELEMENT]);

	if(!m_bMain)
		m_Renderer->DrawTexture(&m_WinElements[WIN_MAIN_HIGHLIGHTED_ELEMENT]);
*/
	if(m_bCredits)
		m_Renderer->DrawTexture(&m_WinElements[WIN_CREDITS_ELEMENT]);

	if(!m_bCredits)
		m_Renderer->DrawTexture(&m_WinElements[WIN_CREDITS_HIGHLIGHTED_ELEMENT]);

	if(!m_bFadeOut && m_FadeTimer >= 0.0f && !m_bEsc)	
		m_Renderer->DrawTexture(&m_WinElements[WIN_BLACK_BACKGROUND_ELEMENT]);
	if (m_bFadeOut)	
		m_Renderer->DrawTexture(&m_WinElements[WIN_BLACK_BACKGROUND_ELEMENT]);	

	if(m_Game->GetUsingController() == false)
	{
		m_Renderer->DrawTexture(&m_WinElements[WIN_ALL3_ELEMENT]);
	}
	else if(m_Game->GetUsingController() == true)
	{
		m_Renderer->DrawTexture(&m_WinElements[WIN_ALL3_CONTROLLER_ELEMENT]);
	}

	//DRAW Reticle
	m_Renderer->DrawTexture( &m_reticle );
	m_Renderer->DisableDraw();
}

void WinState::ProcessStates(eWinChoices _choice)
{
	m_bFadeOut = true;
}
