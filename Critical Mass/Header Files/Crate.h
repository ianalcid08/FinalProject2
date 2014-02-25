/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Crate.h
Author : Evan Wright

Description : 

Created :  07/16/2013
Modified : 08/13/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef CRATE_H_
#define CRATE_H_

#include "LivingEntity.h"
class CCrate : public CLivingEntity
{

	int			m_Health;
	float		m_rotationX;
	float		m_rotationY;
	float		m_deathTimer;
	float		m_FallingTimer;
	Vec3f		m_startingPt;
	bool		m_AmIBeingPulled;
	bool		m_PrevPulled;

	//movement
	Vec3f m_MoveUp;
	Vec3f m_MoveRight;
	Vec3f m_MoveForward;

public:
	CCrate( void );
	~CCrate( void ) { Release(); }

	void Init(float _rotX, float _rotY);
	void TakeDamage(int _damage);
	void InitShaders();
	void Reset();
	void CrateBeams(Vec3f _pos);
	void UpdateVelocity( void );

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );
	
	int GetHealth() {return m_Health;}
	float GetDeathTimer() {return m_deathTimer;}
	float GetFallingTimer() {return m_FallingTimer;}
	bool GetBeingPulled() {return m_AmIBeingPulled;}

	void SetHealth(int _hp) {m_Health = _hp;}
	void SetDeathTimer(float _timer) {m_deathTimer = _timer;}
	void SetFallingTimer(float _timer) {m_FallingTimer = _timer;}
	void SetBeingPulled(bool _pulled) {m_AmIBeingPulled = _pulled;}

};

#endif
