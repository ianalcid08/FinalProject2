#include "../Header Files/StdAfx.h"
#include "../Header Files/QuitGameState.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/Game.h"
#include "../Header Files/StateManager.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................


CQuitGameState::CQuitGameState(void)
{
	this->SetClosing(false);
}


CQuitGameState::~CQuitGameState(void)
{
	this->Shutdown();
}

CQuitGameState* CQuitGameState::GetInstance(void)
{
	static CQuitGameState s_Instance;
	return &s_Instance;
}

void CQuitGameState::Init(CGame* _game)
{
	m_Timer = 0.0f;
	m_Renderer = _game->GetRenderer();
	m_Game = _game;

	SetID( QUITGAME_STATE );
}

void CQuitGameState::Shutdown()
{
	Exit();
}

void CQuitGameState::Enter()
{
	m_bFadeOut = false;
	m_FadeTimer = 1.0f;
	m_Timer = 0.0f;

	m_bSoundYes = true;
	m_bSoundNo = true;

	m_bYes = true;
	m_bNo = false;

	//BACKGROUND
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_QuitElements[QUIT_BACKGROUND_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_PAUSE_BG), 
		RECTANGLE(0.0f, 512.0f, 0.0f, 512.0f), (m_Game->GetScreenWidth() / 2.0f) - 256 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) - 200 * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);

		//NON-HIGHLIGHTED BUTTONS ------------------------------------------------------------------------------------------------
#pragma region Non-Highlighted Buttons

	m_QuitElements[QUIT_YES_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_YES), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 160 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 32 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_QuitElements[QUIT_NO_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_NO), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) + 64 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 38 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_QuitElements[QUIT_TEXT_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(QUITGAME), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 128  * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) - 64  * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

#pragma endregion

	//HIGHLIGHTED BUTTONS ------------------------------------------------------------------------------------------------
#pragma region Highlighted Buttons

	m_QuitElements[QUIT_YES_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_YES_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) - 160 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 32 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	m_QuitElements[QUIT_NO_HIGHLIGHTED_ELEMENT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(BUTTON_NO_HIGHLIGHTED), 
		RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Game->GetScreenWidth() / 2.0f) + 64 * (m_Renderer->GetScreenHeight() / 720.0f),
		(m_Game->GetScreenHeight() / 2.0f) + 38 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

#pragma endregion

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);

	//for(int i = 0; i < QUIT_NUM_ELEMENTS; ++i)
	//	m_QuitElements[i].SetOpacity(0.0f);

	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RETREAT);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RISING);

	for(int i = 0; i < QUIT_NUM_ELEMENTS; ++i)
		m_QuitElements[i].SetOpacity(0.0f);
}

void CQuitGameState::Exit()
{
	m_BackGround.ReleaseGLBatch();

	for( unsigned int i = 0; i < QUIT_NUM_ELEMENTS; ++i )
		m_QuitElements[i].ReleaseGLBatch();

	m_reticle.ReleaseGLBatch();
}

bool CQuitGameState::Input( CInput& _input )
{
	_input.ReadMouse();
	_input.ReadKeyboard();

	static bool ifPressedD_Q = false;
	static bool ifPressedU_Q = false;
	static bool ifPressedEnter_Q = false;

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
	static bool ifPressedEsc_Q = false;
	if(!ifPressedEsc_Q)
	{
		ifPressedEsc_Q = true;
		if(_input.KeyDown(DIK_ESCAPE))
		{
			m_Choice = QUIT_NO;
			m_bFadeOut = true;
			return true;
		}
	}
	else
		ifPressedEsc_Q = false;

		if (m_bFadeOut && m_FadeTimer >= 1.0f )
	{
	switch(m_Choice)
		{
		case QUIT_YES:
			{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
				m_Game->SetIsClosing(true);
				return true;
			}
			break;
		case QUIT_NO:
			{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
				m_Game->GetStateManager()->PopState();
				m_Game->GetStateManager()->ChangeState(MAINMENU_STATE);
				return true;
			}
			break;
		};	
	}

//MOUSE INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Mouse Input for Selecting Buttons

	int _mouseX =			_input.GetMousePosX();
	int _mouseY =			_input.GetMousePosY();

	//YES BUTTON
	float _YesWidth1 =			m_QuitElements[QUIT_YES_ELEMENT].GetRect().GetWidth();		
	float _YesHeight1 =			m_QuitElements[QUIT_YES_ELEMENT].GetRect().GetHeight();		
	float _YesPosX1 =			m_QuitElements[QUIT_YES_ELEMENT].GetPosX();		
	float _YesPosY1 =			m_QuitElements[QUIT_YES_ELEMENT].GetPosY();		
	float _YesCombinedX1 =		_YesPosX1 + _YesWidth1;						
	float _YesCombinedY1 =		_YesPosY1 + _YesHeight1;						

	if (_mouseX >= _YesPosX1 && _mouseX <= _YesCombinedX1 && _mouseY <= _YesCombinedY1 && _mouseY >= _YesPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = QUIT_YES;
				if (m_bSoundYes)
				{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_bSoundYes = false;
					m_bSoundNo = true;
				}
			}
			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
				{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

					m_bFadeOut = true;
				}
			}
		}
	}
	//NO BUTTON
	float _NoWidth1 =			m_QuitElements[QUIT_NO_ELEMENT].GetRect().GetWidth();		
	float _NoHeight1 =			m_QuitElements[QUIT_NO_ELEMENT].GetRect().GetHeight();		
	float _NoPosX1 =			m_QuitElements[QUIT_NO_ELEMENT].GetPosX();		
	float _NoPosY1 =			m_QuitElements[QUIT_NO_ELEMENT].GetPosY();		
	float _NoCombinedX1 =		_NoPosX1 + _NoWidth1;						
	float _NoCombinedY1 =		_NoPosY1 + _NoHeight1;						

	if (_mouseX >= _NoPosX1 && _mouseX <= _NoCombinedX1 && _mouseY <= _NoCombinedY1 && _mouseY >= _NoPosY1)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if (m_bMouseActive)
			{
				m_Choice = QUIT_NO;
				if (m_bSoundNo)
				{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_bSoundYes = true;
					m_bSoundNo = false;
				}
			}
			if (m_Timer > 0.5f)
			{
				if (_input.IsLeftClicked())
				{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

					m_bFadeOut = true;
				}
			}
		}
	}
