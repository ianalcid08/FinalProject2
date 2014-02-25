#include "../Header Files/StdAfx.h"
#include "../Header Files/Game.h"
#include "..\Header Files\IState.h"
#include "..\Header Files\GamePlayState.h"
#include "..\Header Files\TutorialState.h"
#include "..\Header Files\MainMenuState.h"
#include "..\Header Files\AssetManager.h"
#include "..\Header Files\Input.h"
#include "..\Header Files\WindowsForm.h"
#include "..\Header Files\AnimationProcessor.h"
#include "..\Header Files\AnimDebugState.h"
#include "../Header Files/OptionsMenu.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Wwise_IDs.h"
//.........................................................


//#include <iostream>
//#include <fstream>
using namespace std;


IState* ProcessEnum(eGameStates _eIn);

CGame::CGame(void)
{
	m_UsingController = false;
}

CGame::~CGame(void)
{
}

CGame* CGame::GetInstance(void)
{
	static CGame instance;
	return &instance;
}

void CGame::Initialize( HINSTANCE _hInstance, HWND _hWnd, HDC _hdc )
{
	m_AltEnterTimer = 0.0f;

	// Get the default display device.
	DISPLAY_DEVICE dd;
	dd.cb = sizeof( DISPLAY_DEVICE );
	EnumDisplayDevices( NULL, 0, _Out_  &dd, 0 );
	DEVMODE tempDevMode;			// device mode
	EnumDisplaySettings( dd.DeviceName, ENUM_CURRENT_SETTINGS, &tempDevMode );
	m_DefaultResolution.x = tempDevMode.dmPelsWidth;
	m_DefaultResolution.y = tempDevMode.dmPelsHeight;

	ShowCursor(FALSE);
	m_renderer.Initialize();
	CAssetManager::GetInstance()->Init();
	m_cOM.Initialize(&m_renderer,&m_cOF);
	m_cOF.Initialize(&m_cOM);
	m_cTimer.Reset();
	m_Difficulty = NORMAL_DIFF;

	this->m_HDC = _hdc;
	m_hInstance = _hInstance;
	m_hWnd = _hWnd;

	m_Input.Initialize( _hInstance, _hWnd, GetScreenWidth(), GetScreenHeight() );
	// SoundManager
	m_Entity.SetPosition(XMFLOAT3(0.0f,0.0f,0.0f));
	AudioSystemWwise::GetInstance()->Initialize(&m_Entity);
	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_03);				
	m_stateManager.Initialize(this);

	SetIsClosing(false);

	GetDeviceGammaRamp(m_HDC, &m_GammaReturn);

	// For Audio.
	int		m_MusicVolume = 3;
	int		m_SFXVolume = 3;
	int		m_Gamma = 3;
	int		m_MouseSen = 4;

	WCHAR	_szFilePath [4096];
	LPWSTR _returnFilePath = nullptr;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &_returnFilePath );
	wcscpy_s(_szFilePath, _returnFilePath);
	CoTaskMemFree(_returnFilePath);
	wcsncat_s(_szFilePath, L"\\volume.bin", wcslen(L"\\volume.bin"));

	std::ifstream volFile;
	volFile.open(_szFilePath, ios_base::in | ios_base::binary);
	volFile.seekg(0, ios_base::beg);
	if (volFile.is_open())
	{
		volFile.read((char*)&m_MusicVolume, sizeof(int));
		volFile.read((char*)&m_SFXVolume, sizeof(int));
		volFile.read((char*)&m_Gamma, sizeof(int));
		volFile.read((char*)&m_MouseSen, sizeof(int));
	}
	volFile.close();

	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::SFX_VOLUME, (float)m_SFXVolume * 10.0f);
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::DX_VOLUME, (float)m_SFXVolume * 10.0f);
	AudioSystemWwise::GetInstance()->SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, (float)m_MusicVolume * 10.0f);

	//Sets the gamma
	Gamma(m_Gamma, m_GammaArray, m_HDC);
	m_SlowTimer = 0.0f;
}

