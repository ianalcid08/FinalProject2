#include "../Header Files/stdafx.h"
#include "../Header Files/Laser.h"
#include "..\Header Files\Capsule.h"
#include "..\Header Files\CollisionLib.h"
#include "..\Header Files\GLSetup.h"
#include "..\Header Files\AssetManager.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

CLaser::CLaser ( void )
{
	this->SetType(ET_LASER);
	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, -1.0f );
	m_LaserTime = 3.0f;
	m_beingUsed = false;

	m_MaxScaleTimer = MAX_SCALE_TIMER;
}

void CLaser::Initialize ( CBaseEntity* _destination )
{
	Vec3f start, endOffset, end;
	m_Destination = _destination;
	glsCopyVector3(start, m_Destination->GetRenderNode().GetPosition());				//sets the start to the players position.
	Vec3f offset;
	glsCopyVector3(offset, _destination->GetRenderNode().GetForwardVector());
	glsScaleVector3(offset, 15.0f);
	glsAddVectors3(start, start, offset);
	glsCopyVector3(endOffset, m_Destination->GetRenderNode().GetForwardVector());		//sets the direction
	glsScaleVector3(endOffset, 35.0f);													//sets the distance of the laser
	glsAddVectors3(end, endOffset, start);


	CCollisionEntity* entity = GetCollisionEntity();
	if( entity )
	{
		delete entity;
	}

	//collision with the capsule...max radius should be around 10.0f
	this->SetCollisionEntity(new Capsule(Segment(start, end), 8.0f));			

	//Sets position, up, right, fwd
	GetRenderNode().SetPosition(start);				
	GetRenderNode().SetForwardVector(_destination->GetRenderNode().GetForwardVector());
	GetRenderNode().SetUpVector(_destination->GetRenderNode().GetUpVector());
	GetRenderNode().SetRightVector(_destination->GetRenderNode().GetRightVector());

	
//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_RAIN_LOOP);

	//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_MISSLE_LOOP);

	//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::las);


	m_LaserScaleTimer = 0.0f;

	m_CurrLevel = 1;
}

void CLaser::InitShaders()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[LASER_SHAPE_TB] );
	GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetObjData(ET_LASER).m_DiffuseHandle );
	GetRenderNode().SetColor(0.0f,0.0f,1.0f,0.8f);

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[SCROLLING_UV] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[SCROLLING_UV] );
	GetRenderNode().SetTimeUniformHandle( CAssetManager::GetInstance()->m_vUniformTimeHandles[SCROLLING_UV] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[SCROLLING_UV] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[SCROLLING_UV] );
	GetRenderNode().SetEnableBlending( true );
}

void CLaser::Release ( void )
{
	CBaseEntity::Release();
}

bool CLaser::Update( float _elapsedTime )
{

	if (m_beingUsed == false)
	{
		this->SetIsActive(false);
		this->SetIsAlive(false);
		this->SetScaleTimer(0.0f); // reset the scale for when the user lets go.
//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_LASER_LOOP);
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_LASER_INTENSITY1);
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_LASER_INTENSITY2);
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_LASER_INTENSITY3);

	}

	if(m_LaserScaleTimer >= m_MaxScaleTimer)
	{
		m_LaserScaleTimer = m_MaxScaleTimer;
	}
	else
	{
		m_LaserScaleTimer += 10.0f*_elapsedTime;
//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_LASER_CHARGE_UP);
		//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::);

	}

	m_beingUsed = false;

	CBaseEntity::Update(_elapsedTime);

	if( m_Destination != nullptr )
	{
		Vec3f start, endOffset, end, right, forward;
		glsCopyVector3(start, m_Destination->GetRenderNode().GetPosition());
		glsCopyVector3(right, m_Destination->GetRenderNode().GetRightVector());
		//sets the offset to 60
		glsCopyVector3(forward, m_Destination->GetRenderNode().GetForwardVector());

		Matrix3x3f downALittle;
		glsRotationMatrix3x3( downALittle, (float)glsDegToRad( 15.0f ), -right[0], -right[1], -right[2] );
		glsRotateVector( endOffset, forward, downALittle );

		glsScaleVector3(endOffset, m_LaserScaleTimer * 8.0f);	
		glsAddVectors3(end,endOffset,start);

		CCollisionEntity* entity = GetCollisionEntity();
		if( entity )
		{
			delete entity;
		}

		// Update the collision entity
		Vec3f moveFor;
		glsCopyVector3(moveFor, m_Destination->GetRenderNode().GetForwardVector());
		glsScaleVector3(moveFor, m_LaserScaleTimer*2.0f + 2.0f);
		glsAddVectors3(start, start, moveFor);
		this->SetCollisionEntity(new Capsule(Segment(start, end), m_LaserScaleTimer*2.0f));	

		float f_scale = m_LaserScaleTimer/3.0f;
		if( m_LaserScaleTimer >= m_MaxScaleTimer )
		{
			f_scale = m_MaxScaleTimer*1.2f;
		}

		Scale(m_LaserScaleTimer*0.5f,m_LaserScaleTimer*0.5f,f_scale);

		Vec3f offset, scaledForward;
		glsCopyVector3(scaledForward,m_Destination->GetRenderNode().GetForwardVector());
		glsScaleVector3(scaledForward,1.0f);
		glsAddVectors3(offset,m_Destination->GetRenderNode().GetPosition(),scaledForward);
		GetRenderNode().SetPosition( offset );
	}
	return true;
}

void CLaser::Scale( float u_size, float r_size, float f_size )
{
	Vec3f tempUp, tempRight, tempForward;
	glsCopyVector3(tempUp, m_Destination->GetRenderNode().GetUpVector());
	glsCopyVector3(tempRight, m_Destination->GetRenderNode().GetRightVector());
	glsCopyVector3(tempForward, m_Destination->GetRenderNode().GetForwardVector());

	// Default Orientation.
	glsScaleVector3(tempUp, u_size);
	glsScaleVector3(tempRight, r_size);
	glsScaleVector3(tempForward, f_size);

	GetRenderNode().SetUpVector(tempUp);
	GetRenderNode().SetRightVector(tempRight);
	GetRenderNode().SetForwardVector(tempForward);
}

void CLaser::HandleReaction( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		if( (*iter)->GetType() != ET_PLAYER && (*iter)->GetType() != this->GetType() )
		{
			if(m_LaserTime == 0.0f)
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);
			}
		}
	}
}
