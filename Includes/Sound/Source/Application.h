//--------------------------------------------------------------------------------
// CApplication
// Written By: Justin Murphy
//--------------------------------------------------------------------------------
#ifndef Application_h
#define Application_h
//--------------------------------------------------------------------------------
#include "Common.h"
//--------------------------------------------------------------------------------
class CApplication 
{
protected:
	const wchar_t*	m_szWindowTitle;
	int			m_nWindowWidth;
	int			m_nWindowHeight;
	bool		m_bIsActive;
	bool		m_bIsDone;

	static CApplication*	s_pInstance;
	virtual void InitPrimaryWindow();

public:
	CApplication();
	virtual ~CApplication();

	// Init the application.  All user-level init should go here
	virtual void Init(); 

	// Run the application.  This shouldn't need to change
	virtual void Run();

	// Handles all code that should be run each frame
	// timeDelta == amount of time (in seconds) since last call
	// timeTotal == amount of time (in seconds) since start
	virtual void DoFrame( float timeDelta, float timeTotal );
	// update but when window doesn't have focus
	virtual void DoIdleFrame( float timeDelta, float timeTotal );

	// Scene Init is called by Init.  This way a user-level class 
	// doesn't need to worry about setting up D3D or handling any 
	// errors during setup.
	virtual void SceneInit();
	virtual void SceneEnd();

	virtual void Pause() { m_bIsActive = false; }
	virtual void UnPause() { m_bIsActive = true; }

	static CApplication* GetApplication() { return s_pInstance; }
};
//--------------------------------------------------------------------------------
inline CApplication* Application()
{
	return CApplication::GetApplication();
}
//--------------------------------------------------------------------------------
HINSTANCE AppInstance();
//--------------------------------------------------------------------------------
// This must be defined by any client that uses this framework
CApplication*	CreateApplication(); // returns a pointer to a valid application object
//--------------------------------------------------------------------------------
#endif // Application_h
//--------------------------------------------------------------------------------