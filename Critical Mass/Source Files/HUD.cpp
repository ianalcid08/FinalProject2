/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : HUD.cpp
Author : Evan Wright

Description : Displays important data to the player, like: player health, player ammo, 
			  current weapon, minimap, etc. The info will be organized in the most 
			  appropriate way possible.  This information will be updated by a pointer 
			  passed into it.

Created :  07/18/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/HUD.h"
#include "../Header Files/Player.h"
#include "../Header Files/UIElement.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"

//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CHUD::CHUD( void )
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initializes necessary HUD variables.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CHUD::Initialize( CRenderer* _renderer )
{
	m_playThisOnlyOnce = false;
	m_PlayLevelUpSeq = false;
	m_PauseMoveTimer = 0.0f;
	m_levelUpSeqTimer = 0.0f;
	m_whichWeapon = -1;
	m_weaponLevel = -1;

	m_FadeHUD = false;
	m_PlayerHealth = 0;
	m_PlayerAmmo = 0;
	m_PlayerLives = 0;
	m_PlayerAmmoType = 0;

	short ScreenWidth = _renderer->GetScreenWidth();
	short ScreenHeight = _renderer->GetScreenHeight();


	m_Renderer = _renderer;

	Rebuild( _renderer );
}

void CHUD::Rebuild( CRenderer* _renderer )
{
	m_Renderer = _renderer;

	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_HUDElements[HUD_MAINFRAME].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(HUD_FRAME),
		RECTANGLE(0.0f, 164.0f, 0.0f, 173.0f),
		m_Renderer->GetScreenWidth() - 160.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f,
		0.0f, 0.9f, white);

#pragma region Ammo_Bar_and_Fill
	//m_HUDElements[HUD_AMMO_BG].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_AMMO_BG].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_AMMO_BG].SetHeight(30.0f);
	//m_HUDElements[HUD_AMMO_BG].SetWidth(200.0f);
	////m_HUDElements[HUD_AMMO_BG].SetPosX(m_HUDElements[HUD_HP_BG].GetPosX());
	//m_HUDElements[HUD_AMMO_BG].SetPosX(m_HUDElements[HUD_AMMOTYPE].GetPosX() - (m_HUDElements[HUD_AMMO_BG].GetWidth() - (SCREEN_BUFFER - 10)));
	//m_HUDElements[HUD_AMMO_BG].SetPosY(SCREEN_BUFFER);
	//m_HUDElements[HUD_AMMO_BG].BuildGeometry();
	//m_HUDElements[HUD_AMMO_BG].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_FRAME));
	//m_HUDElements[HUD_AMMO_BG].LoadTGAAsTextureRect("Assets/Textures/health_bar_frame.tga");
	m_HUDElements[HUD_AMMO_BG].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_FRAME),
		RECTANGLE(0.0f, 231.0f, 0.0f, 64.0f),
		m_HUDElements[HUD_MAINFRAME].GetPosX() - 165.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		(SCREEN_BUFFER + 15.0f) * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 0.8f, white);


	//m_HUDElements[HUD_AMMO].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_AMMO].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_AMMO].SetHeight(30.0f);
	//m_HUDElements[HUD_AMMO].SetWidth(253.0f);
	//m_HUDElements[HUD_AMMO].SetPosX(m_HUDElements[HUD_AMMO_BG].GetPosX());
	//m_HUDElements[HUD_AMMO].SetPosY(m_HUDElements[HUD_AMMO_BG].GetPosY());
	//m_HUDElements[HUD_AMMO].BuildGeometry();
	//m_HUDElements[HUD_AMMO].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_FILL));
	//m_HUDElements[HUD_AMMO].LoadTGAAsTextureRect("Assets/Textures/health_bar_fill.tga");
	m_HUDElements[HUD_AMMO].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_AMMO_BAR_FILL),
		RECTANGLE(0.0f, 214.0f, 0.0f, 53.0f),
		m_HUDElements[HUD_AMMO_BG].GetPosX() + 6.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_AMMO_BG].GetPosY() + 7.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 0.775f, white);

	m_HUDElements[HUD_WEAPON_LVL].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(ICON_WEAPON_LEVELBAR), RECTANGLE(0.0f, 168.0f, 0.0f, 43.0f),
		m_HUDElements[HUD_AMMO_BG].GetPosX() + 50.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		(SCREEN_BUFFER * 4.0f) * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 0.8f, white);


#pragma endregion

#pragma region Health_Bar_and_Fill
	//m_HUDElements[HUD_HEALTH_LABEL].BuildGeometry();
	//m_HUDElements[HUD_HP_BG].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_HP_BG].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_HP_BG].SetHeight(30.0f);
	//m_HUDElements[HUD_HP_BG].SetWidth(200.0f);
	////m_HUDElements[HUD_HP_BG].SetPosX(m_HUDElements[HUD_HEALTH_LABEL].GetPosX() + m_HUDElements[HUD_HEALTH_LABEL].GetWidth() - 24);
	//m_HUDElements[HUD_HP_BG].SetPosX(SCREEN_BUFFER);
	//m_HUDElements[HUD_HP_BG].SetPosY(SCREEN_BUFFER);
	//m_HUDElements[HUD_HP_BG].BuildGeometry();
	//m_HUDElements[HUD_HP_BG].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_FRAME));
	//m_HUDElements[HUD_HP_BG].LoadTGAAsTextureRect("Assets/Textures/health_bar_frame.tga");

	//This is the metal frame that goes around the health bar
	m_HUDElements[HUD_HP_BG].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_FRAME),
		RECTANGLE(0.0f, 231, 0.0f, 64.0f),
		SCREEN_BUFFER * (m_Renderer->GetScreenHeight() / 720.0f),
		SCREEN_BUFFER * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 0.9f, white);

	//m_HUDElements[HUD_HEALTH].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_HEALTH].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_HEALTH].SetHeight(30.0f);
	//m_HUDElements[HUD_HEALTH].SetWidth(200.0f);
	//m_HUDElements[HUD_HEALTH].SetPosX(m_HUDElements[HUD_HP_BG].GetPosX());
	//m_HUDElements[HUD_HEALTH].SetPosY(m_HUDElements[HUD_HP_BG].GetPosY());
	//m_HUDElements[HUD_HEALTH].BuildGeometry();
	//m_HUDElements[HUD_HEALTH].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_AMMO_BAR_FILL));
	//m_HUDElements[HUD_HEALTH].LoadTGAAsTextureRect("Assets/Textures/ammo_bar_fill.tga");

	m_HUDElements[HUD_HEALTH].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_FILL),
		RECTANGLE(0.0f, 214.0f, 0.0f, 53.0f),
		m_HUDElements[HUD_HP_BG].GetPosX() + 4.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_HP_BG].GetPosY() + 4.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 0.9f, white);
