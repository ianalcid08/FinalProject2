/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Shield.h
Author :  Corey Morehead

Description : This Class will hold any variables and set up for the Explosion class.

Created :  07/25/2013
Modified : 08/15/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "BaseEntity.h"

class CExplosion : public CBaseEntity
{
	float		m_Life;
	bool		m_Consumed;
	bool		m_IsEMP;
	unsigned int m_Ownership;

	float		m_Size;
	float		m_ExpandTimer;
	bool		m_FinishedExpansion;

public:
	CExplosion( void );
	~CExplosion( void ) {Release();}

	// IEntity interface:
	void Release( void );
	bool Update(float _elapsedTime );
	void HandleReaction( void );
	void Initialize( Vec3f _position, Matrix4x4f _worldPosition );
	void TakeDamage( int _damage );

	// Helper function for procedural animation
	void Scale( float size );
	
	// Accessor
	inline float GetLife( void )		{return m_Life;}
	inline bool GetIfConsumed( void )	{return m_Consumed;}
	inline bool GetIsEMP( void ) const	{return m_IsEMP;}
	inline float GetSize( void ) const	{return m_Size;}
	inline unsigned int GetOwner( void ) const {return m_Ownership;}

	// Mutator
	inline void SetLife( float _life )			{m_Life = _life;}
	inline void SetIfConsumed( bool _consumed ) {m_Consumed;}
	inline void SetIsEMP( bool _is )			{m_IsEMP = _is;}
	inline void SetExpandTimer( float _timer )	{m_ExpandTimer = _timer;}
	inline void SetSize( float _size )			{m_Size = _size;}
	inline void SetOwner(unsigned int _owner)	{m_Ownership = _owner;}
};

#endif