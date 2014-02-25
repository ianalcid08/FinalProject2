/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : ObjectManager.cpp
Author : Evan Wright

Description : This module holds all the currently active and inactive objects given to 
			  it from the factory.  The Object Manager will call each active entities 
			  update function passing along the Timer’s delta time.  It will also call 
			  their respective renders.  It will also use the Collosion library to 
			  check collisions.

Created :  07/17/2013
Modified : 07/19/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../Header Files/stdafx.h"
#include "../header files/objectmanager.h"
#include "../header files/ScriptManager.h"
#include "../header files/VFXManager.h"
#include "../header files/AIManager.h"
#include "../header files/IEntity.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/CollisionLib.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CObjectManager::CObjectManager( void )
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Initializes the ObjectManager
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CObjectManager::Initialize( CRenderer* _renderer, CObjectFactory* _objFactory  )
{
	m_Renderer = _renderer;
	m_ObjFactory = _objFactory;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CObjectManager::Release( void )
{
	RemoveAllEntities();
}

void CObjectManager::RenderEntities(void)
{
	for( int i = 0; i < EG_NUM_TYPES; ++i )
	{
		int size = m_EntityList[i].size();
		for( EntityListIter iter = m_EntityList[i].begin(); iter != m_EntityList[i].end(); ++iter)
		{
			int type = (*iter)->GetType();
			m_Renderer->AddToRenderList( &((CBaseEntity*)(*iter))->GetRenderNode() );	
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loops through the object list, calling the updates on the object.
Parameters:
[in] _elapsedTime - the time passed since the last frame.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CObjectManager::UpdateEntities( float _elapsedTime )
{
	for( int i = 0; i < EG_NUM_TYPES; ++i )
	{
		int size = m_EntityList[i].size();
		for( EntityListIter iter = m_EntityList[i].begin(); iter != m_EntityList[i].end(); ++iter)
		{
			if( (*iter)->GetIsActive() && (*iter)->GetIsAlive() )
			{
				if( !(*iter)->Update(_elapsedTime) )
				{
					m_ObjFactory->Destroy((*iter));
				}
			}
			else
			{
				m_ObjFactory->Destroy((*iter));
			}

		}
	}
}

void CObjectManager::CheckCollisions( void )
{
	for( EntityListIter outerIter = m_EntityList[EG_NONSTATIC].begin(); outerIter != m_EntityList[EG_NONSTATIC].end(); ++outerIter )
	{
		if( !(*outerIter)->GetIsActive() && !(*outerIter)->GetIsAlive() )
			continue;

		for( int i = 0; i < EG_NUM_TYPES; ++i )
		{
			for( EntityListIter innerIter = m_EntityList[i].begin(); innerIter != m_EntityList[i].end(); ++innerIter )
			{
				if( (*innerIter)->GetIsActive() && (*innerIter)->GetIsAlive() && (*outerIter) != (*innerIter) )
				{
					if(CheckCollision( ((CBaseEntity*)(*outerIter)), ((CBaseEntity*)(*innerIter)) ))
					{
						if(((CBaseEntity*)(*outerIter))->GetType() == ET_LASER || ((CBaseEntity*)(*innerIter))->GetType() == ET_LASER)
							int x = 0;
						if(((CBaseEntity*)(*outerIter))->GetType() == ET_SHIELD || ((CBaseEntity*)(*innerIter))->GetType() == ET_SHIELD)
							int x = 0;

						m_CollisionManager.AddCollision((*outerIter), (*innerIter));
					}
				}
			}
		}
	}

	m_CollisionManager.Execute();
}

void CObjectManager::AddinBrokenShieldShit(IEntity* _entity)
{
	for(int i = 0; i < EG_NUM_TYPES; ++i)
	{
		for(unsigned int x = 0; x < m_EntityList[i].size(); ++x)
		{
			if( _entity == m_EntityList[i][x] )
				return;
		}
	}

	AddEntity(_entity, EG_STATIC);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Adds objects from the factory.
Parameters:
[in] _entity - the entity to add to the entity list.
[in] _group - group to add the entity to.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CObjectManager::AddEntity( IEntity* _entity, eEntityGroups _group )
{
	m_EntityList[_group].push_back(_entity);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Finds and removes the specified object from the list and adds them 
		 back to the object factory.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CObjectManager::RemoveEntity( IEntity* _entity, eEntityGroups _group )
{
	EntityList list;
	for( unsigned int i = 0; i < m_EntityList[_group].size(); ++i )
	{
		if( m_EntityList[_group][i] != _entity )
		{
			list.push_back(m_EntityList[_group][i]);
		}
	}

	m_EntityList[_group].clear();
	m_EntityList[_group].resize(list.size());
	for(unsigned int i = 0; i < m_EntityList[_group].size(); ++i)
		m_EntityList[_group][i] = list[i];
}

void CObjectManager::ResetforWave(void)
{
	for(unsigned int i = 0; i < EG_NUM_TYPES; ++i)
	{
		for(unsigned int x = 0; x < m_EntityList[i].size(); ++x)
		{
			if(m_EntityList[i][x]->GetType() == ET_LARGEENEMY || m_EntityList[i][x]->GetType() == ET_SMALLENEMY ||
				m_EntityList[i][x]->GetType() == ET_MEDIUMENEMY || m_EntityList[i][x]->GetType() == ET_BOSSENEMY ||
				m_EntityList[i][x]->GetType() == ET_SHIELD || m_EntityList[i][x]->GetType() == ET_REDSTRIKE ||
				m_EntityList[i][x]->GetType() == ET_EMP || m_EntityList[i][x]->GetType() == ET_MORTAR )
			{
				m_ObjFactory->Destroy(m_EntityList[i][x]);
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Removes the objects from the list and adds them back into the object factory.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CObjectManager::RemoveAllEntities( void )
{
	// TODO:: ObjectFactory needed
	for(unsigned int i = 0; i < EG_NUM_TYPES; ++i)
	{
		for(unsigned int x = 0; x < m_EntityList[i].size(); ++x)
		{
			m_ObjFactory->Destroy(m_EntityList[i][x]);
		}
		m_EntityList[i].clear();
	}
}
void CObjectManager::RemoveHealthPickups( void )
{
	for(unsigned int i = 0; i < m_EntityList[EG_NONSTATIC].size(); ++i)
	{
		if(m_EntityList[EG_NONSTATIC][i]->GetType() == ET_HEALTH_PICKUP_LARGE)
		{
			//m_EntityList[EG_NONSTATIC][i]->SetIsActive(false);
			//m_EntityList[EG_NONSTATIC][i]->SetIsAlive(false);
			m_ObjFactory->Destroy(m_EntityList[EG_NONSTATIC][i]);
		}
	}
}
