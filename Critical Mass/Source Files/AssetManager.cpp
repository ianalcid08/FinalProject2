/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : AssetManager.h
Author : Evan Wright

Description : This module will be responsible for loading and containing texture and 
			  shaders assets as well as animations and meshes.  It will contain an array 
			  of data for every game object where assets must be loaded in. All of the 
			  assets are loaded at the beginning of the game in order to cut down or even 
			  eliminate long loading times during gameplay.  Accessors will be available 
			  to access all the elements in an array of object data and will use an int 
			  ID to get the correct index for data.

Created :  07/18/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/AssetManager.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CAssetManager::CAssetManager( void )
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CAssetManager::~CAssetManager( void )
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Gets an instance to the singleton.
Returns: The instance of the singleton.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CAssetManager* CAssetManager::GetInstance( void )
{
	static CAssetManager instance;
	return &instance;
}
void CAssetManager::Init()
{
	LoadShaders();
	LoadTriangleBatches();

	m_ObjectDataTotal[ET_PLAYER].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/Player_Texture.tga");
	m_ObjectDataTotal[ET_PLAYER].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/test_spec.tga");
	m_ObjectDataTotal[ET_PLAYER].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/test_emissive.tga");
	m_ObjectDataTotal[ET_PLAYER].m_Animations[PL_WALK].LoadAnimFromFile("Assets/Models/Player_Anim_Walk.bin", PL_WALK, true);
	m_ObjectDataTotal[ET_PLAYER].m_Animations[PL_IDLE].LoadAnimFromFile("Assets/Models/Player_Anim_Idle.bin", PL_IDLE, true);
	m_ObjectDataTotal[ET_PLAYER].m_Animations[PL_DEATH].LoadAnimFromFile("Assets/Models/Player_Anim_Death.bin", PL_DEATH, true);
	m_ObjectDataTotal[ET_PLAYER].m_Animations[PL_HURT].LoadAnimFromFile("Assets/Models/Player_Anim_Hurt.bin", PL_HURT, false);
	m_ObjectDataTotal[ET_PLAYER].m_Animations[PL_LOSE].LoadAnimFromFile("Assets/Models/Player_Anim_Lose.bin", PL_LOSE, false);

	//m_ObjectDataTotal[ET_BULLET_PLAYER].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/TestCube.tga");
	m_ObjectDataTotal[ET_BULLET_PLAYER].m_hTextureSpecular = m_ObjectDataTotal[ET_BULLET_PLAYER].m_hTextureEmissive;
	m_ObjectDataTotal[ET_BULLET_PLAYER].m_hTextureEmissive = m_ObjectDataTotal[ET_PLAYER].m_hTextureEmissive;
	
	m_ObjectDataTotal[ET_WORLD].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/moon_Diffuse.tga", GL_REPEAT );
	m_ObjectDataTotal[ET_WORLD].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/moon_SPEC.tga");
	m_ObjectDataTotal[ET_WORLD].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/null_texture.tga");
	//m_ObjectDataTotal[ET_WORLD].m_hTextureNormal = LoadTGAAsTexture("Assets/Textures/test_normals.tga");

	m_ObjectDataTotal[ET_PLANET].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/planet_d.tga", GL_REPEAT );
	m_ObjectDataTotal[ET_PLANET].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/planet_s.tga");

	/////////////////////////////// TESTING COMET & PLANET & ASTEROID /////////////////////////////////////
	//m_ObjectDataTotal[ET_WORLD].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/Comet_Diffuse.tga", GL_REPEAT );
	//m_ObjectDataTotal[ET_WORLD].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/Comet_SPEC.tga");
	//m_ObjectDataTotal[ET_WORLD].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/Comet_Emissive.tga");

	//m_ObjectDataTotal[ET_WORLD].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/moon_Diffuse.tga", GL_REPEAT );
	//m_ObjectDataTotal[ET_WORLD].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/moon_SPEC.tga");
	//m_ObjectDataTotal[ET_WORLD].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/null_texture.tga");
	//
	//m_ObjectDataTotal[ET_WORLD].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/moon_Diffuse.tga", GL_REPEAT );
	//m_ObjectDataTotal[ET_WORLD].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/moon_SPEC.tga");
	//m_ObjectDataTotal[ET_WORLD].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/null_texture.tga");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_ObjectDataTotal[ET_HEALTH_PICKUP_SMALL].m_DiffuseHandle =		LoadTGAAsTexture("Assets/Textures/Health_Model_D.tga");
	m_ObjectDataTotal[ET_HEALTH_PICKUP_SMALL].m_hTextureSpecular =  LoadTGAAsTexture("Assets/Textures/Health_Model_S.tga");
	m_ObjectDataTotal[ET_HEALTH_PICKUP_SMALL].m_hTextureEmissive =  LoadTGAAsTexture("Assets/Textures/Health_Model_Em.tga");//m_ObjectDataTotal[ET_PLAYER].m_hTextureEmissive;

	m_ObjectDataTotal[ET_HEALTH_PICKUP_LARGE].m_DiffuseHandle =		LoadTGAAsTexture("Assets/Textures/Health_Model_D.tga");
	m_ObjectDataTotal[ET_HEALTH_PICKUP_LARGE].m_hTextureSpecular =	LoadTGAAsTexture("Assets/Textures/Health_Model_S.tga");
	m_ObjectDataTotal[ET_HEALTH_PICKUP_LARGE].m_hTextureEmissive =	LoadTGAAsTexture("Assets/Textures/Health_Model_Em.tga");

	//m_ObjectDataTotal[ET_DEPOT].m_ModelData.LoadMeshFromFile("Assets/Models/MiningStationShape.mesh");
	m_ObjectDataTotal[ET_DEPOT].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/Depot_D.tga");
	m_ObjectDataTotal[ET_DEPOT].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/Depot_S.tga");
	m_ObjectDataTotal[ET_DEPOT].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/Depot_E.tga");
	m_ObjectDataTotal[ET_DEPOT].m_Animations[DepotAnim_Activate].LoadAnimFromFile("Assets/Models/depot_anim_activate.bin",DepotAnim_Activate, false);
	m_ObjectDataTotal[ET_DEPOT].m_Animations[DepotAnim_Deactivate].LoadAnimFromFile("Assets/Models/depot_anim_deactivate.bin",DepotAnim_Deactivate, false);
	m_ObjectDataTotal[ET_DEPOT].m_Animations[DepotAnim_Deactivated_Idle].LoadAnimFromFile("Assets/Models/depot_anim_deactivated_idle.bin",DepotAnim_Deactivated_Idle,false);

	////////// LANDING PADS (NEED TO CHANGE FOR NEW ASSET) ///////////////////
	m_ObjectDataTotal[ET_LANDINGPAD].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/LandingPad_D.tga" );
	m_ObjectDataTotal[ET_LANDINGPAD].m_hTextureSpecular = m_ObjectDataTotal[ET_PLAYER].m_hTextureSpecular;
	m_ObjectDataTotal[ET_LANDINGPAD].m_hTextureEmissive = m_ObjectDataTotal[ET_PLAYER].m_hTextureEmissive;

	//m_ObjectDataTotal[ET_TEST_OBJ].m_ModelData.LoadMeshFromFile("Assets/Models/test_boxShape.mesh");
	//m_ObjectDataTotal[ET_TEST_OBJ].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/TestCube.tga");
	//m_ObjectDataTotal[ET_TEST_OBJ].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/test_spec.tga");
	//m_ObjectDataTotal[ET_TEST_OBJ].m_hTextureEmissive = m_ObjectDataTotal[ET_PLAYER].m_hTextureEmissive;

	m_ObjectDataTotal[ET_EXPLOSION].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/Explosion.tga", GL_REPEAT);

	m_ObjectDataTotal[ET_MORTAR].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/NuclearBomb_Model_Diffuse.tga");
	m_ObjectDataTotal[ET_MORTAR].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/NuclearBomb_Model_Specular.tga");
	
	m_ObjectDataTotal[ET_SHIELD].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/GreyScaleShield.tga");
	// This is specific to the shield.  Since it has no spec, we're using that handle to hold its
	// dissolve factor texture.
	m_ObjectDataTotal[ET_SHIELD].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/dissolve_noise.tga");
	m_ObjectDataTotal[ET_SHIELD].m_hTextureEmissive = m_ObjectDataTotal[ET_PLAYER].m_hTextureEmissive;

	//m_ObjectDataTotal[ET_PULL].m_ModelData.LoadMeshFromFile("Assets/Models/PullCone.mesh");
	m_ObjectDataTotal[ET_PULL].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/dissolve_noise.tga", GL_REPEAT);

	//m_ObjectDataTotal[ET_LASER].m_ModelData.LoadMeshFromFile("Assets/Models/PH_LaserShape.mesh");
	m_ObjectDataTotal[ET_LASER].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/dissolve_noise.tga", GL_REPEAT);

	m_ObjectDataTotal[ET_MEDIUMENEMY].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/MediumEnemy_D.tga");
	m_ObjectDataTotal[ET_MEDIUMENEMY].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/MediumEnemy_S.tga");
	m_ObjectDataTotal[ET_MEDIUMENEMY].m_Animations[MEDIUM_MOVE_FORWARD].LoadAnimFromFile("Assets/Models/MediumEnemy_Anim_Move.bin", MEDIUM_MOVE_FORWARD, true);
	m_ObjectDataTotal[ET_MEDIUMENEMY].m_Animations[MEDIUM_MOVE_LEFT].LoadAnimFromFile("Assets/Models/MediumEnemy_Anim_MoveLeft.bin", MEDIUM_MOVE_LEFT,true);
	m_ObjectDataTotal[ET_MEDIUMENEMY].m_Animations[MEDIUM_MOVE_RIGHT].LoadAnimFromFile("Assets/Models/MediumEnemy_Anim_MoveRight.bin", MEDIUM_MOVE_RIGHT,true);
	m_ObjectDataTotal[ET_MEDIUMENEMY].m_Animations[MEDIUM_MOVE_BACKWARDS].LoadAnimFromFile("Assets/Models/MediumEnemy_Anim_MoveBack.bin", MEDIUM_MOVE_BACKWARDS,true);
	m_ObjectDataTotal[ET_MEDIUMENEMY].m_Animations[MEDIUM_HURT].LoadAnimFromFile("Assets/Models/MediumEnemy_Anim_Hurt.bin", MEDIUM_HURT,false);
	m_ObjectDataTotal[ET_MEDIUMENEMY].m_Animations[MEDIUM_FIREPLASMA].LoadAnimFromFile("Assets/Models/MediumEnemy_Anim_Plasma.bin", MEDIUM_FIREPLASMA,false);
	m_ObjectDataTotal[ET_MEDIUMENEMY].m_Animations[MEDIUM_FIREMORTAR].LoadAnimFromFile("Assets/Models/MediumEnemy_Anim_Mortar.bin", MEDIUM_FIREMORTAR,false);

	m_ObjectDataTotal[ET_LARGEENEMY].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/LargeEnemy_D.tga");
	m_ObjectDataTotal[ET_LARGEENEMY].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/LargeEnemy_S.tga");
	m_ObjectDataTotal[ET_LARGEENEMY].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/LargeEnemy_E.tga");
	m_ObjectDataTotal[ET_LARGEENEMY].m_Animations[LARGE_IDLE].LoadAnimFromFile("Assets/Models/LargeEnemy_Anim_Idle.bin", LARGE_IDLE, true);
	m_ObjectDataTotal[ET_LARGEENEMY].m_Animations[LARGE_DEATH].LoadAnimFromFile("Assets/Models/LargeEnemy_Anim_Death.bin", LARGE_DEATH, false);
	m_ObjectDataTotal[ET_LARGEENEMY].m_Animations[LARGE_ALPHA].LoadAnimFromFile("Assets/Models/LargeEnemy_Anim_Alpha.bin", LARGE_ALPHA, true);	

	m_ObjectDataTotal[ET_SMALLENEMY].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/Small_Enemy_Diffuse.tga");
	m_ObjectDataTotal[ET_SMALLENEMY].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/Small_Enemy_Specular.tga");
	m_ObjectDataTotal[ET_SMALLENEMY].m_hTextureEmissive = m_ObjectDataTotal[ET_PLAYER].m_hTextureEmissive;

	m_ObjectDataTotal[ET_BOSSENEMY].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/BossEnemy_D.tga");
	m_ObjectDataTotal[ET_BOSSENEMY].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/LargeEnemy_S.tga");
	m_ObjectDataTotal[ET_BOSSENEMY].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/LargeEnemy_E.tga");
	m_ObjectDataTotal[ET_BOSSENEMY].m_Animations[LARGE_IDLE].LoadAnimFromFile("Assets/Models/LargeEnemy_Anim_Idle.bin", LARGE_IDLE, true);
	m_ObjectDataTotal[ET_BOSSENEMY].m_Animations[LARGE_DEATH].LoadAnimFromFile("Assets/Models/LargeEnemy_Anim_Death.bin", LARGE_DEATH, false);
	m_ObjectDataTotal[ET_BOSSENEMY].m_Animations[LARGE_ALPHA].LoadAnimFromFile("Assets/Models/LargeEnemy_Anim_Alpha.bin", LARGE_ALPHA, true);

	// FOR SMALL ENEMY
	m_SmallEnemyData[BLACK].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/SmallEnemy_Black.tga");
	m_SmallEnemyData[RED].m_DiffuseHandle = m_SmallEnemyData[BLACK].m_DiffuseHandle;
	m_SmallEnemyData[YELLOW].m_DiffuseHandle = m_SmallEnemyData[BLACK].m_DiffuseHandle;
	m_SmallEnemyData[BLUE].m_DiffuseHandle = m_SmallEnemyData[BLACK].m_DiffuseHandle;

	m_SmallEnemyData[BLACK].m_hTextureSpecular = m_ObjectDataTotal[ET_PLAYER].m_hTextureSpecular;
	m_SmallEnemyData[RED].m_hTextureSpecular = m_SmallEnemyData[BLACK].m_hTextureSpecular;
	m_SmallEnemyData[YELLOW].m_hTextureSpecular = m_SmallEnemyData[BLACK].m_hTextureSpecular;
	m_SmallEnemyData[BLUE].m_hTextureSpecular = m_SmallEnemyData[BLACK].m_hTextureSpecular;

	m_SmallEnemyData[BLACK].m_hTextureEmissive = m_ObjectDataTotal[ET_PLAYER].m_hTextureEmissive;
	m_SmallEnemyData[RED].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/test_emissive_red.tga");
	m_SmallEnemyData[YELLOW].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/test_emissive_yellow.tga");
	m_SmallEnemyData[BLUE].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/test_emissive_blue.tga");

	/////// NEW STUFFS ////////
	m_ObjectDataTotal[ET_REDBARREL].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/RedBarrel_Diffuse.tga");
	m_ObjectDataTotal[ET_REDBARREL].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/RedBarrel_Specular.tga");
	//m_ObjectDataTotal[ET_REDBARREL].m_hTextureNormal =LoadTGAAsTexture("Assets/Textures/RedBarrel_Normal.tga");
	// Crate
	//m_ObjectDataTotal[ET_CRATE].m_ModelData.LoadMeshFromFile("Assets/Models/Crate.mesh");
	m_ObjectDataTotal[ET_CRATE].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/crate_diffuse_map.tga");
	m_ObjectDataTotal[ET_CRATE].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/crate_specular_map.tga");
	m_ObjectDataTotal[ET_CRATE].m_hTextureEmissive =LoadTGAAsTexture("Assets/Textures/crate_emissive_map.tga");

	m_ObjectDataTotal[ET_EMP].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/EMP_Explosion.tga");

	//m_ObjectDataTotal[ET_LIGHT_POLE].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/light_pole.tga");
	m_ObjectDataTotal[ET_LIGHT_POLE].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/light_pole.tga");
	m_ObjectDataTotal[ET_LIGHT_POLE].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/LightPost_Model_E.tga");
	m_ObjectDataTotal[ET_LIGHT_POLE].m_hTextureSpecular = LoadTGAAsTexture("Assets/Textures/LightPost_Model_S.tga");

	m_ObjectDataTotal[ET_SUN].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/sun_d.tga");
	m_ObjectDataTotal[ET_ASTEROID].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/ufo_d.tga");
	m_ObjectDataTotal[ET_ASTEROID].m_hTextureEmissive = LoadTGAAsTexture("Assets/Textures/ufo_e.tga");

	//MENU BACKGROUNDS
#if defined(_DEBUG) || defined(_BETA)
	m_Textures[M_WIN_BACKGROUND_1024x768] = LoadTGAAsTextureRect("Assets/Textures/WM_BACKGROUND_1024x768.tga");
	m_Textures[M_LOSE_BACKGROUND_1024x768] = LoadTGAAsTextureRect("Assets/Textures/LM_BACKGROUND_1024x768.tga");
	m_Textures[M_MAIN_BACKGROUND_1024x768] = LoadTGAAsTextureRect("Assets/Textures/MM_BACKGROUND_1024x768.tga");
	m_Textures[M_OPTIONS_BACKGROUND_1024x768] = LoadTGAAsTextureRect("Assets/Textures/OM_BACKGROUND_1024x768.tga");
	m_Textures[M_DIFFICULTY_BACKGROUND_1024x768] = LoadTGAAsTextureRect("Assets/Textures/DM_BACKGROUND_1024x768.tga");
	m_Textures[M_CREDITS_BACKGROUND_1024x768] = LoadTGAAsTextureRect("Assets/Textures/CM_BACKGROUND_1280x720.tga");
	m_Textures[M_CONTROLLER_BACKGROUND_1024x768] = LoadTGAAsTextureRect("Assets/Textures/IM_CONTROLLER_BACKGROUND_1024x768.tga");
	m_Textures[M_KEYBOARD_BACKGROUND_1024x768] = LoadTGAAsTextureRect("Assets/Textures/IM_KEYBOARD_BACKGROUND_1024x768.tga");
	m_Textures[M_CRITICALMASS_MM_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/GameLogo.tga");
#else
	m_Textures[M_WIN_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/WM_BACKGROUND_1280x720.tga");
	m_Textures[M_LOSE_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/LM_BACKGROUND_1280x720.tga");
	m_Textures[M_MAIN_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/MM_BACKGROUND_1280x720.tga");
	m_Textures[M_OPTIONS_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/OM_BACKGROUND_1280x720.tga");
	m_Textures[M_DIFFICULTY_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/DM_BACKGROUND_1280x720.tga");
	m_Textures[M_CREDITS_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/CM_BACKGROUND_1280x720.tga");
	m_Textures[M_CONTROLLER_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/IM_CONTROLLER_BACKGROUND_1280x720.tga");
	m_Textures[M_KEYBOARD_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/IM_KEYBOARD_BACKGROUND_1280x720.tga");
	m_Textures[M_CRITICALMASS_MM_BACKGROUND_1280x720] = LoadTGAAsTextureRect("Assets/Textures/GameLogo.tga");
#endif
	m_Textures[M_LOSE_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/GameOver.tga");
	m_Textures[M_WIN_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/VictoryScreen.tga");
	m_Textures[M_BOARDER] = LoadTGAAsTextureRect("Assets/Textures/MenuFrame.tga");
	
	m_Textures[CONTROLLER_HELP] = LoadTGAAsTextureRect("Assets/Textures/GamepadTutorial.tga");
	m_Textures[KEYBOARD_HELP] = LoadTGAAsTextureRect("Assets/Textures/KeyboardTutorial.tga");

	m_Textures[M_CONTROLLER_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/GamepadTutorial.tga");
	m_Textures[M_KEYBOARD_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/KeyboardTutorial.tga");

	m_Textures[M_PAUSE_BG] = LoadTGAAsTextureRect("Assets/Textures/Background.tga");
	m_Textures[M_TUTORIAL_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/TM_BACKGROUND.tga");

	m_Textures[PUSHCD_BG] = LoadTGAAsTextureRect("Assets/Textures/PushCD_BG.tga");
	m_Textures[PUSHCD_FILL] = LoadTGAAsTextureRect("Assets/Textures/PushCD_Fill.tga");

	//BUTTONS
	m_Textures[BUTTON_MUSIC] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MUSIC.tga");
	m_Textures[BUTTON_SFX] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_SFX.tga");
	m_Textures[BUTTON_DEFAULT] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_DEFAULT.tga");
	m_Textures[BUTTON_GAMMA] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_GAMMA.tga");
	m_Textures[BUTTON_CREDITS] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_CREDITS.tga");
	m_Textures[BUTTON_MM] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MAINMENU.tga");
	m_Textures[BUTTON_PLAY] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_PLAY.tga");
	m_Textures[BUTTON_OPTIONS] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_OPTIONS.tga");
	m_Textures[BUTTON_BACK] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_BACK.tga");
	m_Textures[BUTTON_MOUSE] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MOUSE.tga");
	m_Textures[BUTTON_QUIT] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_QUIT.tga");
	m_Textures[BUTTON_NO] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_NO.tga");
	m_Textures[BUTTON_YES] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_YES.tga");
	m_Textures[BUTTON_EASY] =   LoadTGAAsTextureRect("Assets/Textures/BUTTON_EASY.tga");
	m_Textures[BUTTON_NORMAL] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MEDIUM.tga");
	m_Textures[BUTTON_HARD] =   LoadTGAAsTextureRect("Assets/Textures/BUTTON_HARD.tga");
	//m_Textures[BUTTON_BACK] = m_Textures[BUTTON_QUIT];
	//m_Textures[BUTTON_RESUME] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_RESUME.tga");
	//m_Textures[BUTTON_START] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_START.tga");

	m_Textures[HELP_MENU] = LoadTGAAsTextureRect("Assets/Textures/helpScreen.tga");

	m_Textures[BUTTON_RESOLUTION] = LoadTGAAsTextureRect("Assets/Textures/Resolution.tga");
	m_Textures[BUTTON_ENABLED] = LoadTGAAsTextureRect("Assets/Textures/Enabled.tga");
	m_Textures[BUTTON_DISABLED] = LoadTGAAsTextureRect("Assets/Textures/Disabled.tga");
	m_Textures[BUTTON_GAMEPAD] = LoadTGAAsTextureRect("Assets/Textures/GamePad.tga");
	m_Textures[BUTTON_ACCEPT] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_Accept.tga");
	m_Textures[BUTTON_CANCEL] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_Cancel.tga");
	m_Textures[BUTTON_LEFTARROW] = LoadTGAAsTextureRect("Assets/Textures/LeftArrow.tga");
	m_Textures[BUTTON_RIGHTARROW] = LoadTGAAsTextureRect("Assets/Textures/RightArrow.tga");


	//HIGHLIGHTED BUTTONS
	m_Textures[BUTTON_MUSIC_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MUSIC_HIGHLIGHTED.tga");
	m_Textures[BUTTON_SFX_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_SFX_HIGHLIGHTED.tga");
	m_Textures[BUTTON_DEFAULT_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_DEFAULT_HIGHLIGHTED.tga");
	m_Textures[BUTTON_GAMMA_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_GAMMA_HIGHLIGHTED.tga");
	m_Textures[BUTTON_CREDITS_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_CREDITS_HIGHLIGHTED.tga");
	m_Textures[BUTTON_MAINMENU_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MAINMENU_HIGHLIGHTED.tga");
	m_Textures[BUTTON_PLAY_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_PLAY_HIGHLIGHTED.tga");
	m_Textures[BUTTON_OPTIONS_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_OPTIONS_HIGHLIGHTED.tga");
	m_Textures[BUTTON_BACK_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_BACK_HIGHLIGHTED.tga");
	m_Textures[BUTTON_MOUSE_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MOUSE_HIGHLIGHTED.tga");
	m_Textures[BUTTON_QUIT_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_QUIT_HIGHLIGHTED.tga");
	m_Textures[BUTTON_NO_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_NO_HIGHLIGHTED.tga");
	m_Textures[BUTTON_YES_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_YES_HIGHLIGHTED.tga");
	m_Textures[BUTTON_EASY_HIGHLIGHTED] =   LoadTGAAsTextureRect("Assets/Textures/BUTTON_EASY_HIGHLIGHTED.tga");
	m_Textures[BUTTON_NORMAL_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MEDIUM_HIGHLIGHTED.tga");
	m_Textures[BUTTON_HARD_HIGHLIGHTED] =   LoadTGAAsTextureRect("Assets/Textures/BUTTON_HARD_HIGHLIGHTED.tga");
	//m_Textures[BUTTON_BACK_HIGHLIGHTED] = m_Textures[BUTTON_BACK_HIGHLIGHTED];
	m_Textures[BUTTON_RESUME_HIGHLIGHTED] =   LoadTGAAsTextureRect("Assets/Textures/BUTTON_RESUME_HIGHLIGHTED.tga");
	m_Textures[BUTTON_START_HIGHLIGHTED] =   LoadTGAAsTextureRect("Assets/Textures/BUTTON_START_HIGHLIGHTED.tga");

	m_Textures[BUTTON_RESOLUTION_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/Resolution_Highlighted.tga");
	m_Textures[BUTTON_ENABLED_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MUSIC_HIGHLIGHTED.tga");
	m_Textures[BUTTON_DISABLED_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MUSIC_HIGHLIGHTED.tga");
	m_Textures[BUTTON_GAMEPAD_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/Gamepad_Highlighted.tga");
	m_Textures[BUTTON_ACCEPT_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_Accept_HighLighted.tga");
	m_Textures[BUTTON_CANCEL_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_Cancel_Highlighted.tga");
	m_Textures[BUTTON_LEFTARROW_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/LeftArrow_Highlighted.tga");
	m_Textures[BUTTON_RIGHTARROW_HIGHLIGHTED] = LoadTGAAsTextureRect("Assets/Textures/RightArrow_Highlighted.tga");



	//BACKGROUNDS
#if defined(_DEBUG) || defined(_BETA)
	m_Textures[M_BLACK_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/BLACK_BACKGROUND_1024.tga");
	m_Textures[M_GP_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/GP_BACKGROUND_1024.tga");
	m_Textures[M_TEAM_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/TEAM_BACKGROUND_1024.tga");
	m_Textures[M_STUDIO_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/STUDIO_BACKGROUND_1024.tga");
#else
	m_Textures[M_BLACK_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/BLACK_BACKGROUND.tga");
	m_Textures[M_GP_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/GP_BACKGROUND.tga");
	m_Textures[M_TEAM_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/TEAM_BACKGROUND.tga");
	m_Textures[M_STUDIO_BACKGROUND] = LoadTGAAsTextureRect("Assets/Textures/STUDIO_BACKGROUND.tga");
#endif

	//MENU ITEMS
	//m_Textures[M_SLIDE_BAR] = LoadTGAAsTextureRect("Assets/Textures/slide_bar.tga");
	m_Textures[M_SLIDE_BAR] = LoadTGAAsTextureRect("Assets/Textures/Bar.tga");
	m_Textures[M_SLIDE_POINTER] = LoadTGAAsTextureRect("Assets/Textures/slide_pointer.tga");
	m_Textures[M_CREDITS_TEXT] = LoadTGAAsTextureRect("Assets/Textures/CreditsFinal.tga");

	//CONTROLS
	m_Textures[MENU_MOVEMENT] = LoadTGAAsTextureRect("Assets/Textures/NavControlsARROWS.tga");
	m_Textures[MENU_MOVEMENT_CONTROLLER] = LoadTGAAsTextureRect("Assets/Textures/NavControlsDPAD.tga");
	m_Textures[MENU_OK] = LoadTGAAsTextureRect("Assets/Textures/NavControlsACCEPT.tga");
	m_Textures[MENU_OK_CONTROLLER] = LoadTGAAsTextureRect("Assets/Textures/NavControlsABUTTON.tga");
	m_Textures[MENU_BACK] = LoadTGAAsTextureRect("Assets/Textures/NavControlsBACK.tga");
	m_Textures[MENU_BACK_CONTROLLER] = LoadTGAAsTextureRect("Assets/Textures/NavControlsBBUTTON.tga");
	m_Textures[MENU_ALL3] = LoadTGAAsTextureRect("Assets/Textures/NavControlsKEYBOARD.tga");
	m_Textures[MENU_ALL3_CONTROLLER] = LoadTGAAsTextureRect("Assets/Textures/NavControlsGAMEPAD.tga");


	//HUD ITEMS
	m_Textures[H_HEALTH_BAR_FRAME] = LoadTGAAsTextureRect("Assets/Textures/hud_rework_healthbar_frame.tga");
	m_Textures[H_AMMO_BAR_FILL] = LoadTGAAsTextureRect("Assets/Textures/DefaultBar.tga");
	m_Textures[WEAPONS_COOLDOWN] = LoadTGAAsTextureRect("Assets/Textures/CoolDown.tga");
	m_Textures[WEAPON_LEVEL_TAB] = LoadTGAAsTextureRect("Assets/Textures/HUD_WeaponLevelTab.tga");
	m_Textures[H_XPBAR] = LoadTGAAsTextureRect("Assets/Textures/DefaultBar.tga");
	m_Textures[H_TAB_LVL1] = LoadTGAAsTextureRect("Assets/Textures/LVL1.tga");
	m_Textures[H_TAB_LVL2] = LoadTGAAsTextureRect("Assets/Textures/LVL2.tga");
	m_Textures[H_TAB_LVL3] = LoadTGAAsTextureRect("Assets/Textures/LVL3.tga");
	m_Textures[HUD_FRAME] = LoadTGAAsTextureRect("Assets/Textures/hud_rework_ammosymbol_frame.tga");


	//m_Textures[H_AMMO_BAR_FILL] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_HealthBar.tga");
	m_Textures[H_HEALTH_BAR_FILL] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_HealthBar.tga");
	m_Textures[H_EMPTY_GUN] = LoadTGAAsTextureRect("Assets/Textures/EmptyGun2.tga");
	//m_Textures[H_LIVES_BAR_FILL] = LoadTGAAsTextureRect("Assets/Textures/lives_bar_fill.tga");
	m_Textures[H_WAVE1] = LoadTGAAsTextureRect("Assets/Textures/Wave1.tga");
	m_Textures[H_PLASMA_BALL_ICON] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_OrbitalStrike.tga");
	m_Textures[H_SHOTGUN_ICON] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_Boltgun.tga");
	m_Textures[H_YELLOW_BAR_FILL] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoBar_Yellow.tga");
	m_Textures[H_LASER_BEAM_ICON] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_Laser.tga");
	m_Textures[H_BLUE_BAR_FILL] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoBar_Blue.tga");
	m_Textures[H_WAVE2] = LoadTGAAsTextureRect("Assets/Textures/Wave2.tga");
	m_Textures[H_WAVE3] = LoadTGAAsTextureRect("Assets/Textures/Wave3.tga");
	//m_Textures[H_GUAGE] = LoadTGAAsTextureRect("Assets/Textures/health_bar_frameH.tga");
	//m_Textures[H_GUAGE_NEEDLE] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoBar_Blue.tga");
	m_Textures[H_PULL_GUAGE] = LoadTGAAsTextureRect("Assets/Textures/health_bar_frameH.tga");
	m_Textures[H_PULL_NEEDLE] = LoadTGAAsTextureRect("Assets/Textures/blue_bar_fillH.tga");
	m_Textures[H_PICKUP_NOTIFY] = LoadTGAAsTextureRect("Assets/Textures/HealthDeployed.tga");
	m_Textures[HUD_WAVE1_TEX] = LoadTGAAsTextureRect("Assets/Textures/WaveOneThree.tga");
	m_Textures[HUD_WAVE2_TEX] = LoadTGAAsTextureRect("Assets/Textures/WaveTwoThree.tga");
	m_Textures[HUD_WAVE3_TEX] = LoadTGAAsTextureRect("Assets/Textures/WaveThreeThree.tga");
	//m_Textures[LIVES_TOKENS] = LoadTGAAsTextureRect("Assets/Textures/HUD_TextureLifeSymbol.tga");
	m_Textures[LIVES_TOKENS] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_LifeSymbol.tga");
	m_Textures[ONE_DEPOT_LEFT] = LoadTGAAsTextureRect("Assets/Textures/OneDepotLeft.tga");
	m_Textures[H_AMMO_BAR_LABEL] = LoadTGAAsTextureRect("Assets/Textures/Ammo.tga");
	m_Textures[H_HEALTH_BAR_LABEL] = LoadTGAAsTextureRect("Assets/Textures/Health.tga");
	m_Textures[H_LIVES_LABEL] = LoadTGAAsTextureRect("Assets/Textures/Lives.tga");
	m_Textures[G_RETICULE] = LoadTGAAsTextureRect("Assets/Textures/Reticule.tga");
	m_Textures[NUMBER_ONE] = LoadTGAAsTextureRect("Assets/Textures/One.tga");
	m_Textures[NUMBER_TWO] = LoadTGAAsTextureRect("Assets/Textures/Two.tga");
	m_Textures[NUMBER_THREE] = LoadTGAAsTextureRect("Assets/Textures/Three.tga");
	m_Textures[NUMBER_FOUR] = LoadTGAAsTextureRect("Assets/Textures/Four.tga");
	m_Textures[NUMBER_FIVE] = LoadTGAAsTextureRect("Assets/Textures/Five.tga");
	m_Textures[NUMBER_SIX] = LoadTGAAsTextureRect("Assets/Textures/Six.tga");
	m_Textures[NUMBER_SEVEN] = LoadTGAAsTextureRect("Assets/Textures/Seven.tga");
	m_Textures[NUMBER_EIGHT] = LoadTGAAsTextureRect("Assets/Textures/Eight.tga");
	m_Textures[NUMBER_NINE] = LoadTGAAsTextureRect("Assets/Textures/Nine.tga");
	m_Textures[NUMBER_ZERO] = LoadTGAAsTextureRect("Assets/Textures/Zero.tga");
	m_Textures[H_WAVE] = LoadTGAAsTextureRect("Assets/Textures/Wave.tga");
	m_Textures[WAVE_MANAGER_WAVE_COMPLETE] = LoadTGAAsTextureRect("Assets/Textures/WaveComplete.tga");
	m_Textures[ONSLAUGHT_MODE] = LoadTGAAsTextureRect("Assets/Textures/Onslaught.tga");
	// For Weapon Level
	m_Textures[ICON_WEAPON_LEVEL1] = LoadTGAAsTextureRect("Assets/Textures/LVL1.tga");
	m_Textures[ICON_WEAPON_LEVEL2] = LoadTGAAsTextureRect("Assets/Textures/LVL2.tga");
	m_Textures[ICON_WEAPON_LEVEL3] = LoadTGAAsTextureRect("Assets/Textures/LVL3.tga");
	m_Textures[ICON_WEAPON_LEVELBAR] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_Weapon_LevelBar.tga");

	//m_Textures[HUD_RED_AMMO_FRAME_ICON_TEXTURE] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_OrbitalStrike.tga");
	//m_Textures[HUD_BLUE_AMMO_FRAME_ICON_TEXTURE] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_Laser.tga");
	//m_Textures[HUD_YELLOW_AMMO_FRAME_ICON_TEXTURE] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_Boltgun.tga");

	//Level Up Stuff - Tom
	m_Textures[HUD_LEVELUP_TEXT_TEXTURE] = LoadTGAAsTextureRect("Assets/Textures/LevelUpText.tga");
	m_Textures[HUD_RED_LEVELUP_ICON_TEXTURE] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_OrbitalStrike.tga");
	m_Textures[HUD_BLUE_LEVELUP_ICON_TEXTURE] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_Laser.tga");
	m_Textures[HUD_YELLOW_LEVELUP_ICON_TEXTURE] = LoadTGAAsTextureRect("Assets/Textures/HUD_Texture_AmmoSymbol_Boltgun.tga");

	//TUTORIAL HUD
	//NEW ASSESTS FOR TUTORIAL
	m_Textures[T_HEALTHPICKUP] = LoadTGAAsTextureRect("Assets/Textures/PUSHCratestoSmashAndCollectHeatlh.tga");
	m_Textures[TUTORIAL_DEPOT] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_DestroytheDepot.tga");
	m_Textures[TUTORIAL_LERP_PUSH_HIGHLIGHT] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_HIGHLIGHT.tga");
	m_Textures[H_TUTORIAL_PUSH] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_TrySpacebartoPUSH.tga");
	m_Textures[H_TUTORIAL_PULL] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_TryRightClicktoPush.tga");
	m_Textures[TUTORIAL_SHOOT_RED] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_TryLeftClicktoShoot.tga");
	m_Textures[T_ARROW_RIGHT] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_ReversedArrow.tga");
	m_Textures[TUTORIAL_WEAPON_LEVEL] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_WeaponLevel.tga");
	m_Textures[TUTORIAL_TAB] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_TryTabtoShowWpnExp.tga");
	m_Textures[TUTORIAL_BLUE_WEAPON] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_BlueWeapon.tga");
	m_Textures[TUTORIAL_LIVES_TEXT] = LoadTGAAsTextureRect("Assets/Textures/Total Lives.tga");
	m_Textures[TUTORIAL_AMMOTYPE_TEXT] = LoadTGAAsTextureRect("Assets/Textures/ammotype.tga");
	m_Textures[TUTORIAL_AMMOAMOUNT_TEXT] = LoadTGAAsTextureRect("Assets/Textures/Ammo Amount.tga");
	m_Textures[TUTORIAL_ARROW] = LoadTGAAsTextureRect("Assets/Textures/arrowTEST.tga");
	m_Textures[TUTORIAL_HEALTH_TEXT] = LoadTGAAsTextureRect("Assets/Textures/healthbarTEST.tga");
	m_Textures[TUTORIAL_LANDING_PAD] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_LandingPadRespawnandHealth.tga");
	m_Textures[TUTORIAL_RED_WEAPON] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_RedWeapon.tga");
	m_Textures[H_TUTORIAL_ESC] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_TryEscforhePauseMenu.tga");
	m_Textures[TUTORIAL_YELLOW_WEAPON] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_YellowWeapon.tga");
	m_Textures[BUTTON_RESUME] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_ResumeGame.tga");
	m_Textures[BUTTON_START] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_StartGame.tga");
	m_Textures[BUTTON_MAINMENU] = LoadTGAAsTextureRect("Assets/Textures/BUTTON_MAINMENU.tga");
	m_Textures[TUTORIAL_PURPLE_SHIELD] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_PurpleShield.tga");
	m_Textures[H_TUTORIAL_COMPLETE] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_Complete.tga");
	m_Textures[TUTORIAL_TEXT] = LoadTGAAsTextureRect("Assets/Textures/TUTORIAL_TEXT.tga");
	
	m_Textures[QUITGAME] = LoadTGAAsTextureRect("Assets/Textures/QuitQuestionMark.tga");
	

	//MUZZLE
	m_Textures[MUZZLEFLASH1] = LoadTGAAsTexture("Assets/Textures/MuzzleFlash1.tga");
	m_Textures[MUZZLEFLASH2] = LoadTGAAsTexture("Assets/Textures/MuzzleFlash2.tga");

	m_Textures[BULLET_TRAIL] = LoadTGAAsTexture("Assets/Textures/BulletTrail.tga");
	m_Textures[SMALL_ENEMY_DAMAGE] = LoadTGAAsTexture("Assets/Textures/SmallEnemy_FX_Damage.tga");
	m_Textures[BLOOD_POOL] = LoadTGAAsTexture("Assets/Textures/BloodPool.tga");
	m_Textures[HP_ARROW] = LoadTGAAsTexture("Assets/Textures/HPArrow.tga");
	m_Textures[ENERGY_RING] = LoadTGAAsTexture("Assets/Textures/EnergyRing.tga");

	//LIGHTNING
	m_Textures[BOLT_1] = LoadTGAAsTexture("Assets/Textures/Bolt1.tga");
	m_Textures[BOLT_2] = LoadTGAAsTexture("Assets/Textures/Bolt2.tga");
	m_Textures[BOLT_3] = LoadTGAAsTexture("Assets/Textures/Bolt3.tga");




	//For particle Effects
	m_FXTexturesArr[WHITE_PARTICLE] = LoadTGAAsTexture("Assets/Textures/WhiteParticle.tga");
	m_FXTexturesArr[DUST_TEXTURE]   = LoadTGAAsTexture("Assets/Textures/Misc_FX_Dust.tga");
	//Misc_FX_Dust

	// Geometry Effects
#pragma region Geometry_Effects_Data
	//Test Scaling Box
	m_GeoEffectDataArray[GEO_SCALING_BOX].m_DiffuseHandle = m_SmallEnemyData[BLUE].m_DiffuseHandle;
	m_GeoEffectDataArray[GEO_SCALING_BOX].m_ModelData.LoadMeshFromFile("Assets/Models/ScalingBox.mesh");
	m_GeoEffectDataArray[GEO_SCALING_BOX].m_Animations[GEO_SCALING_BOX_ANIM].LoadAnimFromFile("Assets/Models/ScalingBoxAnimation.bin", GEO_SCALING_BOX_ANIM, true);

	//Beacon Geometry
	//m_GeoEffectDataArray[GEO_BEACON].m_ModelData.LoadMeshFromFile("Assets/Models/Beacon.mesh");
	m_GeoEffectDataArray[GEO_BEACON].m_DiffuseHandle = LoadTGAAsTexture("Assets/Textures/beacon.tga");
	m_GeoEffectDataArray[GEO_BEACON].m_Animations[GEO_BEACON_ANIM].LoadAnimFromFile("Assets/Models/BeaconAnim.bin", GEO_BEACON_ANIM, true);

#pragma endregion

	// Numbers for the options menu
	m_Textures[OM_NUMBER_0] = LoadTGAAsTextureRect( "Assets/Textures/0.tga" );
	m_Textures[OM_NUMBER_1] = LoadTGAAsTextureRect( "Assets/Textures/1.tga" );
	m_Textures[OM_NUMBER_2] = LoadTGAAsTextureRect( "Assets/Textures/2.tga" );
	m_Textures[OM_NUMBER_3] = LoadTGAAsTextureRect( "Assets/Textures/3.tga" );
	m_Textures[OM_NUMBER_4] = LoadTGAAsTextureRect( "Assets/Textures/4.tga" );
	m_Textures[OM_NUMBER_5] = LoadTGAAsTextureRect( "Assets/Textures/5.tga" );
	m_Textures[OM_NUMBER_6] = LoadTGAAsTextureRect( "Assets/Textures/6.tga" );
	m_Textures[OM_NUMBER_7] = LoadTGAAsTextureRect( "Assets/Textures/7.tga" );
	m_Textures[OM_NUMBER_8] = LoadTGAAsTextureRect( "Assets/Textures/8.tga" );
	m_Textures[OM_NUMBER_9] = LoadTGAAsTextureRect( "Assets/Textures/9.tga" );
	m_Textures[OM_NUMBER_X] = LoadTGAAsTextureRect( "Assets/Textures/X.tga" );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Cleans up resources
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CAssetManager::Release( void )
{
	// Clean up object textures
	for(unsigned int i = 0; i < ET_NUMTYPES; ++i)
	{
		UnloadTexture(m_ObjectDataTotal[i].m_DiffuseHandle);
		UnloadTexture(m_ObjectDataTotal[i].m_hTextureSpecular);
		UnloadTexture(m_ObjectDataTotal[i].m_hTextureNormal);
		UnloadTexture(m_ObjectDataTotal[i].m_hTextureEmissive);
	}
	
	// Clean up small enemy textures
	for( unsigned int i = 0; i <= BLACK; ++i )
	{
		UnloadTexture( m_SmallEnemyData[i].m_DiffuseHandle );
		UnloadTexture( m_SmallEnemyData[i].m_hTextureEmissive );
		UnloadTexture( m_SmallEnemyData[i].m_hTextureNormal );
		UnloadTexture( m_SmallEnemyData[i].m_hTextureSpecular );
	}

	// Clean up Particle FX textures
	for( unsigned int i = 0; i < MAX_FX_TEXTURES; ++i )
	{
		UnloadTexture( m_FXTexturesArr[i] );
	}

	// Clean up Geometry FX textures
	for( unsigned int i = 0; i < MAX_GEO_FXTYPES; ++i )
	{
		UnloadTexture( m_GeoEffectDataArray[i].m_DiffuseHandle );
		UnloadTexture( m_GeoEffectDataArray[i].m_hTextureEmissive );
		UnloadTexture( m_GeoEffectDataArray[i].m_hTextureNormal );
		UnloadTexture( m_GeoEffectDataArray[i].m_hTextureSpecular );
	}

	// Clean up HUD textures
	for( unsigned int i = 0; i < MAX_TEXTURES; ++i )
	{
		UnloadTexture( m_Textures[i] );
	}

	// Clean up Triangle Batches
	for( unsigned int i = 0; i < MAX_TB; ++i )
	{
		m_vTriangleBatches[i].ReleaseVBOs();
	}

	// Clean up shaders
	for( unsigned int i = 0; i < NUM_SHADERS; ++i )
	{
		glDeleteProgram( m_vShaderHandles[i] );
	}
	m_vShaderHandles.clear();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loads all of the shaders for the game
Parameters: none
Returns: void
NOTE:  Shaders MUST be loaded in order according to eShaders in Definitions.h!
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CAssetManager::LoadShaders( void )
{
// Shaders MUST be loaded in the order in which they are listed in Definitions.h.
	m_vShaderHandles.resize( NUM_SHADERS, 0 );
	m_vTextureDiffuseHandles.resize( NUM_SHADERS, 0 );
	m_vTextureNormalHandles.resize( NUM_SHADERS, 0 );
	m_vTextureSpecularHandles.resize( NUM_SHADERS, 0 );
	m_vTextureUnit0Handles.resize( NUM_SHADERS, 0 );
	m_vTextureUnit1Handles.resize( NUM_SHADERS, 0 );
	m_vTextureUnit2Handles.resize( NUM_SHADERS, 0 );
	
	m_vUniformLightPosHandles[0].resize( NUM_SHADERS, 0 );
	m_vUniformLightPosHandles[1].resize( NUM_SHADERS, 0 );
	m_vUniformLightPosHandles[2].resize( NUM_SHADERS, 0 );
	m_vUniformLightPosHandles[3].resize( NUM_SHADERS, 0 );
	m_vUniformLightPosHandles[4].resize( NUM_SHADERS, 0 );
	m_vUniformLightPosHandles[5].resize( NUM_SHADERS, 0 );
	m_vUniformLightPosHandles[6].resize( NUM_SHADERS, 0 );
	m_vUniformLightPosHandles[7].resize( NUM_SHADERS, 0 );
	m_vUniformLightDirHandles.resize( NUM_SHADERS, 0 );
	m_vUniformLinearAttenuationHandles.resize( NUM_SHADERS, 0 );
	m_vUniformQuadraticAttenuationHandles.resize( NUM_SHADERS, 0 );
	m_vUniformAmbientIntensityHandles.resize( NUM_SHADERS, 0 );
	m_vUniformDiffuseIntensityHandles.resize( NUM_SHADERS, 0 );
	m_vUniformMVPHandles.resize( NUM_SHADERS, 0 );			
	m_vUniformMVHandles.resize( NUM_SHADERS, 0 );
	m_vUniformProjHandles.resize( NUM_SHADERS, 0 );	
	m_vUniformScaleMatrixHandles.resize( NUM_SHADERS, 0 );
	m_vUniformCameraHandles.resize( NUM_SHADERS, 0 );			
	m_vUniformNormalHandles.resize( NUM_SHADERS, 0 );			
	m_vUniformInverseCameraHandles.resize( NUM_SHADERS, 0 );	
	m_vUniformColorHandles.resize( NUM_SHADERS, 0 );
	m_vUniformJointHandles.resize( NUM_SHADERS, 0 );
	m_vUniformBindHandles.resize( NUM_SHADERS, 0 );
	m_vUniformTimeHandles.resize( NUM_SHADERS, 0 );
	m_vUniformDissolveFactorHandles.resize( NUM_SHADERS, 0 );
	m_vUniformHitPositionHandles.resize( NUM_SHADERS, 0 );

// 0 - TEXTURE ONLY
	GLuint hTempShader = glsLoadShaderPairWithAttributes(
		"Assets/Shaders/textureOnly.vs", "Assets/Shaders/textureOnly.ps", 2,
		GLS_ATTRIBUTE_VERTEX, "vVertex",
		GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	m_vShaderHandles[TEXTURE_ONLY] = hTempShader;
	glsCheckGLErrors();

	GLuint hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[TEXTURE_ONLY] = hTempUniform;
	glsCheckGLErrors();

	GLuint hTempTextureUnit = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[TEXTURE_ONLY] = hTempTextureUnit;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[TEXTURE_ONLY] = hTempUniform;
	glsCheckGLErrors();

// 1 - SKYBOX
	hTempShader = glsLoadShaderPairWithAttributes(
		"Assets/Shaders/skybox.vs", "Assets/Shaders/skybox.ps", 1,
		GLS_ATTRIBUTE_VERTEX, "vVertex" );
	m_vShaderHandles[SKYBOX] = hTempShader;

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[SKYBOX] = hTempUniform;

// 2 - CUBE_MAP
	hTempShader = glsLoadShaderPairWithAttributes( 
		"Assets/Shaders/cubemap.vs", "Assets/Shaders/cubemap.ps", 2,
		GLS_ATTRIBUTE_NORMAL, "vNormal",
		GLS_ATTRIBUTE_VERTEX, "vVertex" );
	m_vShaderHandles[CUBE_MAP] = hTempShader;

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[CUBE_MAP] = hTempUniform;

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[CUBE_MAP] = hTempUniform;

	hTempUniform = glGetUniformLocation( hTempShader, "mvMatrix" );
	m_vUniformMVHandles[CUBE_MAP] = hTempUniform;

	hTempUniform = glGetUniformLocation( hTempShader, "normalMatrix" );
	m_vUniformNormalHandles[CUBE_MAP] = hTempUniform;

	hTempUniform = glGetUniformLocation( hTempShader, "mInverseCamera" );
	m_vUniformInverseCameraHandles[CUBE_MAP] = hTempUniform;

// 3 - ANIMATED ONLY
	hTempShader = glsLoadShaderPairWithAttributes(
		"Assets/Shaders/AnimatedMesh.vs", "Assets/Shaders/AnimatedMesh.ps", 4,
		GLS_ATTRIBUTE_VERTEX, "vVertex",
		GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0",
		GLS_ATTRIBUTE_JINDICES, "nJointIndex",
		GLS_ATTRIBUTE_WEIGHTS, "fJointWeight");

	m_vShaderHandles[ANIMATED_ONLY] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[ANIMATED_ONLY] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[ANIMATED_ONLY] = hTempUniform;
	glsCheckGLErrors();

	hTempTextureUnit = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[ANIMATED_ONLY] = hTempTextureUnit;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mSkin" );
	m_vUniformJointHandles[ANIMATED_ONLY] = hTempUniform;
	glsCheckGLErrors();

// 3 - ANIMATED Dir
hTempShader = glsLoadShaderPairWithAttributes(
	"Assets/Shaders/Animated_DirLight.vs", "Assets/Shaders/Animated_DirLight.ps", 5,
	GLS_ATTRIBUTE_VERTEX, "vVertex",
	GLS_ATTRIBUTE_NORMAL, "vNormal",
	GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0",
	GLS_ATTRIBUTE_JINDICES, "nJointIndex",
	GLS_ATTRIBUTE_WEIGHTS, "fJointWeight");

	m_vShaderHandles[ANIMATED_DIR] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[ANIMATED_DIR] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[ANIMATED_DIR] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[ANIMATED_DIR] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mSkin" );
	m_vUniformJointHandles[ANIMATED_DIR] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "normalMatrix" );
	m_vUniformNormalHandles[ANIMATED_DIR] = hTempUniform;
	glsCheckGLErrors();
	
	hTempUniform = glGetUniformLocation( hTempShader, "vLightDirection" );
	m_vUniformLightDirHandles[ANIMATED_DIR] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "ambientIntensity" );
	m_vUniformAmbientIntensityHandles[ANIMATED_DIR] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "diffuseIntensity" );
	m_vUniformDiffuseIntensityHandles[ANIMATED_DIR] = hTempUniform;
	glsCheckGLErrors();

// 3 - ANIMATED Point
hTempShader = glsLoadShaderPairWithAttributes(
	"Assets/Shaders/Animated_PointLight.vs", "Assets/Shaders/Animated_PointLight.ps", 5,
	GLS_ATTRIBUTE_VERTEX, "vVertex",
	GLS_ATTRIBUTE_NORMAL, "vNormal",
	GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0",
	GLS_ATTRIBUTE_JINDICES, "nJointIndex",
	GLS_ATTRIBUTE_WEIGHTS, "fJointWeight");

	m_vShaderHandles[ANIMATED_POINT] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvMatrix" );
	m_vUniformMVHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "cameraMatrix" );
	m_vUniformCameraHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "normalMatrix" );
	m_vUniformNormalHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vLightPosition" );
	m_vUniformLightPosHandles[0][ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "ambientIntensity" );
	m_vUniformAmbientIntensityHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "diffuseIntensity" );
	m_vUniformDiffuseIntensityHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "linearAttenuation" );
	m_vUniformLinearAttenuationHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "quadraticAttenuation" );
	m_vUniformQuadraticAttenuationHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mSkin" );
	m_vUniformJointHandles[ANIMATED_POINT] = hTempUniform;
	glsCheckGLErrors();

// 4 - NO_TEX_DIR_LIGHT
	hTempShader = glsLoadShaderPairWithAttributes( 
		"Assets/Shaders/noTexDirLight.vs", "Assets/Shaders/noTexDirLight.ps", 2,
		GLS_ATTRIBUTE_NORMAL, "vNormal",
		GLS_ATTRIBUTE_VERTEX, "vVertex" );
	m_vShaderHandles[NO_TEX_DIR_LIGHT] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[NO_TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "normalMatrix" );
	m_vUniformNormalHandles[NO_TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[NO_TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();
	
	hTempUniform = glGetUniformLocation( hTempShader, "vLightDirection" );
	m_vUniformLightDirHandles[NO_TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "ambientIntensity" );
	m_vUniformAmbientIntensityHandles[NO_TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "diffuseIntensity" );
	m_vUniformDiffuseIntensityHandles[NO_TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

// 5 - TEX_DIR_LIGHT
	hTempShader = glsLoadShaderPairWithAttributes( 
		"Assets/Shaders/texDirLight.vs", "Assets/Shaders/texDirLight.ps", 3,
		GLS_ATTRIBUTE_NORMAL, "vNormal",
		GLS_ATTRIBUTE_VERTEX, "vVertex",
		GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	m_vShaderHandles[TEX_DIR_LIGHT] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "normalMatrix" );
	m_vUniformNormalHandles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vLightDirection" );
	m_vUniformLightDirHandles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "ambientIntensity" );
	m_vUniformAmbientIntensityHandles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "diffuseIntensity" );
	m_vUniformDiffuseIntensityHandles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit1" );
	m_vTextureUnit1Handles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit2" );
	m_vTextureUnit2Handles[TEX_DIR_LIGHT] = hTempUniform;
	glsCheckGLErrors();

// 6 - TEX_POINT
	hTempShader = glsLoadShaderPairWithAttributes( 
		"Assets/Shaders/texPointLight.vs", "Assets/Shaders/texPointLight.ps", 3,
		GLS_ATTRIBUTE_NORMAL, "vNormal",
		GLS_ATTRIBUTE_VERTEX, "vVertex",
		GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	m_vShaderHandles[TEX_POINT_LIGHT] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvMatrix" );
	m_vUniformMVHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "cameraMatrix" );
	m_vUniformCameraHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "normalMatrix" );
	m_vUniformNormalHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vLightPosition" );
	m_vUniformLightPosHandles[0][TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "ambientIntensity" );
	m_vUniformAmbientIntensityHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "diffuseIntensity" );
	m_vUniformDiffuseIntensityHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "linearAttenuation" );
	m_vUniformLinearAttenuationHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "quadraticAttenuation" );
	m_vUniformQuadraticAttenuationHandles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit1" );
	m_vTextureUnit1Handles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit2" );
	m_vTextureUnit2Handles[TEX_POINT_LIGHT] = hTempUniform;
	glsCheckGLErrors();

// 7 - RIPPLE_EFFECT
	hTempShader = glsLoadShaderPairWithAttributes( 
		"Assets/Shaders/rippleEffect.vs", "Assets/Shaders/rippleEffect.ps", 2,
		GLS_ATTRIBUTE_VERTEX, "vVertex",
		GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	m_vShaderHandles[RIPPLE_EFFECT] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[RIPPLE_EFFECT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvMatrix" );
	m_vUniformMVHandles[RIPPLE_EFFECT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "cameraMatrix" );
	m_vUniformCameraHandles[RIPPLE_EFFECT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[RIPPLE_EFFECT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "time" );
	m_vUniformTimeHandles[RIPPLE_EFFECT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[RIPPLE_EFFECT] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vHitPosition" );
	m_vUniformHitPositionHandles[RIPPLE_EFFECT] = hTempUniform;
	glsCheckGLErrors();


// 8 - BILLBOARD
	hTempShader = glsLoadShaderPairWithAttributes(
		"Assets/Shaders/Particle.vs", "Assets/Shaders/textureOnly.ps", 2,
		GLS_ATTRIBUTE_VERTEX, "vVertex",
		GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	m_vShaderHandles[BILLBOARD] = hTempShader;

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[BILLBOARD] = hTempUniform;
	glsCheckGLErrors();

	hTempTextureUnit = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[BILLBOARD] = hTempTextureUnit;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[BILLBOARD] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "scaleMatrix" );
	m_vUniformScaleMatrixHandles[BILLBOARD] = hTempUniform;
	glsCheckGLErrors();

// 9 - DISSOLVE
	hTempShader = glsLoadShaderPairWithAttributes(
		"Assets/Shaders/dissolve.vs", "Assets/Shaders/dissolve.ps", 2,
		GLS_ATTRIBUTE_VERTEX, "vVertex",
		GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	m_vShaderHandles[DISSOLVE] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[DISSOLVE] = hTempUniform;
	glsCheckGLErrors();

	hTempTextureUnit = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[DISSOLVE] = hTempTextureUnit;
	glsCheckGLErrors();

	hTempTextureUnit = glGetUniformLocation( hTempShader, "textureUnit1" );
	m_vTextureUnit1Handles[DISSOLVE] = hTempTextureUnit;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[DISSOLVE] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "dissolveFactor" );
	m_vUniformDissolveFactorHandles[DISSOLVE] = hTempUniform;
	glsCheckGLErrors();

	//SCROLLING_UV

	hTempShader = glsLoadShaderPairWithAttributes( 
		"Assets/Shaders/ScrollingUV.vs", "Assets/Shaders/ScrollingUV.ps", 2,
		GLS_ATTRIBUTE_VERTEX, "vVertex",
		GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	m_vShaderHandles[SCROLLING_UV] = hTempShader;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "mvpMatrix" );
	m_vUniformMVPHandles[SCROLLING_UV] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "vColor" );
	m_vUniformColorHandles[SCROLLING_UV] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "elapsedTime" );
	m_vUniformTimeHandles[SCROLLING_UV] = hTempUniform;
	glsCheckGLErrors();

	hTempUniform = glGetUniformLocation( hTempShader, "textureUnit0" );
	m_vTextureUnit0Handles[SCROLLING_UV] = hTempUniform;
	glsCheckGLErrors();

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loads all of the triangle batches for the game.
Parameters: none
Returns: void
NOTE:  Batches must be loaded in order according to eTriangleBatches that will be in definitions.h
	   Triangle batches will eventually be removed entirely in favor of render nodes handling their own
	   vertex buffers.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CAssetManager::LoadTriangleBatches( void )
{
	CMesh tempMesh;

	/*tempMesh.LoadMeshFromFile("Assets/Models/test_boxShape.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[BOX_SHAPE_TB] );*/

	// This is HAX until the asset manager holds meshes for animated objects
	m_playerMesh.LoadMeshFromFile( "Assets/Models/Player_Model.mesh" );
	BuildTriangleBatch( m_playerMesh, m_vTriangleBatches[PLAYER_SHAPE_TB], true );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/Small_Enemy_Model.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[SMALL_ENEMY_SHAPE_TB] );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/MediumEnemy_Model.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[MEDIUM_ENEMY_SHAPE_TB], true );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/LargeEnemy_Model.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[LARGE_ENEMY_SHAPE_TB], true);

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/LargeEnemy_Model.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[BOSS_ENEMY_SHAPE_TB], true);

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/TestBullet.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[TEST_BULLET_TB] );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/MoonShape.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[PLANET_SHAPE_TB] );
	///////////////////// TESTING COMET & PLANET & ASTEROID //////////////////////////////////
	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/ufo.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[ASTEROID_TB] );

	//tempMesh = CMesh();
	//tempMesh.LoadMeshFromFile("Assets/Models/MoonShape.mesh");
	//BuildTriangleBatch( tempMesh, m_vTriangleBatches[PLANET_SHAPE_TB] );
	//
	//tempMesh = CMesh();
	//tempMesh.LoadMeshFromFile("Assets/Models/MoonShape.mesh");
	//BuildTriangleBatch( tempMesh, m_vTriangleBatches[PLANET_SHAPE_TB] );
	//////////////////////////////////////////////////////////////////////////////////////////

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/Depot.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[MINING_STATION_TB], true );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/LandingPad_Model.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[LANDING_PAD_TB] );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/PullCone.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[PULL_CONE_TB] );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/Mesh_Laser.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[LASER_SHAPE_TB] );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/Model_RedBarrel.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[BARREL_SHAPE_TB] );

	tempMesh = CMesh();
	//tempMesh.LoadMeshFromFile("Assets/Models/Crate.mesh");
	tempMesh.LoadMeshFromFile("Assets/Models/Environment_Crate.mesh");
	
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[CRATE_TB] );

	tempMesh = CMesh();
	//tempMesh.LoadMeshFromFile("Assets/Models/EMP_Targeter.mesh");
	tempMesh.LoadMeshFromFile("Assets/Models/OrbitalStrike_FX_TargetRingShape_temp.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[EMP_TARGETER_TB] );

	tempMesh = CMesh();
	//tempMesh.LoadMeshFromFile("Assets/Models/light_pole.mesh");
	tempMesh.LoadMeshFromFile("Assets/Models/light_pole.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[LIGHT_POLE_TB] );

	tempMesh = CMesh();
	//tempMesh.LoadMeshFromFile("Assets/Models/NuclearBomb_Model.mesh");
	tempMesh.LoadMeshFromFile("Assets/Models/nuke.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[MORTAR_BASIC_TB] );

	tempMesh = CMesh();
	//tempMesh.LoadMeshFromFile("Assets/Models/Mesh_Mortar_FinalBlast.mesh");
	tempMesh.LoadMeshFromFile("Assets/Models/Mesh_Mortar_FinalBlast.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[MORTAR_FINALBLAST_TB] );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/Mesh_BlueStrike.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[BLUE_STRIKE_TB] );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/Health_ModelShape.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[HEALTHPICKUP_TB] );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/beacon.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[GEO_BEACON_TB], true );

	tempMesh = CMesh();
	tempMesh.LoadMeshFromFile("Assets/Models/Quad.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[GEO_QUAD_TB] );

	/*m_GeoEffectDataArray[GEO_SCALING_BOX].m_ModelData.LoadMeshFromFile("Assets/Models/ScalingBox.mesh");
	BuildTriangleBatch( tempMesh, m_vTriangleBatches[SCALING_BOX_TB], true );*/

	//m_GeoEffectDataArray[GEO_SCALING_CONE].m_ModelData.LoadMeshFromFile("Assets/Models/ScalingCone.mesh");
	//BuildTriangleBatch( m_GeoEffectDataArray[GEO_SCALING_CONE].m_ModelData, m_vTriangleBatches[SCALING_BOX_TB], true );

	glsCreateSphere( m_vTriangleBatches[EXPLOSION_TB], 10.0f );

	glsCreateSphere( m_vTriangleBatches[SHIELD_TB], 25.0f );

	glsCreateSphere( m_vTriangleBatches[PUSH_TB], 8.0f );

};

// Comment me
void CAssetManager::BuildTriangleBatch( CMesh& _mesh, GLTriangleBatch& _triangleBatch, bool _Animated )
{
	_triangleBatch.BeginMesh( _mesh.GetPositions().size(), _mesh.GetTriangles().size() * 3 );
	for(size_t Triangle = 0; Triangle < _mesh.GetTriangles().size(); Triangle++)
	{
		Vec3f vec[3];
		Vec3f norm[3];
		Vec2f UV[3];
		GLubyte jIndex[12];
		Vec4f jWeight[3];

		// loops throught the three verts in the triangle
		for(int Vertex = 0; Vertex < 3; Vertex++)
		{
			if( _Animated )
			{
				for(unsigned int Influence = 0; Influence < 4; ++Influence)
				{
					if(_mesh.GetInfluences().size() > 0 && _mesh.GetInfluences()[_mesh.GetTriangles()[Triangle].Indicies[Vertex]].size() - 1 >= Influence)
					{
						jIndex[Vertex * 4 + Influence] = (GLubyte)_mesh.GetInfluences()[_mesh.GetTriangles()[Triangle].Indicies[Vertex]][Influence].m_JointIndex;
						jWeight[Vertex][Influence] = _mesh.GetInfluences()[_mesh.GetTriangles()[Triangle].Indicies[Vertex]][Influence].m_Weight;
					}
					else
					{
						jIndex[Vertex * 4 + Influence] = 0;
						jWeight[Vertex][Influence] = 0.0f;
					}
				}
			}

			// loops through for the size of the vector
			for(int Index = 0; Index < 3; Index++)
			{
				vec[Vertex][Index] = _mesh.GetPositions()[_mesh.GetTriangles()[Triangle].Indicies[Vertex]].v[Index];
				norm[Vertex][Index] = _mesh.GetNormals()[_mesh.GetTriangles()[Triangle].Indicies[Vertex]].v[Index];

				if( Index < 2 )
					UV[Vertex][Index] = _mesh.GetUVs()[_mesh.GetTriangles()[Triangle].Indicies[Vertex]].v[Index];
			}
		}

		// adds the triangle to the batch
		if( _Animated )
			_triangleBatch.AddTriangle( vec, norm, UV, jIndex, jWeight );
		else
			_triangleBatch.AddTriangle( vec, norm, UV );
	}
	_triangleBatch.End();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loads a texture from a file!
Parameters:
[in] _fileName - The file name of the texture.
[in] _wrapMode - 
[in] _filter - 
Returns: Returns the index of the texture.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
GLuint CAssetManager::LoadTGAAsTexture( const char* _fileName, GLenum _wrapMode, GLenum _filter )
{
	GLuint hTexture;

	glGenTextures( 1, &hTexture );
	glBindTexture( GL_TEXTURE_2D, hTexture );

	GLbyte *pColorBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	pColorBits = glsReadTGABits( _fileName, &nWidth, &nHeight, &nComponents, &eFormat );
	if( !pColorBits )
	{
		MessageBoxA( 0, "Failed to load targa texture.", _fileName, MB_OK );
		return 0;  // This may want to be -1
	}

	glTexImage2D( GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0 /*no border ever*/,
		eFormat, GL_UNSIGNED_BYTE, pColorBits );

	free( pColorBits ); // one of our function calls called malloc

	if( _filter != GL_LINEAR && _filter != GL_NEAREST )
		glGenerateMipmap( GL_TEXTURE_2D );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode );
	glsCheckGLErrors();

	return hTexture;
}

GLuint CAssetManager::LoadTGAAsTextureRect( const char *szFileName, GLenum wrapMode, GLenum filter )
{
	GLuint hTexture;

	glGenTextures( 1, &hTexture );
	glBindTexture( GL_TEXTURE_RECTANGLE, hTexture );

	GLbyte *pColorBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	pColorBits = glsReadTGABits( szFileName, &nWidth, &nHeight, &nComponents, &eFormat );
	if( !pColorBits )
	{
		MessageBoxA( 0, "Failed to load targa texture.", szFileName, MB_OK );
		return 0;
	}

	glTexImage2D( GL_TEXTURE_RECTANGLE, 0, nComponents, nWidth, nHeight, 0 /*no border ever*/,
		eFormat, GL_UNSIGNED_BYTE, pColorBits );

	free( pColorBits ); // one of our function calls called malloc

	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, filter );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, filter );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, wrapMode );

	return hTexture;
}

GLuint CAssetManager::LoadTGAsAsCubeTexture( const char *_fileName[6] )
{
	GLuint textureCubeMap;
	glGenTextures( 1, &textureCubeMap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, textureCubeMap );
	
	GLbyte *pColorBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	GLenum cube[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for( int i = 0; i < 6; ++i )
	{
		(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
		pColorBits = glsReadTGABits( _fileName[i], &nWidth, &nHeight, &nComponents, &eFormat );
		if( !pColorBits )
		{
			MessageBoxA( 0, "Failed to load targa texture.", _fileName[i], MB_OK );
			return 0;
		}

		//glTexImage2D( cube[i], 0, nComponents, nWidth, nHeight, 0 /*no border ever*/,
		//	eFormat, GL_UNSIGNED_BYTE, pColorBits );
		
		// We're enabling texture compression, because the cube maps use six large textures
		// apiece and we'd rather have bad load time than poor runtime performance.  We may
		// switch over to DDS textures later.
		glTexImage2D( cube[i], 0, GL_COMPRESSED_RGBA, nWidth, nHeight, 0 /*no border ever*/,
			eFormat, GL_UNSIGNED_BYTE, pColorBits );

		free( pColorBits ); // one of our function calls called malloc
	}

	glGenerateMipmap( GL_TEXTURE_CUBE_MAP );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	return textureCubeMap;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Unloads a texture
Parameters: none
Returns: void
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CAssetManager::UnloadTexture( GLuint _hTexture )
{
	glDeleteTextures( 1, &_hTexture );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loads an animation from a file!
Parameters:
[in] _fileName - The file name of the animation.
Returns: Returns the index of the animation.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
unsigned int CAssetManager::LoadAnimation( char* _fileName )
{
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loads a mesh from a file!
Parameters:
[in] _fileName - The file name of the mesh.
Returns: Returns the index of the mesh.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
unsigned int CAssetManager::LoadMesh( char* _fileName, int _objID )
{
	//switch( _objID )
	//{
	//case ET_PLAYER:
	//	{
	//		m_ObjectDataTotal[_objID].m_ModelData.LoadMeshFromFile("Assets/Models/PH_PlayerShape.mesh");
	//	}
	//	break;
	//case ET_BULLET_PLAYER:
	//	{
	//		//m_ObjectDataTotal[_objID].m_ModelData.LoadMeshFromFile("Assets/Models/PH_PlayerShape.mesh");
	//	}
	//	break;

	//}
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Grabs a texture
Parameters:
[in] _index - The index of the texture.
Returns: Returns the Texture.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
unsigned int CAssetManager::GetTexture( unsigned int _index )
{
	return m_Textures[_index];
}