void CGame::Gamma ( double _gamma, tGamma &_gammaArray, HDC _hdc )
{
	memset(&_gammaArray, 0, sizeof(tGamma));

	_gamma /= 9.0;
	_gamma -= 0.25;
	_gamma *= 1.25;
	//_gamma = ClampGamma(_gamma);

	for (unsigned short index = 0; index < 256; index++)
	{
		_gammaArray.m_GammaColorRed[index] = unsigned short (min (255, index * (_gamma + 1.0))) << 8;
		_gammaArray.m_GammaColorGreen[index] = unsigned short (min (255, index * (_gamma + 1.0))) << 8;
		_gammaArray.m_GammaColorBlue[index] = unsigned short (min (255, index * (_gamma + 1.0))) << 8;
	}
	//bool temp1 = GetDeviceGammaRamp(m_HDC, &m_GammaArray);
	SetDeviceGammaRamp(_hdc, &_gammaArray);
}

void CGame::Shutdown( void )
{
	m_stateManager.Shutdown();
	CAssetManager::GetInstance()->Release();
	m_renderer.Shutdown();
	m_Sound.Shutdown();
	memset(&m_GammaArray, 0, sizeof(tGamma));
	SetDeviceGammaRamp(m_HDC, &m_GammaReturn);		//This Restores the windows gamma
}

