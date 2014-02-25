//--------------------------------------------------------------------------------
// Written by Justin Murphy
//--------------------------------------------------------------------------------
#include "MainWindow.h"
#include "Application.h"
#include "InputManager.h"
#include "PrecisionTimer.h"
//#include "CEngineInterface.h"
//--------------------------------------------------------------------------------
CMainWindow* CMainWindow::s_pInstance = 0;
//--------------------------------------------------------------------------------
LRESULT CALLBACK GlobalWndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam )
{
	return MainWindow()->WndProc( hWnd, uMsg, wParam, lParam );
}
//--------------------------------------------------------------------------------
CMainWindow::CMainWindow(int nWindowWidth,
	int nWindowHeight,
	const wchar_t* szWindowName) :
	m_nWindowWidth( nWindowWidth ),
	m_nWindowHeight( nWindowHeight ),
	m_hWnd( 0 )
{
	// Only one primary window should be created
	if( s_pInstance )
		assert(0);//throw CError("MainWindow already Exists!\n" );
	
	s_pInstance = this;
	m_bIsActive = false;
	m_szWindowName = szWindowName;

	// initialize neccessary systems
	CInputManager::CreateInstance();
}
//--------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
{
	CInputManager::DestroyInstance();
	s_pInstance = 0;
}
//--------------------------------------------------------------------------------
LRESULT CMainWindow::WndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam )
{
	//// Return immediately if there was an error thrown
	//if( g_pThrownError )
	//	return DefWindowProc(hWnd, uMsg, wParam, lParam);	
	// do input message loop
	bool bNoInput = InputManager()->HandleWindowMessages(uMsg, wParam, lParam);
	bool bRunDefault = false;
	static bool m_bMinOrMaxed = false;

	//try
	//{
		switch( uMsg ) 
		{
		case WM_CREATE:
			{
				MainWindow()->m_hWnd = hWnd;
				MainWindow()->m_bIsActive = true;
			}
			break;
		case WM_ACTIVATE:
			{
				if( LOWORD(wParam) == WA_INACTIVE )
					Application()->GetApplication()->Pause();
				else
				{
					InputManager()->ResetInput();
					// be sure to run update so it doesn't spam release key presses
					InputManager()->UpdateKeyStates(CPrecisionTimer::GetDeltaTime());
					Application()->GetApplication()->UnPause();
				}
			}
			break;
		case WM_PAINT:
			{
				ValidateRect( hWnd, 0 );
			}
			break;
		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			break;
		case WM_MENUCHAR:
			{
				// Don't beep when we alt-enter.
				return MAKELRESULT(0, MNC_CLOSE);
			}
			break;
		case WM_GETMINMAXINFO:
			{
				//Stop the window from becoming to small.
				((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
				((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
			}
			break;
		case WM_SIZE:
			{
				if( wParam == SIZE_MAXIMIZED )
				{
					//EventSystem()->SendEvent("ResetDevice", 0, CEventManager::eEventInstant);
				}
				else if( wParam == SIZE_RESTORED )
				{
					//EventSystem()->SendEvent("ResetDevice", 0, CEventManager::eEventInstant);
				}
			}
			break;
		default:
			bRunDefault = bNoInput;
			break;
		}	
	//}

	/*catch( CError& err )
	{
		g_pThrownError = new CError( err );
		return 0;
	}*/

	if( bRunDefault )
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	return 0;
}
//--------------------------------------------------------------------------------
void CMainWindow::RegisterClass( WNDCLASSEX* pWc )
{
	WNDCLASSEX wcex;
	if( pWc )
		wcex = *pWc;
	else
	{
		wcex.cbSize = sizeof(WNDCLASSEX); 

		wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
		wcex.lpfnWndProc	= GlobalWndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= AppInstance();
		wcex.hIcon			= 0;
		wcex.hCursor		= LoadCursor(0, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= m_szWindowName;
		wcex.hIconSm =wcex.hIcon;
	}

	if( NULL == RegisterClassEx(&wcex) )
		assert(0);//throw CError("[CMainWindow::RegisterClass]: Registering window class failed!\n");
}
//--------------------------------------------------------------------------------
void CMainWindow::InitInstance()
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = m_nWindowWidth;
	rc.bottom = m_nWindowHeight;
	DWORD style = WS_CAPTION | WS_SYSMENU ;
	AdjustWindowRect( &rc, style, FALSE );

	CreateWindow( //exStyle,
		m_szWindowName, 
		m_szWindowName, 
		style,
		(GetSystemMetrics(SM_CXSCREEN)>>1) - (m_nWindowWidth>>1), 
		(GetSystemMetrics(SM_CYSCREEN)>>1) - (m_nWindowHeight>>1), 
		rc.right - rc.left, 
		rc.bottom - rc.top, 
		0, 
		0, 
		AppInstance(), 
		0);

	if ( !m_hWnd )
		assert(0);//throw CError("[CMainWindow::InitInstance]: Creating window failed!\n");

	// Draw
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
}
//--------------------------------------------------------------------------------
bool CMainWindow::HasMessages()
{
	MSG msg;
	if( 0 == PeekMessage( &msg, 0, 0U, 0U, PM_NOREMOVE ) )
		return false;
	return true;
}
//--------------------------------------------------------------------------------
int CMainWindow::GetWindowWidth() const 
{ 
	return m_nWindowWidth;
}
//--------------------------------------------------------------------------------
int CMainWindow::GetWindowHeight() const 
{
	return m_nWindowHeight; 
}
//--------------------------------------------------------------------------------
const HWND CMainWindow::GetHWnd() const
{
	return m_hWnd;
}
//--------------------------------------------------------------------------------
bool CMainWindow::IsActive()
{ 
	return m_bIsActive; 
}
//--------------------------------------------------------------------------------
bool CMainWindow::MessagePump()
{
	MSG msg;
	PeekMessage( &msg, 0, 0U, 0U, PM_REMOVE );

	if( WM_QUIT == msg.message )
		return false;

	TranslateMessage( &msg );
	DispatchMessage( &msg );

	//// Check to see if an error occured during the message pump
	//if( g_pThrownError )
	//{
	//	CError out = *g_pThrownError;
	//	delete g_pThrownError;
	//	throw out;
	//}

	return true;
}
//--------------------------------------------------------------------------------