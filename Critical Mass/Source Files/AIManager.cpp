/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : AIManager.h
Author : Joshua Villarreal / Evan Wright

Description : Manages the Slot System for the AI in Critical Mass

Created :  07/16/2013
Modified : 07/24/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/AIManager.h"
#include "../Header Files/SmallEnemy.h"
#include "../Header Files/MediumEnemy.h"
#include "../Header Files/LargeEnemy.h"
#include "../Header Files/BossEnemy.h"
#include "../Header Files/Player.h"
#include "../Header Files/Shield.h"
#include "../Header Files/CollisionLib.h"

#include "../Header Files/Conditions.h"
#include "../Header Files/Behaviors.h"
#include "../Header Files/Selector.h"
#include "../Header Files/Sequence.h"

#include "../Header Files/Definitions.h"

CAIManager::CAIManager(void)
{

}

void CAIManager::Initialize( CPlayer* _target )
{
	m_Target = _target;
	m_IsPaused = false;

	// Temp var
	float chase_range = 2000.0f;
	float attack_range = 15.0f;

	// Create the small enemy behavior tree!
	for(int i = 0; i < MAX_ENEMY_SMALL; ++i)
	{
		BehaviorNode* lookat	= new Target_LookAt();					
		BehaviorNode* outrange	= new Check_OutsideRange(attack_range,-2.0f,2.0f);	
		BehaviorNode* timer		= new TimerFinished(0.5f);				

		BehaviorNode* idle		= new Idle();							
		BehaviorNode* chase		= new Movement_Chase();					
		BehaviorNode* strafe	= new Movement_Strafe();				
		BehaviorNode* oscilate	= new Movement_Oscillation(0.5f,1.0f);	
		BehaviorNode* rush		= new Attack_Rush();					
		BehaviorNode* charging	= new Attack_Prepare();					

		Sequence* seq2			= new Sequence(3,outrange,chase,oscilate);
		Sequence* seq3			= new Sequence(2,timer,rush);
		Sequence* seq4			= new Sequence(2,strafe,charging);
		Selector* sel1			= new Selector(3,seq2,seq3,seq4);		
		Sequence* seq1			= new Sequence(2,lookat,sel1);
		BehaviorNode* root		= new Selector(2,seq1,idle);

		m_SmallTrees[i].SetRoot(root);
	}

	// Temp var
	chase_range = 25.0f;
	attack_range = 50.0f;
	// Create the medium enemy behavior tree!
	for(int i = 0; i < MAX_ENEMY_MEDIUM; ++i)
	{
		BehaviorNode* inrange			= new Check_InsideRange(chase_range,0.0f,1.0f);	
		BehaviorNode* outrange			= new Check_OutsideRange(attack_range,-1.0f,0.0f);	
		BehaviorNode* timer1			= new TimerFinished(2.5f);				
		BehaviorNode* timer2			= new TimerFinished(5.0f);					
		BehaviorNode* timer3			= new TimerRunning(2.0f);					
		BehaviorNode* chase				= new Movement_Chase();					
		BehaviorNode* strafe			= new Movement_Strafe();
		BehaviorNode* tell				= new Attack_Prepare();	
		BehaviorNode* plasmaCannon		= new Attack_Shoot_PlasmaCannon();
		BehaviorNode* flee				= new Movement_Flee();
		BehaviorNode* emp				= new Attack_Shoot_EMP(5,3,12.0f);
		BehaviorNode* lookat1			= new Target_LookAt();	
		BehaviorNode* lookat2			= new Target_LookAt();	
		BehaviorNode* lookat3			= new Target_LookAt();	
		BehaviorNode* lookat4			= new Target_LookAt();	

		Sequence* seq1 = new Sequence(3,lookat1,inrange,flee);
		Sequence* seq2 = new Sequence(3,lookat2,outrange,chase);
		Sequence* seq3 = new Sequence(4,lookat3,strafe,timer1,plasmaCannon);
		Sequence* seq4 = new Sequence(5,lookat4,timer2,tell,timer3,emp);

		BehaviorNode* root = new Selector(4,seq1,seq2,seq4,seq3);

		m_MediumTrees[i].SetRoot(root);
	}

	// Temp var
	chase_range = 10.0f;
	attack_range = 75.0f;
	// Create the Large enemy behavior tree!
	for(int i = 0; i < MAX_ENEMY_LARGE; ++i)
	{	
		BehaviorNode* inrange			= new Check_InsideRange(chase_range,0.0f,1.0f);	
		BehaviorNode* outrange			= new Check_OutsideRange(attack_range,-1.0f,0.0f);		
		BehaviorNode* timer2			= new TimerFinished(3.0f);					
		BehaviorNode* timer3			= new TimerRunning(0.5f);					
		BehaviorNode* chase				= new Movement_Chase();					
		BehaviorNode* strafe			= new Movement_Strafe();
		BehaviorNode* tell				= new Attack_Prepare();	
		BehaviorNode* flee				= new Movement_Flee();
		BehaviorNode* lookat1			= new Target_LookAt();	
		BehaviorNode* lookat2			= new Target_LookAt();	
		BehaviorNode* lookat3			= new Target_LookAt();	
		BehaviorNode* lookat4			= new Target_LookAt();	
		BehaviorNode* ewake				= new Attack_ExplosiveWake(3,70,0.0f,150.0f);

		Sequence* seq1 = new Sequence(3,lookat1,inrange,flee);
		Sequence* seq2 = new Sequence(3,lookat2,outrange,chase);
		Sequence* seq3 = new Sequence(2,lookat3,strafe);
		Sequence* seq4 = new Sequence(5,timer2,lookat4,tell,timer3,ewake);

		BehaviorNode* root = new Selector(4,seq1,seq2,seq4,seq3);

		m_LargeTrees[i].SetRoot(root);
	}

	float boss_flury_rate	= 0.8f;
	float boss_flury_delay	= 0.1f;
	float boss_mortar_rate	= 3.0f;
	float boss_mortar_delay	= 2.0f;
	float boss_alpha_rate	= 3.0f;
	float boss_alpha_delay	= 2.0f;

	BehaviorNode* lookat2	= new Target_LookAt();
	BehaviorNode* timeFin2	= new TimerFinished(boss_mortar_rate);
	BehaviorNode* chase2	= new Movement_Chase();
	BehaviorNode* timeRun2	= new TimerRunning(boss_mortar_delay);
	BehaviorNode* tell2		= new Attack_Prepare();	
	BehaviorNode* mField	= new Attack_MortarField(5.0f,0.5f);
	BehaviorNode* attack2	= new Sequence(3,tell2,timeRun2,mField);
	BehaviorNode* phase1	= new Sequence(3,chase2,timeFin2,attack2);

	BehaviorNode* lookat3	= new Target_LookAt();
	BehaviorNode* timeFin3	= new TimerFinished(boss_alpha_rate);
	BehaviorNode* chase3	= new Movement_Chase();
	BehaviorNode* timeRun3	= new TimerRunning(boss_alpha_delay);
	BehaviorNode* tell3		= new Attack_Prepare();	
	BehaviorNode* aStrike	= new Attack_ExplosiveWake();
	BehaviorNode* attack3	= new Sequence(3,tell3,timeRun3,aStrike);
	BehaviorNode* phase2	= new Sequence(3,chase3,timeFin3,attack3);

	BehaviorNode* lookat1	= new Target_LookAt();
	BehaviorNode* timeFin1	= new TimerFinished(boss_flury_rate);
	BehaviorNode* chase1	= new Movement_Chase();
	BehaviorNode* tell1		= new Attack_Prepare();	
	BehaviorNode* flury		= new Attack_Shoot_PlasmaCannon(12,360.0f);
	BehaviorNode* attack1	= new Sequence(2,tell1,flury);
	BehaviorNode* phase3	= new Sequence(3,chase1,timeFin1,attack1);

	BehaviorNode* root1		= new Sequence(2,lookat1,phase1);
	BehaviorNode* root2		= new Sequence(2,lookat2,phase2);
	BehaviorNode* root3		= new Sequence(2,lookat3,phase3);

	m_BossTree1.SetRoot(root1);
	m_BossTree2.SetRoot(root2);
	m_BossTree3.SetRoot(root3);
}

