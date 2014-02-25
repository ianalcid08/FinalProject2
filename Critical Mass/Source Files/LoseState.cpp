/*///////////////////////////////////////////////////////////////////////////////////
/FileName:		LoseState.h
/Author:		Thomas Stefl
Corey Morehead
/Last Modified: 7/30/2013
08/10/13
/Purpose:		This is the state that will handle input and rendering of necessary
/				items when the player loses the game.
*////////////////////////////////////////////////////////////////////////////////////
#include "../Header Files/stdafx.h"
#include "../Header Files/LoseState.h"
#include "../Header Files/Game.h"
#include "../Header Files/AssetManager.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Wwise_IDs.h"

LoseState::LoseState(void)
{
	this->SetClosing(false);
	//this->SetID(LOSE_STATE);
}

LoseState::~LoseState(void)
{
	Shutdown();
}

LoseState* LoseState::GetInstance()
{
	static LoseState s_Instance;
	return &s_Instance;
}

void LoseState::Init(CGame* _game)
{
	this->m_Game = _game;
	this->m_Renderer = m_Game->GetRenderer();
	this->m_Choice = 0;
	this->m_AM = CAssetManager::GetInstance();
	this->m_bMain = true;
	this->m_bPlay = false;
	this->m_bFadeOut = false;

	SetID( LOSE_STATE );
}

void LoseState::Enter()
{
	//ShowCursor(TRUE);
	this->m_Renderer = m_Game->GetRenderer();
	this->m_Choice = 0;
	this->m_AM = CAssetManager::GetInstance();
	this->m_bMain = true;
	this->m_bPlay = false;
	this->m_bFadeOut = false;
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;

	m_bSoundPlay = true;
	m_bSoundMain = true;
	m_bSoundCheck = true;
	m_bEsc = true;

	GLint temp;
	temp = CAssetManager::GetInstance()->GetTexture(M_LOSE_BACKGROUND);

	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_LoseElements[LOSE_BLACK_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

	//m_LoseElements[LOSE_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_LOSE_BACKGROUND), 
	//	RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

	m_LoseElements[LOSE_BACKGROUND_ELEMENT].Setup(m_Renderer, temp, RECTANGLE(0.0f, 1280.0f, 0.0f, 720.0f),
		(float)m_Renderer->GetScreenWidth() / 2.0f - 640.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		(float)m_Renderer->GetScreenHeight() / 2.0f - 360.0f * (m_Renderer->GetScreenWidth() / 1280.0f),
		0.0f, (m_Renderer->GetScreenWidth() / 1280.0f) * (720.0f / m_Renderer->GetScreenHeight()), white);

	m_BlackLetterbox.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);


#pragma region Non-Highlighted Buttons

		
	m_LoseElements[LOSE_ALL3_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 180 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.75f, white );

	m_LoseElements[LOSE_ALL3_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3_CONTROLLER),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 190 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.75f, white );

	m_LoseElements[LOSE_MAIN_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MM),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f), 
		(float)(m_Game->GetScreenWidth() /2) - 90.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		((float)m_Game->GetScreenHeight() / 2) + 155 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 0.8f, white);

	m_LoseElements[LOSE_PLAY_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_PLAY),
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f), 
		(float)(m_Game->GetScreenWidth() /2) + 168 * (m_Renderer->GetScreenHeight() / 720.0f),
		((float)m_Game->GetScreenHeight() / 2) + 150 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 0.6f, white);
#pragma endregion

	//HIGHLIGHTED BUTTONS----------------------------------------------------------------------------------------------------
#pragma region Highlighted buttons

	m_LoseElements[LOSE_MAIN_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_MAINMENU_HIGHLIGHTED), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f), 
		m_LoseElements[LOSE_MAIN_ELEMENT].GetPosX(),
		m_LoseElements[LOSE_MAIN_ELEMENT].GetPosY(),
		0.0f, 0.8f, white);

	m_LoseElements[LOSE_PLAY_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_PLAY_HIGHLIGHTED), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f), 
		m_LoseElements[LOSE_PLAY_ELEMENT].GetPosX(),
		m_LoseElements[LOSE_PLAY_ELEMENT].GetPosY(),
		0.0f, 0.6f, white);

#pragma endregion

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);
}

void LoseState::Exit()
{
	while( ShowCursor( FALSE ) >= 0 );

	for( unsigned int i = 0; i < LOSE_NUM_ELEMENTS; ++i )
	{
		m_LoseElements[i].ReleaseGLBatch();
	}
	m_BlackLetterbox.ReleaseGLBatch();
	m_reticle.ReleaseGLBatch();
}

void LoseState::Shutdown()
{
	for( unsigned int i = 0; i < LOSE_NUM_ELEMENTS; ++i )
	{
		m_LoseElements[i].ReleaseGLBatch();
	}
	m_BlackLetterbox.ReleaseGLBatch();
	m_reticle.ReleaseGLBatch();
}

