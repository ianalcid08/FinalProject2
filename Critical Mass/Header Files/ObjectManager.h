/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : ObjectManager.h
Author : Evan Wright

Description : This module holds all the currently active and inactive objects given to 
			  it from the factory.  The Object Manager will call each active entities 
			  update function passing along the Timer’s delta time.  It will also call 
			  their respective renders.  It will also use the Collosion library to 
			  check collisions.

Created :  07/17/2013
Modified : 07/19/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

class ScriptManager;
class Collision;
class VFXManager;
class CollisionEntity;
class AIManager;
class IEntity;


#include "Definitions.h"
#include "ObjectFactory.h"
#include "Renderer.h"
#include "CollisionManager.h"

#include <iostream>
#include <vector>

typedef std::vector<IEntity*> EntityList;
typedef std::vector<IEntity*>::iterator EntityListIter;

class CRenderer;

class CObjectManager
{
private:

	friend class CObjectFactory;


	CObjectFactory* m_ObjFactory;
	CRenderer*	 m_Renderer;

	EntityList m_EntityList[EG_NUM_TYPES];
	EntityList m_RenderList;

	CollisionManager m_CollisionManager;

	//Functions
	void AddEntity( IEntity* _entity, eEntityGroups _group );
	void RemoveEntity( IEntity* _entity, eEntityGroups _group );
public:

	CObjectManager( void );
	~CObjectManager( void ) { Release(); }

	void Initialize( CRenderer* _renderer, CObjectFactory* _objFactory );
	void Release( void );

	void UpdateEntities( float _elapsedTime );
	void CheckCollisions( void );
	void RenderEntities(void);

	void RemoveAllEntities( void );
	void RemoveHealthPickups ( void );

	void AddinBrokenShieldShit(IEntity* _entity);

	void ResetforWave(void);
};

#endif