void CAIManager::Reset(void)
{
	m_SmallEnemies.clear();
	m_MediumEnemies.clear();
	m_LargeEnemies.clear();
	m_Boss = nullptr;
}

void CAIManager::Release( void )
{
	for(int i = 0; i < MAX_ENEMY_SMALL; ++i)
		m_SmallTrees[i].Release();

	for(int i = 0; i < MAX_ENEMY_MEDIUM; ++i)
		m_MediumTrees[i].Release();

	for(int i = 0; i < MAX_ENEMY_LARGE; ++i)
		m_LargeTrees[i].Release();

	m_BossTree1.Release();
	m_BossTree2.Release();
	m_BossTree3.Release();

	m_Target = nullptr;
}

void CAIManager::Update( float _elapsedTime )
{
	UpdateList(_elapsedTime,m_SmallEnemies);
	UpdateList(_elapsedTime,m_MediumEnemies);
	UpdateList(_elapsedTime,m_LargeEnemies);

	if( m_Boss != nullptr && m_Target != nullptr )
	{
		CBossEnemy* boss = ((CBossEnemy*)m_Boss);

#if _DEBUG
		if(m_IsPaused)
		{
			boss->SetIsPaused(true);
			return;
		}
		else if( boss->GetIsPaused() == true )
			boss->SetIsPaused(false);
#endif
		if(boss->GetPhase() == 1)
		{
			boss->SetMoveSpeed(8.0f);
			boss->SetTree(&m_BossTree1);
		}
		else if(boss->GetPhase() == 2)
		{
			boss->SetMoveSpeed(10.0f);
			boss->SetTree(&m_BossTree2);
		}
		else if(boss->GetPhase() == 3)
		{
			boss->SetMoveSpeed(13.0f);
			boss->SetTree(&m_BossTree3);
		}
		else
			int x = 0; // DO SOME SPECIAL DEATH STUFF MAYBE HERE

		if( boss->GetIsActive() && boss->GetIsAlive() )
		{
			if(boss->GetState() < Enemy_NegativeState )
				boss->GetTree()->Update(boss,_elapsedTime);
			else
				boss->GetTree()->Reset();
		}
		else
			boss->GetTree()->Reset();
	}
}