bool LoseState::Input(CInput& _input )
{
	_input.ReadMouse();
	_input.ReadKeyboard();

	static bool ifPressedD_LOSE = false;
	static bool ifPressedU_LOSE = false;
	static bool ifPressedEnter_LOSE = false;

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

	static bool ifPressedEsc_LOSE = false;
	if(!ifPressedEsc_LOSE)
	{
		ifPressedEsc_LOSE = true;
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
		ifPressedEsc_LOSE = false;

	//MOUSE INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Mouse Input for Selecting Buttons

	int _mouseX =			_input.GetMousePosX();
	int _mouseY =			_input.GetMousePosY();

	//PLAY BUTTON
	float _playWidth1 =			m_LoseElements[LOSE_PLAY_ELEMENT].GetRect().GetWidth();		//256
	float _playHeight1 =		m_LoseElements[LOSE_PLAY_ELEMENT].GetRect().GetHeight();		//64
	float _playPosX1 =			m_LoseElements[LOSE_PLAY_ELEMENT].GetPosX();		//280
	float _playPosY1 =			m_LoseElements[LOSE_PLAY_ELEMENT].GetPosY();		//612
	float _playCombinedX1 =		_playPosX1 + _playWidth1;						//536
	float _playCombinedY1 =		_playPosY1 + _playHeight1;						//676

	if (_mouseX >= _playPosX1 && _mouseX <= _playCombinedX1 && _mouseY <= _playCombinedY1 && _mouseY >= _playPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
		{
			if (m_bMouseActive)
			{
				m_Choice = L_PLAY;
				//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].SetPosY(_playPosY1);
				//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if (m_bSoundPlay)
				{
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_bSoundMain = true;
					m_bSoundPlay = false;
				}
			}
			if (m_Timer > 0.5f)
			{
				static bool ifPressedMouse_MM = false;
				if (_input.IsLeftClicked())
					if(!ifPressedMouse_MM)
					{
						m_bFadeOut = true;
						//						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

						//ProcessStates((eLoseChoices)m_Choice);
					}
					else
						ifPressedMouse_MM = false;
			}
		}
	}

	//MAIN MENU BUTTON
	float _mainWidth1 =			m_LoseElements[LOSE_MAIN_ELEMENT].GetRect().GetWidth();		//256
	float _mainHeight1 =		m_LoseElements[LOSE_MAIN_ELEMENT].GetRect().GetHeight();		//64
	float _mainPosX1 =			m_LoseElements[LOSE_MAIN_ELEMENT].GetPosX();		//280
	float _mainPosY1 =			m_LoseElements[LOSE_MAIN_ELEMENT].GetPosY();		//612
	float _mainCombinedX1 =		_mainPosX1 + _mainWidth1;						//536
	float _mainCombinedY1 =		_mainPosY1 + _mainHeight1;						//676

	if (_mouseX >= _mainPosX1 && _mouseX <= _mainCombinedX1 && _mouseY <= _mainCombinedY1 && _mouseY >= _mainPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)	//Fadded in, set to 0, and not FadeOut.
		{
			if (m_bMouseActive)
			{
				m_Choice = L_MAIN_MENU;
				//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].SetPosY(_mainPosY1);
				//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if (m_bSoundMain)
				{
					//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_bSoundMain = false;
					m_bSoundPlay = true;
				}
			}
			if (m_Timer > 0.5f)
			{
				static bool ifPressedMouse_MM = false;
				if (_input.IsLeftClicked())
					if(!ifPressedMouse_MM)
					{
						m_bFadeOut = true;
						//						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

						//ProcessStates((eLoseChoices)m_Choice);
					}
					else
						ifPressedMouse_MM = false;
			}
		}
	}

#pragma endregion

	//KEYBOARD INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Keyboard Input for Selecting Buttons

	//float temp = m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].GetPosY();
	if(_input.KeyDown(DIK_A) || _input.KeyDown(DIK_LEFT) || _input.GetController().GetControllerState().Gamepad.sThumbLY < - 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT )
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedD_LOSE)
			{
				m_Choice++;
				if(m_Choice == L_NUM_CHOICES)
				{
					m_Choice = L_MAIN_MENU;
					//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].SetPosY(128.0f);
					//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].BuildGeometry();
				}
				ifPressedD_LOSE = true;
			}	
		}
	}
	else
		ifPressedD_LOSE = false;

	if(_input.KeyDown(DIK_D) || _input.KeyDown(DIK_RIGHT) || _input.GetController().GetControllerState().Gamepad.sThumbLY > 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedU_LOSE)
			{
				m_Choice--;
				//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].SetPosX( 0.0f);
				//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].SetPosY( m_Choice * 128.0f + 128.0f);
				//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].BuildGeometry();
				if(m_Choice < 0)
				{
					m_Choice = L_PLAY;
					//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].SetPosY(256.0f);
					//m_LoseElements[LOSE_HIGHLIGHTED_ELEMENT].BuildGeometry();
				}
				ifPressedU_LOSE = true;
			}	
		}
	}
	else
		ifPressedU_LOSE = false;

	if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
	{
		if(!ifPressedEnter_LOSE)
		{		
			m_bFadeOut = true;
			//ProcessStates((eLoseChoices)m_Choice);
			ifPressedEnter_LOSE = true;
		}
	}
	else
		ifPressedEnter_LOSE = false;

	if (m_Choice == L_PLAY)
	{
		if (m_bSoundPlay)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundMain = true;
			m_bSoundPlay = false;
		}
	}
	if (m_Choice == L_MAIN_MENU)
	{
		if (m_bSoundMain)
		{
			//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundMain = false;
			m_bSoundPlay = true;
		}
	}

	if (m_Choice == L_PLAY)
	{
		m_bPlay = false;
		m_bMain = true;
	}
	if (m_Choice == L_MAIN_MENU)
	{
		m_bPlay = true;
		m_bMain = false;
	}

