/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Camera.cpp
Author : Evan Wright

Description : This module will handle the camera’s frustum position/orientation based 
			  on the player’s input.  The player’s position and orientation will be 
			  manipulated by the camera’s frustum data. Renderer will use this to decide 
			  whether or not to render the current active objects.  

Created :  07/16/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/Camera.h"
#include "../Header Files/Player.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/Frustum.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/RenderNode.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CCamera::CCamera( void )
{
	// The default orientation:
	m_Orientation[0]	= 1.0f;
	m_Orientation[1]	= 0.0f;
	m_Orientation[2]	= 0.0f;
	m_Orientation[3]	= 0.0f;
	m_Orientation[4]	= 1.0f;
	m_Orientation[5]	= 0.0f;
	m_Orientation[6]	= 0.0f;
	m_Orientation[7]	= 0.0f;
	m_Orientation[8]	= 1.0f;

	m_Position[0] = m_Position[1] = m_Position[2] = 0.0f;

	m_Attached = false;

	m_Target = nullptr;
	m_CamDistance = 80.0f;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CCamera::~CCamera( void )
{
}

void CCamera::ResetCam(void)
{
	// The default orientation:
	m_Orientation[0]	= 1.0f;
	m_Orientation[1]	= 0.0f;
	m_Orientation[2]	= 0.0f;
	m_Orientation[3]	= 0.0f;
	m_Orientation[4]	= 1.0f;
	m_Orientation[5]	= 0.0f;
	m_Orientation[6]	= 0.0f;
	m_Orientation[7]	= 0.0f;
	m_Orientation[8]	= 1.0f;

	m_Position[0] = m_Position[1] = m_Position[2] = 0.0f;

	m_Attached = false;

	m_Target = nullptr;
	m_CamDistance = 80.0f;
	m_StartLose = false;
	m_StartWin = false;
	m_StartMenu = false;
}

void CCamera::SetTarget(CBaseEntity* _target)
{
	m_Target = _target;
	m_Attached = true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Translates the camera by _x, _y, _z units
Parameters:
[in] _x units to translate by on the X axis
[in] _y units to translate by on the Y axis
[in] _z units to translate by on the Z axis
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::Translate( float _x, float _y, float _z )
{
	if(_x)
	{
		m_Position[0] += m_Orientation[0] * _x;
		m_Position[1] += m_Orientation[1] * _x;
		m_Position[2] += m_Orientation[2] * _x;
	}
	if(_y)
	{
		m_Position[0] += m_Orientation[3] * _y;
		m_Position[1] += m_Orientation[4] * _y;
		m_Position[2] += m_Orientation[5] * _y;
	}
	if(_z)
	{
		m_Position[0] += m_Orientation[6] * _z;
		m_Position[1] += m_Orientation[7] * _z;
		m_Position[2] += m_Orientation[8] * _z;
	}
}

void CCamera::Translate( Vec3f _translation )
{
	Translate( _translation[0], _translation[1], _translation[2] );
}
	
void CCamera::Rotate(float _rad, float _x, float _y, float _z)
{
	Matrix3x3f Rotation;
	glsRotationMatrix3x3(Rotation, _rad, _x, _y, _z);
	glsMatrixMultiply3x3(m_Orientation, m_Orientation, Rotation);
}
	
void CCamera::SetStartWin(bool _b)
{
	m_StartWin = _b;
	m_Timer = 0.0f;
}

void CCamera::SetStartLose(bool _b)
{
	if(m_StartLose != _b)
	{
		m_StartLose = _b;
		m_Timer = 0.0f;
	}
}

bool CCamera::Update( float _delta )
{
	if( m_Attached )
	{
		if(m_StartLose)
			DeathPan(_delta);
		else if( m_StartWin )
			WinPan(_delta);
		else if( m_StartMenu )
			MenuPan(_delta);
		else
			ReOrient();
	}

	m_ElapsedTime = _delta;
	
	return true;
}

void CCamera::MenuPan(float _elapsedTime)
{
	glsCopyVector3(m_Position, *m_Target->GetPosition());
	Rotate(float(glsDegToRad(7.0f) * _elapsedTime), 1.0f, -1.0f, 0.0f);
	Translate(0,0,-(95.0f + m_CamDistance));
}

void CCamera::WinPan(float _elapsedTime)
{
	m_Timer += _elapsedTime;

	ReOrient();

	//static bool state = true;
	//if(state)
	//{
	//	float clamp = (m_Timer / 0.8f);
	//	glsCopyVector3(m_Position, *m_Target->GetPosition());
	//	Rotate(float(glsDegToRad(90.0f) * clamp), 1.0f, 0.0f, 0.0f);
	//	Translate(0,0,-m_CamDistance);
	//	//m_CamDistance -= 40.0f * _elapsedTime;
	//	if(m_Timer >= 0.8f)
	//	{
	//		m_Timer = 0.0f;
	//		state = false;
	//	}
	//}
	//else
	//{
		float clamp = (m_Timer / 1.6f);
		glsCopyVector3(m_Position, *m_Target->GetPosition());
		Rotate((float)glsDegToRad(90.0f), 1.0f, 0.0f, 0.0f);
		Rotate(float(glsDegToRad(90.0f) * clamp), -1.0f, 0.0f, 0.0f);
		Translate(0,0,-m_CamDistance);
		//m_CamDistance += 40.0f * _elapsedTime;
	//	if(m_Timer >= 0.8f)
	//	{
	//		m_Timer = 0.0f;
	//		state = true;
	//	}
	//}
}

void CCamera::DeathPan(float _elapsedTime)
{
	if(!m_Target)
		return;

	CRenderNode* playerRenderNode = &m_Target->GetRenderNode();
	
	// Use players up as the forward: so we look at him.
	m_Orientation[6] = playerRenderNode->GetUpVector()[0];
	m_Orientation[7] = playerRenderNode->GetUpVector()[1];
	m_Orientation[8] = playerRenderNode->GetUpVector()[2];

	// Orthonormalize dat matrix!
	Vec3f up, right, forward;
	glsLoadVector3(right,	m_Orientation[0],m_Orientation[1],m_Orientation[2]);
	glsLoadVector3(up,		m_Orientation[3],m_Orientation[4],m_Orientation[5]);
	glsLoadVector3(forward, m_Orientation[6],m_Orientation[7],m_Orientation[8]);

	glsCrossProduct3( up, forward, right );
	glsNormalizeVector3( up );

	glsCrossProduct3( right, up, forward );
	glsNormalizeVector3( right );

	for(int i = 0; i < 3; ++i)
		m_Orientation[i] = right[i];

	for(int i = 0; i < 3; ++i)
		m_Orientation[i+3] = up[i];

	// Move the camera back, whatever amount you choose is enough!
	Vec3f moveCam;
	glsCopyVector3( moveCam, playerRenderNode->GetUpVector() );

	if(m_Timer <= 8.0f)
		m_Timer += 8.0f * _elapsedTime;

	m_CamDistance += m_Timer * _elapsedTime;
	glsScaleVector3( moveCam, -m_CamDistance );
	glsAddVectors3( m_Position, playerRenderNode->GetPosition(), moveCam );
}

void CCamera::Input(CInput& _input)
{
	if(m_Attached)
		return;

	/*if(_input.KeyDown(DIK_I))
		Translate(0,0,20.0f * m_ElapsedTime);

	if(_input.KeyDown(DIK_K))
		Translate(0,0,-20.0f * m_ElapsedTime);

	if(_input.KeyDown(DIK_J))
		Translate(20.0f * m_ElapsedTime,0,0);

	if(_input.KeyDown(DIK_L))
		Translate(-20.0f * m_ElapsedTime,0,0);

	if(_input.KeyDown(DIK_UP))
		Rotate(30.0f * 3.1415f / 180.0f * m_ElapsedTime, 1.0f, 0.0f, 0.0f);

	if(_input.KeyDown(DIK_DOWN))
		Rotate(30.0f * 3.1415f / 180.0f * m_ElapsedTime, -1.0f, 0.0f, 0.0f);
	
	if(_input.KeyDown(DIK_LEFT))
		Rotate(30.0f * 3.1415f / 180.0f * m_ElapsedTime, 0.0f, 1.0f, 0.0f);

	if(_input.KeyDown(DIK_RIGHT))
		Rotate(30.0f * 3.1415f / 180.0f * m_ElapsedTime, 0.0f, -1.0f, 0.0f);*/
}

void CCamera::HandleReaction( IEntity* _other )
{

}

void CCamera::ReOrient()
{
	if(!m_Target)
		return;
	if(((CPlayer*)m_Target)->GetType() == ET_PLAYER)
		m_CamDistance = 70.0f;
	else
		m_CamDistance = 85.0f;

	CRenderNode* playerRenderNode = &m_Target->GetRenderNode();
	
	// Use players up as the forward: so we look at him.
	m_Orientation[6] = playerRenderNode->GetUpVector()[0];
	m_Orientation[7] = playerRenderNode->GetUpVector()[1];
	m_Orientation[8] = playerRenderNode->GetUpVector()[2];

	// Orthonormalize dat matrix!
	Vec3f up, right, forward;
	glsLoadVector3(right,	m_Orientation[0],m_Orientation[1],m_Orientation[2]);
	glsLoadVector3(up,		m_Orientation[3],m_Orientation[4],m_Orientation[5]);
	glsLoadVector3(forward, m_Orientation[6],m_Orientation[7],m_Orientation[8]);

	glsCrossProduct3( up, forward, right );
	glsNormalizeVector3( up );

	glsCrossProduct3( right, up, forward );
	glsNormalizeVector3( right );

	for(int i = 0; i < 3; ++i)
		m_Orientation[i] = right[i];

	for(int i = 0; i < 3; ++i)
		m_Orientation[i+3] = up[i];

	// Move the camera back, whatever amount you choose is enough!
	Vec3f moveCam;
	glsCopyVector3( moveCam, playerRenderNode->GetUpVector() );
	glsScaleVector3( moveCam, -m_CamDistance );
	glsAddVectors3( m_Position, playerRenderNode->GetPosition(), moveCam );
}
bool CCamera::MainMenuLerpage(float _elapsedTime, IEntity* _startpt, IEntity* _endpt)
{
	Vec3f startingpos, endingpos, direction, offset, fromWorldCenter, worldcenter = { 0.0f, 75.0f, 0.0f };
	float length, altitude;
	// Get the positions of the 2 objects.
	glsCopyVector3(startingpos,*((CPlayer*)_startpt)->GetPosition());
	glsCopyVector3(endingpos,*((CBaseEntity*)_endpt)->GetPosition());

	// Get a vector to the ending position from the center of the world
	// and use it to find the altitude of the ending position
	glsSubtractVectors3( fromWorldCenter, endingpos, worldcenter );
	altitude = glsGetVectorLength3( fromWorldCenter );

	// Change the altitude of the ending position to bring it to ground level (75)
	glsCopyVector3( offset, ((CBaseEntity*)_endpt)->GetRenderNode().GetUpVector() );
	glsScaleVector3( offset, altitude - 75.0f );
	glsAddVectors3( endingpos, endingpos, offset );
	
	// Get the direction to walk
	glsSubtractVectors3( direction, endingpos, startingpos );
	length = glsGetVectorLength3( direction );
	glsNormalizeVector3( direction );

	if(length < 1.0)
		return false;

	// Get the player's new right vector
	Vec3f right;
	glsCrossProduct3( right, direction, ((CBaseEntity*)_endpt)->GetRenderNode().GetUpVector() );
	glsNormalizeVector3( right );
	
	// Set the facing direction
	((CPlayer*)_startpt)->SetMoveForward( direction );

	// Don't jitter the player's facing direction.
	if( length > 1.0 )
	{
		// Set the player's forward and up vectors
		glsScaleVector3( right, -1.0f );
		((CPlayer*)_startpt)->GetRenderNode().SetForwardVector( direction );
		((CPlayer*)_startpt)->GetRenderNode().SetRightVector( right );
	}

	// Walk in the direction we're facing.
	((CPlayer*)_startpt)->MoveForward( 10000000 );
	return true;
}

void CCamera::SetLookAt(Vec3f _look)
{
	// Orthonormalize dat matrix!
	Vec3f up, right, forward;
	glsLoadVector3(right,	m_Orientation[0],m_Orientation[1],m_Orientation[2]);
	glsLoadVector3(up,		m_Orientation[3],m_Orientation[4],m_Orientation[5]);
	glsLoadVector3(forward, m_Orientation[6],m_Orientation[7],m_Orientation[8]);

	glsCopyVector3(forward,_look);
	
	glsCrossProduct3( up, forward, right );
	glsNormalizeVector3( up );

	glsCrossProduct3( right, up, forward );
	glsNormalizeVector3( right );

	for(int i = 0; i < 3; ++i)
		m_Orientation[i] = right[i];
	for(int i = 0; i < 3; ++i)
		m_Orientation[i+3] = up[i];
	for(int i = 0; i < 3; ++i)
		m_Orientation[i+6] = forward[i];
}
