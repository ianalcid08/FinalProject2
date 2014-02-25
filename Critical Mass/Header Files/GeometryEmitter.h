/*///////////////////////////////////////////////////////
/Filename      : GeometryEmiter.h
/Author 	   : Tom Stefl
/Date:		   : 8/8/2013
/Last Modified : 8/8/2013
/Purpose	   :  This file acts at the emitter for the different geometry 
/				  effects in the game.  It will  hold a position that all
/				  of the geometry effects will 
/*////////////////////////////////////////////////////////
#ifndef _GEOMETRY_EMITTER_H_
#define _GEOMETRY_EMITTER_H_

#include "../Header Files/GeometryEffect.h"

class IEntity;

class GeometryEmitter
{
private:
	GeometryEffect m_GeoEffect;
	bool m_IsActive;

	float m_Duration;
	Vec3f m_Position;
	bool m_IsLooping;
	IEntity * m_SourceEntity;

public:
	GeometryEmitter(void);
	~GeometryEmitter(void);
	void Update(float _fElapsedTime);
	void InitializeEmitter(unsigned int _index, IEntity* _entity, const Vec3f _position, float _duration, unsigned int _enumEffectType, unsigned int _enumFXAnimType);
	inline GeometryEffect* GetGeoEffect()  {return &m_GeoEffect;}
	void Render();
	//bool IsActive();

	//Accessors
	//GeometryEffect* GetEffect() {return &m_GeoEffect;}
	float GetDuration()	{return m_Duration;}
	Vec3f* GetPosition() {return &m_Position;}
	bool IsActive() {return m_IsActive;}

	//Mutators
	void SetDuration(float _newDuration)	{m_Duration = _newDuration;}
	void SetPosition(const Vec3f _newPosition);
	void SetIsLooping(bool _newBool)		{m_IsLooping = _newBool;}
	void SetIsActive(bool _newBool)			{m_IsActive = _newBool;}

};
#endif

