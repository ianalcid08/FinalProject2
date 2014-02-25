/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Boss.h
Author : Evan Wright

Description : The final challenge in the game! THE BOSS!

Created :  09/17/2013
Modified : 09/17/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _BOSS_ENEMY_H_
#define _BOSS_ENEMY_H_

#include "Enemy.h"

//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

#define BOSS_ENEMY_TIMER_HURT		0.2f
#define BOSS_ENEMY_SPEED_MOVE		10.0f
#define BOSS_ENEMY_TIMER_DEATH		18.0f
#define BOSS_ENEMY_TIMER_E_INTERVAL	0.4f
#define MAX_HP_BOSS_ENEMY			800
#define BOSS_SHIELD_FADE_WAIT	4.0f
#define BOSS_SHIELD_FADE_IN		1.0f

class CShield;
class CBossEnemy : public CEnemy
{
	// Timers for various status'
	float m_LaserTimer;
	float m_DeathTimer;
	float m_HurtTimer;
	float m_JustSpawnedTimer;

	// For moving that isn't in the same direction as the look.
	Vec3f m_MoveUp;
	Vec3f m_MoveRight;
	Vec3f m_MoveForward;

	// Has no shield in the third stage!
	CShield*	m_Shield;
	bool		m_NoShield;
	float		m_ShieldFadeIn;
	float		m_ShieldFadeWait;

	// For phase
	unsigned int m_Phase;
	bool		 m_PhaseTauntPlayed;
	std::vector<AkUniqueID> m_TauntIDs;

	// For death effects and stuff
	float m_ExplosionInterval;

public:
	CBossEnemy( void );
	~CBossEnemy( void ) { Release(); }

	void Init( void );
	void Reset( void );
	void UpdateVelocity( void );

	void TakeDamage( int _amount );
	void Kill( void );
	void PlayRandomTaunt( void );

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	// Accessors
	CShield* GetShield( void ) { return m_Shield; }
	unsigned int GetPhase( void ) { return m_Phase; }

	// Mutators
	void SetShield( CShield* _shield ) { m_Shield = _shield; }
};

#endif