#pragma endregion

	return true;
}

void LoseState::Update(float _fElapsedTime)
{
	m_Timer += _fElapsedTime;

	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
	{
		m_FadeTimer -= _fElapsedTime;											//count up to 1
		m_LoseElements[LOSE_BLACK_BACKGROUND_ELEMENT].SetOpacity(m_FadeTimer);     	// set the Opacity to that timer	
		if (m_FadeTimer <= 0.0f)
			m_FadeTimer = 0.0f;
	}
	if (m_bFadeOut)
	{
		m_FadeTimer += _fElapsedTime;											//count down to 0
		m_LoseElements[LOSE_BLACK_BACKGROUND_ELEMENT].SetOpacity(m_FadeTimer);     	// set the Opacity to that timer	
	}

	if (m_bFadeOut && m_FadeTimer >= 1.0f && !m_bEsc)
	{
		m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
		return;
	}

	if (m_bFadeOut && m_FadeTimer >= 1.0f && m_bEsc)
	{
		switch(m_Choice)
		{
		case L_MAIN_MENU:
			{
				m_bFadeOut = false;
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_LOSE_SAD_MUSIC);
				//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_WIN_CHEERING);
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_04);
				m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
				return;
			}	
		case L_PLAY:
			{
				m_bFadeOut = false;
				//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_LOSE_SAD_MUSIC);
				//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_WIN_CHEERING);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_01);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_02);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_03);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_04);
				m_Game->GetStateManager()->ChangeState(TUTORIAL_STATE);
				return;
			}	
		};
	}
}

void LoseState::Render()
{
	m_Renderer->EnableDraw();

	m_Renderer->DrawTexture( &m_BlackLetterbox );
	m_Renderer->DrawTexture(&m_LoseElements[LOSE_BACKGROUND_ELEMENT]);

	if(m_bMain)
		m_Renderer->DrawTexture(&m_LoseElements[LOSE_MAIN_ELEMENT]);
	if(!m_bMain)
		m_Renderer->DrawTexture(&m_LoseElements[LOSE_MAIN_HIGHLIGHTED_ELEMENT]);

	if(m_bPlay)
		m_Renderer->DrawTexture(&m_LoseElements[LOSE_PLAY_ELEMENT]);
	if (!m_bPlay)
		m_Renderer->DrawTexture(&m_LoseElements[LOSE_PLAY_HIGHLIGHTED_ELEMENT]);

	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
		m_Renderer->DrawTexture(&m_LoseElements[LOSE_BLACK_BACKGROUND_ELEMENT]);
	if (m_bFadeOut)
		m_Renderer->DrawTexture(&m_LoseElements[LOSE_BLACK_BACKGROUND_ELEMENT]);	

	if(m_Game->GetUsingController() == false)
	{
		m_Renderer->DrawTexture(&m_LoseElements[LOSE_ALL3_ELEMENT]);
	}
	else if(m_Game->GetUsingController() == true)
	{
		m_Renderer->DrawTexture(&m_LoseElements[LOSE_ALL3_CONTROLLER_ELEMENT]);
	}

	//DRAW Reticle
	m_Renderer->DrawTexture( &m_reticle );
	m_Renderer->DisableDraw();
}

void LoseState::ProcessStates(eLoseChoices _choice)
{
	//switch(_choice)
	//{
	//case L_MAIN_MENU:
	//	{
	//		m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
	//		return;
	//	}	
	//case L_PLAY:
	//	{
	//		m_Game->GetStateManager()->ChangeState(GAMEPLAY_STATE);
	//		return;
	//	}	
	//};
}
