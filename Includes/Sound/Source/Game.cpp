//--------------------------------------------------------------------------------
// Written by Justin Murphy
//--------------------------------------------------------------------------------
#include "Game.h"
#include "InputManager.h"
#include "MainWindow.h"
#include "Renderer.h"
#include "Wwise_IDs.h"
#include "Camera.h"
//--------------------------------------------------------------------------------
bool CGame::m_bShowCursor = true;
bool CGame::m_bKeepCursorPos = true;
//--------------------------------------------------------------------------------
CGame::CGame()  : CApplication()
{
	m_szWindowTitle = L"Murphy Wwise Integration Tutorial";
	m_nWindowWidth = 1280;
	m_nWindowHeight = 800;
}
//--------------------------------------------------------------------------------
void CGame::SceneInit()
{
	// initialize DX
	Renderer::InitDevice(MainWindow()->GetHWnd());

	// Start tracking needed input
	InputManager()->ActivateKey(VK_UP);
	InputManager()->ActivateKey(VK_DOWN);
	InputManager()->ActivateKey(VK_RIGHT);
	InputManager()->ActivateKey(VK_LEFT);
	InputManager()->ActivateKey(VK_SPACE);
	InputManager()->ActivateKey('1');
	InputManager()->ActivateKey('2');
	InputManager()->ActivateKey('3');
	InputManager()->ActivateKey(VK_ESCAPE);

    // Initialize the view
	m_pCamera = new Camera();
	m_pCamera->LookAt(XMFLOAT3(0.0f, 1.0f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pCamera->SetLens( XM_PIDIV2, MainWindow()->GetWindowWidth()/(FLOAT)MainWindow()->GetWindowHeight(), 0.01f, 100.0f );

	g_AudioSystem.Initialize();
	g_AudioSystem.SetBasePath( L"../Resources/SoundBanks/" );
	g_AudioSystem.LoadSoundBank(L"Init.bnk");
	g_AudioSystem.LoadSoundBank(L"SoundBank.bnk");
	g_AudioSystem.RegisterListener(m_pCamera,"Main Camera");
	g_AudioSystem.RegisterEntity(&m_Entity2, "Main Entity");
	g_AudioSystem.PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_01);
	g_AudioSystem.PostEvent(AK::EVENTS::PLAY_FX_3D_CAR_LOOP,&m_Entity2);
}
//--------------------------------------------------------------------------------
void CGame::Update( float timeDelta, float totalTime )
{
	// Adjust Music Volume
	static float volume = 50.f;
	if(InputManager()->GetKeyDown(VK_UP))
	{
		if(volume < 100.f)
			volume += timeDelta * 20.f;
		g_AudioSystem.SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, volume);
	}
	if(InputManager()->GetKeyDown(VK_DOWN))
	{
		if(volume > 0.f)
			volume -= timeDelta * 20.f;
		g_AudioSystem.SetRTCPValue(AK::GAME_PARAMETERS::MX_VOLUME, volume);
	}
	
	//Adjust Audio Scale
	//NOTE: only do this to get the placeholder bank integrated with your engine,
	//once you are ready to recieve a real soundbank tell the Sound Engineer the scale you are usingwww
	static float AudioScaling = 1.f;
	if(InputManager()->GetKeyDown(VK_RIGHT))
	{
		if(AudioScaling < 100.f)
			AudioScaling += timeDelta * 0.1f;
		g_AudioSystem.SetWorldScale( AudioScaling );
	}
	if(InputManager()->GetKeyDown(VK_LEFT))
	{
		if(AudioScaling > 0.f)
			AudioScaling -= timeDelta * 0.1f;
		g_AudioSystem.SetWorldScale( AudioScaling );
	}

	//post a 3D sound event at specific location
	if(InputManager()->GetKeyPress(VK_SPACE))
		g_AudioSystem.PostEvent(AK::EVENTS::PLAY_FX_3D_EXPLOSION,XMFLOAT3(0,0,0));

	//play/stop music track
	//NOTE: multiple instances can be played, calling stop will stop all of them
	if(InputManager()->GetKeyPress('1'))
		g_AudioSystem.PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_01);
	if(InputManager()->GetKeyPress('2'))
		g_AudioSystem.PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_01);

	//Post a 3D sound without parameters
	if(InputManager()->GetKeyPress('3'))
		g_AudioSystem.PostEvent(AK::EVENTS::PLAY_FX_3D_EXPLOSION);

	
	// update the camera
	m_pCamera->Update(timeDelta);

    // 1st Cube: Rotate around the origin
	XMMATRIX World1 = XMMatrixRotationY( totalTime );
	XMStoreFloat4x4(&m_Entity1.m_mWorld,World1);

    // 2nd Cube:  Rotate around origin
    XMMATRIX mSpin = XMMatrixRotationZ( -totalTime );
    XMMATRIX mOrbit = XMMatrixRotationY( -totalTime * 2.0f );
	XMMATRIX mTranslate = XMMatrixTranslation( -4.0f, 0.0f, 0.0f );
	XMMATRIX mScale = XMMatrixScaling( 0.3f, 0.3f, 0.3f );

	XMMATRIX World2 = mScale * mSpin * mTranslate * mOrbit;

	//store the entities new location
	XMStoreFloat4x4(&m_Entity2.m_mWorld,World2);

	g_AudioSystem.Update();

	if(InputManager()->GetKeyPress(VK_ESCAPE))
		PostQuitMessage(0);
}
//--------------------------------------------------------------------------------
void CGame::Render()
{
	Renderer::BeginRender();
	Renderer::RenderCube(&m_Entity1,m_pCamera);
	Renderer::RenderCube(&m_Entity2,m_pCamera);
	Renderer::Present();
}
//--------------------------------------------------------------------------------
void CGame::SceneEnd()
{
	if(m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	g_AudioSystem.Shutdown();
	Renderer::CleanupDevice();
}
//--------------------------------------------------------------------------------
void CGame::Pause() 
{ 
	m_bIsActive = false;
	Show_Cursor();
#ifndef _DEBUG
	if(g_AudioSystem.GetInitialized())
		g_AudioSystem.PostEvent(AK::EVENTS::PAUSE_ALL);
#endif
}
//--------------------------------------------------------------------------------
void CGame::UnPause() 
{
	m_bIsActive = true;
#ifndef _DEBUG
	if(g_AudioSystem.GetInitialized())
		g_AudioSystem.PostEvent(AK::EVENTS::RESUME_ALL);
#endif
}
//--------------------------------------------------------------------------------
void CGame::DoFrame(float fDeltaTime, float totalTime)
{
	Update(fDeltaTime,totalTime);
	Render();
}
//--------------------------------------------------------------------------------
void CGame::DoIdleFrame( float timeDelta, float totalTime )
{
	// for debugging continue to run and update the game
#ifdef _DEBUG
	Update(timeDelta,totalTime);
#else
	g_AudioSystem.Update();
#endif
}
//--------------------------------------------------------------------------------
CApplication* CreateApplication() {	return new CGame(); }
//--------------------------------------------------------------------------------
CGame::~CGame() 
{
}
//--------------------------------------------------------------------------------
void CGame::Show_Cursor()
{
	if(!m_bShowCursor)
	{
		ShowCursor(true);
		m_bShowCursor = true;
		if(m_bKeepCursorPos)
			SetCursorPos(m_InitialCursorPos.x,m_InitialCursorPos.y);
	}
}
//--------------------------------------------------------------------------------
void CGame::Hide_Cursor()
{
	if(m_bShowCursor)
	{
		ShowCursor(false);
		m_bShowCursor = false;
		if(m_bKeepCursorPos)
			GetCursorPos(&m_InitialCursorPos);
	}
}
//--------------------------------------------------------------------------------