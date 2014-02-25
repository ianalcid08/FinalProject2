/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : AIManager.h
Author : Joshua Villarreal / Evan Wright

Description : Manages the Slot System for the AI in Critical Mass

Created :  07/16/2013
Modified : 07/24/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef AIMANANGER_H_
#define AIMANANGER_H_

//#include "Slot.h"
#include "Definitions.h"
#include "Renderer.h"
#include "RenderNode.h"
#include "BehaviorTree.h"
#include <vector>

// Forward decl
class CLivingEntity;
class CPlayer;
class CAIManager
{
	std::vector<CLivingEntity*> m_SmallEnemies;
	std::vector<CLivingEntity*> m_MediumEnemies;
	std::vector<CLivingEntity*> m_LargeEnemies;
	CLivingEntity*				m_Boss;

	// The behavior trees for the small enemies to use.
	BehaviorTree m_SmallTrees[MAX_ENEMY_SMALL];
	BehaviorTree m_MediumTrees[MAX_ENEMY_MEDIUM];
	BehaviorTree m_LargeTrees[MAX_ENEMY_LARGE];
	BehaviorTree m_BossTree1, m_BossTree2, m_BossTree3;

	CPlayer*	m_Target;
	bool		m_IsPaused;

public:
	CAIManager( void );
	~CAIManager( void ) { Release(); }

	void Initialize( CPlayer* _target );
	void Release( void );

	void Update( float _elapsedTime );
	void UpdateList( float _elapsedTime, std::vector<CLivingEntity*>& list );

	void AddSmall( CLivingEntity* _small );
	void AddMedium( CLivingEntity* _medium );
	void AddLarge( CLivingEntity* _large );
	void AddBoss( CLivingEntity* _boss );

	void DestroySmall( bool _killBoss = true );
	void DestroyList( std::vector<CLivingEntity*>& list );

	void Reset(void);

	// Accessors
	int GetNumSmallEnemies( void )  { return m_SmallEnemies.size(); }
	int GetNumMediumEnemies( void ) { return m_MediumEnemies.size();}
	int GetNumLargeEnemies( void )  { return m_LargeEnemies.size(); }
	CLivingEntity* GetBoss( void )	{ return m_Boss;				}

	bool GetIsPaused( void )		{ return m_IsPaused;			}
	CPlayer* GetTarget( void )		{ return m_Target;				}
	// Mutators
	void SetTarget( CPlayer* _target ) { m_Target = _target;		}
	void SetIsPaused( bool _isPaused ) { m_IsPaused = _isPaused;	}
};

#endif
