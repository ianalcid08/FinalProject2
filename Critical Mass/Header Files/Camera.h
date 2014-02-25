/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Camera.h
Author : Evan Wright

Description : This module will handle the camera’s frustum position/orientation based 
			  on the player’s input.  The player’s position and orientation will be 
			  manipulated by the camera’s frustum data. Renderer will use this to decide 
			  whether or not to render the current active objects.  

Created :  07/16/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef CAMERA_H_
#define CAMERA_H_

#include "glsMath.h"
#include "BaseEntity.h"
#include "Input.h"

class CCamera
{
	Vec3f m_vOffset;
	CBaseEntity* m_Target;

	Matrix3x3f m_Orientation;
	Vec3f m_Position;

	bool	m_Attached;
	bool	m_StartWin;
	bool	m_StartLose;
	bool	m_StartMenu;
	float	m_ElapsedTime;
	float	m_CamDistance;
	float   m_Timer;
	float	m_screenWidth;
	float	m_screenHeight;

	void ReOrient();
	void DeathPan(float _elapsedTime);
	void WinPan(float _elapsedTime);
	void MenuPan(float _elapsedTime);

public:

	CCamera( void );
	~CCamera( void );

	void Translate( Vec3f _translation );
	void Translate( float _x, float _y, float _z );
	void Rotate(float _rad, float _x, float _y, float _z);

	bool Update(float _delta);
	void Input(CInput& _input);

	void SetTarget(CBaseEntity* _target);
	void SetStartWin(bool _b);
	void SetStartLose(bool _b);
	void SetStartMenu(bool _b) { m_StartMenu = _b; }
	void SetPosition(Vec3f _pos) { glsCopyVector3(m_Position, _pos); }
	void UnAttach(void) { m_Attached = false; }
	
	void SetLookAt(Vec3f _look);

	Vec3f* GetPosition() {return &m_Position;}
	Matrix3x3f* GetOrientation() {return &m_Orientation;}
	void SetOrientation(Matrix3x3f _orient) {glsCopyMatrix3x3(m_Orientation,_orient);}
	
	virtual void HandleReaction( IEntity* _other );

	void ResetCam(void);
	bool MainMenuLerpage(float _elapsedTime, IEntity* _startpt, IEntity* _endpt);

};

#endif
