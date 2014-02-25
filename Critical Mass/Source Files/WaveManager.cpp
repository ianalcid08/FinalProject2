/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : WaveManager.h
Author :  Corey Morehead + Evan Wright

Description : This Class will hold any variables and set up for the WaveManager class.

Created :  07/27/2013
Modified : 08/12/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/StdAfx.h"
#include "../Header Files/WaveManager.h"
#include "../Header Files/Depot.h"
#include "../Header Files/AIManager.h"
#include "../Header Files/ObjectFactory.h"
#include "../Header Files/Depot.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/Definitions.h"

#include "../Header Files/MessageSystem.h"
#include "../../Includes/tinyxml/tinyxml.h"

#include "../Header Files/glsMath.h"
#include "../Header Files/Renderer.h"

//Sound includes...........................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/Entity3D.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;

using std::stringstream;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CWaveManager::CWaveManager( void )
{
	m_SpawnTimerSmall	= 0.0f;
	m_SpawnTimerMedium	= 0.0f;
	m_SpawnTimerLarge	= 0.0f;

	m_NextWaveTimer = 0.0f;
	m_Intermission = false;
	m_ChangeSong = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CWaveManager::Release( void )
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loads the waves based on the sent in file name, NOT using XML.
Parameters:
[in] _fileName - the file to load.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CWaveManager::LoadWaves( const char* _fileName )
{
	m_Waves.clear();
	// Check load of the wave through our shizz.
	fstream file(_fileName, ios_base::in);
	if(file.is_open())
	{
		int numWaves = 0;
		file >> numWaves;
		for(int i = 0; i < numWaves; ++i)
		{
			Wave that;
			file.ignore(INT_MAX,'\n');
			file >> that.m_NumActiveDepots;

			file.ignore(INT_MAX,'\n');
			file >> that.m_TotalSmall;
			file.ignore(INT_MAX,'\n');
			file >> that.m_TotalMedium;
			file.ignore(INT_MAX,'\n');
			file >> that.m_TotalLarge;

			file.ignore(INT_MAX,'\n');
			file >> that.m_SpawnRateSmall;
			file.ignore(INT_MAX,'\n');
			file >> that.m_SpawnRateMedium;
			file.ignore(INT_MAX,'\n');
			file >> that.m_SpawnRateLarge;

			that.m_BossWave = false;

			m_Waves.push_back(that);
		}
		file.close();

		m_Waves[m_Waves.size()-1].m_BossWave = true;

		m_CurrentWave = 0;
		m_NumLargeSpawned = 0;
		Reset();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Resets the wave, to the sent in wave XML.
Parameters:
[in] _fildeName - The wave XML to load in.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CWaveManager::Reset( void )
{

	if(m_ChangeSong)
	{
//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_04);
//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_05);
	}

	m_Intermission = false;
	m_WaveStart = true;
	m_ChangeSong = false;

	m_SpawnTimerSmall  = 0.0f;
	m_SpawnTimerMedium = 0.0f;
	m_SpawnTimerLarge  = 0.0f;

	m_NumLargeSpawned = 0;

	m_ActiveDepotIndex.clear();
	for(unsigned int i = 0; i < m_Waves[m_CurrentWave].m_NumActiveDepots; ++i )
		m_ActiveDepotIndex.push_back(i);

	m_InOnslaught = false;
	int num = rand() % 4000;
	m_OnslaughtTimer = (float)num / 100.0f + 1.5f;
}

void CWaveManager::Initialize( CRenderer* _Render )
{
	m_Renderer = _Render;

	//m_Texture.SetScreenHeight(_Render->GetScreenHeight());
	//m_Texture.SetScreenWidth(_Render->GetScreenWidth());
	//m_Texture.SetHeight(128.0f);
	//m_Texture.SetWidth(128.0f);
	//m_Texture.SetPosX(_Render->GetScreenWidth() * 0.5f - 64.0f);
	//m_Texture.SetPosY(_Render->GetScreenHeight() * 0.5f - 192.0f);
	//m_Texture.BuildGeometry();
	//m_Texture.SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_ONE));
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Texture.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(NUMBER_ONE), 
									RECTANGLE(0.0f, 128.0f, 0.0f, 128.0f), _Render->GetScreenWidth() * 0.5f - 64.0f * (m_Renderer->GetScreenHeight() / 720.0f), _Render->GetScreenHeight() * 0.5f - 192.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);

	//m_WaveCompletedText.SetScreenHeight(_Render->GetScreenHeight());
	//m_WaveCompletedText.SetScreenWidth(_Render->GetScreenWidth());
	//m_WaveCompletedText.SetHeight(64);
	//m_WaveCompletedText.SetWidth(256);
	//m_WaveCompletedText.SetPosX(_Render->GetScreenWidth() * 0.5f-128);
	//m_WaveCompletedText.SetPosY(128);
	//m_WaveCompletedText.BuildGeometry();
	//m_WaveCompletedText.SetTextureHandle(CAssetManager::GetInstance()->GetTexture(WAVE_MANAGER_WAVE_COMPLETE));
	m_WaveCompletedText.Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(WAVE_MANAGER_WAVE_COMPLETE), 
									RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f), _Render->GetScreenWidth() * 0.5f-128 * (m_Renderer->GetScreenHeight() / 720.0f), 128 * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks if a wave is complete, based on the sent in depots.
Parameters:
[in] _arrayDepots[6] - the depots
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool CWaveManager::IsWaveComplete( IEntity* _arrayDepots[6] )
{
	for(unsigned int i = 0; i < 6; i++)
	{
		if( ((CDepot*)_arrayDepots[i])->GetState() != DEACTIVATED)
			return false;
	}

	if(m_WaveStart == true)
		return false;


	if(!m_Intermission)
	{
		if(m_CurrentWave == m_Waves.size()-1)
		{
			m_NextWaveTimer = 0.0f;
			m_Intermission = true;
		}
		else
		{
			m_NextWaveTimer = 5.0f;
			m_Intermission = true;
		}
	}

	return true;
}

bool CWaveManager::WaveCompleted( void )
{
	bool timer = m_NextWaveTimer <= 0.0f;

	if( timer && m_Intermission )
	{
		m_CurrentWave++;
		if( this->AllWavesCompleted() )
			return false;

		Reset();

		return true;
	}
	else
		return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Activates the closest depots to the target.
Parameters:
[in] _target - the target to check around.
[in] _arrayDepots[6] - the depots
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CWaveManager::FindClosestDepots( CBaseEntity* _target, IEntity* _arrayDepots[6] )
{
	Vec3f pos;
	glsCopyVector3(pos, _target->GetRenderNode().GetPosition());
	float length[6];
	int indices[6];
	for(unsigned int i = 0; i < 6; ++i)
	{
		length[i] = glsGetDistance3( pos, ((CBaseEntity*)_arrayDepots[i])->GetRenderNode().GetPosition() );
		indices[i] = i;
	}

	for(unsigned int i = 0; i < 6; ++i)
	{
		for(unsigned int j = 0; j < 6; ++j)
		{
			if( length[i] < length[j] )
			{
				float temp = length[i];
				length[i] = length[j];
				length[j] = temp;

				int index = indices[i];
				indices[i] = indices[j];
				indices[j] = index;
			}
		}
	}

	Wave currentWave = m_Waves[m_CurrentWave];
	if( currentWave.m_NumActiveDepots > 6 )
		currentWave.m_NumActiveDepots = 1;

	m_ActiveDepotIndex.clear();
	for(unsigned int i = 0; i < currentWave.m_NumActiveDepots; ++i)
	{
		((CDepot*)_arrayDepots[indices[i]])->Activate(currentWave.m_BossWave);
		((CDepot*)_arrayDepots[indices[i]])->SetHealth(MAX_HP_DEPOT);
		m_ActiveDepotIndex.push_back(indices[i]);
	}
}

void CWaveManager::DrawElements(void)
{
	if( m_NextWaveTimer > 0.0f )
	{
		if(m_NextWaveTimer < 3.0f)
		{
			m_Renderer->DrawTexture(&m_Texture);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the WaveManager!
Parameters:
[in] _elapsedTime - delta time.
[in] _arrayDepots[6] - the depots
[in] _aiManager - The AI Manager to use if an enemy is created.
[in] _objectFactory - Object Factory used to create an enemy, if needed.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CWaveManager::Update( float _elapsedTime, IEntity* _arrayDepots[6], CAIManager* _aiManager )
{
	if(m_Waves.size()-1 < (unsigned int)m_CurrentWave)
	{
		// THE GAME IS COMPLETED!
		return;
	}

	Wave currentWave = m_Waves[m_CurrentWave];

	if( m_WaveStart && m_NextWaveTimer <= 0.0f )
	{
		FindClosestDepots( _aiManager->GetTarget(), _arrayDepots );

		m_NextWaveTimer = 0.0f;
		m_Intermission = false;
		m_WaveStart = false;
	}
	else if( m_NextWaveTimer > 0.0f )
	{
		// Countdown for the next wave!
		m_NextWaveTimer -= _elapsedTime;
		m_Texture.GetRect().SetHeight(128.0f);
		m_Texture.GetRect().SetWidth(128.0f);
		m_Texture.SetPosX(m_Renderer->GetScreenWidth() * 0.5f - 64.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_Texture.SetPosY(m_Renderer->GetScreenHeight() * 0.5f - 192.0f * (m_Renderer->GetScreenHeight() / 720.0f));
		m_Texture.SetOpacity(100.0f);
		m_OnslaughtTimer = 0.0f;
		if(m_NextWaveTimer < 3.0f && m_NextWaveTimer > 2.0f)
			m_Texture.SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_THREE));
		else if(m_NextWaveTimer < 2.0f && m_NextWaveTimer >= 1.0f)
			m_Texture.SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_TWO));
		else if( m_NextWaveTimer < 1.0f )
			m_Texture.SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_ONE));

		m_Texture.BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	}

	// Loop through all the depots checking if a depot is dead and hasn't already 
	// been set to deactivating/deactivated.
	for(int i = 0; i < 6; i++)
	{
		CDepot* depot = ((CDepot*)_arrayDepots[i]);
		if(depot->GetHealth() <= 0 && depot->GetState() != DEACTIVATED)
		{
			depot->Deactivate();
			for(unsigned int x = 0; x < m_ActiveDepotIndex.size(); ++x)
			{
				if(m_ActiveDepotIndex[x] == i)
				{
					m_ActiveDepotIndex.erase(m_ActiveDepotIndex.begin()+x);
				}
			}
		}
	}

	for(unsigned int i = 0; i < 6; i++)
	{
		if(((CDepot*)_arrayDepots[i])->GetState() == ACTIVATED)
			m_WaveStart = false;
	}

	if(m_WaveStart)
		return;

	m_OnslaughtTimer -= _elapsedTime;

	if(m_OnslaughtTimer <= 0.0f && !m_InOnslaught)
	{
		m_InOnslaught = true;
		m_OnslaughtTimer = 25.0f;

		/*m_Texture.SetHeight(53);
		m_Texture.SetWidth(224);
		m_Texture.SetPosX(m_Renderer->GetScreenWidth() * 0.5f - m_Texture.GetWidth() * 0.5f);
		m_Texture.SetPosY(m_Renderer->GetScreenHeight() * 0.5f - m_Texture.GetHeight() * 4.5f);
		m_Texture.SetTextureHandle(CAssetManager::GetInstance()->GetTexture(ONSLAUGHT_MODE));
		m_Texture.BuildGeometry();*/

//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::Play_MX_TRACK_04);
//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_03);
//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_AGGRO);
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_DEPOT_ACTIVATE_ALARM);
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_RADIO_INCOMING);

		//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::p);




		m_FadeTimer = 0.0f;
	}

	if(m_OnslaughtTimer > 0.0f && m_InOnslaught && !currentWave.m_BossWave)
	{
		if(currentWave.m_TotalSmall > 0)
		{
			float numSmall  = ((float)currentWave.m_TotalSmall*((float)m_ActiveDepotIndex.size()/(float)currentWave.m_NumActiveDepots));
			if( _aiManager->GetNumSmallEnemies() < (int)numSmall )
			{
				int numtoSpawn = (int)numSmall -  _aiManager->GetNumSmallEnemies();
				for(int i = 0; i < numtoSpawn; i++)
				{
					int index = m_ActiveDepotIndex[rand()%m_ActiveDepotIndex.size()];
					CDepot* depot = (CDepot*)_arrayDepots[index];
					CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(depot,ET_SMALLENEMY));
				}
			}
		}

		if(currentWave.m_TotalMedium > 0)
		{
			float numMed = ((float)currentWave.m_TotalMedium*((float)m_ActiveDepotIndex.size()/(float)currentWave.m_NumActiveDepots));
			if( _aiManager->GetNumMediumEnemies() < (int)numMed )
			{
				int numtoSpawn = (int)numMed -  _aiManager->GetNumMediumEnemies();
				for(int i = 0; i < numtoSpawn; i++)
				{
					int index = m_ActiveDepotIndex[rand()%m_ActiveDepotIndex.size()];
					CDepot* depot = (CDepot*)_arrayDepots[index];
					CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(depot,ET_MEDIUMENEMY));
				}
			}		
		}

		if( currentWave.m_TotalLarge > 0 && (unsigned int)_aiManager->GetNumLargeEnemies() < currentWave.m_TotalLarge )
		{
			int index = m_ActiveDepotIndex[rand()%m_ActiveDepotIndex.size()];
			CDepot* depot = (CDepot*)_arrayDepots[index];
			CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(depot,ET_LARGEENEMY));
		}

		/*m_FadeTimer += _elapsedTime * 5.0f;
		m_Texture.SetOpacity((sin(m_FadeTimer) + 1.0f) / 2.0f);
		m_Renderer->AddToUIList(&m_Texture);*/
		m_ChangeSong = true;
		return;
	}

	if(m_ChangeSong)
	{
		//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_04);
		//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::Play_MX_TRACK_03);
		m_ChangeSong = false;
	}

	// Spawn enemies, if you can, at the cap of m_TotalSmallEnemy
	if(m_ActiveDepotIndex.size() != 0)
	{
		if( m_ActiveDepotIndex.size() == 0 )
			return; 

		int index = m_ActiveDepotIndex[rand()%m_ActiveDepotIndex.size()];
		CDepot* depot = (CDepot*)_arrayDepots[index];

		// Spawn Small Enemies.
		if( m_SpawnTimerSmall <= 0.0f && currentWave.m_TotalSmall > 0)
		{
			float numSmall  = ((float)currentWave.m_TotalSmall*((float)m_ActiveDepotIndex.size()/(float)currentWave.m_NumActiveDepots));
			if( _aiManager->GetNumSmallEnemies() < numSmall )
				CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(depot,ET_SMALLENEMY));

			m_SpawnTimerSmall = currentWave.m_SpawnRateSmall;
		}

		// Spawn Medium Enemies
		if( m_SpawnTimerMedium <= 0.0f && currentWave.m_TotalMedium > 0)
		{
			float numMedium = ((float)currentWave.m_TotalMedium*((float)m_ActiveDepotIndex.size()/(float)currentWave.m_NumActiveDepots));

			// Cap the value to the max num medium enemies.
			if( numMedium > currentWave.m_TotalMedium )
				numMedium = (float)currentWave.m_TotalMedium;
			else if( numMedium <= 0.0f )
				numMedium = 1.0f;

			// Spawn if the number of medium enemies isn't larger than the number to add.
			if( _aiManager->GetNumMediumEnemies() < numMedium )
				CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(depot,ET_MEDIUMENEMY));

			m_SpawnTimerMedium = currentWave.m_SpawnRateMedium;
		}

		// Spawn Large Enemies.
		if( m_SpawnTimerLarge <= 0.0f && currentWave.m_TotalLarge > 0)
		{
			float numLarge  = ((float)currentWave.m_TotalLarge*((float)m_ActiveDepotIndex.size()/(float)currentWave.m_NumActiveDepots));
			if( _aiManager->GetNumLargeEnemies() < numLarge - m_NumLargeSpawned )
			{
				CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(depot,ET_LARGEENEMY));
				m_NumLargeSpawned++;
			}

			m_SpawnTimerLarge = currentWave.m_SpawnRateLarge;
		}

		if(currentWave.m_BossWave)
		{
			// DO BOSS STUFF HERE, MAYBE?	
			if( _aiManager->GetBoss() == nullptr )
			{
				// Spawn boss message...
				CMessageSystem::GetInstance()->SendMessage(new CCreateEnemyMessage(depot,ET_BOSSENEMY));
				
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_01);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_02);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_03);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_04);
			}
			else
			{
				if(_aiManager->GetBoss()->GetIsActive() == false)
				{
					for(unsigned int i = 0; i < 6; ++i)
					{
						CDepot* depot = (CDepot*)_arrayDepots[i];
						CShield* shield = (CShield*)depot->GetShield();
						if(shield != nullptr)
							shield->SetHealth(0);
					}
					m_ActiveDepotIndex.clear();
				}
			}
		}
	}

	// Decrement the spawn timers.
	m_SpawnTimerSmall  -= _elapsedTime;
	m_SpawnTimerMedium -= _elapsedTime;
	m_SpawnTimerLarge  -= _elapsedTime;
}
