/*////////////////////////////////////////////////////////////////////////////////////
/*FileName: XBOXConroller.cpp
/*Author:	Tom Stefl
/*Purpose:  This module is used to detect what buttons are pressed on
/*			the XBOX game controller.
/*////////////////////////////////////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/XBOXController.h"

/*/////////////////////////////////////////////////////////////
This function sets the controllers ID to the passed in 
player ID number.  Player 1 will be registered as 0,
player 2 will be registered as 1... so on.
/*/////////////////////////////////////////////////////////////
XBOXController::XBOXController()
{

	m_controllerID = 0;

}


XBOXController::XBOXController(int _playerIDNum)
{
	m_controllerID = _playerIDNum - 1;
}


XBOXController::~XBOXController(void)
{
}
/*/////////////////////////////////////////////////////////////
This function gets the current state of the controller. 
The controllerState member variable is a structure that will
be filled out.  This function also gives x and y coordinates for
the left and right thumb sticks.
/*/////////////////////////////////////////////////////////////
XINPUT_STATE XBOXController::GetControllerState()
{
	ZeroMemory(&m_controllerState, sizeof(XINPUT_STATE));

	XInputGetState(m_controllerID, &m_controllerState);

	leftStickPosX = m_controllerState.Gamepad.sThumbLX;
	leftStickPosY = m_controllerState.Gamepad.sThumbLY;
	rightStickPosX = m_controllerState.Gamepad.sThumbLX;
	rightStickPosY = m_controllerState.Gamepad.sThumbLY;

	return m_controllerState;
}
/*/////////////////////////////////////////////////////////////
This function will detect whether or not a controller device
is connected to the client.  If a controller is not detected
it will return false.
/*/////////////////////////////////////////////////////////////
bool XBOXController::IsConnected()
{
	ZeroMemory(&m_controllerState, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState(m_controllerID,&m_controllerState);

	if(result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*/////////////////////////////////////////////////////////////
This function is to turn the vibration for one or both of the 
controllers vibration motors on or off.  The maximum value a 
vibration value can be set to 65355.  The controller will
continue to vibrate until it is set back to 0.  
VIBRATE MUST BE SET BACK TO 0 TO BE TURNED OFF
/*/////////////////////////////////////////////////////////////
void XBOXController::controllerVibrate(int _leftVal, int _rightVal)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = _leftVal;
	vibration.wRightMotorSpeed = _rightVal;

	XInputSetState(m_controllerID,&vibration);

}
