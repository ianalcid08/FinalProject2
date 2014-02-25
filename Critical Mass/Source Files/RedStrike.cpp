/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : RedStrike.cpp
Author : Evan Wright

Description : This is the marker on the ground, that calls the mortars to the ground, in it's area.
			  Creates mortars, handles how fast the spawn and how long they spawn.

Created :  08/22/2013
Modified : 08/22/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/RedStrike.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"

//#define STRIKE_DURATION		5.0f
//#define STRIKE_RATE			0.1f
//#define STRIKE_TIMER_START	0.1f
//#define STRIKE_TIMER_END		4.0f
//#define STRIKE_SIZE			4.0f

CRedStrike::CRedStrike( void )
{
	this->SetType(ET_REDSTRIKE);

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector(  1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(2.0f, pos));

	STRIKE_DURATION		= 5.0f;
	STRIKE_RATE			= 0.1f;
	STRIKE_TIMER_START	= 0.1f;
	STRIKE_TIMER_END	= 4.0f;
	STRIKE_SIZE			= 4.0f;

	Reset(STRIKE_DURATION,STRIKE_RATE,STRIKE_TIMER_START,STRIKE_TIMER_END,STRIKE_SIZE);
}

void CRedStrike::Enter( Vec3f position, bool _useFinalBlast )
{
	m_UseFinalBlast = _useFinalBlast;

	GetRenderNode().SetPosition( position );

	// Orient based on planets surface, so it's RESTING on the surface of the planetoid.
	Vec3f planet = { 0.0f, 75.0f, 0.0f };
	Vec3f newUp, newRight, newForward;
	glsSubtractVectors3( newUp, planet, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( newUp );
	glsNormalizeVector3( newUp );
	GetRenderNode().SetUpVector( newUp );

	// CLAMP TO THE PLANET!
	float off = 0.0f;
	if( length < 75.0f )
		off = length - 75;
	else if(length > 75.0f)
		off = length - 75;
	if(off != 0.0f)
	{
		Vec3f pos, newPos;
		Vec3f move;
		glsCopyVector3( pos, this->GetRenderNode().GetPosition() );
		glsCopyVector3( move, this->GetRenderNode().GetUpVector() );
		glsScaleVector3( move, off );
		glsAddVectors3( newPos, pos, move );	
		GetRenderNode().SetPosition( newPos );
	}

	glsCrossProduct3( newForward, this->GetRenderNode().GetRightVector(), newUp );
	glsNormalizeVector3( newForward );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, newUp, newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );

	// Now move it a bit off the ground, so the base of the mesh is touching the ground.
	Vec3f tempUp, pos;
	glsCopyVector3(tempUp,this->GetRenderNode().GetUpVector());
	glsScaleVector3(tempUp,-0.0f);
	glsAddVectors3(pos,this->GetRenderNode().GetPosition(),tempUp);
	GetRenderNode().SetPosition( pos );

	glsCopyVector3(m_StartUp,this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_StartRight,this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_StartForward,this->GetRenderNode().GetForwardVector());
}

void CRedStrike::Init( void )
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[EMP_TARGETER_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SHIELD).m_DiffuseHandle );
	GetRenderNode().SetColor( 1.0f, 0.0f, 0.0f, 1.0f );

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );

	Reset(STRIKE_DURATION,STRIKE_RATE,STRIKE_TIMER_START,STRIKE_TIMER_END,STRIKE_SIZE);
}

void CRedStrike::Reset( float _duration, float _rate, float _timerStart, float _timerEnd, float _strikeSize )
{
	STRIKE_DURATION		= _duration;
	STRIKE_RATE			= _rate;
	STRIKE_TIMER_START	= _timerStart;
	STRIKE_TIMER_END	= _timerEnd;
	STRIKE_SIZE			= _strikeSize;
	
	m_Rate			= STRIKE_RATE;
	m_StartTimer	= STRIKE_TIMER_START;
	m_EndTimer		= STRIKE_TIMER_END;
	m_Timer			= 0.0f;
	m_Size			= STRIKE_SIZE;
}

void CRedStrike::Release( void )
{
	CBaseEntity::Release();
}

bool CRedStrike::Update( float _elapsedTime )
{
	// If it needs to start the marker needs to be scaled!
	if( m_StartTimer > 0.0f )
	{
		// Resize it, to become the max size!
		Scale( ((float(STRIKE_TIMER_START)-m_StartTimer)/float(STRIKE_TIMER_START))*m_Size );
		m_StartTimer -= _elapsedTime;
	}
	else if( STRIKE_DURATION > m_Timer )
	{
		Scale(m_Size);
		// If the timer is greater than the rate spawn a mortar
		if( m_Timer > m_Rate )
		{
			// TODO:: SPAWN MORTAR
			Vec3f pos;
			float m_Area = (float)(rand()%(int)STRIKE_SIZE*7);
			float randAngle = (float)glsDegToRad(float(rand()%360));
			pos[0] = this->GetRenderNode().GetPosition()[0] + m_Area * cos(randAngle) * m_StartForward[0] + m_Area * sin(randAngle) * m_StartRight[0];
			pos[1] = this->GetRenderNode().GetPosition()[1] + m_Area * cos(randAngle) * m_StartForward[1] + m_Area * sin(randAngle) * m_StartRight[1];
			pos[2] = this->GetRenderNode().GetPosition()[2] + m_Area * cos(randAngle) * m_StartForward[2] + m_Area * sin(randAngle) * m_StartRight[2];
			CMessageSystem::GetInstance()->SendMessageW(new CCreateMortarMessage(pos[0],pos[1],pos[2],false));
			m_Rate += STRIKE_RATE;
		}
		m_Timer += _elapsedTime;
	}
	else if( m_EndTimer > 0.0f && m_UseFinalBlast )
	{
		if( m_EndTimer == STRIKE_TIMER_END )
		{
			Vec3f pos;
			glsCopyVector3(pos, this->GetRenderNode().GetPosition() );
			CMessageSystem::GetInstance()->SendMessageW(new CCreateMortarMessage(pos[0],pos[1],pos[2],true));
		}
		m_EndTimer -= _elapsedTime;
	}
	else
	{
		this->SetIsActive(false);
		this->SetIsAlive(false);
		Reset(STRIKE_DURATION,STRIKE_RATE,STRIKE_TIMER_START,STRIKE_TIMER_END,STRIKE_SIZE);
	}
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CRedStrike::Scale( float size )
{
	Vec3f tempUp, tempRight, tempForward;
	glsCopyVector3(tempUp, m_StartUp);
	glsCopyVector3(tempRight, m_StartRight);
	glsCopyVector3(tempForward, m_StartForward);

	// Default Orientation.
	glsScaleVector3(tempUp, size/4.0f);
	glsScaleVector3(tempRight, size);
	glsScaleVector3(tempForward, size);

	GetRenderNode().SetUpVector( tempUp );
	GetRenderNode().SetRightVector( tempRight );
	GetRenderNode().SetForwardVector( tempForward );
}

void CRedStrike::HandleReaction( void )
{

}