#pragma endregion

	//KEYBOARD INPUT FOR SELECTING BUTTONS----------------------------------------------------------------------	
#pragma region Keyboard Input for Selecting Buttons

	if(_input.KeyDown(DIK_A) || _input.KeyDown(DIK_LEFT) || _input.GetController().GetControllerState().Gamepad.sThumbLY < - 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT )
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedD_Q)
			{
				m_Choice++;
				if(m_Choice == QUIT_NUM_CHOICES)
				{
					m_Choice = QUIT_YES;
				}
				ifPressedD_Q = true;
			}	
		}
	}
	else
		ifPressedD_Q = false;

	if(_input.KeyDown(DIK_D) || _input.KeyDown(DIK_RIGHT) || _input.GetController().GetControllerState().Gamepad.sThumbLY > 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if (m_FadeTimer <= 1.0f && !m_bFadeOut)
		{
			if(!ifPressedU_Q)
			{
				m_Choice--;
				if(m_Choice < 0)
				{
					m_Choice = QUIT_NO;
				}
				ifPressedU_Q = true;
			}
		}
	}
	else
		ifPressedU_Q = false;

	if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
	{
		if(!ifPressedEnter_Q)
		{
			m_bFadeOut = true;
			ifPressedEnter_Q = true;
		}
	}
	else
		ifPressedEnter_Q = false;
#pragma endregion


	if (m_Choice == QUIT_YES)
	{
		if (m_bSoundYes)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundYes = false;
			m_bSoundNo = true;
		}
	}
	if (m_Choice == QUIT_NO)
	{
		if (m_bSoundNo)
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

			m_bSoundYes = true;
			m_bSoundNo = false;
		}
	}

	if (m_Choice == QUIT_YES)
	{
		m_bYes = false;
		m_bNo = true;
	}
	if (m_Choice == QUIT_NO)
	{
		m_bYes = true;
		m_bNo = false;
	}
	return true;
}

void CQuitGameState::Update(float _fElapsedTime)
{
	m_Timer += _fElapsedTime;

	if(!m_bFadeOut && m_FadeTimer <= 1.0f)
	{
		FadeIn(_fElapsedTime);
	}
	if (m_bFadeOut)
	{
		FadeOut(_fElapsedTime);	
	}
}

void CQuitGameState::Render()
{
	m_Renderer->EnableDraw();

	m_Renderer->DrawTexture(&m_QuitElements[QUIT_BACKGROUND_ELEMENT]);

	if (m_bYes)
		m_Renderer->DrawTexture(&m_QuitElements[QUIT_YES_ELEMENT]);
	if (!m_bYes)
		m_Renderer->DrawTexture(&m_QuitElements[QUIT_YES_HIGHLIGHTED_ELEMENT]);
	if (m_bNo)
		m_Renderer->DrawTexture(&m_QuitElements[QUIT_NO_ELEMENT]);
	if (!m_bNo)
		m_Renderer->DrawTexture(&m_QuitElements[QUIT_NO_HIGHLIGHTED_ELEMENT]);

	m_Renderer->DrawTexture(&m_QuitElements[QUIT_TEXT_ELEMENT]);

	//DRAW Reticle
	m_Renderer->DrawTexture( &m_reticle );
	m_Renderer->DisableDraw();
}

void CQuitGameState::FadeIn(float _fTime)
{
	m_FadeTimer -= _fTime;		

	for(int i = 0; i < QUIT_NUM_ELEMENTS; ++i)
		m_QuitElements[i].SetOpacity(1.0f - m_FadeTimer);     

	if (m_FadeTimer <= 0.0f)
		m_FadeTimer = 0.0f;
}

void CQuitGameState::FadeOut(float _fTime)
{
	m_FadeTimer += _fTime;					

	for(int i = 0; i < QUIT_NUM_ELEMENTS; ++i)
		m_QuitElements[i].SetOpacity(1.0f - m_FadeTimer);     
}
