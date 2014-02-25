#ifndef _OBJFAC_
#define _OBJFAC_
/*****************************************************************
File name:			ObjectFactory.h
Date:				7/17/13
Modify Date:		7/25/13
Modify Initials:	CM
Author:				Ian Alcid
Purpose:			Class to handle the creation and destruction of all objects.
******************************************************************/
#include "Definitions.h"
#include <vector>
#include "AcidPool.h"
#include "Camera.h"
#include "Crate.h"
#include "Depot.h"
#include "LaserTrap.h"
#include "Player.h"
#include "Projectile.h"
#include "World.h"
#include "HealthPickup.h"
#include "Shield.h"
#include "Explosion.h"
#include "Pull.h"
#include "Push.h"
#include "Laser.h"
#include "SkyBox.h"
#include "RedBarrel.h"
#include "EMP.h"
#include "RedStrike.h"
#include "BlueStrike.h"
#include "Mortar.h"
#include "LandingPads.h"
#include "SmallEnemy.h"
#include "MediumEnemy.h"
#include "LargeEnemy.h"
#include "BossEnemy.h"

class CObjectManager;
class CRenderer;

class CObjectFactory
{
	CObjectManager*		m_OM;

	std::vector<unsigned int>	m_OpenList[ET_NUMTYPES]; // objects that are created but not used yet.
	std::vector<IEntity*>		m_Destroyed[ET_NUMTYPES]; // objects in list to go back into open list and remove from OM and render list

	CAcidPool		m_AcidPoolsArray[MAX_ACIDPOOLS];
	CCamera			m_CamerasArray[MAX_CAMERAS];
	CCrate			m_CratesArray[MAX_CRATES];
	CDepot			m_DepotArray[MAX_DEPOTS];
	CBossEnemy		m_BossEnemyArray[1];
	CLargeEnemy		m_LargeEnemyArray[MAX_ENEMY_LARGE];
	CMediumEnemy	m_MediumEnemyArray[MAX_ENEMY_MEDIUM];
	CSmallEnemy		m_SmallEnemyArray[MAX_ENEMY_SMALL];
	CLaserTrap		m_LaserArray[MAX_LASERTRAPS];
	CPlayer			m_PlayerArray[MAX_PLAYERS];
	CProjectile		m_PlayerBulletArray[MAX_BULLETS_PLAYER];
	CProjectile		m_EnemyBulletArray[MAX_BULLETS_ENEMY];
	CWorld			m_WorldArray[MAX_LEVELS];
	CHealthPickup	m_HealthPickupArrayS[MAX_S_HEALTH_PICKUPS];
	CHealthPickup	m_HealthPickupArrayL[MAX_L_HEALTH_PICKUPS];
	CShield			m_ShieldArray[MAX_SHIELDS];
	CExplosion		m_ExplosionArray[MAX_EXPLOSION];
	CPull			m_PullArray[1];
	CPush			m_PushArray[1];
	CLaser			m_LaserGunArray[1];
	CSkyBox			m_SkyBoxArray[1];
	CRedBarrel		m_RedBarrel[MAX_BARRELS];
	CEMP			m_EMPArray[MAX_EMP];
	CMortar			m_MortarArray[MAX_MORTAR];
	CRedStrike		m_RedStrikeArray[MAX_REDSTRIKE];
	CBlueStrike		m_BlueStrikeArray[MAX_BLUESTRIKE];
	CLandingPads	m_LandingPadsArray[MAX_LANDINGPADS];

	/*****************************************************************
	* void MakeDefault(): assigns the default values to the given object
	*****************************************************************/
	void MakeDefault(IEntity& _object);
	/*****************************************************************
	* bool isValid(): Safety Check for seeing if the object is correct
	*****************************************************************/
	bool isValid( int _ID, int _index);
public:
	CObjectFactory(void);
	~CObjectFactory(void);
	void Initialize(CObjectManager* _pOM);

	/*****************************************************************
	* bool Create(int _nObjectID, IEntity** _objOut)): creates a base object of 
	* the given type.
	*****************************************************************/
	bool Create(eEntityType _objID, IEntity** _objOut);
	/*****************************************************************
	* bool Destroy(int _nObjectID): Makes objects as inactive
	* 
	*****************************************************************/
	bool Destroy(IEntity* _obj);
	/*****************************************************************
	* void ProcessDestroy(): Makes sure objects are in sync with the renderer
	* 
	*****************************************************************/
	void ProcessDestroy();

};
#endif


