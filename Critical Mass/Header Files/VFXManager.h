/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : VFXManager.h
Author : Tom Stefl & Evan Wright

Description : Handles all visual effects in the game. On startup allocates a pool of 
			  memory to use for all emitters in the game. Uses particles and meshes to 
			  create unique and appealing effects for the game, such as; explosions, 
			  sparks, gaining power-ups, etc. When an emitter is finished it’s memory 
			  is recycled back into the VFXs memory pool for use when another emitter 
			  is needed.

Created :  07/18/2013
Modified : 07/18/2013

Last Modified: 8/6/2013
Last Modified by:  Tom Stefl

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _VFXMANAGER_H_
#define _VFXMANAGER_H_

#include "glsMath.h"
#include <vector>

#include "../Header Files/Emitter.h"
#include "../Header Files/Particle.h"
#include "../Header Files/GeometryEffect.h"
#include "../Header Files/GeometryEmitter.h"


#define MAX_EMITTERS		50
#define MAX_GEO_EMITTERS	2000


class CRenderer;



class CVFXManager
{
	//Front load all of the Particles Array
	CEmitter			m_EmitterArray[MAX_EMITTERS];
	GeometryEmitter		m_GeoEmitterArray[MAX_GEO_EMITTERS];
	GeometryEmitter     m_MuzzleFlash;


	//Stuff for the HP compass
	GeometryEmitter		m_HPArrow;

public:
	CRenderer*	m_renderer;
	CVFXManager( void );
	~CVFXManager(void) { }
	void Initialize( CRenderer* _renderer );
	void Update( float _elapsedTime );
	void Render();
	//void LoadEmitter(char* _szFile,unsigned int _index, Vec3f _position);

	//TO DO - eventually get rid of start and replace with request
	void StartEmitter(Vec3f _position, float _duration);
	void StartGeoEmmiter(Vec3f _position,float _duration, unsigned int _enumEffectType, unsigned int _enumFXAnimType);

	unsigned int RequestEmitter(unsigned int _enumFXType,Vec3f _position, IEntity* _entity = nullptr);

	void TurnOffEmitter(unsigned int _index);

	void ResetGeoEmitters(void);

	//Accessors
	CEmitter* GetEmitterArr(int _index) {return &m_EmitterArray[_index];}
	GeometryEmitter* GetMuzzleFlashFX() {return &m_MuzzleFlash;}
	GeometryEmitter* GetHPArrowFX() {return &m_HPArrow;}

	void SetHPArrowActive(bool _newBool);
	

};

#endif
