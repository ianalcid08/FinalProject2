/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : RedBarrel.h
Author : Corey Morehead

Description : This Class will hold any variables and set up for the Barrel class.

Created :  08/06/2013
Modified : 08/06/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef REDBARREL_H_
#define REDBARREL_H_

#include "BaseEntity.h"

class CRedBarrel : public CBaseEntity
{
public:
	CRedBarrel( void );
	~CRedBarrel( void ) { Release(); }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );
	void Initialize ( double _X, double _Y );
	void TakeDamage (int _damage);

	// Accessors
	inline int GetHealth( void ) { return m_Health; }

	// Mutators
	inline void SetHealth( int _damage ) { m_Health = _damage; }

	void InitShaders();

private:

	int						m_Health;
	float					m_rotX, m_rotY;
	float					m_InvulnerableTimer;
	float					m_LaserTimer;
	bool					m_Explode;
	float					m_FlashTimer;
};

#endif
