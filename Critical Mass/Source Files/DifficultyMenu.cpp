#include "../Header Files/stdafx.h"
#include "../Header Files/DifficultyMenu.h"
/*****************************************************************
File name:			DifficultyMenu.cpp
Date:				7/17/13
Modify Date:		7/17/13
Modify Initials:	IA
Author:				Ian Alcid
Purpose:			Class for Difficulty state.
******************************************************************/
#include "../Header Files/Input.h"
#include "../Header Files/Game.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../../Includes/Sound/Source/Entity3D.h"

#include <string>
using namespace std;

CDifficultyMenu* CDifficultyMenu::GetInstance( void )
{
	static CDifficultyMenu s_Instance;

	return &s_Instance;
}

CDifficultyMenu::CDifficultyMenu(void)
{
	this->SetClosing(false);
}

CDifficultyMenu::~CDifficultyMenu(void)
{

}

void CDifficultyMenu::Init(CGame* _game)
{	
	this->m_Game = _game;
	this->m_Renderer = m_Game->GetRenderer();
	this->m_AM = CAssetManager::GetInstance();
	this->m_Choice = CHOICE_EASY;
	m_PrevMousePosX= 0;
	m_PrevMousePosY= 0;
	m_SoundNormal = true;
	m_SoundHard = true;
	m_SoundBack = true;
	m_Timer = 0.0f;
	m_FadeTimer = 0.0f;
	m_FadeOut = false;

	SetID( DIFFICULTY_STATE );
}

void CDifficultyMenu::Shutdown()
{
	for( unsigned int i = 0; i < DM_MAX_ELEMENTS; ++i )
	{
		m_DMElements[i].ReleaseGLBatch();
	}
	m_reticle.ReleaseGLBatch();
}

