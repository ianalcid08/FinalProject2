//--------------------------------------------------------------------------------
// MainWindow
// Written By: Justin Murphy
//
// Handles window creation, class registration, message processing, and updating the application
//--------------------------------------------------------------------------------
#ifndef MainWindow_h
#define MainWindow_h
//--------------------------------------------------------------------------------
#include "Common.h"
//--------------------------------------------------------------------------------
class CMainWindow
{
private:
	HWND				m_hWnd;
	const wchar_t*		m_szWindowName;
	static CMainWindow*	s_pInstance;
	int					m_nWindowWidth;
	int					m_nWindowHeight;
	bool				m_bIsActive;

public:
	CMainWindow( int nWindowWidth, int nWindowHeight,
		const wchar_t* szWindowName = L"MurphyGaming Tutorial Series");
	~CMainWindow();

	virtual LRESULT WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void RegisterClass( WNDCLASSEX* pWc = 0 );
	virtual void InitInstance();

	const HWND GetHWnd() const;
	bool IsActive();
	bool HasMessages();
	// True -> continue running
	// False -> got a WM_QUIT message. Stop the application.
	bool MessagePump();

	static CMainWindow* GetInstance()
	{
		return s_pInstance;
	}

	int GetWindowWidth() const;
	int GetWindowHeight() const;
};
//--------------------------------------------------------------------------------
inline CMainWindow* MainWindow()
{ 
	return CMainWindow::GetInstance(); 
}
//--------------------------------------------------------------------------------
#endif // MainWindow_h
//--------------------------------------------------------------------------------