#pragma endregion


	//m_HUDElements[HUD_WEAPON_LVLBAR].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_AMMO_BAR_FILL), RECTANGLE(0.0f, 214.0f, 0.0f, 53.0f),
							//m_HUDElements[HUD_AMMO_BG].GetPosX() + 60.0f, m_HUDElements[HUD_AMMO_BG].GetPosY() + SCREEN_BUFFER * 3.5, 0.0f, 0.5f, white);
	m_HUDElements[HUD_WEAPON_LVLBAR].Setup(m_Renderer,CAssetManager::GetInstance()->GetTexture(H_AMMO_BAR_FILL),
		RECTANGLE(0.0f,214.0f,0.0f,53.0f),
		m_HUDElements[HUD_AMMO].GetPosX() + 60.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_AMMO].GetPosY() + 50.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 0.5f, white);

	m_HUDElements[HUD_EXP_FRAME].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_FRAME),
		RECTANGLE(0.0f, 231.0f, 0.0f, 64.0f),
		m_HUDElements[HUD_WEAPON_LVLBAR].GetPosX(),
		m_HUDElements[HUD_WEAPON_LVLBAR].GetPosY(),
		0.0f, 0.5f, white);

	//m_HUDElements[HUD_HEALTH_LABEL].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_HEALTH_LABEL].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_HEALTH_LABEL].SetHeight(32.0f);
	//m_HUDElements[HUD_HEALTH_LABEL].SetWidth(128.0f);
	//m_HUDElements[HUD_HEALTH_LABEL].SetPosX(SCREEN_BUFFER);
	//m_HUDElements[HUD_HEALTH_LABEL].SetPosY(SCREEN_BUFFER);
	//m_HUDElements[HUD_HEALTH_LABEL].BuildGeometry();
	//m_HUDElements[HUD_HEALTH_LABEL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_LABEL));
	//m_HUDElements[HUD_HEALTH_LABEL].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_HEALTH_BAR_LABEL), 
														//RECTANGLE(0.0f, 128.0f, 0.0f, 32.0f), SCREEN_BUFFER, SCREEN_BUFFER, 0.0f, 1.0f, white);

	//m_HUDElements[HUD_WEAPON_TAB].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_WEAPON_TAB].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_WEAPON_TAB].SetHeight(225.0f);
	//m_HUDElements[HUD_WEAPON_TAB].SetWidth(252.0f);
	//m_HUDElements[HUD_WEAPON_TAB].SetPosX(_renderer->GetScreenWidth() - 15.0f);
	//m_HUDElements[HUD_WEAPON_TAB].SetPosY(_renderer->GetScreenHeight() - m_HUDElements[HUD_WEAPON_TAB].GetHeight());
	//m_HUDElements[HUD_WEAPON_TAB].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(WEAPON_LEVEL_TAB));
	//m_HUDElements[HUD_WEAPON_TAB].BuildGeometry();
	m_HUDElements[HUD_WEAPON_TAB].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(WEAPON_LEVEL_TAB),
		RECTANGLE(0.0f, 252.0f, 0.0f, 225.0f),
		m_Renderer->GetScreenWidth() - 15.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		m_Renderer->GetScreenHeight() - SCREEN_BUFFER - (225.0f - SCREEN_BUFFER) * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);
	m_ShowTab = false;

	//m_HUDElements[HUD_AMMO_LABEL].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_AMMO_LABEL].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_AMMO_LABEL].SetHeight(32.0f);
	//m_HUDElements[HUD_AMMO_LABEL].SetWidth(128.0f);
	//m_HUDElements[HUD_AMMO_LABEL].SetPosX(SCREEN_BUFFER);
	//m_HUDElements[HUD_AMMO_LABEL].SetPosY(SCREEN_BUFFER * 3);
	//m_HUDElements[HUD_AMMO_LABEL].BuildGeometry();
	//m_HUDElements[HUD_AMMO_LABEL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_AMMO_BAR_LABEL));
	m_HUDElements[HUD_AMMO_LABEL].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_AMMO_BAR_LABEL), RECTANGLE(0.0f, 128.0f, 0.0f, 32.0f),
		SCREEN_BUFFER * (m_Renderer->GetScreenHeight() / 720.0f),
		SCREEN_BUFFER * 3 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_HUDElements[HUD_AMMOTYPE].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_AMMOTYPE].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_AMMOTYPE].SetHeight(128.0f);
	//m_HUDElements[HUD_AMMOTYPE].SetWidth(128.0f);
	//m_HUDElements[HUD_AMMOTYPE].SetPosX((_renderer->GetScreenWidth() - m_HUDElements[HUD_AMMOTYPE].GetWidth() - SCREEN_BUFFER));
	//m_HUDElements[HUD_AMMOTYPE].SetPosY(SCREEN_BUFFER);
	//m_HUDElements[HUD_AMMOTYPE].BuildGeometry();
	//m_HUDElements[HUD_AMMOTYPE].GetGlBatch().Color4f(0.5f, 0.5f, 0.5f, 1.0f);
	//m_HUDElements[HUD_AMMOTYPE].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_EMPTY_GUN));
	m_HUDElements[HUD_AMMOTYPE].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_EMPTY_GUN),RECTANGLE(0.0f, 128.0f, 0.0f, 128.0f),
		m_HUDElements[HUD_MAINFRAME].GetPosX() + 38.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_MAINFRAME].GetPosY() + 38.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 0.6f, white);

	//m_HUDElements[HUD_WEAPON_CD].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_WEAPON_CD].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_WEAPON_CD].SetHeight(30.0f);
	//m_HUDElements[HUD_WEAPON_CD].SetWidth(253.0f);
	//m_HUDElements[HUD_WEAPON_CD].SetPosX(m_HUDElements[HUD_AMMO_BG].GetPosX());
	//m_HUDElements[HUD_WEAPON_CD].SetPosY(m_HUDElements[HUD_AMMO_BG].GetPosY());
	//m_HUDElements[HUD_WEAPON_CD].BuildGeometry();
	//m_HUDElements[HUD_WEAPON_CD].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(WEAPONS_COOLDOWN));
	m_HUDElements[HUD_WEAPON_CD].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(WEAPONS_COOLDOWN), RECTANGLE(0.0f, 253.0f, 0.0f, 30.0f),
		m_HUDElements[HUD_AMMO_BG].GetPosX(),
		m_HUDElements[HUD_AMMO_BG].GetPosY(),
		0.0f, 1.0f, white);

	// For Weapon Level mark

	//Vec3f weaponlvlbar_pos = {m_HUDElements[HUD_AMMO_BG].GetPosX() + 40, m_HUDElements[HUD_AMMO_BG].GetPosY() + SCREEN_BUFFER * 3, 0.0f};
	//m_HUDElements[HUD_WEAPON_LVLBAR].Setup(128.0f,16.0f,_renderer->GetScreenWidth(),_renderer->GetScreenHeight(),weaponlvlbar_pos,0.0f);
	//m_HUDElements[HUD_WEAPON_LVLBAR].BuildGeometry();
	//m_HUDElements[HUD_WEAPON_LVLBAR].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(ICON_WEAPON_LEVELBAR));

	/*m_HUDElements[HUD_WEAPON_LVLBAR].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(ICON_WEAPON_LEVELBAR), RECTANGLE(0.0f, 128.0f, 0.0f, 16.0f),
							weaponlvlbar_pos[0], weaponlvlbar_pos[1], 0.0f, 1.0f, white);*/

	//Vec3f weaponlvl_pos = {m_HUDElements[HUD_WEAPON_LVLBAR].GetPosX() + m_HUDElements[HUD_WEAPON_LVLBAR].GetRect().GetWidth() - 95, m_HUDElements[HUD_WEAPON_LVLBAR].GetPosY() - 75, 0.0f};
	/*m_HUDElements[HUD_WEAPON_LVL].Setup(128.0f,128.0f,_renderer->GetScreenWidth(),_renderer->GetScreenHeight(),weaponlvl_pos,0.0f);
	m_HUDElements[HUD_WEAPON_LVL].BuildGeometry();*/

	m_WeaponXPos = m_HUDElements[HUD_AMMO_BG].GetPosX() + m_HUDElements[HUD_AMMO_BG].GetRect().GetWidth() - 15.0f * (m_Renderer->GetScreenHeight() / 720.0f);



	//Added by awesome Tom
	/*m_HUDElements[WAVE_NOTICE].SetScreenHeight(_renderer->GetScreenHeight());d
	m_HUDElements[WAVE_NOTICE].SetScreenWidth(_renderer->GetScreenWidth());
	m_HUDElements[WAVE_NOTICE].SetHeight(128.0f);
	m_HUDElements[WAVE_NOTICE].SetWidth(256.0f);
	m_HUDElements[WAVE_NOTICE].SetPosX(400.0f);
	m_HUDElements[WAVE_NOTICE].SetPosY(200.0f);
	m_HUDElements[WAVE_NOTICE].BuildGeometry();
	m_HUDElements[WAVE_NOTICE].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_WAVE1));*/
	//m_HUDElements[WAVE_NOTICE].LoadTGAAsTextureRect("Assets/Textures/Wave1.tga");
	m_HUDElements[WAVE_NOTICE].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_WAVE1), RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
		400.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		200.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_HUDElements[HUD_BXP].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_BXP].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_BXP].SetHeight(23.0f);
	//m_HUDElements[HUD_BXP].SetWidth(0.0f);
	////143.0f
	//m_HUDElements[HUD_BXP].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f));
	//m_HUDElements[HUD_BXP].SetPosY(m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 106.0f);
	//m_HUDElements[HUD_BXP].BuildGeometry();
	//m_HUDElements[HUD_BXP].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_XPBAR));
	m_HUDElements[HUD_BXP].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_XPBAR), RECTANGLE(0.0f, 0.0f, 0.0f, 23.0f),
		(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 106.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_HUDElements[HUD_YXP].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_YXP].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_YXP].SetHeight(23.0f);
	//m_HUDElements[HUD_YXP].SetWidth(0.0f);
	//m_HUDElements[HUD_YXP].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f));
	//m_HUDElements[HUD_YXP].SetPosY(m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 31.0f);
	//m_HUDElements[HUD_YXP].BuildGeometry();
	//m_HUDElements[HUD_YXP].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_XPBAR));
	m_HUDElements[HUD_YXP].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_XPBAR), RECTANGLE(0.0f, 0.0f, 0.0f, 23.0f),
		(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 31.0f  * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_HUDElements[HUD_RXP].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_RXP].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_RXP].SetHeight(23.0f);
	//m_HUDElements[HUD_RXP].SetWidth(0.0f);
	//m_HUDElements[HUD_RXP].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f));
	//m_HUDElements[HUD_RXP].SetPosY(m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 178.0f);
	//m_HUDElements[HUD_RXP].BuildGeometry();
	//m_HUDElements[HUD_RXP].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_XPBAR));
	m_HUDElements[HUD_RXP].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_XPBAR), RECTANGLE(0.0f, 0.0f, 0.0f, 23.0f),
		(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 178.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//HUD_RLVL, HUD_BLVL, HUD_YLVL
	/*m_HUDElements[HUD_YLVL].SetScreenHeight(_renderer->GetScreenHeight());
	m_HUDElements[HUD_YLVL].SetScreenWidth(_renderer->GetScreenWidth());
	m_HUDElements[HUD_YLVL].SetHeight(70.0f);
	m_HUDElements[HUD_YLVL].SetWidth(82.0f);
	m_HUDElements[HUD_YLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
	m_HUDElements[HUD_YLVL].SetPosY(m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 1.0f);
	m_HUDElements[HUD_YLVL].BuildGeometry();
	m_HUDElements[HUD_YLVL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TAB_LVL1));*/
	m_HUDElements[HUD_YLVL].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_TAB_LVL1), RECTANGLE(0.0f, 168.0f, 0.0f, 43.0f),
		(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 55.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 0.5f, white);

	//m_HUDElements[HUD_BLVL].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_BLVL].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_BLVL].SetHeight(70.0f);
	//m_HUDElements[HUD_BLVL].SetWidth(82.0f);
	//m_HUDElements[HUD_BLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
	//m_HUDElements[HUD_BLVL].SetPosY(m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 70.0f);
	//m_HUDElements[HUD_BLVL].BuildGeometry();
	//m_HUDElements[HUD_BLVL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TAB_LVL1));
	m_HUDElements[HUD_BLVL].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_TAB_LVL1), RECTANGLE(0.0f, 168.0f, 0.0f, 43.0f),
		(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 125.0f * (m_Renderer->GetScreenHeight() / 720.0f) , 0.0f, 0.5f, white);

	//m_HUDElements[HUD_RLVL].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_RLVL].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_RLVL].SetHeight(70.0f);
	//m_HUDElements[HUD_RLVL].SetWidth(82.0f);
	//m_HUDElements[HUD_RLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
	//m_HUDElements[HUD_RLVL].SetPosY(m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 142.0f);
	//m_HUDElements[HUD_RLVL].BuildGeometry();
	//m_HUDElements[HUD_RLVL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_TAB_LVL1));
	m_HUDElements[HUD_RLVL].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_TAB_LVL1), RECTANGLE(0.0f, 168.0f, 0.0f, 43.0f),
		(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f * (m_Renderer->GetScreenHeight() / 720.0f)),
		m_HUDElements[HUD_WEAPON_TAB].GetPosY() + 195.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 0.5f, white);

	//m_HUDElements[HUD_WAVE_DISPLAY].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_WAVE_DISPLAY].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_WAVE_DISPLAY].SetHeight(32.0f);
	//m_HUDElements[HUD_WAVE_DISPLAY].SetWidth(128.0f);
	//m_HUDElements[HUD_WAVE_DISPLAY].SetPosX(0.0f);
	//m_HUDElements[HUD_WAVE_DISPLAY].SetPosY(128.0f);
	//m_HUDElements[HUD_WAVE_DISPLAY].BuildGeometry();
	//m_HUDElements[HUD_WAVE_DISPLAY].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(HUD_WAVE1_TEX));

	//m_HUDElements[HUD_LOADED].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_LOADED].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_LOADED].SetHeight(100.0f);
	//m_HUDElements[HUD_LOADED].SetWidth(100.0f);
	//m_HUDElements[HUD_LOADED].SetPosX((float)_renderer->GetScreenWidth() - 200.0f);
	//m_HUDElements[HUD_LOADED].SetPosY(0.0f);
	//m_HUDElements[HUD_LOADED].BuildGeometry();
	//m_HUDElements[HUD_LOADED].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_EMPTY_GUN));hhhhb

	//m_HUDElements[HUD_LIVES_LABEL].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_LIVES_LABEL].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_LIVES_LABEL].SetHeight(32);
	//m_HUDElements[HUD_LIVES_LABEL].SetWidth(128);
	//m_HUDElements[HUD_LIVES_LABEL].SetPosX(SCREEN_BUFFER);
	//m_HUDElements[HUD_LIVES_LABEL].SetPosY(SCREEN_BUFFER * 4);
	//m_HUDElements[HUD_LIVES_LABEL].BuildGeometry();
	//m_HUDElements[HUD_LIVES_LABEL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_LIVES_LABEL));
	m_HUDElements[HUD_LIVES_LABEL].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_LIVES_LABEL), RECTANGLE(0.0f, 128.0f, 0.0f, 32.0f),
		m_HUDElements[HUD_HEALTH_LABEL].GetPosX() * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_HEALTH_LABEL].GetPosY() + m_HUDElements[HUD_HEALTH_LABEL].GetRect().GetHeight() + 5 * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	//m_HUDElements[HUD_LIVES_TOKEN1].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_LIVES_TOKEN1].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_LIVES_TOKEN1].SetHeight(50);
	//m_HUDElements[HUD_LIVES_TOKEN1].SetWidth(50);
	////m_HUDElements[HUD_LIVES_TOKEN1].SetPosX(m_HUDElements[HUD_LIVES_LABEL].GetPosX() + m_HUDElements[HUD_LIVES_LABEL].GetWidth() - 40);
	//m_HUDElements[HUD_LIVES_TOKEN1].SetPosX(m_HUDElements[HUD_HP_BG].GetPosX());
	//m_HUDElements[HUD_LIVES_TOKEN1].SetPosY(m_HUDElements[HUD_LIVES_LABEL].GetPosY());
	//m_HUDElements[HUD_LIVES_TOKEN1].BuildGeometry();
	//m_HUDElements[HUD_LIVES_TOKEN1].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(LIVES_TOKENS));
	m_HUDElements[HUD_LIVES_TOKEN1].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(LIVES_TOKENS), RECTANGLE(0.0f, 50.0f, 0.0f, 50.0f),
		m_HUDElements[HUD_HP_BG].GetPosX() * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_HP_BG].GetPosY() + SCREEN_BUFFER * 3.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		0.0f, 1.0f, white);

	// Rotation Test
	//white[3] = 0.5f;
	//m_HUDElements[HUD_LIVES_TOKEN1].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(LIVES_TOKENS), RECTANGLE(0.0f, 50.0f, 0.0f, 50.0f),
	//					m_Renderer->GetScreenWidth() * 0.5f, m_Renderer->GetScreenHeight() * 0.5f, 180.0f, 1.0f, white);
	///white[3] = 1.0f;

	//m_HUDElements[HUD_LIVES_TOKEN2].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_LIVES_TOKEN2].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_LIVES_TOKEN2].SetHeight(50);
	//m_HUDElements[HUD_LIVES_TOKEN2].SetWidth(50);
	//m_HUDElements[HUD_LIVES_TOKEN2].SetPosX(m_HUDElements[HUD_LIVES_TOKEN1].GetPosX() + m_HUDElements[HUD_LIVES_TOKEN1].GetWidth());
	//m_HUDElements[HUD_LIVES_TOKEN2].SetPosY(m_HUDElements[HUD_LIVES_LABEL].GetPosY());
	//m_HUDElements[HUD_LIVES_TOKEN2].BuildGeometry();
	//m_HUDElements[HUD_LIVES_TOKEN2].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(LIVES_TOKENS));
	m_HUDElements[HUD_LIVES_TOKEN2].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(LIVES_TOKENS), RECTANGLE(0.0f, 50.0f, 0.0f, 50.0f),
		m_HUDElements[HUD_LIVES_TOKEN1].GetPosX() + m_HUDElements[HUD_LIVES_TOKEN1].GetRect().GetWidth() * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_LIVES_TOKEN1].GetPosY(), 0.0f, 1.0f, white);

	//m_HUDElements[HUD_LIVES_TOKEN3].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_LIVES_TOKEN3].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_LIVES_TOKEN3].SetHeight(50);
	//m_HUDElements[HUD_LIVES_TOKEN3].SetWidth(50);
	//m_HUDElements[HUD_LIVES_TOKEN3].SetPosX(m_HUDElements[HUD_LIVES_TOKEN2].GetPosX() + m_HUDElements[HUD_LIVES_TOKEN2].GetWidth());
	//m_HUDElements[HUD_LIVES_TOKEN3].SetPosY(m_HUDElements[HUD_LIVES_LABEL].GetPosY());
	//m_HUDElements[HUD_LIVES_TOKEN3].BuildGeometry();
	//m_HUDElements[HUD_LIVES_TOKEN3].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(LIVES_TOKENS));
	m_HUDElements[HUD_LIVES_TOKEN3].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(LIVES_TOKENS), RECTANGLE(0.0f, 50.0f, 0.0f, 50.0f),
		m_HUDElements[HUD_LIVES_TOKEN2].GetPosX() + m_HUDElements[HUD_LIVES_TOKEN2].GetRect().GetWidth() * (m_Renderer->GetScreenHeight() / 720.0f), m_HUDElements[HUD_LIVES_TOKEN1].GetPosY(), 0.0f, 1.0f, white);

	//m_HUDElements[HUD_ONE_DEPOT_LEFT].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_ONE_DEPOT_LEFT].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_ONE_DEPOT_LEFT].SetHeight(64);
	//m_HUDElements[HUD_ONE_DEPOT_LEFT].SetWidth(256);
	//m_HUDElements[HUD_ONE_DEPOT_LEFT].SetPosX(_renderer->GetScreenWidth() * 0.5f - 128);
	//m_HUDElements[HUD_ONE_DEPOT_LEFT].SetPosY(SCREEN_BUFFER *2);
	//m_HUDElements[HUD_ONE_DEPOT_LEFT].BuildGeometry();
	//m_HUDElements[HUD_ONE_DEPOT_LEFT].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(ONE_DEPOT_LEFT));
	m_HUDElements[HUD_ONE_DEPOT_LEFT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(ONE_DEPOT_LEFT), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		m_Renderer->GetScreenWidth() * 0.5f - 128.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		SCREEN_BUFFER * 2 * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);

	//m_HUDElements[HUD_PICKUPS].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_PICKUPS].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_PICKUPS].SetHeight(64);
	//m_HUDElements[HUD_PICKUPS].SetWidth(256);
	//m_HUDElements[HUD_PICKUPS].SetPosX((_renderer->GetScreenWidth() * 0.5f)-(m_HUDElements[HUD_PICKUPS].GetWidth() * 0.5f));
	//m_HUDElements[HUD_PICKUPS].SetPosY(_renderer->GetScreenHeight() * 0.1f);
	//m_HUDElements[HUD_PICKUPS].BuildGeometry();
	//m_HUDElements[HUD_PICKUPS].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_PICKUP_NOTIFY));
	m_HUDElements[HUD_PICKUPS].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_PICKUP_NOTIFY), RECTANGLE(0.0f, 256.0f, 0.0f, 64.0f),
		(m_Renderer->GetScreenWidth() * 0.5f) - 128.0f  * (m_Renderer->GetScreenHeight() / 720.0f),
		m_Renderer->GetScreenHeight() * 0.1f, 0.0f, 1.0f, white);

	//m_HUDElements[HUD_WAVE].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_WAVE].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_WAVE].SetHeight(128);
	//m_HUDElements[HUD_WAVE].SetWidth(256);
	//m_HUDElements[HUD_WAVE].SetPosX((float)_renderer->GetScreenWidth() * 0.5f - 200.0f);
	//m_HUDElements[HUD_WAVE].SetPosY((float)_renderer->GetScreenHeight() - 200.0f);
	//m_HUDElements[HUD_WAVE].BuildGeometry();
	//m_HUDElements[HUD_WAVE].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_WAVE));
	m_HUDElements[HUD_WAVE].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(H_WAVE), RECTANGLE(0.0f, 256.0f, 0.0f, 128.0f),
		(float)m_Renderer->GetScreenWidth() * 0.5f - 200.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		(float)m_Renderer->GetScreenHeight() - 200.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);


	/*m_HUDElements[HUD_WAVE_NUMBER_FIRST].SetScreenHeight(_renderer->GetScreenHeight());
	m_HUDElements[HUD_WAVE_NUMBER_FIRST].SetScreenWidth(_renderer->GetScreenWidth());
	m_HUDElements[HUD_WAVE_NUMBER_FIRST].SetHeight(128);
	m_HUDElements[HUD_WAVE_NUMBER_FIRST].SetWidth(128);
	m_HUDElements[HUD_WAVE_NUMBER_FIRST].SetPosX(m_HUDElements[HUD_WAVE].GetPosX() + m_HUDElements[HUD_WAVE].GetWidth());
	m_HUDElements[HUD_WAVE_NUMBER_FIRST].SetPosY(m_HUDElements[HUD_WAVE].GetPosY());
	m_HUDElements[HUD_WAVE_NUMBER_FIRST].BuildGeometry();
	m_HUDElements[HUD_WAVE_NUMBER_FIRST].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_ZERO));*/
	m_HUDElements[HUD_WAVE_NUMBER_FIRST].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(NUMBER_ZERO), RECTANGLE(0.0f, 128.0f, 0.0f, 128.0f),
		m_HUDElements[HUD_WAVE].GetPosX() + m_HUDElements[HUD_WAVE].GetRect().GetWidth() * (m_Renderer->GetScreenHeight() / 720.0f), m_HUDElements[HUD_WAVE].GetPosY(), 0.0f, 1.0f, white);

	//m_HUDElements[HUD_WAVE_NUMBER_SECOND].SetScreenHeight(_renderer->GetScreenHeight());
	//m_HUDElements[HUD_WAVE_NUMBER_SECOND].SetScreenWidth(_renderer->GetScreenWidth());
	//m_HUDElements[HUD_WAVE_NUMBER_SECOND].SetHeight(128);%
	//m_HUDElements[HUD_WAVE_NUMBER_SECOND].SetWidth(128);
	//m_HUDElements[HUD_WAVE_NUMBER_SECOND].SetPosX(m_HUDElements[HUD_WAVE_NUMBER_FIRST].GetPosX() + (m_HUDElements[HUD_WAVE_NUMBER_FIRST].GetWidth() * 0.5f)); //move the element that will display the ones over
	//m_HUDElements[HUD_WAVE_NUMBER_SECOND].SetPosY(m_HUDElements[HUD_WAVE].GetPosY());
	//m_HUDElements[HUD_WAVE_NUMBER_SECOND].BuildGeometry();
	//m_HUDElements[HUD_WAVE_NUMBER_SECOND].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_ONE));
	m_HUDElements[HUD_WAVE_NUMBER_SECOND].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(NUMBER_ONE), RECTANGLE(0.0f, 128.0f, 0.0f, 128.0f),
		m_HUDElements[HUD_WAVE_NUMBER_FIRST].GetPosX() + (m_HUDElements[HUD_WAVE_NUMBER_FIRST].GetRect().GetWidth() * 0.5f) * (m_Renderer->GetScreenHeight() / 720.0f), m_HUDElements[HUD_WAVE].GetPosY(), 0.0f, 1.0f, white);

	Vec4f black = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_HUDElements[HUD_PUSHCD_BG].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(PUSHCD_BG), RECTANGLE(0.0f, 164.0f, 0.0f, 173.0f),
		m_HUDElements[HUD_AMMOTYPE].GetPosX() - 15.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_AMMOTYPE].GetPosY() - 35.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, black);

	m_HUDElements[HUD_PUSHCD_BG2].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(PUSHCD_BG), RECTANGLE(0.0f, 164.0f, 0.0f, 173.0f),
		m_HUDElements[HUD_PUSHCD_BG].GetPosX(), m_HUDElements[HUD_PUSHCD_BG].GetPosY(), 180.0f, 1.0f, black);
	
	Vec4f green = { 0.3f, 1.0f, 0.3f, 1.0f };
	m_HUDElements[HUD_PUSHCD_FILL].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(PUSHCD_BG), RECTANGLE(0.0f, 164.0f, 0.0f, 173.0f),
		m_HUDElements[HUD_PUSHCD_BG].GetPosX() + 4.0f * (m_Renderer->GetScreenHeight() / 720.0f),
		m_HUDElements[HUD_PUSHCD_BG].GetPosY() + 6.0f * (m_Renderer->GetScreenHeight() / 720.0f), 180.0f, 0.8f, green);

	// Can't do this.  Make WaveNoticeElements into an array of pointers instead.
	//memcpy(&m_WaveNoticeElements[0],&m_HUDElements[HUD_WAVE],sizeof(CUIElement));
	//memcpy(&m_WaveNoticeElements[1],&m_HUDElements[HUD_WAVE_NUMBER_FIRST],sizeof(CUIElement));
	//memcpy(&m_WaveNoticeElements[2],&m_HUDElements[HUD_WAVE_NUMBER_SECOND],sizeof(CUIElement));

	m_WaveNoticeElements[0] = &m_HUDElements[HUD_WAVE];
	m_WaveNoticeElements[1] = &m_HUDElements[HUD_WAVE_NUMBER_FIRST];
	m_WaveNoticeElements[2] = &m_HUDElements[HUD_WAVE_NUMBER_SECOND];


	m_HUDElements[HUD_LEVELUP_TEXT].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(HUD_LEVELUP_TEXT_TEXTURE), RECTANGLE(0.0f, 478.0f, 0.0f, 65.0f),
		0.0f, SCREEN_BUFFER * 12.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);

	m_HUDElements[HUD_BLUE_LEVELUP_ICON].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(HUD_BLUE_LEVELUP_ICON_TEXTURE), RECTANGLE(0.0f, 128.0f, 0.0f, 128.0f),
		0.0f, SCREEN_BUFFER * 12.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);

	m_HUDElements[HUD_RED_LEVELUP_ICON].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(HUD_RED_LEVELUP_ICON_TEXTURE), RECTANGLE(0.0f, 128.0f, 0.0f, 128.0f),
		0.0f, SCREEN_BUFFER * 12.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);

	m_HUDElements[HUD_YELLOW_LEVELUP_ICON].Setup(m_Renderer, CAssetManager::GetInstance()->GetTexture(HUD_YELLOW_LEVELUP_ICON_TEXTURE), RECTANGLE(0.0f, 128.0f, 0.0f, 128.0f),
		0.0f, SCREEN_BUFFER * 12.0f * (m_Renderer->GetScreenHeight() / 720.0f), 0.0f, 1.0f, white);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CHUD::Release( void )
{
	for( unsigned int i = 0; i < TOTAL_HUD_ELEMENTS; ++i )
	{
		m_HUDElements[i].ReleaseGLBatch();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the HUD display information!
Parameters:
[in] _elapsedTime - the time passed since the last frame.
[in] _player - the player of ze game!
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CHUD::Update( float _elapsedTime, CPlayer* _player )
{
	m_PlayerHealth	= _player->GetHealth();
	m_PlayerAmmo	= (int)_player->GetGun().GetShotTotal();
	m_PlayerLives = _player->GetLives();
	m_PlayerAmmoType = _player->GetGun().GetConsumed();
	m_PlayerStored = _player->GetGun().GetStored();
	m_PlayerIsLoaded = _player->GetIsLoaded();

	m_PushCD = _player->GetPushTimer();
	m_PullCD = _player->GetPullTimer();

	float tempRatio1 = (float)m_PlayerHealth / (float)MAX_HP_PLAYER;
	m_HUDElements[HUD_HEALTH].GetRect().SetWidth(tempRatio1 * 214.0f);
	m_HUDElements[HUD_HEALTH].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	if(!m_PlayerIsLoaded)
	{
		m_HUDElements[HUD_LOADED].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_EMPTY_GUN));	
	}

	Vec4f blue,red,yellow;
	glsLoadVector4(blue,0.0f,0.0f,1.0f,1.0f);
	glsLoadVector4(red,1.0f,0.0f,0.0f,1.0f);
	glsLoadVector4(yellow,1.0f,1.0f,0.0f,1.0f);


	int maxAmmo = 0;
	switch(m_PlayerAmmoType)
	{
	case RED:
		{
			maxAmmo = (int)_player->GetGun().GetWeaponMaxAmmo();
			m_HUDElements[HUD_AMMOTYPE].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_PLASMA_BALL_ICON));
			//m_HUDElements[HUD_AMMOTYPE].
			m_HUDElements[HUD_AMMO].SetColor(red);	
		}
		break;
	case YELLOW:
		{
			maxAmmo = (int)_player->GetGun().GetWeaponMaxAmmo();
			m_HUDElements[HUD_AMMOTYPE].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_SHOTGUN_ICON));
			//m_HUDElements[HUD_AMMO].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_YELLOW_BAR_FILL));
			m_HUDElements[HUD_AMMO].SetColor(yellow);	
		}
		break;
	case BLUE:
		{
			maxAmmo = (int)_player->GetGun().GetWeaponMaxAmmo();
			m_HUDElements[HUD_AMMOTYPE].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_LASER_BEAM_ICON));
			//m_HUDElements[HUD_AMMO].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_BLUE_BAR_FILL));
			m_HUDElements[HUD_AMMO].SetColor(blue);	
		}
		break;
	case EMPTY:
		{
			maxAmmo = 0;
			m_HUDElements[HUD_AMMOTYPE].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(H_EMPTY_GUN));	
		}
		break;
	};

	float tempRatio2 = (maxAmmo == 0) ? 0 : (float)m_PlayerAmmo / (float)maxAmmo;
	m_HUDElements[HUD_AMMO].GetRect().SetWidth(tempRatio2 * (214.0f));
	//m_HUDElements[HUD_AMMO].SetPosX(m_WeaponXPos - m_HUDElements[HUD_AMMO].GetRect().GetWidth());
	//m_HUDElements[HUD_AMMO].SetPosX(((m_HUDElements[HUD_AMMO_BG].GetPosX() + m_HUDElements[HUD_AMMO_BG].GetRect().GetWidth()) - 60.0f) );// + m_HUDElements[HUD_AMMO_BG].GetRect().GetWidth() - m_HUDElements[HUD_AMMO].GetRect().GetWidth());
	m_HUDElements[HUD_AMMO].SetPosX(m_HUDElements[HUD_AMMO_BG].GetRect().GetWidth() * m_HUDElements[HUD_AMMO_BG].GetScale() + m_HUDElements[HUD_AMMO_BG].GetPosX() - (m_HUDElements[HUD_AMMO].GetRect().GetWidth() * m_HUDElements[HUD_AMMO].GetScale()) - 9.0f * (m_Renderer->GetScreenHeight() / 720.0f));
	m_HUDElements[HUD_AMMO].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	if(m_PlayerAmmoType == BLUE || m_PlayerAmmoType == YELLOW)
		m_HUDElements[HUD_WEAPON_CD].GetRect().SetWidth( 0.0f );
	else
		m_HUDElements[HUD_WEAPON_CD].GetRect().SetWidth( 214.0f * _player->GetGun().GetWeaponCD() );

	m_HUDElements[HUD_WEAPON_CD].SetPosX(m_WeaponXPos - m_HUDElements[HUD_WEAPON_CD].GetRect().GetWidth());


	m_HUDElements[HUD_WEAPON_CD].SetOpacity(0.1f);
	m_HUDElements[HUD_WEAPON_CD].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	float tempRatio3 = (float)m_PlayerLives / (float)MAX_LIVES_PLAYER;
	m_HUDElements[HUD_LIVES].GetRect().SetWidth(tempRatio3 * 200.0f);
	m_HUDElements[HUD_LIVES].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	
	if(m_ShowTab)
	{
		if(m_HUDElements[HUD_WEAPON_TAB].GetPosX() > m_Renderer->GetScreenWidth() - m_HUDElements[HUD_WEAPON_TAB].GetRect().GetWidth() * (m_Renderer->GetScreenHeight() / 720.0f))
		{
			//143.0f
			float newPos = m_HUDElements[HUD_WEAPON_TAB].GetPosX() - 3000.0f * _elapsedTime;
			m_HUDElements[HUD_WEAPON_TAB].SetPosX(newPos);
			m_HUDElements[HUD_BXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));
			m_HUDElements[HUD_RXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));
			m_HUDElements[HUD_YXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));

			m_HUDElements[HUD_YLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
			m_HUDElements[HUD_BLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
			m_HUDElements[HUD_RLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
		}
		else
		{
			m_HUDElements[HUD_WEAPON_TAB].SetPosX(m_Renderer->GetScreenWidth() - m_HUDElements[HUD_WEAPON_TAB].GetRect().GetWidth() * (m_Renderer->GetScreenHeight() / 720.0f) - SCREEN_BUFFER);
			m_HUDElements[HUD_BXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));
			m_HUDElements[HUD_RXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));
			m_HUDElements[HUD_YXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));

			m_HUDElements[HUD_YLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
			m_HUDElements[HUD_BLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
			m_HUDElements[HUD_RLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
		}

		//m_ShowTab = false;
	}
	else
	{
		if(m_HUDElements[HUD_WEAPON_TAB].GetPosX() < m_Renderer->GetScreenWidth() - 15.0f  * (m_Renderer->GetScreenHeight() / 720.0f))
		{
			float newPos = m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 3000.0f * _elapsedTime;
			m_HUDElements[HUD_WEAPON_TAB].SetPosX(newPos);
			m_HUDElements[HUD_BXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));
			m_HUDElements[HUD_RXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));
			m_HUDElements[HUD_YXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));

			m_HUDElements[HUD_YLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
			m_HUDElements[HUD_BLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
			m_HUDElements[HUD_RLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
		}
		else
		{
			m_HUDElements[HUD_WEAPON_TAB].SetPosX( m_Renderer->GetScreenWidth() - 15.0f );
			m_HUDElements[HUD_BXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));
			m_HUDElements[HUD_RXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));
			m_HUDElements[HUD_YXP].SetPosX(m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 92.0f * (m_Renderer->GetScreenHeight() / 720.0f));

			m_HUDElements[HUD_YLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
			m_HUDElements[HUD_BLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
			m_HUDElements[HUD_RLVL].SetPosX((m_HUDElements[HUD_WEAPON_TAB].GetPosX() + 14.0f));
		}
	}

	//BLUE
	float CurrentXP = (float)_player->GetGun().GetExperience(BLUE);
	float NeededXP = (float)_player->GetGun().GetNeededExperience(BLUE);
	float XPRatio = CurrentXP / NeededXP;
		
	if( _player->GetGun().GetWeaponLevel(BLUE) >= 3 )
		m_HUDElements[HUD_BXP].GetRect().SetWidth(128.0f);
	else
		m_HUDElements[HUD_BXP].GetRect().SetWidth(XPRatio * 128.0f);

	//RED
	CurrentXP = (float)_player->GetGun().GetExperience(RED);
	NeededXP = (float)_player->GetGun().GetNeededExperience(RED);
	XPRatio = CurrentXP / NeededXP;
		
	if( _player->GetGun().GetWeaponLevel(RED) >= 3 )
		m_HUDElements[HUD_RXP].GetRect().SetWidth(128.0f);
	else
		m_HUDElements[HUD_RXP].GetRect().SetWidth(XPRatio * 128.0f);

	// YELLOW
	CurrentXP = (float)_player->GetGun().GetExperience(YELLOW);
	NeededXP = (float)_player->GetGun().GetNeededExperience(YELLOW);
	XPRatio = CurrentXP / NeededXP;
		
	if( _player->GetGun().GetWeaponLevel(YELLOW) >= 3 )
		m_HUDElements[HUD_YXP].GetRect().SetWidth(128.0f);
	else
		m_HUDElements[HUD_YXP].GetRect().SetWidth(XPRatio * 128.0f);

	m_HUDElements[HUD_WEAPON_TAB].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	m_HUDElements[HUD_BXP].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	m_HUDElements[HUD_RXP].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	m_HUDElements[HUD_YXP].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	int lvl = _player->GetGun().GetWeaponLevel(YELLOW);

	int hLvl1 = CAssetManager::GetInstance()->GetTexture(H_TAB_LVL1);
	int hLvl2 = CAssetManager::GetInstance()->GetTexture(H_TAB_LVL2);
	int hLvl3 = CAssetManager::GetInstance()->GetTexture(H_TAB_LVL3);

	if(lvl == 1)
		m_HUDElements[HUD_YLVL].SetTextureHandle(hLvl1);
	else if( lvl == 2 )
		m_HUDElements[HUD_YLVL].SetTextureHandle(hLvl2);
	else
		m_HUDElements[HUD_YLVL].SetTextureHandle(hLvl3);

	lvl = _player->GetGun().GetWeaponLevel(BLUE);
	if(lvl == 1)
		m_HUDElements[HUD_BLVL].SetTextureHandle(hLvl1);
	else if( lvl == 2 )
		m_HUDElements[HUD_BLVL].SetTextureHandle(hLvl2);
	else
		m_HUDElements[HUD_BLVL].SetTextureHandle(hLvl3);

	lvl = _player->GetGun().GetWeaponLevel(RED);
	if(lvl == 1)
		m_HUDElements[HUD_RLVL].SetTextureHandle(hLvl1);
	else if( lvl == 2 )
		m_HUDElements[HUD_RLVL].SetTextureHandle(hLvl2);
	else
		m_HUDElements[HUD_RLVL].SetTextureHandle(hLvl3);

	m_HUDElements[HUD_YLVL].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	m_HUDElements[HUD_BLVL].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	m_HUDElements[HUD_RLVL].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	
	float PushRatio = m_PushCD / PUSH_COOLDOWN;
	m_HUDElements[HUD_PUSHCD_FILL].SetRotatation(180.0f - 180.0f * PushRatio);
	m_HUDElements[HUD_PUSHCD_FILL].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());

	if(m_FadeHUD)
	{
		for(int i = 0; i < TOTAL_HUD_ELEMENTS; i++)
		{
			m_HUDElements[i].SetOpacity(*m_HUDElements[i].GetOpacity() - (0.3f * _elapsedTime));
		}
		return;
	}

	if(_player->GetGun().GetConsumed() != EMPTY)
	{
		if(_player->GetGun().GetWeaponLevel() == 1)
			m_HUDElements[HUD_WEAPON_LVL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(ICON_WEAPON_LEVEL1));
		else if(_player->GetGun().GetWeaponLevel() == 2)
			m_HUDElements[HUD_WEAPON_LVL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(ICON_WEAPON_LEVEL2));
		else if(_player->GetGun().GetWeaponLevel() == 3)
			m_HUDElements[HUD_WEAPON_LVL].SetTextureHandle(CAssetManager::GetInstance()->GetTexture(ICON_WEAPON_LEVEL3));

		float curr = (float)_player->GetGun().GetExperience();
		float need = (float)_player->GetGun().GetNeededExperience();
		float tempRatiolvl = curr / need;
		
		if( _player->GetGun().GetWeaponLevel() >= 3 )
			m_HUDElements[HUD_WEAPON_LVLBAR].GetRect().SetWidth(214.0f);
		else
			m_HUDElements[HUD_WEAPON_LVLBAR].GetRect().SetWidth(tempRatiolvl * 214.0f);
		m_HUDElements[HUD_WEAPON_LVLBAR].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	}

	for(int i = 0; i < TOTAL_HUD_ELEMENTS; i++)
	{
		m_HUDElements[i].SetOpacity(1.0f);
	}

	static bool lowHP;
	if(this->m_PlayerHealth <= 25)
	{
		
		if(lowHP)
		{
			m_HUDElements[HUD_HEALTH].SetOpacity(0.1f);
			lowHP = false;
		}
		else
		{
			m_HUDElements[HUD_HEALTH].SetOpacity(1.0f);
			lowHP = true;
		}
		//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_BEEP);
	}


	if(m_PlayLevelUpSeq)
	{
		PlayLevelUpSequence(_elapsedTime);
	}

}
/*////////////////////////////////////////////////////////////
/	This function takes an unsigned int for the exact wave
/	number that you want to display to the screen.
/	For example; if you ChangeWaveNotice(2)  "Wave 2" 
/	will be rendered to the screen when that UI element is
/	rendered
/*/
void CHUD::ChangeWaveNotice(unsigned int _newWaveNum)
{
//	m_WaveDisplayVec.clear();
	//Add "WAVE" to be displayed to the screen
	//m_WaveDisplayVec.push_back(&m_HUDElements[HUD_WAVE]);

	//find out if the number has one or two places 1->99
	if(!(_newWaveNum/10)) //Truncates so 1->9 = 0
	{
		m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_ZERO));
