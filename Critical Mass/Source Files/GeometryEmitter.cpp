/*///////////////////////////////////////////////////////
/Filename      : GeometryEmiter.cpp
/Author 	   : Tom Stefl
/Date:		   : 8/8/2013
/Last Modified : 8/8/2013
/Purpose	   :  This file acts at the emitter for the different geometry 
/				  effects in the game.  It will  hold a position that all
/				  of the geometry effects will 
/*////////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/GeometryEmitter.h"
#include "../Header Files/IEntity.h"

GeometryEmitter::GeometryEmitter(void)
{
	Vec3f zero = {0.0f,0.0f,0.0f};
	glsCopyVector3(m_Position,zero);
	m_Duration = 0.0f;
	m_IsActive = false;
	
}

GeometryEmitter::~GeometryEmitter(void)
{

}

void GeometryEmitter::InitializeEmitter(unsigned int _index, IEntity* _entity, const Vec3f _position, float _duration, unsigned int _enumEffectType, unsigned int _enumFXAnimType)
{
	//Turn on the emitter
	m_Duration = _duration;
	m_IsActive = true;
	m_IsLooping = true;
	//put the emitter in the game
	glsCopyVector3(m_Position,_position);
	//itialize the geo effect in the emitter
	m_GeoEffect.InitializeEffect(_enumEffectType, _enumFXAnimType, _duration, _position, _entity);
	m_SourceEntity = _entity;

}

void GeometryEmitter::Update(float _fElapsedTime)
{
	if(!m_IsLooping)					//If the effect is not looping infinitely, update the duration
	{
		m_Duration -= _fElapsedTime;
	}

	if(m_Duration <= 0.0f && !m_IsLooping)
	{
		m_IsActive = false;
	}

	if((m_SourceEntity == nullptr || !m_SourceEntity->GetIsActive()) || (!m_IsLooping && m_Duration <= 0.0f) || !m_GeoEffect.IsActive())// if the source entity no longer exists, stop the emitter
	{
		m_IsActive = false;
	}

	if(m_IsActive)	//Update the effect if the emitter is active
	{
		m_GeoEffect.Update(_fElapsedTime);
	}
}

void GeometryEmitter::Render()
{

}

void GeometryEmitter::SetPosition(const Vec3f _newPosition)
{
	glsCopyVector3(m_Position,_newPosition);
}

