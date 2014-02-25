#pragma once


#include <Windows.h>
#include <vector>
#include "Timer.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "AssetManager.h"


using namespace std;

//enum eGameStates{ INTRO_STATE, MAINMENU_STATE,GAMEPLAY_STATE,DIFFICULTY_STATE, OPTIONS_STATE, CREDITS_STATE,PAUSE_STATE,RETRY_STATE,NUM_STATES};

//class CRenderer
class IState;
class CInput;

class CWindowsForm
{
	HWND						m_hWnd;
	HINSTANCE					m_hInstance;
	//CRenderer*					m_Renderer;
	int							m_ScreenWidth;
	int							m_ScreenHeight;
	bool						m_IsWindowed;

	bool						m_IsRunning;

	bool						Input( void );
	void						Update( void );
	void						Render( void );
	static CWindowsForm			m_Instance;
	std::vector<IState*>		m_CurrState;
	CTimer						m_cTimer;
	
	CObjectFactory				m_cOF;
	CObjectManager				m_cOM;
	CInput*						m_pInput;
	float						m_fDelta;

public:
	CWindowsForm(void);
	~CWindowsForm(void);
	CWindowsForm( const CWindowsForm& );				
	CWindowsForm& operator= ( const CWindowsForm& );
	static CWindowsForm* GetInstance();

	// Accessors & Mutators
	int		GetScreenWidth( void )	const{ return m_ScreenWidth; }
	int		GetScreenHeight( void ) const{ return m_ScreenHeight; }
	bool	IsWindowed( void )		const{ return m_IsWindowed; }
	HWND	GetHwnd(void)			const {return m_hWnd;}
	HINSTANCE GetHInstance(void)	const {return m_hInstance;}
	bool	GetIsRunning()			const {return m_IsRunning;}
	float GetDelta() const { return m_fDelta; }

	CObjectFactory* GetOF() { return &m_cOF; }
	CObjectManager* GetOM() { return &m_cOM; }

	void	SetIsWindowed(bool b) {m_IsWindowed = b;}
	void	SetIsRunning(bool b) {m_IsRunning = b;}

	void	Init( HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed );
	bool	Play();
	void	Shutdown();

	// State Machine
	/*void ChangeState(eGameStates _eNewState);
	void PushState(eGameStates _eNewState);
	void PopState();*/
};