#pragma region Switch_Statement_for_Ones
		//Waves 1->9
		switch(_newWaveNum)
		{
		case 1:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_ONE));
			}
			break;
		case 2:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_TWO));
			}
			break;
		case 3:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_THREE));
			}
			break;
		case 4:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_FOUR));
			}
			break;
		case 5:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_FIVE));
			}
			break;
		case 6:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_SIX));
			}
			break;
		case 7:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_SEVEN));
			}
			break;
		case 8:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_EIGHT));
			}
			break;
		case 9:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_NINE));
			}
			break;
		};
#pragma endregion

		//Add the updated Ones number to the vector of elements to be displayed
		//m_HUDElements[HUD_WAVE_NUMBER_FIRST].BuildGeometry();
		//m_WaveDisplayVec.push_back(&m_HUDElements[HUD_WAVE_NUMBER_FIRST]);
	}
	else // this means the number is 10 or greater.  We will not have more than 99 waves... dun...dun... DUN!
	{
#pragma region Switch_Statement_for_Tens
		//Waves 1->9
		switch(_newWaveNum/10) // this will give you the number in the tens place
		{
		case 1:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_ONE));
			}
			break;
		case 2:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_TWO));
			}
			break;
		case 3:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_THREE));
			}
			break;
		case 4:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_FOUR));
			}
			break;
		case 5:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_FIVE));
			}
			break;
		case 6:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_SIX));
			}
			break;
		case 7:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_SEVEN));
			}
			break;
		case 8:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_EIGHT));
			}
			break;
		case 9:
			{
				m_WaveNoticeElements[1]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_NINE));
			}
			break;
		};
