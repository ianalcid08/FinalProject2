/*
	File:		LightPole.h
	Author:		Bob Pasekoff
	Updated:	8/15/2013
*/

#ifndef _LIGHT_POLE_
#define _LIGHT_POLE_

#include "BaseEntity.h"
#include "Light.h"

class CLightPole : public CBaseEntity
{
	float			m_rotX, m_rotY;
	CPointLight		m_pointLight;

public:
	CLightPole( void );
	~CLightPole( void ){ Release(); }

	CPointLight&	GetPointLight( void ) { return m_pointLight; }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );
	void Initialize ( double _X, double _Y );
};


#endif //_LIGHT_POLE_