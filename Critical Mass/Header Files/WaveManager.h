/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : WaveManager.h
Author :  Corey Morehead

Description : This Class will hold any variables and set up for the WaveManager class.

Created :  07/23/2013
Modified : 08/12/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WAVEMANAGER_H_
#define WAVEMANAGER_H_

#include "glsMath.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/Depot.h"
#include "../Header Files/SmallEnemy.h"
#include "../Header Files/MediumEnemy.h"
#include "../Header Files/LargeEnemy.h"
#include "../Header Files/Crate.h"

#include "../Header Files/UIElement.h"

class CRenderer;
class CAIManager;
class CWaveManager
{
	struct Wave
	{
		unsigned int	m_NumActiveDepots;

		unsigned int	m_TotalSmall;
		unsigned int	m_TotalMedium;
		unsigned int	m_TotalLarge;

		float			m_SpawnRateSmall;
		float			m_SpawnRateMedium;
		float			m_SpawnRateLarge;

		bool			m_BossWave;
	};

	std::vector<Wave>	m_Waves;
	std::vector<int>	m_ActiveDepotIndex;

	int					m_CurrentWave;
	int					m_NumLargeSpawned;

	float				m_SpawnTimerSmall;
	float				m_SpawnTimerMedium;
	float				m_SpawnTimerLarge;
	float				m_NextWaveTimer;
	float				m_FadeTimer;
	float				m_OnslaughtTimer;
	
	bool				m_WaveStart;
	bool				m_Intermission;
	bool				m_InOnslaught;
	bool				m_ChangeSong;

	// Ah, what is this?  --B
	CUIElement			m_Texture;
	CRenderer*			m_Renderer;		

	void FindClosestDepots( CBaseEntity* _target, IEntity* _arrayDepots[6] );

public:

	CUIElement m_WaveCompletedText;
	CWaveManager( void );
	~CWaveManager( void ) {this->Release();}
	
	void Initialize( CRenderer* _Render );
	void Release( void );

	void Update( float _elapsedTime, IEntity* _arrayDepots[6], CAIManager* _aiManager );

	// Accessors
	inline int GetCurrentWave( void )		{ return m_CurrentWave;		}
	inline float GetNextWaveTimer( void )	{ return m_NextWaveTimer;	}

	//Setters
	inline void SetCurrWave(int _newInt) {m_CurrentWave = _newInt;}

	// Helper functions.
	bool IsWaveComplete( IEntity* _arrayDepots[6] );
	bool WaveCompleted( void );
	void Reset( void );
	void LoadWaves( const char* fileName );
	inline void TempSetBossWave( void )		{ m_Waves[m_Waves.size()-1].m_BossWave = true;			}
	inline bool AllWavesCompleted( void )	{ return m_Waves.size() <= (unsigned int)m_CurrentWave;	}

	void DrawElements( void );
};

#endif