#pragma endregion
	

		//this will update and move the position over for the ones place
#pragma region Switch_Statement_for_Tens
		//Waves 1->9
		switch(_newWaveNum%10) // this will give you the number in the tens place
		{
		case 1:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_ONE));
			}
			break;
		case 2:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_TWO));
			}
			break;
		case 3:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_THREE));
			}
			break;
		case 4:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_FOUR));
			}
			break;
		case 5:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_FIVE));
			}
			break;
		case 6:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_SIX));
			}
			break;
		case 7:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_SEVEN));
			}
			break;
		case 8:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_EIGHT));
			}
			break;
		case 9:
			{
				m_WaveNoticeElements[2]->SetTextureHandle(CAssetManager::GetInstance()->GetTexture(NUMBER_NINE));
			}
			break;
		};
#pragma endregion
		//m_WaveDisplayVec.push_back(&m_HUDElements[HUD_WAVE_NUMBER_SECOND]);																	

	}
}

void CHUD::PlayLevelUpSequence(float _fElapsedTime)
{
	if(!m_playThisOnlyOnce)
	{

		switch(m_weaponLevel)
		{

		case 2:
			{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_GUN_LEVEL_UP);	

			}
			break;
		case 3:
			{
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_GUN_LEVEL_UP_ROCK);	
			}
			break;
		};

		m_playThisOnlyOnce = true;
	}

	m_levelUpSeqTimer += _fElapsedTime * 4.0f ;
	if(m_levelUpSeqTimer > 5.0f)
	{
		ResetLevelUpSequence();
	}
	float tempPos = m_HUDElements[HUD_LEVELUP_TEXT].GetPosX();
	tempPos += _fElapsedTime * 7500.0f;

	if(m_PauseMoveTimer <= 0.0f || m_PauseMoveTimer >= 2.5f)
	{
		m_HUDElements[HUD_LEVELUP_TEXT].SetPosX(tempPos);
		m_HUDElements[HUD_LEVELUP_TEXT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	}

	if(m_PauseMoveTimer > 0.0f && m_PauseMoveTimer <= 1.5f)
	{
		m_HUDElements[HUD_LEVELUP_TEXT].SetPosX(m_Renderer->GetScreenWidth()*0.5f -  m_HUDElements[HUD_LEVELUP_TEXT].GetRect().GetWidth()*0.5f);
		m_HUDElements[HUD_LEVELUP_TEXT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
	}

	if(m_HUDElements[HUD_LEVELUP_TEXT].GetPosX() >= m_Renderer->GetScreenWidth()*0.5f - m_HUDElements[HUD_LEVELUP_TEXT].GetRect().GetWidth()*0.5f)
	{
		m_PauseMoveTimer += _fElapsedTime *4.0f;
	}

}
void CHUD::ResetLevelUpSequence()
{
	m_playThisOnlyOnce = false;
	m_PlayLevelUpSeq = false;
	m_levelUpSeqTimer = 0.0f;
	m_PauseMoveTimer = 0.0f;
	m_whichWeapon = -1;
	m_weaponLevel = -1;
	m_HUDElements[HUD_LEVELUP_TEXT].SetPosX(0.0f);
	m_HUDElements[HUD_LEVELUP_TEXT].BuildGeometry(m_Renderer->GetScreenWidth(), m_Renderer->GetScreenHeight());
}