void CAIManager::UpdateList( float _elapsedTime, std::vector<CLivingEntity*>& list )
{
	std::vector<CLivingEntity*> stillActive;
	for(unsigned int i = 0; i < list.size(); i++)
	{
		if( list[i] != nullptr && m_Target != nullptr )
		{
			CEnemy* enemy = ((CEnemy*)list[i]);
			
#if _DEBUG
				if(m_IsPaused)
				{
					enemy->SetIsPaused(true);
					stillActive.push_back(enemy);
					continue;
				}
				else if( enemy->GetIsPaused() == true )
					enemy->SetIsPaused(false);
#endif

			if( enemy->GetIsActive() && enemy->GetIsAlive() )
			{
				if(enemy->GetState() < Enemy_NegativeState )
					enemy->GetTree()->Update(enemy,_elapsedTime);
				else
					enemy->GetTree()->Reset();

				stillActive.push_back(enemy);
			}
			else
				enemy->GetTree()->Reset();
		}
	}
	if(list.size() != stillActive.size())
		list.resize(stillActive.size());
	for(unsigned int i = 0; i < stillActive.size(); ++i)
		list[i] = stillActive[i];
}

void CAIManager::AddSmall( CLivingEntity* _small )
{ 
	((CEnemy*)_small)->SetTarget(m_Target);
	m_SmallEnemies.push_back(_small); 
	((CEnemy*)_small)->SetID(m_SmallEnemies.size());

	((CEnemy*)_small)->SetTree(&m_SmallTrees[m_SmallEnemies.size()-1]);
}

void CAIManager::AddMedium( CLivingEntity* _medium )
{
	((CEnemy*)_medium)->SetTarget(m_Target);
	m_MediumEnemies.push_back(_medium);
	((CEnemy*)_medium)->SetID(m_MediumEnemies.size());

	((CEnemy*)_medium)->SetTree(&m_MediumTrees[m_MediumEnemies.size()-1]);
}

void CAIManager::AddLarge( CLivingEntity* _large )
{
	((CEnemy*)_large)->SetTarget(m_Target);
	m_LargeEnemies.push_back(_large);
	((CEnemy*)_large)->SetID(m_LargeEnemies.size());

	((CEnemy*)_large)->SetTree(&m_LargeTrees[m_LargeEnemies.size()-1]);
}

void CAIManager::AddBoss( CLivingEntity* _boss )
{
	if(_boss)
	{
		m_Boss = _boss;

		((CEnemy*)m_Boss)->SetTarget(m_Target);
		((CEnemy*)m_Boss)->SetID(0);
		((CEnemy*)m_Boss)->SetTree(&m_BossTree1);	
	}
}

void CAIManager::DestroySmall( bool _killBoss )
{
	DestroyList(m_SmallEnemies);
	DestroyList(m_MediumEnemies);
	for(unsigned int i = 0; i < m_LargeEnemies.size(); ++i)
	{
		((CLargeEnemy*)m_LargeEnemies[i])->GetShield()->SetIsActive(false);
		((CLargeEnemy*)m_LargeEnemies[i])->GetShield()->SetIsAlive(false);

	}
	DestroyList(m_LargeEnemies);
	if(m_Boss && _killBoss)
	{
		((CEnemy*)m_Boss)->GetTree()->Reset();
		m_Boss->SetHealth(0);
	}
}

void CAIManager::DestroyList( std::vector<CLivingEntity*>& list )
{
	for(unsigned int i = 0; i < list.size(); ++i)
	{
		((CEnemy*)list[i])->GetTree()->Reset();
		list[i]->SetHealth(0);
	}
}
