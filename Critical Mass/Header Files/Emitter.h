/*///////////////////////////////////////////////////////
/FileName:		Emitter.h
/Author:		Thomas Stefl
/Date:			8/6/2013
/Last Modified: 8/6/2013
/Purpose:		The purpose of the emittier is to
/				hold an array of particles, update them
/				and initialize them.
/*///////////////////////////////////////////////////////
#ifndef _EMITTER_H_
#define _EMITTER_H_

//Pre-defines
#define MAX_PARTICLES 50
#define PARTICLE_LIFESPAN_VARIATION 5  //this is the maximum difference in time if the particle's have 
									   //varied life spans
//Includes
#include "../Header Files/glsMath.h"
#include "../Header Files/Particle.h"
#include "../Header Files/IEntity.h"

class CRenderer;

class CEmitter
{
private:
	Particle  m_ParticleArr[MAX_PARTICLES];
	Vec3f m_Position;								//The emitter's position in the world
	IEntity* m_SourceEntity;		

	//offsets
	float m_RightOffset;
	float m_ForwardOffset;
	float m_UpOffset;

	float m_SpawnRate;
	float m_DeltaTime;   //a timer that is checked against spawn rate to determine whether or not a new particle should be spawned
	float m_Duration;
	float m_Radius;

	//Preferences/Settings
	//HAX
	//Author: Tom Stefl
	//Desc: Using bools instead of bit flags, not a huge deal but would like to change in the future
	bool m_IsRandomPos;
	bool m_IsActive;
	bool m_IsLooping;
	bool m_IsRandomParticleLifeSpans;
	bool m_IsTargetted;
	bool m_IsRandomVelocities;
	bool m_AllAtOnce;
	bool m_Beam;
	bool m_IsPull;
	bool m_IsSmoke;
	bool m_IsPush;
	bool m_IsEnemyDamage;
	
	int m_ParticleCap;   // The maximum number of particles a specific emitter can use.

	//Initial particle information
	Vec3f m_InitialParticleVelocity;
	Vec3f m_InitialParticleAcceleration;
	Vec3f m_ParticleColor;
	float m_InitialParticleLifeSpan;

public:
	CEmitter();
	void InitializeEmitter(bool _isActive, float _particleLifeSpan, float _spawnRate, Vec3f _particleVelocity,
		                   Vec3f _particleAcceleration, bool _isLooping, bool _isRandomLifebool, bool _isRandPos, 
						   bool _isTargetted, float _radius, float _forwardOffset,float _rightOffset, float _upOffset,
						   float _duration, bool _isRandVel);

	~CEmitter();
	void Update(float _fElapsedTime);
	void Render();
	void ReInitParticle(int _index);
	bool IsActive();  //{return m_IsActive;}
	void SetRandCircPoint(Vec3f _emitterPos, Vec3f _targetPos = nullptr);
	void LoadEmitter(char* _szFile);
	void ResetEmitter();
	
	//Mutators
	void SetPosition(const Vec3f _newPosition);
	inline void SetIsActive(bool _newBool)			{m_IsActive = _newBool;}
	inline void SetSpawnRate(float _newFloat)		{m_SpawnRate = _newFloat;}
	inline void SetLifeSpan(float _newLifeSpan)		{m_InitialParticleLifeSpan = _newLifeSpan;}
	inline void SetIsTargetted(bool _newBool)		{m_IsTargetted = _newBool;}
	inline void SetIsLooping(bool _newBool)			{m_IsLooping = _newBool;}
	inline void SetDuration(float _newFloat)		{m_Duration = _newFloat;}
	inline void SetIsRandomVelocities(bool _newBool){m_IsRandomVelocities = _newBool;}
	inline void SetEntity(IEntity * _newEntityPtr)	{m_SourceEntity = _newEntityPtr;}
	inline void SetForwardOffset(float _newFloat)   {m_ForwardOffset = _newFloat;}
	inline void SetRightOffset(float _newFloat)		{m_RightOffset = _newFloat;}
	inline void SetUpOffset(float _newFloat)		{m_UpOffset = _newFloat;}
	inline void SetAllAtOnce(bool _newBool)			{m_AllAtOnce = _newBool;}
	inline void SetBeam(bool _newBool)				{m_Beam = _newBool;}
	inline void SetIsSmoke(bool _newBool)			{m_IsSmoke = _newBool;}
	inline void SetIsPush(bool _newBool)			{m_IsPush = _newBool;}
	inline void SetIsPull(bool _newBool)			{m_IsPull = _newBool;}
	inline void SetIsEnemyDamage(bool _newBool)		{m_IsEnemyDamage = _newBool;}

	inline void SetSpawnRadius(float _newFloat)		{m_Radius = _newFloat;}


	inline void SetParticleCap(int _newFloat)     {m_ParticleCap = _newFloat;}
	void SetParticleColor(Vec3f _newColor);

	void SetTarget(Vec3f _newTarget);
	void SetInitPaticleVelocity(Vec3f _newVelocity);
	void SetInitPaticleAcceleration(Vec3f _newAcceleration);

	//Accessors
	inline Vec3f* GetPosition()		{return &m_Position;}
	inline Particle* GetParticleArr()  {return m_ParticleArr;}
};

#endif
