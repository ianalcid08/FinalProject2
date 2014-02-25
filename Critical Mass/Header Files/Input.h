/*////////////////////////////////////////////////////////////////////////////////////
/*FileName: Input.h
/*Author:	Tom Stefl
/*Purpose:  This module is used to detect mouse and keyboard input.
/*			
/*////////////////////////////////////////////////////////////////////////////////////


#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

#include <dinput.h>
#include <XInput.h>

#include "XBOXController.h"



class CInput
{
private:
	HWND  m_hwndHandle;  // pointer to the window we made

	IDirectInput8*			m_directInput;
	IDirectInputDevice8*	m_keyboard;

	XBOXController m_XBOXController;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth;
	int m_screenHeight;

	POINT m_cursorLoc;
	POINT m_prevMouse;
	int m_deltaMouseX;
	int m_deltaMouseY;
	RECT m_LastWindowLoc;


public:
	CInput();
	~CInput();

	bool Initialize(HINSTANCE _hInstance, HWND _hwnd, int _screenWidth, int _screenHeight);
	void Shutdown();
	bool Frame();

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	bool IsEscapePressed();
	bool IsEnterPressed();
	bool IsAltEnterPressed();

	bool KeyDown(DWORD _key);
	bool KeyUp(DWORD _key);

	bool m_LeftMouseClick;
	bool m_RightMouseClick;


	//Accessors
	int GetMousePosX()					 {return m_cursorLoc.x;}
	int GetMousePosY()					 {return m_cursorLoc.y;}
	int GetDeltaMousePosX()				 {return m_deltaMouseX;}
	int GetDeltaMousePosY()				 {return m_deltaMouseY;}
	bool IsLeftClicked()				 {return m_LeftMouseClick;}
	bool IsRightClicked()				 {return m_RightMouseClick;}

	XBOXController GetController()		 {return m_XBOXController;}
	//Mutators






};

#endif
