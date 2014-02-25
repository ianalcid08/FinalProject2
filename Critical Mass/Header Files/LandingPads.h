#ifndef _LANDING_PADS_H_
#define _LANDING_PADS_H_

#include "BaseEntity.h"
#include "RenderNode.h"



class CLandingPads  : public CBaseEntity
{

	Vec3f		m_Lowest, m_Highest;
	float		m_rotX, m_rotY;
public:
	CLandingPads(void);
	~CLandingPads(void) { Release(); }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );
	void Initialize ( double _X, double _Y );
	void DestroyShield( void );
	void Activate( void );
	void DeActivated( void );
	void InitShaders( void );
	void ResetToDown( void );
};

#endif