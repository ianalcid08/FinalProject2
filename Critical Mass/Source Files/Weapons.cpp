#include "../Header Files/stdafx.h"
#include "../Header Files/Weapons.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/Projectile.h"
#include "../Header Files/Objectfactory.h"
#include "../Header Files/Player.h"
#include "../Header Files/Laser.h"
//Sound----------------------------------------------------
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Wwise_IDs.h"
//--------------------------------------------------------
#include "../Header Files/MessageSystem.h"

#pragma region /*Red Weapon Stats*/

struct Red_Stats
{
	float Duration;
	float Rate;
	float Timer_Start;
	float Timer_End;
	float Size;
	Red_Stats(float _d=0.0f, float _r=0.0f, float _ts=0.0f, float _te=0.0f, float _s=0.0f) :
		Duration(_d), Rate(_r), Timer_Start(_ts), Timer_End(_te), Size(_s) {}
};

#define RED_ROF_LVL1		0.8f
#define RED_ROF_LVL2		2.5f
#define RED_ROF_LVL3		2.5f
static float RED_ROF[3] = {RED_ROF_LVL1, RED_ROF_LVL2, RED_ROF_LVL3};

#define RED_AMMO_LVL1		5.0f
#define RED_AMMO_LVL2		3.0f
#define RED_AMMO_LVL3		2.0f
static float RED_AMMO[3] = {RED_AMMO_LVL1, RED_AMMO_LVL2, RED_AMMO_LVL3};

#define RED_STATS_LVL1 Red_Stats(1.0f,0.5f,0.3f,5.0f,1.0f)
#define RED_STATS_LVL2 Red_Stats(3.0f,0.5f,0.2f,4.0f,2.5f)
#define RED_STATS_LVL3 Red_Stats(5.0f,0.1f,0.1f,4.0f,4.0f)
static Red_Stats RED_STATS[3] = {RED_STATS_LVL1, RED_STATS_LVL2, RED_STATS_LVL3};

// Set up so it adjusts based on how many shots you can fire.
// That way it's easier to adjust how many times fired to level up
// based off of the amount you CAN fire per clip.
#define RED_EXPERIENCE_LVL1	EXP_RED_LVL1*3
#define RED_EXPERIENCE_LVL2	RED_EXPERIENCE_LVL1*3
#define RED_EXPERIENCE_LVL3	RED_EXPERIENCE_LVL2*4
static unsigned int RED_EXPERIENCE[3] = 
{
	(unsigned int)RED_EXPERIENCE_LVL1, 
	(unsigned int)RED_EXPERIENCE_LVL2, 
	(unsigned int)RED_EXPERIENCE_LVL3
};

#pragma endregion

#pragma region /*Yellow Weapon Stats*/

#define YELLOW_ROF_LVL1		0.25f//0.25f
#define YELLOW_ROF_LVL2		0.1f//0.10f
#define YELLOW_ROF_LVL3		0.06f
static float YELLOW_ROF[3] = {YELLOW_ROF_LVL1, YELLOW_ROF_LVL2, YELLOW_ROF_LVL3};

#define YELLOW_AMMO_LVL1		15.0f
#define YELLOW_AMMO_LVL2		50.0f
#define YELLOW_AMMO_LVL3		80.0f
static float YELLOW_AMMO[3] = {YELLOW_AMMO_LVL1, YELLOW_AMMO_LVL2, YELLOW_AMMO_LVL3};

// Set up so it adjusts based on how many shots you can fire.
// That way it's easier to adjust how many times fired to level up
// based off of the amount you CAN fire per clip.dddddddddddddddddss
#define YELLOW_EXPERIENCE_LVL1	EXP_YELLOW_LVL1*3
#define YELLOW_EXPERIENCE_LVL2	YELLOW_EXPERIENCE_LVL1*3
#define YELLOW_EXPERIENCE_LVL3	YELLOW_EXPERIENCE_LVL2*4
static unsigned int YELLOW_EXPERIENCE[3] = 
{
	(unsigned int)YELLOW_EXPERIENCE_LVL1, 
	(unsigned int)YELLOW_EXPERIENCE_LVL2, 
	(unsigned int)YELLOW_EXPERIENCE_LVL3
};

#pragma endregion

#pragma region /*Blue Weapon Stats*/

#define BLUE_ROF_LVL1		1.0f
#define BLUE_ROF_LVL2		1.0f
#define BLUE_ROF_LVL3		1.0f
static float BLUE_ROF[3] = {BLUE_ROF_LVL1, BLUE_ROF_LVL2, BLUE_ROF_LVL3};