bool CGame::Input( void )
{
	m_Input.ReadKeyboard();
	//m_Input.Frame();
	m_Input.ReadMouse();

	if( m_AltEnterTimer > 0.0f )
		return true;

	if( m_Input.IsAltEnterPressed() )
	{
		static POINT altEnterRes = { m_DefaultResolution.x, m_DefaultResolution.y };

		if( m_renderer.GetFullScreen() )
		{
			// Get the default display device.
			DISPLAY_DEVICE dd;
			dd.cb = sizeof( DISPLAY_DEVICE );
			EnumDisplayDevices( NULL, 0, _Out_  &dd, 0 );
			DEVMODE tempDevMode;			// device mode
			EnumDisplaySettings( dd.DeviceName, ENUM_CURRENT_SETTINGS, &tempDevMode );
			altEnterRes.x = tempDevMode.dmPelsWidth;
			altEnterRes.y = tempDevMode.dmPelsHeight;

			ChangeResolution( m_DefaultResolution.x, m_DefaultResolution.y );
			SetWindowLongPtr( m_hWnd, GWL_STYLE, WS_POPUPWINDOW | WS_CAPTION );
			SetWindowPos( m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
			
			RECT clientRect;
			GetClientRect( m_hWnd, &clientRect );
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;
			m_renderer.ResizeWindow( width, height );

			m_renderer.SetFullScreen( false );
		}
		else
		{
			ChangeResolution( altEnterRes.x, altEnterRes.y );
			SetWindowLongPtr( m_hWnd, GWL_STYLE, WS_POPUP );
			SetWindowPos( m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW  | SWP_NOSIZE | SWP_NOMOVE );

			RECT clientRect;
			GetClientRect( m_hWnd, &clientRect );
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;
			m_renderer.ResizeWindow( width, height );

			m_renderer.SetFullScreen( true );
		}

		CGameplayState::GetInstance()->ResetUI();
		CGameplayState::GetInstance()->GetHUD()->Release();
		CGameplayState::GetInstance()->GetHUD()->Initialize( &m_renderer );

		CTutorialState::GetInstance()->ResetUI();
		CTutorialState::GetInstance()->GetHUD()->Release();
		CTutorialState::GetInstance()->GetHUD()->Initialize( &m_renderer );

		eGameStates ID = (eGameStates)m_stateManager.CetCurrStateID();
		if( ID != GAMEPLAY_STATE && ID != TUTORIAL_STATE )
		{
			if( ID == OPTIONS_STATE )
			{
				m_stateManager.PopState();
				m_stateManager.PushState( ID );
			}
			else
			{
				m_stateManager.ChangeState( ID );
			}
		}

		m_AltEnterTimer = 0.25f;

		return true;
	}

	//if( GetAsyncKeyState('F') & 0x01 )
	//{
	//	cout << this->m_cTimer.GetFPS() << endl;
	//}

	if( m_Input.KeyDown( DIK_F1 ) )
		GetStateManager()->PushState(HELP_STATE);

	//if( GetAsyncKeyState('9') & 0x01 )
		//m_stateManager.ChangeState(ANIM_STATE);

	if(!m_stateManager.GetNumCurrStates())    // If there is no state, leave;
		return false;

	m_stateManager.Input(m_Input);

	return true;
}

void CGame::Update( void )
{
	m_Sound.Update();

	if(!m_stateManager.GetNumCurrStates())    // If there is no state, leave;
		return;

	m_fDelta = m_cTimer.GetDeltaTime();
	m_cTimer.Update();

	if( m_fDelta > 0.125f)
	{
		m_fDelta = 0.125f;
	}

	if( m_SlowTimer > 0.0f )
	{
		m_SlowTimer -= m_fDelta;
		m_fDelta = m_fDelta / TIMER_SLOW_RATE;
	}

	m_AltEnterTimer -= m_fDelta;

	m_stateManager.Update( m_fDelta);
}

void CGame::Render( void )
{
	m_renderer.DrawRenderList();

	//for(auto iter = m_CurrState.begin(); iter != m_CurrState.end(); ++iter)
	//{
	//	(*iter)->Render();
	//}

	if(!m_stateManager.GetNumCurrStates())    // If there is no state, leave;
		return;

	m_stateManager.Render();
	//m_renderer.DrawUI();
}

bool CGame::Play(void)
{
	//if(!Input() || !(CWindowsForm::GetInstance()->GetIsRunning()))
		//return false;
	Input();
	Update();
	Render();

	return true;
}


#pragma region STATE_MANAGER_FUNCTIONALITY_MOVED_TO_STATE_MANAGER_BY_TOM_STEFL


//void CGame::ChangeState(eGameStates _eNewState)
//{
//	// Exit the current state (if any)
//	//if (m_CurrState.size() != 0)
//	//{
//	//	for (auto iter = m_CurrState.rbegin(); iter != m_CurrState.rend(); ++iter)
//	//		(*iter)->Shutdown();
//	//	m_CurrState.clear();
//	//}
//
//	//IState* pNewState = ProcessEnum(_eNewState);
//
//	//// Assign the current state
//	//m_CurrState.push_back(pNewState);
//
//	//// Enter the new state (if any)
//	//if (m_CurrState.back() != nullptr)
//	//	m_CurrState.back()->Initialize();
//	
//}
//void CGame::PushState(eGameStates _eNewState)
//{
//	IState* pNewState = ProcessEnum(_eNewState);
//
//	for (auto iter = m_CurrState.begin(); iter != m_CurrState.end(); ++iter) 
//	{
//		if ((*iter) == pNewState) 
//		{
//			m_CurrState.erase(iter);
//			m_CurrState.push_back(pNewState);
//			return;
//		}
//	}
//	m_CurrState.push_back(pNewState);
//
//	// Enter the new state (if any)
//	if (m_CurrState.back() != nullptr)
//	{
//		m_CurrState.back()->Init(this);
//	}
//}
//void CGame::PopState()
//{
//		// If only 1 State is on the Stack dont remove it
//	if (m_CurrState.size() < 2)
//		return;
//
//	// Exit the current state
//	//m_CurrState.back()->Shutdown();
//
//	// Remove the Current state
//	m_CurrState.pop_back();
//}
//IState* ProcessEnum(eGameStates _eIn)
//{
//	IState* pOut = nullptr;
//
//	switch(_eIn)
//	{
//	case INTRO_STATE:
//		{
//			//pOut = CIntroState::GetInstance();
//		}
//		break;
//	case MAINMENU_STATE:
//		{
//			//pOut = CMainMenuState::GetInstance();
//		}
//		break;
//	case OPTIONS_STATE:
//		{
//			//pOut = COptionsState::GetInstance();
//		}
//		break;
//	case CREDITS_STATE:
//		{
//			//pOut = CCreditsState::GetInstance();
//		}
//		break;
//	case PAUSE_STATE:
//		{
//			//pOut = CPauseState::GetInstance();
//		}
//		break;
//	case GAMEPLAY_STATE:
//		{
//			pOut = CGameplayState::GetInstance();
//		}
//		break;
//	case ANIM_STATE:
//		{
//			pOut = CAnimDebugState::GetInstance();
//		}
//		break;
//	}
//	return pOut;
//}

#pragma endregion


void CGame::ChangeResolution( unsigned int _width, unsigned int _height )
{
	DEVMODE devmode;

	devmode.dmPelsHeight = _height;
	devmode.dmPelsWidth = _width;
	devmode.dmSize = sizeof(DEVMODE);
	devmode.dmBitsPerPel = 32;
	devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	ChangeDisplaySettings( &devmode, 0 );
	ResizeWindow( _width, _height );
	SetWindowPos( m_hWnd, HWND_TOP, 0, 0, _width, _height, 0 );

	CGameplayState::GetInstance()->GetHUD()->Release();
	CGameplayState::GetInstance()->GetHUD()->Rebuild( &m_renderer );
	CGameplayState::GetInstance()->ResetUI();
}