bool CDifficultyMenu::Input( CInput& _input )
{	
	_input.ReadMouse();
	_input.ReadKeyboard();

	static bool ifPressedDown = false;
	static bool ifPressedUp = false;
	static bool ifPressedEnter1 = false;
	static bool ifPressedEnter2 = false;
	//static bool ifPressedEsc = false;

	// Updates the reticle location based on the mouse pos.
	m_reticle.SetPosX(_input.GetMousePosX() - m_reticle.GetRect().GetWidth() * 0.5f);
	m_reticle.SetPosY(_input.GetMousePosY() - m_reticle.GetRect().GetHeight() * 0.5f);
	m_reticle.BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	if(_input.GetMousePosX() == m_PrevMousePosX && _input.GetMousePosY() == m_PrevMousePosY)
		m_MouseActive = true;

	if(_input.GetMousePosX() != m_PrevMousePosX || _input.GetMousePosY() != m_PrevMousePosY)
	{
		m_MouseActive = true;
		m_PrevMousePosX = _input.GetMousePosX();
		m_PrevMousePosY = _input.GetMousePosY();
	}

		//Esc (EXIT THE PROGRAM)
	static bool ifPressedEsc_Cred = false;
	if(!ifPressedEsc_Cred)
	{
		if(_input.KeyDown(DIK_ESCAPE))
		{
			m_Choice = CHOICE_BACK;
			m_FadeOut = true;
			ifPressedEsc_Cred = true;
			return true;
		}
	}
	else
		ifPressedEsc_Cred = false;

#pragma region Keyboard inputs
	if(_input.KeyDown(DIK_S) || _input.KeyDown(DIK_DOWN) || _input.GetController().GetControllerState().Gamepad.sThumbLY < - 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN )
	{
		if(m_FadeTimer <= 1.0f && !m_FadeOut)
		{
			if(!ifPressedDown)
			{
				m_Choice++;
				if(m_Choice == CHOICE_TOTAL)
				{
					m_Choice = CHOICE_EASY;
				}
				ifPressedDown = true;
			}
		}
	}
	else
		ifPressedDown = false;

	if(_input.KeyDown(DIK_W) || _input.KeyDown(DIK_UP) || _input.GetController().GetControllerState().Gamepad.sThumbLY > 8000 || _input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		if(m_FadeTimer <= 1.0f && !m_FadeOut)
		{
			if(!ifPressedUp)
			{
				m_Choice--;
				if(m_Choice < 0)
				{
					m_Choice = CHOICE_BACK;
				}
				ifPressedUp = true;
			}
		}
	}
	else
		ifPressedUp = false;
#pragma endregion

	switch(m_Choice)
	{
	case CHOICE_EASY:
		{
			if(m_SoundEasy)
			{
//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

				m_SoundEasy = false;
				m_SoundNormal = true;
				m_SoundHard = true;
				m_SoundBack = true;
				m_isHighlighted = true;
			}
		}
		break;
	case CHOICE_NORMAL:
		{
			if(m_SoundNormal)
			{
//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

				m_SoundEasy = true;
				m_SoundNormal = false;
				m_SoundHard = true;
				m_SoundBack = true;
				m_isHighlighted = true;
			}

		}
		break;
	case CHOICE_HARD:
		{
			if(m_SoundHard)
			{
//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

				m_SoundEasy = true;
				m_SoundNormal = true;
				m_SoundHard = false;
				m_SoundBack = true;
				m_isHighlighted = true;
			}
		}
		break;
	case CHOICE_BACK:
		{
			if(m_SoundBack)
			{
//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

				m_SoundEasy = true;
				m_SoundNormal = true;
				m_SoundHard = true;
				m_SoundBack = false;
				m_isHighlighted = true;
			}
		}
		break;
	};

	if(_input.KeyDown(DIK_RETURN) || (_input.GetController().GetControllerState().Gamepad.wButtons & XINPUT_GAMEPAD_A) )
	{
		if(!ifPressedEnter1)
		{
			m_FadeOut = true;
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

			ifPressedEnter1 = true;
		}
	}
	else
		ifPressedEnter1 = false;

#pragma region Mouse Input
	int _mX = _input.GetMousePosX();
	int _mY = _input.GetMousePosY();
	static bool ifLeftClickEasy = false;
	static bool ifLeftClickMedium = false;
	static bool ifLeftClickHard = false;
	static bool ifLeftClickBack = false;

	// play -> options -> credits
	// EASY
	float _assetwidth		=		m_DMElements[DM_EASY].GetRect().GetWidth();
	float _assetHeight		=		m_DMElements[DM_EASY].GetRect().GetHeight();	
	float _assetPosX		=		m_DMElements[DM_EASY].GetPosX();		
	float _assetPosY		=		m_DMElements[DM_EASY].GetPosY();		
	float _assetCombinedX	=		_assetPosX + _assetwidth;			
	float _assetCombinedY	=		_assetPosY + _assetHeight;

	if(_input.IsLeftClicked())
	{
		if(!ifLeftClickEasy)
		{
			ifLeftClickEasy = true;
			if( _mX >= _assetPosX && _mX <= _assetCombinedX && _mY <= _assetCombinedY && _mY >= _assetPosY )
			{
				if( m_Timer > 0.5f)
				{
					m_FadeOut = true;
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
				//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::);


				}
			}
		}
	}
	else
	{
		ifLeftClickEasy = false;
	}


	if (_mX >= _assetPosX && _mX <= _assetCombinedX && _mY <= _assetCombinedY && _mY >= _assetPosY )
	{
		if(m_FadeTimer <= 1.0f && !m_FadeOut)
		{
			if(m_MouseActive)
			{
				m_Choice = CHOICE_EASY;
				if(m_SoundEasy)
				{
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_SoundEasy = false;
					m_SoundNormal = true;
					m_SoundHard = true;
					m_SoundBack = true;
					m_isHighlighted = true;
				}
			}
		}
	}
	// NORMAL
	_assetwidth		=		m_DMElements[DM_NORMAL].GetRect().GetWidth();
	_assetHeight	=		m_DMElements[DM_NORMAL].GetRect().GetHeight();
	_assetPosX		=		m_DMElements[DM_NORMAL].GetPosX();	
	_assetPosY		=		m_DMElements[DM_NORMAL].GetPosY();	
	_assetCombinedX	=		_assetPosX + _assetwidth;			
	_assetCombinedY	=		_assetPosY + _assetHeight;

	if(_input.IsLeftClicked())
	{
		if(!ifLeftClickMedium)
		{
			ifLeftClickMedium = true;
			if( _mX >= _assetPosX && _mX <= _assetCombinedX && _mY <= _assetCombinedY && _mY >= _assetPosY )
			{
				if( m_Timer > 0.5f)
				{
					m_FadeOut = true;
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}
	else
	{
		ifLeftClickMedium = false;
	}

	if (_mX >= _assetPosX && _mX <= _assetCombinedX && _mY <= _assetCombinedY && _mY >= _assetPosY )
	{
		if(m_FadeTimer <= 1.0f && !m_FadeOut)
		{
			if(m_MouseActive)
			{
				m_Choice = CHOICE_NORMAL;
				if(m_SoundNormal)
				{
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_SoundEasy = true;
					m_SoundNormal = false;
					m_SoundHard = true;
					m_SoundBack = true;
					m_isHighlighted = true;
				}
			}
		}
	}

	// HARD
	_assetwidth		=		m_DMElements[DM_HARD].GetRect().GetWidth();
	_assetHeight	=		m_DMElements[DM_HARD].GetRect().GetHeight();
	_assetPosX		=		m_DMElements[DM_HARD].GetPosX();	
	_assetPosY		=		m_DMElements[DM_HARD].GetPosY();	
	_assetCombinedX	=		_assetPosX + _assetwidth;			
	_assetCombinedY	=		_assetPosY + _assetHeight;

	if(_input.IsLeftClicked())
	{
		if(!ifLeftClickHard)
		{
			ifLeftClickHard = true;
			if( _mX >= _assetPosX && _mX <= _assetCombinedX && _mY <= _assetCombinedY && _mY >= _assetPosY )
			{
				if( m_Timer > 0.5f)
				{
					m_FadeOut = true;
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}
	else
	{
		ifLeftClickHard = false;
	}

	if (_mX >= _assetPosX && _mX <= _assetCombinedX && _mY <= _assetCombinedY && _mY >= _assetPosY )
	{
		if(m_FadeTimer <= 1.0f && !m_FadeOut)
		{
			if(m_MouseActive)
			{
				m_Choice = CHOICE_HARD;
				if(m_SoundHard)
				{
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_SoundEasy = true;
					m_SoundNormal = true;
					m_SoundHard = false;
					m_SoundBack = true;
					m_isHighlighted = true;
				}
			}
		}
	}

	// BACK
	_assetwidth		=		m_DMElements[DM_BACK].GetRect().GetWidth();
	_assetHeight	=		m_DMElements[DM_BACK].GetRect().GetHeight();
	_assetPosX		=		m_DMElements[DM_BACK].GetPosX();	
	_assetPosY		=		m_DMElements[DM_BACK].GetPosY();	
	_assetCombinedX	=		_assetPosX + _assetwidth;			
	_assetCombinedY	=		_assetPosY + _assetHeight;

	if(_input.IsLeftClicked())
	{
		if(!ifLeftClickBack)
		{
			ifLeftClickBack = true;
			if( _mX >= _assetPosX && _mX <= _assetCombinedX && _mY <= _assetCombinedY && _mY >= _assetPosY )
			{
				if( m_Timer > 0.5f)
				{
					m_FadeOut = true;
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

				}
			}
		}
	}
	else
	{
		ifLeftClickBack = false;
	}

	if (_mX >= _assetPosX && _mX <= _assetCombinedX && _mY <= _assetCombinedY && _mY >= _assetPosY )
	{
		if(m_FadeTimer <= 1.0f && !m_FadeOut)
		{
			if(m_MouseActive)
			{
				m_Choice = CHOICE_BACK;
				if(m_SoundBack)
				{
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MENU_HOVER);

					m_SoundEasy = true;
					m_SoundNormal = true;
					m_SoundHard = true;
					m_SoundBack = false;
					m_isHighlighted = true;
				}
			}
		}
	}
#pragma endregion

	return  true;
}

void CDifficultyMenu::Update( float fElapsedTime )
{	 
	m_Timer += fElapsedTime;

	if(!m_FadeOut && m_FadeTimer <= 1.0f)
	{
		m_FadeTimer -= fElapsedTime;											//count up to 1
		for(int i = 0; i < DM_MAX_ELEMENTS; ++i)
			m_DMElements[i].SetOpacity(1.0f - m_FadeTimer);     	// set the Opacity to that timer	
		if (m_FadeTimer <= 0.0f)
			m_FadeTimer = 0.0f;
	}
	if (m_FadeOut)
	{
		m_FadeTimer += fElapsedTime;											//count down to 0
		for(int i = 0; i < DM_MAX_ELEMENTS; ++i)
			m_DMElements[i].SetOpacity(1.0f - m_FadeTimer);       	// set the Opacity to that timer	
	}

	if(m_FadeOut && m_FadeTimer >= 1.0f )
	{
		switch(m_Choice)
		{
		case CHOICE_EASY:
			{
				m_Game->SetDifficulty(EASY_DIFF);
				m_Game->GetStateManager()->PushState(TUTORIAL_STATE);
			}
			break;
		case CHOICE_NORMAL:
			{
				m_Game->SetDifficulty(NORMAL_DIFF);
				m_Game->GetStateManager()->PushState(TUTORIAL_STATE);
			}
			break;
		case CHOICE_HARD:
			{
				m_Game->SetDifficulty(HARD_DIFF);
				m_Game->GetStateManager()->PushState(TUTORIAL_STATE);
			}
			break;
		case CHOICE_BACK:
			{
				this->SetClosing(true);
				return;
			}
			break;
		};
	}


}

void CDifficultyMenu::Render()
{
	m_Renderer->EnableDraw();
	//m_Renderer->DrawTexture(&m_DMElements[DM_BACKGROUND]);

	m_Renderer->DrawTexture(&m_DMElements[GAME_LOGO]);

	if(m_Choice == CHOICE_EASY && m_isHighlighted)
		m_Renderer->DrawTexture(&m_DMElements[DM_EASY_HL]);
	else
		m_Renderer->DrawTexture(&m_DMElements[DM_EASY]);

	if(m_Choice == CHOICE_NORMAL && m_isHighlighted)
		m_Renderer->DrawTexture(&m_DMElements[DM_NORMAL_HL]);
	else 
		m_Renderer->DrawTexture(&m_DMElements[DM_NORMAL]);

	if(m_Choice == CHOICE_HARD && m_isHighlighted)
		m_Renderer->DrawTexture(&m_DMElements[DM_HARD_HL]);
	else 
		m_Renderer->DrawTexture(&m_DMElements[DM_HARD]);

	if(m_Choice == CHOICE_BACK && m_isHighlighted)
		m_Renderer->DrawTexture(&m_DMElements[DM_BACK_HL]);
	else 
		m_Renderer->DrawTexture(&m_DMElements[DM_BACK]);

	//if(!m_FadeOut && m_FadeTimer <= 1.0f)
	//	m_Renderer->DrawTexture(&m_DMElements[DM_BLACK_BG]);

	//if(m_FadeOut)
	//	m_Renderer->DrawTexture(&m_DMElements[DM_BLACK_BG]);

	//DRAW Reticle

	if(m_Game->GetUsingController() == false)
	{
		m_Renderer->DrawTexture(&m_DMElements[DM_ALL3_ELEMENT]);
	}
	else if(m_Game->GetUsingController() == true)
	{
		m_Renderer->DrawTexture(&m_DMElements[DM_ALL3_CONTROLLER_ELEMENT]);
	}

	if(!m_FadeOut)
		m_Renderer->DrawTexture( &m_reticle );
	m_Renderer->DisableDraw();
}

void CDifficultyMenu::Enter()
{
	this->m_Choice = CHOICE_EASY;
	//ShowCursor(TRUE);
	m_isHighlighted = true;
	m_Timer = 0.0f;
	m_FadeTimer = 1.0f;
	m_FadeOut = false;

	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_DMElements[GAME_LOGO].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(M_CRITICALMASS_MM_BACKGROUND_1280x720), 
		RECTANGLE(0.0f, 640.0f, 0.0f, 360.0f), 0.0f, 0.0f, 0.0f, 1.0f, white);

#if defined(_DEBUG) || defined(_BETA)
	/*m_DMElements[DM_BACKGROUND].SetScreenHeight(m_Game->GetScreenHeight());
	m_DMElements[DM_BACKGROUND].SetScreenWidth(m_Game->GetScreenWidth());
	m_DMElements[DM_BACKGROUND].SetHeight(float(m_Game->GetScreenHeight()));
	m_DMElements[DM_BACKGROUND].SetWidth(float(m_Game->GetScreenWidth()));
	m_DMElements[DM_BACKGROUND].SetPosX(0.0f);
	m_DMElements[DM_BACKGROUND].SetPosY(0.0f);
	m_DMElements[DM_BACKGROUND].BuildGeometry();
	m_DMElements[DM_BACKGROUND].SetTextureHandle(m_AM->GetTexture(M_DIFFICULTY_BACKGROUND_1024x768));*/
	m_DMElements[DM_BACKGROUND].Setup(m_Renderer, m_AM->GetTexture(M_DIFFICULTY_BACKGROUND_1024x768), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);
#else
	//m_DMElements[DM_BACKGROUND].SetScreenHeight(m_Game->GetScreenHeight());
	//m_DMElements[DM_BACKGROUND].SetScreenWidth(m_Game->GetScreenWidth());
	//m_DMElements[DM_BACKGROUND].SetHeight(float(m_Game->GetScreenHeight()));
	//m_DMElements[DM_BACKGROUND].SetWidth(float(m_Game->GetScreenWidth()));
	//m_DMElements[DM_BACKGROUND].SetPosX(0.0f);
	//m_DMElements[DM_BACKGROUND].SetPosY(0.0f);
	//m_DMElements[DM_BACKGROUND].BuildGeometry();
	//m_DMElements[DM_BACKGROUND].SetTextureHandle(m_AM->GetTexture(M_DIFFICULTY_BACKGROUND_1280x720));
	m_DMElements[DM_BACKGROUND].Setup(m_Renderer, m_AM->GetTexture(M_DIFFICULTY_BACKGROUND_1280x720), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);
#endif
	/*m_DMElements[DM_BLACK_BG].SetScreenHeight(m_Game->GetScreenHeight());
	m_DMElements[DM_BLACK_BG].SetScreenWidth(m_Game->GetScreenWidth());
	m_DMElements[DM_BLACK_BG].SetHeight(float(m_Game->GetScreenHeight()));
	m_DMElements[DM_BLACK_BG].SetWidth(float(m_Game->GetScreenWidth()));
	m_DMElements[DM_BLACK_BG].SetPosX(0.0f);
	m_DMElements[DM_BLACK_BG].SetPosY(0.0f);
	m_DMElements[DM_BLACK_BG].BuildGeometry();
	m_DMElements[DM_BLACK_BG].SetTextureHandle(m_AM->GetTexture(M_BLACK_BACKGROUND));*/
	m_DMElements[DM_BLACK_BG].Setup(m_Renderer, m_AM->GetTexture(M_BLACK_BACKGROUND), 
		RECTANGLE(0.0f, (float)m_Game->GetScreenWidth(), 0.0f, (float)m_Game->GetScreenHeight()), 0.0f, 0.0f, 0.0f, 720.0f / m_Renderer->GetScreenHeight(), white);

#pragma region Normal Buttons
	//m_DMElements[DM_EASY].SetScreenHeight(m_Game->GetScreenHeight());
	//m_DMElements[DM_EASY].SetScreenWidth(m_Game->GetScreenWidth());
	//m_DMElements[DM_EASY].SetHeight(64.0f);
	//m_DMElements[DM_EASY].SetWidth(256.0f);
	//m_DMElements[DM_EASY].SetPosX(float((m_Game->GetScreenWidth() * 0.5f) + 128.0f));			
	//m_DMElements[DM_EASY].SetPosY(((float)m_Game->GetScreenHeight() * 0.5f) -256);	
	//m_DMElements[DM_EASY].BuildGeometry();
	//m_DMElements[DM_EASY].SetTextureHandle(m_AM->GetTexture(BUTTON_EASY));
	m_DMElements[DM_EASY].Setup(m_Renderer, m_AM->GetTexture(BUTTON_EASY), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() * 0.5f) + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() * 0.5f) - 233 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	/*m_DMElements[DM_NORMAL].SetScreenHeight(m_Game->GetScreenHeight());
	m_DMElements[DM_NORMAL].SetScreenWidth(m_Game->GetScreenWidth());
	m_DMElements[DM_NORMAL].SetHeight(64.0f);
	m_DMElements[DM_NORMAL].SetWidth(256.0f);
	m_DMElements[DM_NORMAL].SetPosX(float((m_Game->GetScreenWidth() * 0.5f) + 128.0f));			
	m_DMElements[DM_NORMAL].SetPosY(((float)m_Game->GetScreenHeight() * 0.5f) -128.0f);
	m_DMElements[DM_NORMAL].BuildGeometry();
	m_DMElements[DM_NORMAL].SetTextureHandle(m_AM->GetTexture(BUTTON_NORMAL));*/
	m_DMElements[DM_NORMAL].Setup(m_Renderer, m_AM->GetTexture(BUTTON_NORMAL), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() * 0.5f) + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() * 0.5f) - 105.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_DMElements[DM_HARD].SetScreenHeight(m_Game->GetScreenHeight());
	//m_DMElements[DM_HARD].SetScreenWidth(m_Game->GetScreenWidth());
	//m_DMElements[DM_HARD].SetHeight(64.0f);
	//m_DMElements[DM_HARD].SetWidth(256.0f);
	//m_DMElements[DM_HARD].SetPosX(float((m_Game->GetScreenWidth() * 0.5f) + 128.0f));	
	//m_DMElements[DM_HARD].SetPosY(((float)m_Game->GetScreenHeight() * 0.5f ));
	//m_DMElements[DM_HARD].BuildGeometry();
	//m_DMElements[DM_HARD].SetTextureHandle(m_AM->GetTexture(BUTTON_HARD));
	m_DMElements[DM_HARD].Setup(m_Renderer, m_AM->GetTexture(BUTTON_HARD), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() * 0.5f) + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() * 0.5f) + 25 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_DMElements[DM_BACK].SetScreenHeight(m_Game->GetScreenHeight());
	//m_DMElements[DM_BACK].SetScreenWidth(m_Game->GetScreenWidth());
	//m_DMElements[DM_BACK].SetHeight(64.0f);
	//m_DMElements[DM_BACK].SetWidth(256.0f);
	//m_DMElements[DM_BACK].SetPosX(float((m_Game->GetScreenWidth() * 0.5f) + 128.0f));	
	//m_DMElements[DM_BACK].SetPosY(((float)m_Game->GetScreenHeight() * 0.5f ) + 128.0f);
	//m_DMElements[DM_BACK].BuildGeometry();
	//m_DMElements[DM_BACK].SetTextureHandle(m_AM->GetTexture(BUTTON_BACK));
	m_DMElements[DM_BACK].Setup(m_Renderer, m_AM->GetTexture(BUTTON_BACK), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() * 0.5f) + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() * 0.5f ) + 155.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
#pragma endregion

#pragma region Highlighted Buttons
	//m_DMElements[DM_EASY_HL].SetScreenHeight(m_Game->GetScreenHeight());
	//m_DMElements[DM_EASY_HL].SetScreenWidth(m_Game->GetScreenWidth());
	//m_DMElements[DM_EASY_HL].SetHeight(64.0f);
	//m_DMElements[DM_EASY_HL].SetWidth(256.0f);
	//m_DMElements[DM_EASY_HL].SetPosX(float((m_Game->GetScreenWidth() * 0.5f) + 128.0f));			
	//m_DMElements[DM_EASY_HL].SetPosY(((float)m_Game->GetScreenHeight() * 0.5f) -256.0f);	
	//m_DMElements[DM_EASY_HL].BuildGeometry();
	//m_DMElements[DM_EASY_HL].SetTextureHandle(m_AM->GetTexture(BUTTON_EASY_HIGHLIGHTED));
	m_DMElements[DM_EASY_HL].Setup(m_Renderer, m_AM->GetTexture(BUTTON_EASY_HIGHLIGHTED), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() * 0.5f) + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() * 0.5f) -233.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_DMElements[DM_NORMAL_HL].SetScreenHeight(m_Game->GetScreenHeight());
	//m_DMElements[DM_NORMAL_HL].SetScreenWidth(m_Game->GetScreenWidth());
	//m_DMElements[DM_NORMAL_HL].SetHeight(64.0f);
	//m_DMElements[DM_NORMAL_HL].SetWidth(256.0f);
	//m_DMElements[DM_NORMAL_HL].SetPosX(float((m_Game->GetScreenWidth() * 0.5f) + 128.0f));			
	//m_DMElements[DM_NORMAL_HL].SetPosY(((float)m_Game->GetScreenHeight() * 0.5f) -128.0f);
	//m_DMElements[DM_NORMAL_HL].BuildGeometry();
	//m_DMElements[DM_NORMAL_HL].SetTextureHandle(m_AM->GetTexture(BUTTON_NORMAL_HIGHLIGHTED));
	m_DMElements[DM_NORMAL_HL].Setup(m_Renderer, m_AM->GetTexture(BUTTON_NORMAL_HIGHLIGHTED), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() * 0.5f) + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() * 0.5f) -105.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_DMElements[DM_HARD_HL].SetScreenHeight(m_Game->GetScreenHeight());
	//m_DMElements[DM_HARD_HL].SetScreenWidth(m_Game->GetScreenWidth());
	//m_DMElements[DM_HARD_HL].SetHeight(64.0f);
	//m_DMElements[DM_HARD_HL].SetWidth(256.0f);
	//m_DMElements[DM_HARD_HL].SetPosX(float((m_Game->GetScreenWidth() * 0.5f) + 128.0f));	
	//m_DMElements[DM_HARD_HL].SetPosY(((float)m_Game->GetScreenHeight() * 0.5f));
	//m_DMElements[DM_HARD_HL].BuildGeometry();
	//m_DMElements[DM_HARD_HL].SetTextureHandle(m_AM->GetTexture(BUTTON_HARD_HIGHLIGHTED));
	m_DMElements[DM_HARD_HL].Setup(m_Renderer, m_AM->GetTexture(BUTTON_HARD_HIGHLIGHTED), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() * 0.5f) + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() * 0.5f) + 25 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_DMElements[DM_BACK_HL].SetScreenHeight(m_Game->GetScreenHeight());
	//m_DMElements[DM_BACK_HL].SetScreenWidth(m_Game->GetScreenWidth());
	//m_DMElements[DM_BACK_HL].SetHeight(64.0f);
	//m_DMElements[DM_BACK_HL].SetWidth(256.0f);
	//m_DMElements[DM_BACK_HL].SetPosX(float((m_Game->GetScreenWidth() * 0.5f) + 128.0f));	
	//m_DMElements[DM_BACK_HL].SetPosY(((float)m_Game->GetScreenHeight() * 0.5f ) + 128.0f);
	//m_DMElements[DM_BACK_HL].BuildGeometry();
	//m_DMElements[DM_BACK_HL].SetTextureHandle(m_AM->GetTexture(BUTTON_BACK_HIGHLIGHTED));
	m_DMElements[DM_BACK_HL].Setup(m_Renderer, m_AM->GetTexture(BUTTON_BACK_HIGHLIGHTED), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		float((m_Game->GetScreenWidth() * 0.5f) + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight() * 0.5f ) + 155.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
#pragma endregion

	m_DMElements[DM_ALL3_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 180 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.75f, white );

	m_DMElements[DM_ALL3_CONTROLLER_ELEMENT].Setup( m_Renderer, CAssetManager::GetInstance()->GetTexture(MENU_ALL3_CONTROLLER),
		RECTANGLE(0.0f, 384.0f, 0.0f, 256.0f),
		float((m_Game->GetScreenWidth() / 2) - 440 * (m_Game->GetScreenHeight() / 720.0f)),
		((float)m_Game->GetScreenHeight()) - 190 * (m_Game->GetScreenHeight() / 720.0f),
		0.0f, 0.75f, white );

	// For reticle
	Vec3f retPos = { m_Game->GetScreenWidth() / 2.0f - 50.0f, m_Game->GetScreenHeight() / 2.0f - 50.0f, 0.0f };
	m_reticle.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(G_RETICULE), RECTANGLE(0.0f, 32, 0.0f, 32), 0.0f, 0.0f, 0.0f, 1.0f, white);

	for(int i = 0; i < DM_MAX_ELEMENTS; ++i)
		m_DMElements[i].SetOpacity(0.0f);
}

void CDifficultyMenu::Exit()
{
	while( ShowCursor( FALSE ) >= 0 );

	for( unsigned int i = 0; i < DM_MAX_ELEMENTS; ++i )
	{
		m_DMElements[i].ReleaseGLBatch();
	}
	m_reticle.ReleaseGLBatch();
}
