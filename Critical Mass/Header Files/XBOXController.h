/*////////////////////////////////////////////////////////////////////////////////////
/*FileName: XBOXConroller.h
/*Author:	Tom Stefl
/*Purpose:  This module is used to detect what buttons are pressed on
/*			the XBOX game controller.
/*////////////////////////////////////////////////////////////////////////////////////


#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <XInput.h>
#pragma comment(lib,"Xinput.lib")

//XINPUT_GAMEPAD_DPAD_UP          0x0001
//XINPUT_GAMEPAD_DPAD_DOWN        0x0002
//XINPUT_GAMEPAD_DPAD_LEFT        0x0004
//XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
//XINPUT_GAMEPAD_START            0x0010
//XINPUT_GAMEPAD_BACK             0x0020
//XINPUT_GAMEPAD_LEFT_THUMB       0x0040
//XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
//XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
//XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
//XINPUT_GAMEPAD_A                0x1000
//XINPUT_GAMEPAD_B                0x2000
//XINPUT_GAMEPAD_X                0x4000
//XINPUT_GAMEPAD_Y                0x8000


#define LEFT_STICK_DEADZONE  7849
#define RIGHT_STICK_DEADZONE 8689
#define TRIGGER_THRESHOLD    30

class XBOXController
{
private:
	XINPUT_STATE m_controllerState;														//struct to capture current button presses
	int m_controllerID;																	// ID for a controller
	int leftStickPosX;					
	int leftStickPosY;
	int rightStickPosX;
	int rightStickPosY;


public:
	XBOXController();													
	XBOXController(int _playerIDNum);													//Initialize controller based on player ID
	~XBOXController(void);
	XINPUT_STATE GetControllerState();
	bool IsConnected();
	void controllerVibrate(int _leftVal = 0, int _rightVal = 0);

	int GetLeftStickPosX() {return leftStickPosX;}
	int GetLeftStickPosY() {return leftStickPosY;}
	int GetRightStickPosX() {return rightStickPosX;}
	int GetRightStickPosY() {return rightStickPosY;}
};

#endif