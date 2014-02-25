/*////////////////////////////////////////////////////////////////////////////////////
/*FileName: Input.cpp
/*Author:	Tom Stefl
/*Purpose:  This module is used to detect mouse and keyboard input.
/*			
/*////////////////////////////////////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/Input.h"


CInput::CInput()
{

	//m_directInput = 0;
	//m_keyboard = 0;
	//m_mouse = 0;

	//prevMouseX = 0;
	//prevMouseY = 0;
	//deltaMouseX = 0;
	//deltaMouseY = 0;
	//deltaMouseWheel = 0;

}

CInput::~CInput()
{
}


/*
This function initializes the keyboard and mouse devices.  
It also sets the passed in SCREENWIDTH and SCREENHEIGHT
variables to a local varaiable for use.  The keyboard is set to
exclusive to ensure that all keyboard input is processed and owned by 
the game.  If at any point initialization cannot be completed 
the function will fail and return false.  This can happen if a device
cannot be detected.
*/
bool CInput::Initialize(HINSTANCE _hinstance, HWND _hwnd, int _screenWidth, int _screenHeight)
{
	HRESULT result;


	m_screenWidth = _screenWidth;
	m_screenHeight = _screenHeight;

	GetCursorPos(&m_cursorLoc);

	m_prevMouse.x = (long)(_screenWidth * 0.5f);
	m_prevMouse.y = (long)(_screenHeight * 0.5f);
	//SetCursorPos(_screenWidth * 0.5f, _screenHeight * 0.5f);

#pragma region Initialize_Keyboard
	result = DirectInput8Create(_hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}


	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	result = m_keyboard->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}


	result = m_keyboard->Acquire();


#pragma endregion


	m_hwndHandle = _hwnd;

	return true;
}


/*
This will release the devices in reverse order.
Necessary for module shutdown.
1.) Unaquires and releases mouse.
2.) Unaquires and releases keyboard.
3.) Releases the direct Input object.
*/
void CInput::Shutdown()
{
	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if(m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

/*
This function calls ReadMouse(), ReadKeyboard(), and 
ProcessInput() functions.  This will be called every frame ddadaw
to accurately get what input is being pressed for that frame.
*/
bool CInput::Frame()
{
	if( GetForegroundWindow() != m_hwndHandle )
		return true;

	bool result;

	result = ReadKeyboard();
	if(!result)
	{
		return false;
	}

	result = ReadMouse();
	if(!result)
	{
		return false;
	}

	if(m_XBOXController.IsConnected())
	{
		m_XBOXController.GetControllerState();
	}

	//ProcessInput();

	return true;
}


/*
This function gets the current keys pressed on a keyboard.
The keys that are pressed are stored withing the m_keyboardState
member.  If not keyboard device is detected at all it will return
false.
*/
bool CInput::ReadKeyboard()
{
	HRESULT result;


	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
		
	return true;
}

/*
This function reads the current mouse input. If no device
is detected this function will return false.
*/
bool CInput::ReadMouse()
{
#pragma region Old_mouse_Stuff

	//HRESULT result;

	//

	//result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	//if(FAILED(result))
	//{
	//	if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
	//	{
	//		m_mouse->Acquire();
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	//deltaMouseX = prevMouseX + m_mouseState.lX;
	//deltaMouseY = prevMouseY + m_mouseState.lY;
	//prevMouseX =  m_mouseState.lX;
	//prevMouseY =  m_mouseState.lY;

	//deltaMouseWheel = prevMouseWheel + m_mouseState.lZ;
	//prevMouseWheel = m_mouseState.lZ;

#pragma endregion

	//Direct input is screwy, using raw for mouse detection

	//Test if left button is clicked
	 if ((GetKeyState(VK_LBUTTON) & 0x80)/*  && !m_LeftMouseClick*/)
	 {
		m_LeftMouseClick = true;
		//std::cout << "Left clicked..." << std::endl;
	 }
	 else
	 {
		m_LeftMouseClick = false;
	 }

	 //check if right mouse is clicked
	 if ((GetKeyState(VK_RBUTTON) & 0x80)/* && !m_RightMouseClick*/)
	 {
		m_RightMouseClick = true;
		//std::cout << "Right clicked..." << std::endl;
	 }
	 else
	 {
		m_RightMouseClick = false;

	 }

	if(GetForegroundWindow() == m_hwndHandle)
		ProcessInput();
	
	return true;
}

/*
This function processes the mouse movement input.
It bounds checks based on the initialized screen width and height.
The mouse position information is stored in m_mouseX and m_mouseY.
*/
void CInput::ProcessInput()
{
	GetCursorPos(&m_cursorLoc);
	WINDOWINFO pwi;
	GetWindowInfo(m_hwndHandle,&pwi);

	m_prevMouse.x = (long)((pwi.rcWindow.left + pwi.rcWindow.right ) * 0.5f);
	m_prevMouse.y = (long)((pwi.rcWindow.top + pwi.rcWindow.bottom) * 0.5f);
	m_deltaMouseX = m_cursorLoc.x - m_prevMouse.x;
	m_deltaMouseY = -(m_cursorLoc.y - m_prevMouse.y);

#if _RELEASE
	//ClipCursor(&rcOldClip);
	ClipCursor(&pwi.rcWindow);
#endif

	ScreenToClient(m_hwndHandle,&m_cursorLoc);
}

bool CInput::IsEscapePressed()
{
	if(m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool CInput::IsEnterPressed()
{
	if(m_keyboardState[DIK_RETURN] & 0x80)
	{
		return true;
	}

	return false;
}

bool CInput::IsAltEnterPressed()
{
	if( ((m_keyboardState[DIK_LALT] & 0x80) || (m_keyboardState[DIK_RALT] & 0x80)) && (m_keyboardState[DIK_RETURN] & 0x80) )
		return true;

	return false;
}

/*
This is used to check and see if a specific key is pressed.
If the key is currently pressed down, the function will return true.

EXAMPLE CODE:
	if(m_keyboardState[DIK_RETURN] & 0x80)
	{
		return true;
	}

	LETTERS: DIK_A or DIK_B

*/
bool CInput::KeyDown(DWORD _key)
{
	if(m_keyboardState[_key] & 0x080)
	{
		return true;
	}
	else
	{
		return false;
	}
	
	
}
/*
This is used to check and see if a specific key is NOT BEING PRESSED.
If the key is currently NOT pressed down, the function will return true.

EXAMPLE CODE:
	if(m_keyboardState[DIK_RETURN] & 0x80)
	{
		return false;
	}

	LETTERS: DIK_A or DIK_B
*/
bool CInput::KeyUp(DWORD _key)
{
	if(m_keyboardState[_key] & 0x080)
	{
		return false;
	}
	else
	{
		return true;
	}
}