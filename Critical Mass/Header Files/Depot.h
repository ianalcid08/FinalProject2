/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Depot.h
Author :  Corey Morehead

Description : This Class will hold any variables and set up for the depot class.

Created :  07/23/2013
Modified : 07/23/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DEPOT_H_
#define DEPOT_H_

#include "LivingEntity.h"
#include "RenderNode.h"
#include "Shield.h"
enum State { ACTIVATED = 0, DEACTIVATED };

class CDepot : public CLivingEntity
{

public:
	CDepot( void );
	~CDepot( void ) { Release(); }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );
	void Initialize ( double _X, double _Y );
	void DestroyShield( void );
	void Activate( bool _invulnerable = false );
	void Deactivate( void );
	void InitShaders( void );
	void Reset( void );
	
	virtual void SwitchAnimation(void);
	
	void SetShieldPos( const Vec3f _pos );


	//Accessor
	inline int GetHealth( void ) { return m_health; }
	inline int GetState( void ) { return m_State; }
	IEntity* GetShield( void ) { return m_Shield; }
	inline unsigned int GetDepotExp ( void ) { return m_DepotExp; }

	//Mutator
	inline void SetHealth( int _health ) { m_health = _health; }
	inline void SetState( int _state ) { m_State = _state; }
	inline void SetShield( IEntity* _shield ) { m_Shield = _shield; }
	inline void SetDepotExp(unsigned int _exp) { m_DepotExp = _exp; }
	CRenderNode tmpNode;

private:
	int			m_health;
	int			m_State;
	
	float		m_rotX, m_rotY;

	IEntity*	m_Shield;
	unsigned int	m_DepotExp;
	float m_DeactivateTimer;
	float m_ActivateTimer;

	bool		m_Deactivated_Sound;
	bool		m_Activated_Sound;
};

#endif
