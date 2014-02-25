/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : HUD.h
Author : Evan Wright

Description : Displays important data to the player, like: player health, player ammo, 
			  current weapon, minimap, etc. The info will be organized in the most 
			  appropriate way possible.  This information will be updated by a pointer 
			  passed into it.

Created :  07/18/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef HUD_H_
#define HUD_H_
#include "UIElement.h"
//#include <vector>

//enum eHUDElements{ HUD_HP_BG,HUD_HEALTH,HUD_AMMO_BG,HUD_AMMO,HUD_AMMOTYPE,HUD_LIVES_BG,HUD_LIVES,WAVE_NOTICE,TOTAL_HUD_ELEMENTS};		
//enum eHUDElements{ HUD_HP_BG,HUD_HEALTH,HUD_AMMO_BG,HUD_AMMO,HUD_AMMOTYPE,HUD_LIVES_BG,HUD_LIVES,HUD_WAVE_DISPLAY,WAVE_NOTICE,TOTAL_HUD_ELEMENTS};	

#define MAX_WAVE_DISPLAY_NUM 3  //"Wave" = 1 + first number = 2 + second number = 3
#define SCREEN_BUFFER 16
enum eHUDElements{ 
	HUD_HP_BG,HUD_HEALTH,HUD_AMMO_BG,HUD_AMMO,HUD_AMMOTYPE,HUD_LIVES,WAVE_NOTICE,HUD_PICKUPS,
	HUD_PUSHCD_FILL, HUD_PUSHCD_BG, HUD_PUSHCD_BG2, HUD_PULL_CD_GUAGE,HUD_PULL_CD_NEEDLE,HUD_LOADED, HUD_LIVES_TOKEN1,
	HUD_LIVES_TOKEN2,HUD_LIVES_TOKEN3, HUD_ONE_DEPOT_LEFT, HUD_WAVE, HUD_WAVE_NUMBER_FIRST, HUD_WAVE_NUMBER_SECOND,
	HUD_HEALTH_LABEL, HUD_AMMO_LABEL, HUD_LIVES_LABEL, HUD_WEAPON_LVL, HUD_WEAPON_LVLBAR, HUD_WEAPON_CD, HUD_WEAPON_TAB,
	HUD_BXP, HUD_YXP, HUD_RXP, HUD_RLVL, HUD_BLVL, HUD_YLVL, HUD_MAINFRAME, HUD_EXP_BAR, HUD_EXP_FRAME,
	
	//Level Up Stuff
	HUD_YELLOW_LEVELUP_ICON,
	HUD_RED_LEVELUP_ICON, 
	HUD_BLUE_LEVELUP_ICON,

	HUD_LEVELUP_TEXT,
	TOTAL_HUD_ELEMENTS};		

class CPlayer;
class CGame;
class CRenderer;
class CAssetManager;


class CHUD
{
	int m_PlayerHealth;
	int m_PlayerAmmo;
	int m_PlayerLives;
	int m_PlayerAmmoType;
	int m_PlayerStored;
	bool m_PlayerIsLoaded;
	bool m_FadeHUD;
	float m_PushCD;
	float m_PullCD;
	CRenderer *m_Renderer;
	float m_WeaponXPos;

	bool m_ShowTab;

	bool m_PlayLevelUpSeq;
	float m_levelUpSeqTimer;
	float m_PauseMoveTimer;
	int m_whichWeapon;
	int m_weaponLevel;

	CUIElement m_HUDElements[TOTAL_HUD_ELEMENTS];
	//Use array

	CUIElement* m_WaveNoticeElements[MAX_WAVE_DISPLAY_NUM];


	bool m_playThisOnlyOnce;
public:

	//std::vector<CUIElement*> m_WaveDisplayVec;
	//vector<CUIElement> m_WaveDisplayVec;	//This is a vector that holds changing lengths of UI elements to show the wave notice

	CHUD( void );
	~CHUD( void ) { Release(); }
	void Initialize( CRenderer* _renderer );
	void Rebuild( CRenderer* _renderer );
	void Release( void );
	void Update( float _elapsedTime, CPlayer* _player );

	CUIElement* GetHUDElements() {return m_HUDElements;}

	void ChangeWaveNotice(unsigned int _newWaveNum);
	CUIElement** GetWaveDisplayArr() {return m_WaveNoticeElements;}

	inline void SetFadeHUD(bool _b) { m_FadeHUD = _b; }
	inline void ShowTab(void) { m_ShowTab = !m_ShowTab; }

	void PlayLevelUpSequence(float _fElapsedTime);
	void ResetLevelUpSequence();
	bool GetShowTab() {return m_ShowTab;}

	bool GetPlayLevelUpSeq()			  {return m_PlayLevelUpSeq;}
	void SetPlayLevelUpSeq(bool _newBool) {m_PlayLevelUpSeq = _newBool;}

	int GetWhichWeapon()				{return m_whichWeapon;}
	void SetWhichWeapon(int _newInt)	{m_whichWeapon = _newInt;}

	int GetWeaponLevel()				{return m_weaponLevel;;}
	void SetWeaponLevel(int _newInt)	{m_weaponLevel = _newInt;}
};

#endif

