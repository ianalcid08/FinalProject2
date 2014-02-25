#include "../Header Files/stdafx.h"
#include "../Header Files/ObjectFactory.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/ObjectManager.h"
#include "../Header Files/Player.h"
#include "../Header Files/RenderNode.h"
#include "../Header Files/glsMath.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/Game.h"
#include "../Header Files/HealthPickup.h"
#include "../Header Files/Shield.h"
#include "../Header Files/Explosion.h"
#include "../Header Files/Pull.h"
#include "../Header Files/Laser.h"
//#include "../Header Files/Projectile.h"

/*****************************************************************
File name:			ObjectFactory.cpp
Date:				7/17/13
Modify Date:		7/17/13
Modify Initials:	IA
Author:				Ian Alcid
Purpose:			Class to handle the creation and destruction of all objects.
******************************************************************/

CObjectFactory::CObjectFactory(void)
{
	m_OM = nullptr;
}


CObjectFactory::~CObjectFactory(void)
{
}
void CObjectFactory::Initialize(CObjectManager* _pOM )
{
	m_OM		= _pOM;

	unsigned int i;

	for( i = 0; i < MAX_PLAYERS; ++i)
	{
		m_OpenList[ET_PLAYER].push_back(i);
		m_PlayerArray[i].Init();
	}

	for( i = 0; i < MAX_ACIDPOOLS; ++i)
	{
		m_OpenList[ET_ACIDPOOL].push_back(i);
		// Init with shaders here
	}

	for( i = 0; i < MAX_ENEMY_SMALL; ++i)
	{
		m_OpenList[ET_SMALLENEMY].push_back(i);
		m_SmallEnemyArray[i].Init();
	}

	for( i = 0; i < MAX_ENEMY_MEDIUM; ++i)
	{
		m_OpenList[ET_MEDIUMENEMY].push_back(i);
		m_MediumEnemyArray[i].Init();
	}

	for( i = 0; i < MAX_ENEMY_LARGE; ++i)
	{
		m_OpenList[ET_LARGEENEMY].push_back(i);
		m_LargeEnemyArray[i].Init();
	}

	for( i = 0; i < MAX_L_HEALTH_PICKUPS; ++i )
	{
		m_OpenList[ET_HEALTH_PICKUP_LARGE].push_back(i);
		m_HealthPickupArrayL[i].Initialize(true);
		// Init for shaders here
	}

	for( i = 0; i < MAX_S_HEALTH_PICKUPS; ++i )
	{
		m_OpenList[ET_HEALTH_PICKUP_SMALL].push_back(i);
		// Init for shaders here
		m_HealthPickupArrayS[i].Initialize(false);
	}

	for( i = 0; i < MAX_BULLETS_PLAYER; i++)
	{
		// Set the bullets ID to a player
		m_PlayerBulletArray[i].SetType(ET_BULLET_PLAYER);
		m_OpenList[ET_BULLET_PLAYER].push_back(i);
		// Init for shaders here
		m_PlayerBulletArray[i].Initialize();

	}
	for( i = 0; i < MAX_BULLETS_ENEMY; ++i)
	{
		// Set the bullets ID to an enemy type
		m_EnemyBulletArray[i].SetType(ET_BULLET_ENEMY);
		m_OpenList[ET_BULLET_ENEMY].push_back(i);
		// Init for shaders here
		m_EnemyBulletArray[i].Initialize();
	}

	//Red Barrels
	for( i = 0; i < MAX_BARRELS; ++i)
	{
		m_OpenList[ET_REDBARREL].push_back(i);
		// Init for shaders here
		m_RedBarrel[i].InitShaders();
	}

	for( i = 0; i < MAX_EMP; ++i)
	{
		m_OpenList[ET_EMP].push_back(i);
		m_EMPArray[i].Init();
	}

	for( i = 0; i < MAX_CRATES; ++i)
	{
		m_OpenList[ET_CRATE].push_back(i);
		// Init for shaders here
		m_CratesArray[i].InitShaders();
	}

	for( i = 0; i < MAX_DEPOTS; ++i)
	{
		m_OpenList[ET_DEPOT].push_back(i);
		// Init for shaders here
		m_DepotArray[i].InitShaders();

	}

	for( i = 0; i < MAX_SHIELDS; ++i)
	{
		m_OpenList[ET_SHIELD].push_back(i);
		// Init for shaders here
		m_ShieldArray[i].InitShaders();

	}

	for( i = 0; i < MAX_LASERTRAPS; ++i)
	{
		m_OpenList[ET_LASERTRAP].push_back(i);
		// Init for shaders here
	}

	for( i = 0; i < MAX_LEVELS; ++i)
	{
		m_OpenList[ET_WORLD].push_back(i);
		// Init for shaders here
		m_WorldArray[i].Init();
	}

	for( i = 0; i< MAX_CAMERAS ; ++i)
	{
		m_OpenList[ET_CAMERA].push_back(i);
		// Init for shaders here
	}

	for( i = 0; i< MAX_EXPLOSION ; ++i)
	{
		m_OpenList[ET_EXPLOSION].push_back(i);
		// Init for shaders here
	}

	for( i = 0; i< MAX_REDSTRIKE ; ++i)
	{
		m_OpenList[ET_REDSTRIKE].push_back(i);
		m_RedStrikeArray[i].Init();
	}
	for( i = 0; i< MAX_BLUESTRIKE ; ++i)
	{
		m_OpenList[ET_BLUESTRIKE].push_back(i);
		m_BlueStrikeArray[i].Init();
	}
	for( i = 0; i< MAX_MORTAR ; ++i)
	{
		m_OpenList[ET_MORTAR].push_back(i);
		m_MortarArray[i].Init();
	}
	for( i = 0; i < MAX_LANDINGPADS; ++i)
	{
		m_OpenList[ET_LANDINGPAD].push_back(i);
		m_LandingPadsArray[i].InitShaders();
	}

	m_OpenList[ET_PULL].push_back(0);
	m_PullArray[0].InitShaders();

	m_OpenList[ET_PUSH].push_back(0);
	m_PushArray[0].InitShaders();

	m_OpenList[ET_LASER].push_back(0);
	m_LaserGunArray[0].InitShaders();

	m_OpenList[ET_SKYBOX].push_back(0);
	m_SkyBoxArray[0].InitShaders();

	m_OpenList[ET_BOSSENEMY].push_back(0);
	m_BossEnemyArray[0].Init();
	/*for( i = 0; i < MAX_BULLETS_PLAYER; ++i)
	m_OpenList[ET_BULLET_PLAYER].push_back(i);*/

}
bool CObjectFactory::Create(eEntityType _objID, IEntity** _objOut)
{
	if( !m_OpenList[_objID].size() )
		return false;

	unsigned int Index = m_OpenList[_objID].back();
	m_OpenList[_objID].pop_back();

	switch(_objID)
	{
	case ET_PLAYER:
		{
			MakeDefault(m_PlayerArray[Index]);
			*_objOut = &m_PlayerArray[Index];
			m_OM->AddEntity(&m_PlayerArray[Index],EG_NONSTATIC);
		}
		break;
	case ET_BULLET_PLAYER:
		{
			MakeDefault(m_PlayerBulletArray[Index]);
			*_objOut = &m_PlayerBulletArray[Index];
			m_OM->AddEntity(&m_PlayerBulletArray[Index],EG_NONSTATIC);
		}
		break;
	case ET_BULLET_ENEMY:
		{
			MakeDefault(m_EnemyBulletArray[Index]);
			*_objOut = &m_EnemyBulletArray[Index];
			m_OM->AddEntity(&m_EnemyBulletArray[Index],EG_NONSTATIC);
		}
		break;
	case ET_WORLD:
		{
			MakeDefault(m_WorldArray[Index]);
			*_objOut = &m_WorldArray[Index];
			m_OM->AddEntity(&m_WorldArray[Index],EG_STATIC);
		}
		break;
	case ET_DEPOT:
		{
			MakeDefault(m_DepotArray[Index]);
			*_objOut = &m_DepotArray[Index];
			m_OM->AddEntity(&m_DepotArray[Index],EG_STATIC);
		}
		break;
	case ET_SHIELD:
		{
			MakeDefault(m_ShieldArray[Index]);
			*_objOut = &m_ShieldArray[Index];
			m_OM->AddEntity(&m_ShieldArray[Index],EG_STATIC);
		}
		break;
	case ET_SMALLENEMY:
		{
			MakeDefault(m_SmallEnemyArray[Index]);
			*_objOut = &m_SmallEnemyArray[Index];
			m_OM->AddEntity(&m_SmallEnemyArray[Index],EG_NONSTATIC);
		}
		break;
	case ET_MEDIUMENEMY:
		{
			MakeDefault(m_MediumEnemyArray[Index]);
			*_objOut = &m_MediumEnemyArray[Index];
			m_OM->AddEntity(&m_MediumEnemyArray[Index],EG_NONSTATIC);
		}
		break;
	case ET_LARGEENEMY:
		{
			MakeDefault(m_LargeEnemyArray[Index]);
			*_objOut = &m_LargeEnemyArray[Index];
			m_OM->AddEntity(&m_LargeEnemyArray[Index],EG_NONSTATIC);
		}
		break;
	case ET_BOSSENEMY:
		{
			MakeDefault(m_BossEnemyArray[Index]);
			*_objOut = &m_BossEnemyArray[Index];
			m_OM->AddEntity(&m_BossEnemyArray[Index],EG_NONSTATIC);
		}
		break;
	case ET_HEALTH_PICKUP_LARGE:
		{
			MakeDefault(m_HealthPickupArrayL[Index]);
			*_objOut = &m_HealthPickupArrayL[Index];
			m_OM->AddEntity(&m_HealthPickupArrayL[Index],EG_NONSTATIC);
		}
		break;
	case ET_EXPLOSION:
		{
			MakeDefault(m_ExplosionArray[Index]);
			*_objOut = &m_ExplosionArray[Index];
			m_OM->AddEntity(&m_ExplosionArray[Index], EG_NONSTATIC);
		}
		break;
	case ET_PULL:
		{
			MakeDefault(m_PullArray[Index]);
			*_objOut = &m_PullArray[Index];
			m_OM->AddEntity(&m_PullArray[Index], EG_NONSTATIC);
		}
		break;
	case ET_PUSH:
		{
			MakeDefault(m_PushArray[Index]);
			*_objOut = &m_PushArray[Index];
			m_OM->AddEntity(&m_PushArray[Index], EG_NONSTATIC);
		}
		break;
	case ET_LASER:
		{
			MakeDefault(m_LaserGunArray[Index]);
			*_objOut = &m_LaserGunArray[Index];
			m_OM->AddEntity(&m_LaserGunArray[Index], EG_NONSTATIC);
		}
		break;
	case ET_SKYBOX:
		{
			MakeDefault(m_SkyBoxArray[Index]);
			*_objOut = &m_SkyBoxArray[Index];
			m_OM->AddEntity(&m_SkyBoxArray[Index], EG_STATIC);
		}
		break;
	case ET_REDBARREL:
		{
			MakeDefault(m_RedBarrel[Index]);
			*_objOut = &m_RedBarrel[Index];
			m_OM->AddEntity(&m_RedBarrel[Index], EG_STATIC);
		}
		break;
	case ET_CRATE:
		{
			MakeDefault(m_CratesArray[Index]);
			*_objOut = &m_CratesArray[Index];
			m_OM->AddEntity(&m_CratesArray[Index], EG_NONSTATIC);
		}
		break;
	case ET_EMP:
		{
			MakeDefault(m_EMPArray[Index]);
			*_objOut = &m_EMPArray[Index];
			m_OM->AddEntity(&m_EMPArray[Index], EG_STATIC);
		}
		break;
	case ET_REDSTRIKE:
		{
			MakeDefault(m_RedStrikeArray[Index]);
			*_objOut = &m_RedStrikeArray[Index];
			m_OM->AddEntity(&m_RedStrikeArray[Index], EG_STATIC);
		}
		break;
	case ET_BLUESTRIKE:
		{
			MakeDefault(m_BlueStrikeArray[Index]);
			*_objOut = &m_BlueStrikeArray[Index];
			m_OM->AddEntity(&m_BlueStrikeArray[Index], EG_STATIC);
		}
		break;
	case ET_MORTAR:
		{
			MakeDefault(m_MortarArray[Index]);
			*_objOut = &m_MortarArray[Index];
			m_OM->AddEntity(&m_MortarArray[Index], EG_NONSTATIC);
		}
		break;
	case ET_LANDINGPAD:
		{
			MakeDefault(m_LandingPadsArray[Index]);
			*_objOut = &m_LandingPadsArray[Index];
			m_OM->AddEntity(&m_LandingPadsArray[Index],EG_STATIC);
		}
		break;
	}
	return true;
}
bool CObjectFactory::Destroy(IEntity* _obj)
{
	if(!_obj)
		return false;

	int test = -1;

	switch (_obj->GetType())
	{
	case ET_PLAYER:
		{
			test = ((int)_obj-(int)&m_PlayerArray[0]) / sizeof(CPlayer);
			if(test < 0 || test >= MAX_PLAYERS )
				return false;
			else if ( !isValid(ET_PLAYER,test) )
				return false;

			m_Destroyed[ET_PLAYER].push_back(&m_PlayerArray[test]);
		}
		break;
	case ET_BULLET_PLAYER:
		{
			test = ((int)_obj-(int)&m_PlayerBulletArray[0]) / sizeof(CProjectile);
			if(test < 0 || test >= MAX_BULLETS_PLAYER )
				return false;
			else if ( !isValid(ET_BULLET_PLAYER,test) )
				return false;

			m_Destroyed[ET_BULLET_PLAYER].push_back(&m_PlayerBulletArray[test]);
		}
		break;
	case ET_BULLET_ENEMY:
		{
			test = ((int)_obj-(int)&m_EnemyBulletArray[0]) / sizeof(CProjectile);
			if(test < 0 || test >= MAX_BULLETS_ENEMY )
				return false;
			else if ( !isValid(ET_BULLET_ENEMY,test) )
				return false;

			m_Destroyed[ET_BULLET_ENEMY].push_back(&m_EnemyBulletArray[test]);
		}
		break;
	case ET_WORLD:
		{
			test = ((int)_obj-(int)&m_WorldArray[0]) / sizeof(CWorld);
			if(test < 0 || test >= MAX_LEVELS )
				return false;
			else if ( !isValid(ET_WORLD,test) )
				return false;

			m_Destroyed[ET_WORLD].push_back(&m_WorldArray[test]);
		}
		break;
	case ET_SMALLENEMY:
		{
			test = ((int)_obj-(int)&m_SmallEnemyArray[0]) / sizeof(CSmallEnemy);
			if(test < 0 || test >= MAX_ENEMY_SMALL )
				return false;
			else if ( !isValid(ET_SMALLENEMY,test) )
				return false;

			m_Destroyed[ET_SMALLENEMY].push_back(&m_SmallEnemyArray[test]);
		}
		break;
	case ET_MEDIUMENEMY:
		{
			test = ((int)_obj-(int)&m_MediumEnemyArray[0]) / sizeof(CMediumEnemy);
			if(test < 0 || test >= MAX_ENEMY_MEDIUM )
				return false;
			else if ( !isValid(ET_MEDIUMENEMY,test) )
				return false;

			m_Destroyed[ET_MEDIUMENEMY].push_back(&m_MediumEnemyArray[test]);
		}
		break;
	case ET_LARGEENEMY:
		{
			test = ((int)_obj-(int)&m_LargeEnemyArray[0]) / sizeof(CLargeEnemy);
			if(test < 0 || test >= MAX_ENEMY_LARGE )
				return false;
			else if ( !isValid(ET_LARGEENEMY,test) )
				return false;

			m_Destroyed[ET_LARGEENEMY].push_back(&m_LargeEnemyArray[test]);
		}
		break;
	case ET_BOSSENEMY:
		{
			test = ((int)_obj-(int)&m_BossEnemyArray[0]) / sizeof(CBossEnemy);
			if(test < 0 || test >= 1 )
				return false;
			else if ( !isValid(ET_BOSSENEMY,test) )
				return false;

			m_Destroyed[ET_BOSSENEMY].push_back(&m_BossEnemyArray[test]);
		}
		break;
	case ET_HEALTH_PICKUP_LARGE:
		{
			test = ((int)_obj-(int)&m_HealthPickupArrayL[0]) / sizeof(CHealthPickup);
			if(test < 0 || test >= MAX_L_HEALTH_PICKUPS )
				return false;
			else if ( !isValid(ET_HEALTH_PICKUP_LARGE,test) )
				return false;

			m_Destroyed[ET_HEALTH_PICKUP_LARGE].push_back(&m_HealthPickupArrayL[test]);
		}
		break;
	case ET_DEPOT:
		{
			test = ((int)_obj-(int)&m_DepotArray[0]) / sizeof(CDepot);
			if(test < 0 || test >= MAX_DEPOTS )
				return false;
			else if ( !isValid(ET_DEPOT,test) )
				return false;

			m_Destroyed[ET_DEPOT].push_back(&m_DepotArray[test]);
		}
		break;
	case ET_SHIELD:
		{
			test = ((int)_obj-(int)&m_ShieldArray[0]) / sizeof(CShield);
			if(test < 0 || test >= MAX_SHIELDS )
				return false;
			else if ( !isValid(ET_SHIELD,test) )
				return false;

			m_Destroyed[ET_SHIELD].push_back(&m_ShieldArray[test]);
		}
		break;
	case ET_EXPLOSION:
		{
			test = ((int)_obj-(int)&m_ExplosionArray[0]) / sizeof(CExplosion);
			if(test < 0 || test >= MAX_EXPLOSION )
				return false;
			else if ( !isValid(ET_EXPLOSION,test) )
				return false;

			m_Destroyed[ET_EXPLOSION].push_back(&m_ExplosionArray[test]);
		}
		break;
	case ET_PULL:
		{
			test = ((int)_obj-(int)&m_PullArray[0]) / sizeof(CPull);
			if(test < 0 || test >= 1 )
				return false;
			else if ( !isValid(ET_PLAYER,test) )
				return false;

			m_Destroyed[ET_PULL].push_back(&m_PullArray[test]);
		}
		break;
	case ET_PUSH:
		{
			test = ((int)_obj-(int)&m_PushArray[0]) / sizeof(CPush);
			if(test < 0 || test >= 1 )
				return false;
			else if ( !isValid(ET_PLAYER,test) )
				return false;

			m_Destroyed[ET_PUSH].push_back(&m_PushArray[test]);
		}
		break;
	case ET_LASER:
		{
			test = ((int)_obj-(int)&m_LaserGunArray[0]) / sizeof(CLaser);
			if(test < 0 || test >= 1 )
				return false;
			else if ( !isValid(ET_LASER,test) )
				return false;

			m_Destroyed[ET_LASER].push_back(&m_LaserGunArray[test]);
		}
		break;
	case ET_SKYBOX:
		{
			test = ((int)_obj-(int)&m_SkyBoxArray[0]) / sizeof(CSkyBox);
			if(test < 0 || test >= 1 )
				return false;
			else if ( !isValid(ET_SKYBOX,test) )
				return false;

			m_Destroyed[ET_SKYBOX].push_back(&m_SkyBoxArray[test]);
		}
		break;
	case ET_REDBARREL:
		{
			test = ((int)_obj-(int)&m_RedBarrel[0]) / sizeof(CRedBarrel);
			if(test < 0 || test >= MAX_BARRELS)
				return false;
			else if ( !isValid(ET_REDBARREL,test) )
				return false;

			m_Destroyed[ET_REDBARREL].push_back(&m_RedBarrel[test]);
		}
		break;
	case ET_CRATE:
		{
			test = ((int)_obj-(int)&m_CratesArray[0]) / sizeof(CCrate);
			if(test < 0 || test >= MAX_CRATES)
				return false;
			else if ( !isValid(ET_CRATE,test) )
				return false;

			m_Destroyed[ET_CRATE].push_back(&m_CratesArray[test]);
		}
		break;
	case ET_EMP:
		{
			test = ((int)_obj-(int)&m_EMPArray[0]) / sizeof(CEMP);
			if(test < 0 || test >= MAX_EMP)
				return false;
			else if ( !isValid(ET_EMP,test) )
				return false;

			m_Destroyed[ET_EMP].push_back(&m_EMPArray[test]);
		}
		break;
	case ET_REDSTRIKE:
		{
			// functionHelp( array*, TYPE, MAX_SIZE, class size )
			test = ((int)_obj-(int)&m_RedStrikeArray[0]) / sizeof(CRedStrike);
			if(test < 0 || test >= MAX_REDSTRIKE)
				return false;
			else if ( !isValid(ET_REDSTRIKE,test) )
				return false;

			m_Destroyed[ET_REDSTRIKE].push_back(&m_RedStrikeArray[test]);
		}
		break;
	case ET_BLUESTRIKE:
		{
			test = ((int)_obj-(int)&m_BlueStrikeArray[0]) / sizeof(CBlueStrike);
			if(test < 0 || test >= MAX_BLUESTRIKE)
				return false;
			else if ( !isValid(ET_BLUESTRIKE,test) )
				return false;

			m_Destroyed[ET_BLUESTRIKE].push_back(&m_BlueStrikeArray[test]);
		}
		break;
	case ET_MORTAR:
		{
			test = ((int)_obj-(int)&m_MortarArray[0]) / sizeof(CMortar);
			if(test < 0 || test >= MAX_MORTAR)
				return false;
			else if ( !isValid(ET_MORTAR,test) )
				return false;

			m_Destroyed[ET_MORTAR].push_back(&m_MortarArray[test]);
		}
		break;
	case ET_LANDINGPAD:
		{
			test = ((int)_obj-(int)&m_LandingPadsArray[0]) / sizeof(CLandingPads);
			if( test < 0 || test >= MAX_LANDINGPADS)
				return false;
			else if( !isValid(ET_LANDINGPAD,test) )
				return false;

			m_Destroyed[ET_LANDINGPAD].push_back(&m_LandingPadsArray[test]);
		}
		break;
	}
	return true;
}
void CObjectFactory::ProcessDestroy()
{
	unsigned int size = 0;
	unsigned int index = 0;
	unsigned int i;

	size = m_Destroyed[ET_PLAYER].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_PLAYER][i]-(int)&m_PlayerArray[0])/sizeof(CPlayer);
		m_OpenList[ET_PLAYER].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_PLAYER][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_PLAYER].clear();

	size = m_Destroyed[ET_BULLET_PLAYER].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_BULLET_PLAYER][i]-(int)&m_PlayerBulletArray[0])/sizeof(CProjectile);
		m_OpenList[ET_BULLET_PLAYER].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_BULLET_PLAYER][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_BULLET_PLAYER].clear();

	size = m_Destroyed[ET_BULLET_ENEMY].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_BULLET_ENEMY][i]-(int)&m_EnemyBulletArray[0])/sizeof(CProjectile);
		m_OpenList[ET_BULLET_ENEMY].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_BULLET_ENEMY][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_BULLET_ENEMY].clear();

	size = m_Destroyed[ET_WORLD].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_WORLD][i]-(int)&m_WorldArray[0])/sizeof(CWorld);
		m_OpenList[ET_WORLD].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_WORLD][i],EG_STATIC);
	}
	m_Destroyed[ET_WORLD].clear();

	size = m_Destroyed[ET_SMALLENEMY].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_SMALLENEMY][i]-(int)&m_SmallEnemyArray[0])/sizeof(CSmallEnemy);
		m_OpenList[ET_SMALLENEMY].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_SMALLENEMY][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_SMALLENEMY].clear();

	size = m_Destroyed[ET_MEDIUMENEMY].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_MEDIUMENEMY][i]-(int)&m_MediumEnemyArray[0])/sizeof(CMediumEnemy);
		m_OpenList[ET_MEDIUMENEMY].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_MEDIUMENEMY][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_MEDIUMENEMY].clear();

	size = m_Destroyed[ET_LARGEENEMY].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_LARGEENEMY][i]-(int)&m_LargeEnemyArray[0])/sizeof(CLargeEnemy);
		m_OpenList[ET_LARGEENEMY].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_LARGEENEMY][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_LARGEENEMY].clear();

	size = m_Destroyed[ET_BOSSENEMY].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_BOSSENEMY][i]-(int)&m_BossEnemyArray[0])/sizeof(CBossEnemy);
		m_OpenList[ET_BOSSENEMY].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_BOSSENEMY][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_BOSSENEMY].clear();

	size = m_Destroyed[ET_HEALTH_PICKUP_LARGE].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_HEALTH_PICKUP_LARGE][i]-(int)&m_HealthPickupArrayL[0])/sizeof(CHealthPickup);
		m_OpenList[ET_HEALTH_PICKUP_LARGE].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_HEALTH_PICKUP_LARGE][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_HEALTH_PICKUP_LARGE].clear();

	size = m_Destroyed[ET_HEALTH_PICKUP_SMALL].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_HEALTH_PICKUP_SMALL][i]-(int)&m_HealthPickupArrayS[0])/sizeof(CHealthPickup);
		m_OpenList[ET_HEALTH_PICKUP_SMALL].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_HEALTH_PICKUP_SMALL][i],EG_STATIC);
	}
	m_Destroyed[ET_HEALTH_PICKUP_SMALL].clear();

	size = m_Destroyed[ET_DEPOT].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_DEPOT][i]-(int)&m_DepotArray[0])/sizeof(CDepot);
		m_OpenList[ET_DEPOT].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_DEPOT][i],EG_STATIC);
	}
	m_Destroyed[ET_DEPOT].clear();

	size = m_Destroyed[ET_SHIELD].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_SHIELD][i]-(int)&m_ShieldArray[0])/sizeof(CShield);
		m_OpenList[ET_SHIELD].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_SHIELD][i],EG_STATIC);
	}
	m_Destroyed[ET_SHIELD].clear();

	size = m_Destroyed[ET_EXPLOSION].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_EXPLOSION][i]-(int)&m_ExplosionArray[0])/sizeof(CExplosion);
		m_OpenList[ET_EXPLOSION].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_EXPLOSION][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_EXPLOSION].clear();

	size = m_Destroyed[ET_PULL].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_PULL][i]-(int)&m_PullArray[0])/sizeof(CPull);
		m_OpenList[ET_PULL].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_PULL][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_PULL].clear();

	size = m_Destroyed[ET_PUSH].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_PUSH][i]-(int)&m_PushArray[0])/sizeof(CPush);
		m_OpenList[ET_PUSH].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_PUSH][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_PUSH].clear();

	size = m_Destroyed[ET_LASER].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_LASER][i]-(int)&m_LaserGunArray[0])/sizeof(CLaser);
		m_OpenList[ET_LASER].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_LASER][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_LASER].clear();

	//skybox
	size = m_Destroyed[ET_SKYBOX].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_SKYBOX][i]-(int)&m_SkyBoxArray[0])/sizeof(CSkyBox);
		m_OpenList[ET_SKYBOX].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_SKYBOX][i],EG_STATIC);
	}
	m_Destroyed[ET_SKYBOX].clear();

	// Red Barrel
	size = m_Destroyed[ET_REDBARREL].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_REDBARREL][i]-(int)&m_RedBarrel[0])/sizeof(CRedBarrel);
		m_OpenList[ET_REDBARREL].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_REDBARREL][i],EG_STATIC);
	}
	m_Destroyed[ET_REDBARREL].clear();

	// Crate
	size = m_Destroyed[ET_CRATE].size();
	for(i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_CRATE][i] - (int)&m_CratesArray[0])/sizeof(CCrate);
		m_OpenList[ET_CRATE].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_CRATE][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_CRATE].clear();

	size = m_Destroyed[ET_EMP].size();
	for(i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_EMP][i] - (int)&m_EMPArray[0])/sizeof(CEMP);
		m_OpenList[ET_EMP].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_EMP][i],EG_STATIC);
	}
	m_Destroyed[ET_EMP].clear();

	size = m_Destroyed[ET_REDSTRIKE].size();
	for(i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_REDSTRIKE][i] - (int)&m_RedStrikeArray[0])/sizeof(CRedStrike);
		m_OpenList[ET_REDSTRIKE].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_REDSTRIKE][i],EG_STATIC);
	}
	m_Destroyed[ET_REDSTRIKE].clear();

	size = m_Destroyed[ET_BLUESTRIKE].size();
	for(i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_BLUESTRIKE][i] - (int)&m_BlueStrikeArray[0])/sizeof(CBlueStrike);
		m_OpenList[ET_BLUESTRIKE].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_BLUESTRIKE][i],EG_STATIC);
	}
	m_Destroyed[ET_BLUESTRIKE].clear();

	size = m_Destroyed[ET_MORTAR].size();
	for(i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_MORTAR][i] - (int)&m_MortarArray[0])/sizeof(CMortar);
		m_OpenList[ET_MORTAR].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_MORTAR][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_MORTAR].clear();

	size = m_Destroyed[ET_LANDINGPAD].size();
	for( i = 0; i < size; ++i)
	{
		index = ((int)m_Destroyed[ET_LANDINGPAD][i] - (int)&m_LandingPadsArray[0])/sizeof(CLandingPads);
		m_OpenList[ET_LANDINGPAD].push_back(index);
		m_OM->RemoveEntity(m_Destroyed[ET_LANDINGPAD][i],EG_NONSTATIC);
	}
	m_Destroyed[ET_LANDINGPAD].clear();
}
void CObjectFactory::MakeDefault(IEntity& _object)
{
	switch(_object.GetType())
	{
	case ET_PLAYER:
		{
			_object.SetIsActive(true);						
			_object.SetIsAlive(true);											

			((CPlayer&)_object).GetGun().Store(EMPTY);
			((CPlayer&)_object).GetGun().Process();
			((CPlayer&)_object).SetHealth(MAX_HP_PLAYER);
			((CPlayer&)_object).SetInvulnerableTime(0.0f);
			((CPlayer&)_object).SetIsDying(false);
			((CPlayer&)_object).SetLives(MAX_LIVES_PLAYER);
			((CPlayer&)_object).SetIsLoaded(false);

			Vec3f temp;
			glsLoadVector3(temp,0.0f,0.0f,0.0f);
			((CPlayer&)_object).GetRenderNode().SetPosition( temp );
			((CPlayer&)_object).SetPosition(temp);
		}
		break;
	case ET_SMALLENEMY:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			((CSmallEnemy&)_object).SetDamage(SMALL_ENEMY_DMG);
			((CSmallEnemy&)_object).SetHealth(2);
			((CSmallEnemy&)_object).Init();
		}
		break;
	case ET_MEDIUMENEMY:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			((CMediumEnemy&)_object).SetHealth(5);
			((CMediumEnemy&)_object).Init();
		}
		break;
	case ET_LARGEENEMY:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			((CLargeEnemy&)_object).SetHealth(MAX_HP_LARGE_ENEMY);
			((CLargeEnemy&)_object).Init();
		}
		break;
	case ET_BOSSENEMY:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			((CBossEnemy&)_object).SetHealth(MAX_HP_BOSS_ENEMY);
			((CBossEnemy&)_object).Init();
		}
		break;
	case ET_REDBARREL:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_DEPOT:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_CRATE:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_LASERTRAP:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_ACIDPOOL:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_SHIELD:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			((CShield&)_object).SetIneffectiveHitTimer(0.0f);
			((CShield&)_object).ResetShield();
		}
		break;
	case ET_EXPLOSION:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			Vec3f temp;
			glsLoadVector3(temp,1.0f,1.0f,1.0f);
			Matrix4x4f tempMat;
			glsLoadIdentity4x4(tempMat);
			((CExplosion&)_object).Initialize(temp,tempMat);
		}
		break;
	case ET_BULLET_PLAYER:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			((CProjectile&)_object).SetWorldPosition(*(m_PlayerArray[0].GetWorldPosition()));
			((CProjectile&)_object).SetLifeTime(2.0f);
			((CProjectile&)_object).SetIfExplode(false);
			((CProjectile&)_object).SetIfConsumed(false);
		}
		break;
	case ET_BULLET_ENEMY:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			((CProjectile&)_object).SetWorldPosition(*(m_PlayerArray[0].GetWorldPosition()));
			((CProjectile&)_object).SetLifeTime(2.0f);
			((CProjectile&)_object).SetIfExplode(false);
		}
		break;
	case ET_WORLD:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			Vec3f temp;
			glsLoadVector3(temp,0.0f,75.0f,0.0f);
			((CWorld&)_object).GetRenderNode().SetPosition( temp );
			((CWorld&)_object).SetPosition(temp);
		}
		break;
	case ET_CAMERA:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_HEALTH_PICKUP_SMALL:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			Vec3f pos;
			glsLoadVector3(pos,0.0f,75.0f,75.0f);
			((CHealthPickup&)_object).GetRenderNode().SetPosition( pos );
			((CHealthPickup&)_object).SetPosition(pos);
		}
		break;
	case ET_HEALTH_PICKUP_LARGE:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);

			Vec3f pos;
			glsLoadVector3(pos,0.0f,75.0f,0.0f);
			((CHealthPickup&)_object).GetRenderNode().SetPosition( pos );
			((CHealthPickup&)_object).Initialize(true);
		}
		break;
	case ET_PULL:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_PUSH:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_LASER:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_EMP:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_REDSTRIKE:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_BLUESTRIKE:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_MORTAR:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	case ET_SKYBOX:
		{
			//_object.SetIsActive(true);
			//_object.SetIsAlive(true);

			//TODO: Finish implementing the sky box.
	
		}
		break;
	case ET_LANDINGPAD:
		{
			_object.SetIsActive(true);
			_object.SetIsAlive(true);
		}
		break;
	}
}
bool CObjectFactory::isValid( int _ID, int _index)
{
	unsigned int size = m_OpenList[_ID].size();

	for(unsigned int i = 0; i < size; ++i)
	{
		if(m_OpenList[_ID][i] == _index)
			return false;
	}

	return true;
}