#define BLUE_AMMO_LVL1		40.0f//5.0f
#define BLUE_AMMO_LVL2		50.0f//30.0f
#define BLUE_AMMO_LVL3		60.0f
static float BLUE_AMMO[3] = {BLUE_AMMO_LVL1, BLUE_AMMO_LVL2, BLUE_AMMO_LVL3};

// Set up so it adjusts based on how many shots you can fire.
// That way it's easier to adjust how many times fired to level up
// based off of the amount you CAN fire per clip.
#define BLUE_EXPERIENCE_LVL1	EXP_BLUE_LVL1*4
#define BLUE_EXPERIENCE_LVL2	BLUE_EXPERIENCE_LVL1*4
#define BLUE_EXPERIENCE_LVL3	BLUE_EXPERIENCE_LVL2*4
static unsigned int BLUE_EXPERIENCE[3] = 
{
	(unsigned int)BLUE_EXPERIENCE_LVL1, 
	(unsigned int)BLUE_EXPERIENCE_LVL2, 
	(unsigned int)BLUE_EXPERIENCE_LVL3
};

#pragma endregion

#define BLUE_LOSS_RATE		15.0f

#define YELLOW_NUM_SHOTS	20
#define YELLOW_ANGLE		25.0f

#define BLUE_TIMER_CHARGE	2.0f

CWeapons::CWeapons( void )
{
	this->m_Consumed	= EMPTY;
	this->m_RateOfFire	= 1.5f;
	this->m_ShotTotal	= (float)MAX_RED_AMMO;
	this->m_TotalTime	= 4.0f;
	this->m_LaserTime	= 0.0f;
	this->m_LaserChargeTimer = 0.0f;
	//m_IsSuperLaserFired = false;

	Reset();

	m_LaserPtr = nullptr;
}

void CWeapons::Reset( void )
{
	for(int i = 0; i < 3; ++i)
	{
		m_WeaponLevel[i] = 1;
		m_WeaponExperience[i] = 0;
	}
}

void CWeapons::Process( void )
{
	m_Consumed = m_Stored;

	switch (m_Consumed)
	{
	case RED:
		{
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_COINPICKUP);

			this->m_ShotTotal = RED_AMMO[m_WeaponLevel[RED]-1];
			this->m_RateOfFire = RED_ROF[m_WeaponLevel[RED]-1];
			break;
		}
	case YELLOW:
		{
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_COINPICKUP);

			this->m_ShotTotal = YELLOW_AMMO[m_WeaponLevel[YELLOW]-1];
			this->m_RateOfFire = YELLOW_ROF[m_WeaponLevel[YELLOW]-1];
			break;
		}
	case BLACK:
		{
			m_Consumed = EMPTY;
			break;
		}
	case BLUE:
		{
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_COINPICKUP);

			//TODO:: set time, other defaults here
			this->m_RateOfFire = BLUE_ROF[m_WeaponLevel[BLUE]-1];
			this->m_ShotTotal = BLUE_AMMO[m_WeaponLevel[BLUE]-1];// + 10000000000000;
			this->m_LaserChargeTimer = BLUE_TIMER_CHARGE;
			break;
		}

		/*m_Stored = EMPTY;*/
	};
	//m_Stored = EMPTY;
}

void CWeapons::AddTime (float _time)
{
	m_TotalTime += _time;
	m_ElapsedTime = _time;
}

float CWeapons::GetWeaponMaxAmmo( void )
{
	LevelUp();

	switch (m_Consumed)
	{
	case RED:
		return RED_AMMO[m_WeaponLevel[RED]-1];

	case YELLOW:
		return YELLOW_AMMO[m_WeaponLevel[YELLOW]-1];

	case BLUE:
		return BLUE_AMMO[m_WeaponLevel[BLUE]-1];
	}

	return 0.0f;
}

unsigned int CWeapons::GetNeededExperience( int wep )
{ 
	if(wep == -1)
		wep = m_Consumed;

	if( wep == RED )
		return RED_EXPERIENCE[m_WeaponLevel[RED]-1];
	else if( wep == BLUE )
		return BLUE_EXPERIENCE[m_WeaponLevel[BLUE]-1];
	else if( wep == YELLOW )
		return YELLOW_EXPERIENCE[m_WeaponLevel[YELLOW]-1];

	return 0;
}

