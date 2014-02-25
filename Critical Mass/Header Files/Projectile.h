/*----------------------------------------------------
TITLE: Projectile.h
PURPOSE: sets the projectile vaules
AUTHOR: Corey Morehead
-----------------------------------------------------*/
#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "../Header Files/MovingEntity.h"

class CGame;

class CProjectile : public CMovingEntity
{
private:
	float	m_life;
	bool	m_Consumed;
	float	m_Speed;
	bool	m_Strike;
	bool	m_FromBoss;


	//Tom
	float m_TrailTimer;

public:
	CProjectile( void );
	~CProjectile( void ) { Release(); }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	//void Initialize (Vec3f _velocity, Vec3f _position, Matrix4x4f _worldPosition);
	void Initialize();
	virtual void HandleReaction( void );

	void Adjust( Vec3f _planet );
	
	bool m_Explode;

	bool	GetIfExplode()	{return m_Explode;}
	float	GetLifeTime()	{return m_life;}
	bool	GetIfConsumed() {return m_Consumed;}
	float	GetSpeed()		{return m_Speed;}
	bool	GetStrike()		{return m_Strike;}

	void	SetIfExplode(bool _explode)		{m_Explode = _explode;}
	void	SetLifeTime(float _lt)			{m_life = _lt;}
	void	SetIfConsumed(bool _consumed)	{m_Consumed = _consumed;}
	void	SetSpeed(float _newFloat)		{m_Speed = _newFloat;}
	void	SetStrike(bool _strike)			{m_Strike = _strike;}

	//int					m_nAnimState;
	//enum					anim_states { ANIM_IDLE = 0, ANIM_DEATH };
};

#endif
