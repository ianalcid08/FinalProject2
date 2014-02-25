/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : BaseEntity.h
Author : Evan Wright

Description : Contains the basic information for a renderable object. 
			  It will be used for the base of all objects in our game. 
			  It has a position, a pointer to the model data contained within 
			  Asset Manager and collision bounding volume. It handles it’s own collision
			  reactions and will behave based on what type of entity it is, such as; player, 
			  enemies, boss, crates, bullets, etc.

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef BASEENTITY_H_
#define BASEENTITY_H_

#include "IEntity.h"
#include "glsMath.h"
#include <Windows.h>
#include "RenderNode.h"
#include "CollisionEntity.h"

//class Model;

class CBaseEntity : public IEntity
{
	Vec3f m_Position;
	Matrix4x4f m_WorldPosition;
	CRenderNode m_RenderNode;
	CCollisionEntity* m_CollisionEntity;
	std::vector<IEntity*> m_Collisions;
	
public:
	CBaseEntity( void );
	~CBaseEntity( void ) { Release(); }
	
	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );

	// Accessors
	inline Vec3f* GetPosition( void ) { return &m_Position; }
	inline Matrix4x4f* GetWorldPosition( void ) { return &m_WorldPosition; }
	inline CCollisionEntity* GetCollisionEntity( void ) { return m_CollisionEntity; }
	inline CRenderNode& GetRenderNode( void ) { return m_RenderNode; }
	inline std::vector<IEntity*>& GetCollisions(void) { return m_Collisions; }
	
	void AddCollision(IEntity* _Col);

	// Mutators
	inline void SetPosition( const Vec3f _pos ) 
	{ 
		for( int i = 0; i < 3; ++i )
			m_Position[i] = _pos[i];
	}
	inline void SetWorldPosition( Matrix4x4f _worldPos ) 
	{ 
		for( int i = 0; i < 16; ++i )
			m_WorldPosition[i] = _worldPos[i];
	}
	inline void SetRenderNode( CRenderNode& _RenderNode ) { m_RenderNode = _RenderNode; }
	inline void SetCollisionEntity( CCollisionEntity* _collision ) { m_CollisionEntity = _collision; }
	//inline void	RenderNode( CRenderNode* _node ) { m_RenderNode = _node; }
};

#endif