void CWeapons::LevelUp(void)
{
	switch(m_Consumed)
	{
	case RED:
		//m_WeaponExperience[RED]++;
		if( (unsigned int)m_WeaponLevel[RED] < 3 && (unsigned int)m_WeaponExperience[RED] >= RED_EXPERIENCE[m_WeaponLevel[RED]-1] )
		{
			m_WeaponLevel[RED]++;
			m_WeaponExperience[RED]=0;
			CMessageSystem::GetInstance()->SendMessage(new CHUDLevelUpMessage(RED,m_WeaponLevel[RED]));

			if(this->GetShotTotal() > RED_AMMO[m_WeaponLevel[RED]-1])
				this->SetShotTotal(RED_AMMO[m_WeaponLevel[RED]-1]);

	//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_LEVEL_UP);
			// Replenishes ammo and resets the stats for the weapon.
			this->m_RateOfFire = RED_ROF[m_WeaponLevel[RED]-1];
		}
		break;

	case YELLOW:
		//m_WeaponExperience[YELLOW]++;
		if( (unsigned int)m_WeaponLevel[YELLOW] < 3 && (unsigned int)m_WeaponExperience[YELLOW] >= YELLOW_EXPERIENCE[m_WeaponLevel[YELLOW]-1] )
		{

			m_WeaponLevel[YELLOW]++;
			m_WeaponExperience[YELLOW]=0;
			CMessageSystem::GetInstance()->SendMessage(new CHUDLevelUpMessage(YELLOW,m_WeaponLevel[YELLOW]));

//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_LEVEL_UP);
			//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::);
			// Replenishes ammo and resets the stats for the weapon.
			this->m_RateOfFire = YELLOW_ROF[m_WeaponLevel[YELLOW]-1];
		}
		break;

	case BLUE:
		//m_WeaponExperience[BLUE]++;
		if( (unsigned int)m_WeaponLevel[BLUE] < 3 && (unsigned int)m_WeaponExperience[BLUE] >= BLUE_EXPERIENCE[m_WeaponLevel[BLUE]-1] )
		{
			m_WeaponLevel[BLUE]++;
			m_WeaponExperience[BLUE]=0;
			CMessageSystem::GetInstance()->SendMessage(new CHUDLevelUpMessage(BLUE,m_WeaponLevel[BLUE]));

//							AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_LEVEL_UP);
			// Replenishes ammo and resets the stats for the weapon.
			this->m_RateOfFire = BLUE_ROF[m_WeaponLevel[BLUE]-1];
		}
		break;
	};
}

