#ifndef _OBJWORLD_
#define _OBJWORLD_
/*****************************************************************
File name:			World.h
Date:				7/20/13
Modify Date:		7/22/13
Modify Initials:	IA
Author:				Ian Alcid
Purpose:			Class to handle the creation and destruction of the world object.
******************************************************************/
#include "BaseEntity.h"
class CWorld : public CBaseEntity
{

public:
	CWorld(void);
	~CWorld(void) { Release(); }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	void Init();
};

#endif