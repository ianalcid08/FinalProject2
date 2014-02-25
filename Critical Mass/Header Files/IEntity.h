/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : IEntity.h
Author : Evan Wright

Description : The interface for all game entities.

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef IENTITY_H_
#define IENTITY_H_

#include "Definitions.h"

class IEntity
{
protected:
	char m_Type;	// objects ID
	char m_Flags;	// 01 active, 10 alive, 11 active and alive

public:
	virtual ~IEntity( void ) = 0 { }

	virtual void Release( void ) = 0;
	virtual bool Update( float _elapsedTime ) = 0;
	virtual void HandleReaction( void ) = 0;

	inline char GetType( void ) const { return this->m_Type; }
	inline bool GetIsAlive( void ) const { return ( (this->m_Flags & IS_ALIVE) == IS_ALIVE ); }
	inline bool GetIsActive( void ) const { return ( (this->m_Flags & IS_ACTIVE) == IS_ACTIVE ); }

	inline void SetType( char _type ) { this->m_Type = _type; }
	inline void SetIsAlive( bool _isAlive ) 
	{ 
		if( _isAlive )
			this->m_Flags |= IS_ALIVE; 
		else
			this->m_Flags &= ~IS_ALIVE;
	}
	inline void SetIsActive( bool _isActive ) 
	{ 
		if( _isActive )
			this->m_Flags |= IS_ACTIVE; 
		else
			this->m_Flags &= ~IS_ACTIVE;
	}
};

#endif