bool CWeapons::Shoot( CObjectFactory* _ObjFactory, CPlayer* _player, Vec3f* _cameraTargetPosition )
{
	if (m_ShotTotal <= 0)
	{
		this->m_Consumed = EMPTY;

	}

	if( m_Consumed != BLUE )
	{
		if (m_RateOfFire > m_TotalTime)
			return false;
		--m_ShotTotal;
	}

	LevelUp();
	switch (m_Consumed)
	{
	case RED:
		{
			bool finalBlast = false;
			if( m_WeaponLevel[RED] == 3 )
				finalBlast = true;

			Red_Stats S = RED_STATS[m_WeaponLevel[RED]-1];
			CMessageSystem::GetInstance()->SendMessageW(new CCreateRedStrikeMessage(_player,finalBlast,S.Duration,S.Rate,S.Timer_Start,S.Timer_End,S.Size));
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_TARGET);

			break;
		}
	case YELLOW:
		{
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_PISTOLSHOT);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MACHINEGUN_FIRE);
			//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::SFX_LASER_CHARGE_VARIATION1);

			for(int i = 0; i < NUMBER_SHOTGUN_BULLETS; i++)
			{
				if( (unsigned int)m_WeaponLevel[YELLOW] == 3)
					CMessageSystem::GetInstance()->SendMessage(new CCreateBulletMessage(_player,false,true,false,RandShotgunAngle(0),0.0f,true));
				else
					CMessageSystem::GetInstance()->SendMessage(new CCreateBulletMessage(_player,false,true,false,RandShotgunAngle(0)));
			}

			break;
		}
	case BLUE:
		{
			// This is the count down for slowmo.
			if(m_LaserChargeTimer > 0.0f && (unsigned int)m_WeaponLevel[BLUE] == 3)
			{
				m_LaserChargeTimer -= m_ElapsedTime;
			/*	if( m_LaserChargeTimer <= 0.0f )
				{
					CMessageSystem::GetInstance()->SendMessage(new CSlowTimeMessage(0.75f));
				}*/
				m_TotalTime = 0.0f;
			}
			else
			{
				m_LaserChargeTimer = 0.0f;
			}

			_player->GetRenderNode().SetColor( 1.0f, 1.0f, 1.0f, 1.0f );

			static float one_second = 0.0f;
			if(m_LaserPtr == nullptr || m_LaserPtr->GetIsAlive() == false)
			{
				_ObjFactory->Create(ET_LASER, (IEntity**)&m_LaserPtr);
				m_LaserPtr->Initialize(_player);
				m_LaserPtr->m_ObjectFactory = _ObjFactory;
				CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_LASER,m_LaserPtr,0.0f,0.0f,0.0f));

				if((unsigned int)m_WeaponLevel[BLUE] == 1)
				{
					m_LaserPtr->SetMaxScaleTimer(2.0f);
					m_LaserPtr->SetCurrLevel(1);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_LOOP_LASER_INTENSITY1);

				}
				else if((unsigned int)m_WeaponLevel[BLUE] == 2)
				{
					m_LaserPtr->SetMaxScaleTimer(3.0f);
					m_LaserPtr->SetCurrLevel(2);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_LOOP_LASER_INTENSITY2);

				}
				else if((unsigned int)m_WeaponLevel[BLUE] == 3)
				{
					m_LaserPtr->SetMaxScaleTimer(7.0f);
					m_LaserPtr->SetCurrLevel(3);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_LOOP_LASER_INTENSITY3);
				}
			}
			else
			{
				if(m_LaserChargeTimer <= 0.0f)
				{
					m_ShotTotal -= BLUE_LOSS_RATE * m_ElapsedTime;
					one_second += BLUE_LOSS_RATE * m_ElapsedTime;
					//if((unsigned int)m_WeaponLevel[BLUE] == 3)
						//this->m_IsSuperLaserFired = true;

					if(one_second >= 1.0f)
					{

						if( (unsigned int)m_WeaponLevel[BLUE] < 3 && (unsigned int)m_WeaponExperience[BLUE] >= BLUE_EXPERIENCE[m_WeaponLevel[BLUE]-1] )
						{
							if((unsigned int)m_WeaponLevel[BLUE] == 1)
								m_LaserPtr->SetMaxScaleTimer(2.0f);
							else if((unsigned int)m_WeaponLevel[BLUE] == 2)
								m_LaserPtr->SetMaxScaleTimer(3.0f);
							else if((unsigned int)m_WeaponLevel[BLUE] == 3)
								m_LaserPtr->SetMaxScaleTimer(5.0f);
						}
						one_second = 0.0f;
					}
				}
					
				if (m_RateOfFire < m_TotalTime)
				{
					m_TotalTime = 0.0f;
				}
				if (m_ShotTotal <= 0)
				{
					m_LaserPtr->SetIsActive(false);
					m_LaserPtr->SetIsAlive(false);
					m_LaserPtr = nullptr;
					m_LaserChargeTimer = BLUE_TIMER_CHARGE;
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_2D_RAIN_LOOP);
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_WEAPONWHOOSH);
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_LASER_LOOP);
					//this->m_IsSuperLaserFired = false;
					m_Consumed = EMPTY;
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_LASER_INTENSITY1);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_LASER_INTENSITY2);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_LASER_INTENSITY3);
					break;
				}
			}
			m_LaserPtr->SetBeingUsed(true);
			return true;
		}
		case EMPTY:
		{
			m_RateOfFire = 1.0f;
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_3D_MISSLE_LOOP);
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_SMASHCRATE);
			break;
		}
	default:
		{
			return false;
		}
	};

	m_TotalTime = 0.0f;

	return true;
}

void CWeapons::SetDefaultBullet (CProjectile* _bullet)
{
	// Adjust bullets starting position
	Vec3f newPos, scaledUp;
	glsCopyVector3(newPos,_bullet->GetRenderNode().GetPosition());	
	glsCopyVector3(scaledUp,_bullet->GetRenderNode().GetUpVector());
	glsScaleVector3(scaledUp, 3.25f);
	glsSubtractVectors3(newPos,_bullet->GetRenderNode().GetPosition(),scaledUp);
	_bullet->SetPosition(newPos);
}

void CWeapons::SetExperience( int _gun, int _exp )
{ 
	m_WeaponExperience[_gun] = _exp;

	int neededEXP = GetNeededExperience(_gun);
	if(m_WeaponExperience[_gun] > neededEXP)
		m_WeaponExperience[_gun] = neededEXP;
}

//////////////////////////////////////////////////////
//This function will return a random value between the 
//spread angle #defines using the x and y.  You pass
//in a 0 for X and a 1 for Y
//////////////////////////////////////////////////////
float CWeapons::RandShotgunAngle(unsigned int _x_OR_y)
{
	if(!_x_OR_y)
	{
#if X_ANGLE_SHOTGUN_SPREAD
		return float(rand() % X_ANGLE_SHOTGUN_SPREAD - (X_ANGLE_SHOTGUN_SPREAD * 0.5f));
#else
		return 0.0f;
#endif
	}
	else
	{
#if Y_ANGLE_SHOTGUN_SPREAD
		return float(rand() % Y_ANGLE_SHOTGUN_SPREAD - (Y_ANGLE_SHOTGUN_SPREAD * 0.5f));
#else
		return 0.0f;
#endif
	}
}

