/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Shield.h
Author :  Corey Morehead

Description : This Class will hold any variables and set up for the shield class.

Created :  07/25/2013
Modified : 07/25/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SHIELD_H_
#define SHIELD_H_

#include "BaseEntity.h"

class CShield : public CBaseEntity
{
public:
	CShield(void);
	~CShield(void) { Release(); }

	// IEntity interface:
	void Release ( void );
	bool Update (float _elapsedTime );
	virtual void HandleReaction ( void );
	void Initialize ( double _X, double _Y, bool _invulnerable );
	void TakeDamage ( int _damage );
	void InitShaders();

	void ResetShield();

	float GetCollisionSize(void);
	void ResizeCollisionEntity(float _size);

	// Accessor
	inline int GetHealth (void ) { return m_Health; }
	inline int GetColorType( void ) { return m_ColorType; }
	inline float GetIneffectiveHitTimer () { return m_ineffectiveHitTimer;}
	inline float GetEffectiveHitTimer() {return m_effectiveHitTimer;}
	inline unsigned int GetOwnership() {return m_Ownership;}
	inline bool GetInvulnerable() {return m_invulnerable;}
	//inline bool GetAwarded() {return m_awarded;}

	// Mutator
	inline void SetHealth (int _health) { m_Health = _health; }
	inline void SetColorType( int _color ) { m_ColorType = _color; }
	inline void SetIneffectiveHitTimer (float _timer) { m_ineffectiveHitTimer = _timer; }
	inline void SetEffectiveHitTimer (float _timer)		{m_effectiveHitTimer = _timer;}
	inline void SetOwnership(unsigned int _owner) {m_Ownership = _owner;}
	//inline void SetAwarded(bool _awarded) {m_awarded = _awarded;}

private:

	int m_Health;
	float m_InvulnerableTimer;
	float m_ineffectiveHitTimer;
	float m_effectiveHitTimer;
	int m_ColorType;
	float m_LaserTimer;
	bool m_ineffective;
	bool m_effective;
	bool m_invulnerable;
	unsigned int m_Ownership;
	bool m_awarded[3];
};

#endif

