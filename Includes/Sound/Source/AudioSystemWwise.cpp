//--------------------------------------------------------------------------------
// Written by Justin Murphy
//--------------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "AudioSystemWwise.h"
#include "Entity3D.h"
#include "Common.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/Tools/Common/AkMonitorError.h>
#include <AK/Tools/Win32/AkPlatformFuncs.h>
#include <AkDefaultIOHookBlocking.h>
#include <string>
#include <sstream>
//--------------------------------------------------------------------------------
#pragma comment(lib, "AkSoundEngineDLL.lib")
//--------------------------------------------------------------------------------
using namespace AK;
//--------------------------------------------------------------------------------
AudioSystemWwise* AudioSystemWwise::m_spAudioSystem = nullptr;
//--------------------------------------------------------------------------------

AudioSystemWwise* AudioSystemWwise::GetInstance (void )
{
	static AudioSystemWwise sInstance;
	return &sInstance;
}

AudioSystemWwise::AudioSystemWwise() :
	m_fWorldScale(1.f),
	m_bInitialized(false)
{
	if ( m_spAudioSystem == nullptr )
		m_spAudioSystem = this;
}
//--------------------------------------------------------------------------------
AudioSystemWwise::~AudioSystemWwise()
{

}
//--------------------------------------------------------------------------------
AudioSystemWwise* AudioSystemWwise::Get()
{
	return( m_spAudioSystem );
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::Initialize(Entity3D* _camera)
{
	// Initialize audio engine
	// Memory.
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 40;

	// Streaming.
	AkStreamMgrSettings stmSettings;
	// 	stmSettings.uMemorySize = 32 * 1024*1024; // 32 MB
	StreamMgr::GetDefaultSettings( stmSettings );
	
	AkDeviceSettings deviceSettings;
	StreamMgr::GetDefaultDeviceSettings( deviceSettings );

	AkInitSettings l_InitSettings;
	AkPlatformInitSettings l_platInitSetings;
	SoundEngine::GetDefaultInitSettings( l_InitSettings );
	SoundEngine::GetDefaultPlatformInitSettings( l_platInitSetings );

	// Setting pool sizes for this game.
	// These sizes are tuned for this game, every game should determine its own optimal values.
	//	l_InitSettings.uDefaultPoolSize           = 4 * 1024 * 1024;  // 4 MB
	//	l_InitSettings.uMaxNumPaths               = 16;
	//	l_InitSettings.uMaxNumTransitions         = 128;
	//	l_platInitSetings.uLEngineDefaultPoolSize    = 4 * 1024 * 1024;  // 4 MB
	//l_platInitSetings.hWnd = hwnd;

	AkMusicSettings musicInit;
	MusicEngine::GetDefaultInitSettings( musicInit );
	musicInit.fStreamingLookAheadRatio = 100;

	AKRESULT eResult = SOUNDENGINE_DLL::Init( &memSettings,
		&stmSettings,
		&deviceSettings,
		&l_InitSettings,
		&l_platInitSetings,
		&musicInit );

	if( eResult != AK_Success )
	{
		// Then, we will run the game without sound
		SOUNDENGINE_DLL::Term();
		return false;
	}
	
	// This is a default base path. The game programmer should override this when the game is initialized.
	SOUNDENGINE_DLL::SetBasePath( L"../Resources/soundbanks/" );
	//SOUNDENGINE_DLL::SetLangSpecificDirName( L"English(US)/" );

	// register 64 dummy game objects for one-shot positioning. See PostEvent(ID, location) function for details
	for ( int i = 64; i < 128; ++i )
	{
		char name[256];
		sprintf_s( name, 256, "Dummy #%i", i-63 );
		SoundEngine::RegisterGameObj( i, name );
	}

	m_bInitialized = true;
	//this->LogWrite(L"AudioSystemWwise has been Initialized");

	SetBasePath( _T("Assets/Sounds/SoundBanks/"));
	LoadSoundBank(L"Init.bnk");
	LoadSoundBank(L"SoundBank.bnk");
	RegisterListener(_camera,"Main Camera");

	return true;
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::Shutdown()
{
	SoundEngine::UnregisterAllGameObj();
	// Terminate audio engine
	SOUNDENGINE_DLL::Term();
	//this->LogWrite(L"AudioSystemWwise has been Shutdown");
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::Update()
{
	AkListenerPosition listenerPosition;
	AkSoundPosition soundPosition;

	// update listener positions and orientations
	for( unsigned int i = 0; i < m_RegisteredListeners.size(); i++)
	{
		const XMFLOAT3 & pos = m_RegisteredListeners[i]->GetPosition();
		
		listenerPosition.Position.X = pos.x;
		listenerPosition.Position.Y = pos.y;
		listenerPosition.Position.Z = pos.z;
	
		const XMFLOAT3 & zAxis = m_RegisteredListeners[i]->GetZAxis();
		listenerPosition.OrientationFront.X = zAxis.x;
		listenerPosition.OrientationFront.Y = zAxis.y;
		listenerPosition.OrientationFront.Z = zAxis.z;
	
		const XMFLOAT3 & yAxis = m_RegisteredListeners[i]->GetYAxis();
		listenerPosition.OrientationTop.X = yAxis.x;
		listenerPosition.OrientationTop.Y = yAxis.y;
		listenerPosition.OrientationTop.Z = yAxis.z;

		SoundEngine::SetListenerPosition(listenerPosition,i);
	}
	
	// set entities positions and orientations
	for ( UINT i = 0; i < m_RegisteredEntities.size(); i++ )
	{
		const XMFLOAT3 & pos = m_RegisteredEntities[i]->GetPosition();
		soundPosition.Position.X = pos.x;
		soundPosition.Position.Y = pos.y;
		soundPosition.Position.Z = pos.z;
	
		const XMFLOAT3 & zAxis = m_RegisteredEntities[i]->GetZAxis();
		soundPosition.Orientation.X = zAxis.x;
		soundPosition.Orientation.Y = zAxis.y;
		soundPosition.Orientation.Z = zAxis.z;
		
		SoundEngine::SetPosition( (AkGameObjectID) m_RegisteredEntities[i], soundPosition );
	}
	// render the audio
	SOUNDENGINE_DLL::Tick();
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::RegisterEntity(const Entity3D* entity, const char* name)
{
	if ( NULL == name )
	{
		static int entityNum = 0;
		std::stringstream ssNum;
		ssNum << ++entityNum;
		std::string strTemp = "entity" + ssNum.str();

		if(SoundEngine::RegisterGameObj( (AkGameObjectID)entity, strTemp.c_str() ) != AK_Success)
		{
			wchar_t buff[MAX_PATH];
			wprintf_s(buff, MAX_PATH, L"%s%s%s", L"*Audio Error*\n Registering: \"", strTemp.c_str(), L"\"\n\nContact the audio programmer and replicate error situation.");
			//this->LogWrite(buff);
			return false;
		}
	}
	else
	{
		if(SoundEngine::RegisterGameObj( (AkGameObjectID)entity, name ) != AK_Success)
		{
			wchar_t buff[MAX_PATH];
			wprintf_s(buff, MAX_PATH, L"%s%s", L"*Audio Error*\n Registering audio entity", L"\n\nContact the audio programmer and replicate error situation.");
			//this->LogWrite(buff);
			return false;
		}
	}

	m_RegisteredEntities.push_back( entity );
	return true;
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::UnRegisterEntity(const Entity3D* entity)
{
	for ( std::vector<const Entity3D *>::iterator itr = m_RegisteredEntities.begin(); itr != m_RegisteredEntities.end(); itr++ )
	{
		if ( *itr == entity )
		{
			m_RegisteredEntities.erase( itr );
			if( SoundEngine::UnregisterGameObj( (AkGameObjectID) entity ) != AK_Success)
			{
				wchar_t buff[MAX_PATH];
				wprintf_s(buff, MAX_PATH, L"%s%s", L"*Audio Error*\n Unegistering audio entity", L"\n\nContact the audio programmer and replicate error situation.");
				//this->LogWrite(buff);
				return false;
			}
			break;
		}
	}
	return true;
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::RegisterListener(const Entity3D* entity, const char* name)
{
	if ( NULL == name )
	{
		std::stringstream ssNum;
		ssNum << (m_RegisteredListeners.size()+1);
		std::string strTemp = "listener" + ssNum.str();

		if(SoundEngine::RegisterGameObj( (AkGameObjectID)entity, strTemp.c_str() ) != AK_Success)
		{
			wchar_t buff[MAX_PATH];
			wprintf_s(buff, MAX_PATH, L"%s%s%s", L"*Audio Error*\n Registering: \"", strTemp.c_str(), L"\"\n\nContact the audio programmer and replicate error situation.");
			//this->LogWrite(buff);
			return false;
		}
	}
	else
	{
		if(SoundEngine::RegisterGameObj( (AkGameObjectID)entity, name ) != AK_Success)
		{
			wchar_t buff[MAX_PATH];
			wprintf_s(buff, MAX_PATH, L"%s%s", L"*Audio Error*\n Registering audio entity", L"\n\nContact the audio programmer and replicate error situation.");
			//this->LogWrite(buff);
			return false;
		}
	}
	
	// ensure to update listeners scaling factor to the worlds
	AK::SoundEngine::SetListenerScalingFactor(m_RegisteredListeners.size(),m_fWorldScale);
	m_RegisteredListeners.push_back( entity );
	m_RegisteredEntities.push_back( entity );
	return true;
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::UnRegisterListener(const Entity3D* entity)
{
	for ( std::vector<const Entity3D *>::iterator itr = m_RegisteredListeners.begin(); itr != m_RegisteredListeners.end(); itr++ )
	{
		if ( *itr == entity )
		{
			m_RegisteredListeners.erase( itr );
			if( SoundEngine::UnregisterGameObj( (AkGameObjectID) entity ) != AK_Success)
			{
				wchar_t buff[MAX_PATH];
				wprintf_s(buff, MAX_PATH, L"%s%s", L"*Audio Error*\n Unegistering audio entity", L"\n\nContact the audio programmer and replicate error situation.");
				//this->LogWrite(buff);
				return false;
			}
			break;
		}
	}
	return UnRegisterEntity(entity);
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::PostEvent(AudioEvent eventId,const Entity3D* eventCaller)
{
	if(SoundEngine::PostEvent( eventId, (AkGameObjectID)eventCaller ) == AK_INVALID_PLAYING_ID)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%u%s", L"*Audio Error*\n PostEvent failed on event: \"", eventId, L"\"\n\nEventID might be incorrect or the associated entity might not be registered.\n\nNOTE: Make sure you're loading the soundbanks before attempting to play a sound.");
		//this->LogWrite(buff);
		return false;
	}
	return true;
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::PostEvent(AudioEvent eventId,const XMFLOAT3& pos)
{
	// cycle through the dummy game objects
	// number of dummies needs to be big enough to avoid setting a different position
	// on the same object twice in the same game frame (64 is probably overkill in this game)
	static int iGameObj = 63;
	if ( ++iGameObj > 127 )
		iGameObj = 64;

	AkSoundPosition soundPosition;
	soundPosition.Orientation.X = 0;
	soundPosition.Orientation.Y = 0;
	soundPosition.Orientation.Z = 1.0f;
	
	soundPosition.Position.X = pos.x;
	soundPosition.Position.Y = pos.y;
	soundPosition.Position.Z = pos.z;

	SoundEngine::SetPosition( iGameObj, soundPosition );
	if(SoundEngine::PostEvent( eventId, iGameObj ) == AK_INVALID_PLAYING_ID)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%u%s", L"*Audio Error*\n PostEvent failed on event: \"", eventId, L"\"\n\nEventID might be incorrect.");
		//this->LogWrite(buff);
		return false;
	}
	return true;
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::PostEvent(AudioEvent eventId, unsigned int listenerId)
{
	if( (m_RegisteredListeners.size() - listenerId) < 0)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%u%s", "*Audio Error*\n PostClientEvent failed on event: \"", eventId, L"\"\n\nThe player is not registered. You cannot play an event on the player if there is no player.");
		//this->LogWrite(buff);
		return false;
	}
	if(SoundEngine::PostEvent(eventId, (AkGameObjectID)m_RegisteredListeners[listenerId]) == AK_INVALID_PLAYING_ID)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%u%s", L"*Audio Error*\n PostClientEvent failed on event: \"", eventId, L"\"\n\nThe player might not be registered or the EventID is incorrect.\n\nNOTE: Make sure you're loading the soundbanks before attempting to play a sound.");
		//this->LogWrite(buff);
		return false;
	}
	return true;
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetMaterialID(const Entity3D* pEntity, AkUniqueID akMaterialSwitchDefine, const unsigned int nMaterialID)
{

}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetRTCPValue(const char* szRTCPName, const AkRtpcValue akRtcpVal)
{
	if(SoundEngine::SetRTPCValue( szRTCPName, akRtcpVal) != AK_Success)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%s%s", L"*Audio Error*\n RTCP Value fail on: \"", szRTCPName, L"\"\n\nRTCP name might be incorrect.");
		//this->LogWrite(buff);
	}
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetRTCPValue(const char* szRTCPName, const AkRtpcValue akRtcpVal, const Entity3D* pEntity)
{
	if(SoundEngine::SetRTPCValue( szRTCPName, akRtcpVal, (AkGameObjectID)pEntity) != AK_Success)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%s%s", L"*Audio Error*\n RTCP Value fail on: \"", szRTCPName, L"\"\n\nRTCP name might be incorrect or the associated entity might not be registered.\n\nNOTE: Make sure you're loading the soundbanks before attempting to play a sound.");
		//this->LogWrite(buff);
	}
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetRTCPValue(const AkRtpcID akRtcpID, const AkRtpcValue akRtcpVal)
{
	if(SoundEngine::SetRTPCValue(akRtcpID, akRtcpVal) != AK_Success)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%u%s", L"*Audio Error*\n RTCP Value fail on: \"", akRtcpID, L"\"\n\nRTCP ID might be incorrect.");
		//this->LogWrite(buff);
	}
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetRTCPValue(const AkRtpcID akRtcpID, const AkRtpcValue akRtcpVal, const Entity3D* pEntity)
{
	if(SoundEngine::SetRTPCValue(akRtcpID, akRtcpVal, (AkGameObjectID)pEntity))
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%u%s", L"*Audio Error*\n RTCP Value fail on: \"", akRtcpID, L"\"\n\nRTCP ID might be incorrect or the associated entity might not be registered.\n\nNOTE: Make sure you're loading the soundbanks before attempting to play a sound.");
		//this->LogWrite(buff);
	}
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetState(const char* szStateGroupName, const char* szStateName)
{
	if(AK::SoundEngine::SetState(szStateGroupName, szStateName) != AK_Success)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%s%s%s%s", L"*Audio Error*\n Setting State Failure.\nState Group Name: \"", szStateGroupName, 
			L"\"\nState Name: ", szStateName,L"\n\nState Group or State Name might not exist.");
		//this->LogWrite(buff);
	}
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetState(const AkStateGroupID akStateGroupID, const AkStateID akStateID)
{
	if(AK::SoundEngine::SetState(akStateGroupID, akStateID ) != AK_Success)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%u%s%u%s", L"*Audio Error*\n Setting State Failure.\nState Group ID: \"", akStateGroupID, 
			L"\"\nState ID: \"", akStateID,L"\"\n\nState Group or State Name might not exist.");
		//this->LogWrite(buff);
	}
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetSwitch(const AkSwitchGroupID akSwitchGroupID, const AkSwitchStateID akStateID, const AkGameObjectID gameObjectID)
{
	if(AK::SoundEngine::SetSwitch(akSwitchGroupID, akStateID, gameObjectID) != AK_Success)
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%u%s%u%s", L"*Audio Error*\n Setting Switch Failure.\nState Group ID: \"", akSwitchGroupID, 
			L"\"\nState ID: \"", akStateID,L"\"\nSwitch Group or Switch Name might not exist.");
		//this->LogWrite(buff);
	}
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::PostTrigger(const char* szTriggerName)
{
	AK::SoundEngine::PostTrigger(szTriggerName, AK_INVALID_GAME_OBJECT );
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::PostTrigger(const AkTriggerID akTriggerID)
{
	AK::SoundEngine::PostTrigger(akTriggerID, AK_INVALID_GAME_OBJECT );
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::PostTrigger(const char* szTriggerName, const Entity3D* pEntity)
{
	AK::SoundEngine::PostTrigger(szTriggerName, (AkGameObjectID)pEntity );
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::PostTrigger(const AkTriggerID akTriggerID, const Entity3D* pEntity)
{
	AK::SoundEngine::PostTrigger(akTriggerID, (AkGameObjectID)pEntity );
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetBasePath(const wchar_t* strPath)
{
	SOUNDENGINE_DLL::SetBasePath( strPath );
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::LoadSoundBank(const wchar_t* szBankName)
{
	AkBankID bankID;
	if ( SoundEngine::LoadBank( szBankName , AK_DEFAULT_POOL_ID, bankID ) == AK_Success )
		m_RegisteredSoundBanks[szBankName] = bankID;
	else
	{
		wchar_t buff[MAX_PATH];
		wprintf_s(buff, MAX_PATH, L"%s%s%s", L"*Audio Error*\n Loading sound bank: \"", szBankName, L"\"\n\nTry checking your soundbank names.");
		//this->LogWrite(buff);
		return false;
	}
	std::wstring buff = L"Loaded SoundBank: ";
	buff += szBankName;
	//this->LogWrite(buff);
	return true;
}
//--------------------------------------------------------------------------------
bool AudioSystemWwise::UnloadSoundBank(const wchar_t* szBankName)
{
	if ( m_RegisteredSoundBanks.find(szBankName) != m_RegisteredSoundBanks.end() )
		if( SoundEngine::UnloadBank( m_RegisteredSoundBanks[szBankName], NULL) != AK_Success)
		{
			wchar_t buff[MAX_PATH];
			wprintf_s(buff, MAX_PATH, L"%s%s%s", L"*Audio Error*\n Unloading sound bank: \"", szBankName, L"\"\n\nTry checking your soundbank names.");
			//this->LogWrite(buff);
			return false;
		}
	return true;
}
//--------------------------------------------------------------------------------
void AudioSystemWwise::SetWorldScale(float fScale)
{
	m_fWorldScale = fScale;
	for(unsigned int i = 0; i < m_RegisteredListeners.size(); i++)
	{
		AK::SoundEngine::SetListenerScalingFactor(i,m_fWorldScale);
	}
}
//--------------------------------------------------------------------------------
#ifdef _DEBUG
void AudioSystemWwise::SendMessage(const char* message)
{
	Monitor::PostString( message, Monitor::ErrorLevel_Message );
}
#endif
//--------------------------------------------------------------------------------