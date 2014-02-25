//--------------------------------------------------------------------------------
// Game
// Written By: Justin Murphy
//
// Overall game class, this class creates, holds and manipulates 
// all objects and any non framework subsystems
//--------------------------------------------------------------------------------
#ifndef Game_h
#define Game_h
//--------------------------------------------------------------------------------
#include "Application.h"
#include "AudioSystemWwise.h"
#include "Entity3D.h"
//--------------------------------------------------------------------------------
class Camera;
//--------------------------------------------------------------------------------
class CGame : public CApplication
{
private:
	// some default objects
	//TODO: MAKE OBJECT MANAGEMENT LESS HARDCODED
	Entity3D				m_Entity1;
	Entity3D				m_Entity2;
	// default view
	Camera*					m_pCamera;
	//TODO: replace with camera
	//XMMATRIX                g_View;
	//XMMATRIX                g_Projection;

	// audio stuff
	AudioSystemWwise		g_AudioSystem;
	
	
	static bool			m_bShowCursor;
	static bool			m_bKeepCursorPos;
	POINT				m_InitialCursorPos; // used to reset the cursor to the correct position after click is released

	virtual void DoFrame(float timeDelta, float totaltime);
	virtual void DoIdleFrame( float timeDelta, float totaltime );
	virtual void SceneInit();
	virtual void SceneEnd();
	
	void Update(float timeDelta, float totaltime);
	void Render();

public:
	int GetWindowWidth()const {return m_nWindowWidth;}
	int GetWindowHeight()const {return m_nWindowHeight;}
	
	virtual void Pause();
	virtual void UnPause();
	
	void Show_Cursor();
	void Hide_Cursor();

	CGame();
	~CGame();
};
//--------------------------------------------------------------------------------
#endif // Game_h
//--------------------------------------------------------------------------------