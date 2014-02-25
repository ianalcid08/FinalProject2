//--------------------------------------------------------------------------------
// Written by Justin Murphy
//--------------------------------------------------------------------------------
#include "Application.h"
#include "MainWindow.h"
#include "InputManager.h"
#include "PrecisionTimer.h"
#include <ctime> // for seeding rand
//--------------------------------------------------------------------------------
CApplication* CApplication::s_pInstance = 0;
HINSTANCE g_hInstance;
//--------------------------------------------------------------------------------
HINSTANCE AppInstance()
{
	return g_hInstance;
}
//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,//-debug
	int       nCmdShow)
{
	//Seed rand
	srand((unsigned)time(0));
	CApplication* pApp;

	g_hInstance = hInstance;
	
	/*try
	{*/
		// Initialize Event System
		//CEventManager::CreateInstance();
		pApp = CreateApplication();

		pApp->Init();
		pApp->SceneInit();
		pApp->Run();
	//}
	//catch( CError& err )
	//{
	//	MessageBox( 0, err.GetText(), "Error!", MB_OK|MB_ICONEXCLAMATION );

	//	// Clean everything up
	//	MMDel(pApp);
	//	return 0;
	//}

	pApp->SceneEnd();

	delete pApp;

	//EventManager()->Shutdown();
	//CEventManager::DestroyInstance();

	return 0;
}
//--------------------------------------------------------------------------------
CApplication::CApplication() :
	m_szWindowTitle(L"Default window name"),
	m_nWindowWidth(800),
	m_nWindowHeight(600),
	m_bIsActive(true)
{
	if( s_pInstance )
	{
		return;//throw CError("Application object already created!\n");
	}
	s_pInstance = this;
}
//--------------------------------------------------------------------------------
CApplication::~CApplication()
{
	// Clean up systems
	//Engine()->Shutdown();
	//MMDel( Engine() );
	delete MainWindow();
}
//--------------------------------------------------------------------------------
void CApplication::Init()
{
	InitPrimaryWindow();
}
//--------------------------------------------------------------------------------
void CApplication::Run()
{
	CPrecisionTimer::Init();
	while( true )
	{
		// Message pump
		while( MainWindow()->HasMessages() )
		{
			if( false == MainWindow()->MessagePump() )
				return;
		}

		CPrecisionTimer::GlobalUpdate();

		if( m_bIsActive  )
		{
			float fdt = CPrecisionTimer::GetDeltaTime();
			static float totalTime = 0;
			totalTime += fdt;
			DoFrame(fdt, totalTime );

			// Update the Input devices after doing frame so the keys will buffer properly
			InputManager()->UpdateKeyStates(CPrecisionTimer::GetDeltaTime());
		}
		else
		{
			float fdt = CPrecisionTimer::GetDeltaTime();
			static float totalTime = 0;
			totalTime += fdt;
			DoIdleFrame(fdt, totalTime );
		}
	}
}
//--------------------------------------------------------------------------------
void CApplication::DoFrame( float timeDelta, float timeTotal )
{
}
//--------------------------------------------------------------------------------
void CApplication::DoIdleFrame( float timeDelta, float timeTotal )
{
}
//--------------------------------------------------------------------------------
void CApplication::SceneInit()
{
}
//--------------------------------------------------------------------------------
void CApplication::InitPrimaryWindow()
{
	new CMainWindow( m_nWindowWidth, m_nWindowHeight, m_szWindowTitle );

	MainWindow()->RegisterClass();
	MainWindow()->InitInstance();
}
//--------------------------------------------------------------------------------
void CApplication::SceneEnd()
{
}
//--------------------------------------------------------------------------------