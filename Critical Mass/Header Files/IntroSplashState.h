#ifndef _INTROSPLASHSTATE_H_
#define _INTROSPLASHSTATE_H_

/*****************************************************************
File name:			CIntroSplashState.h
Date:				08/12/13
Modify Date:		08/12/13
Modify Initials:	CM
Author:				Corey Morehead
Purpose:			This class will show the three images that are required 
					from the GRC.
******************************************************************/

#include "IState.h"

class CGame;
class CRenderer;
class CAssetManager;

enum eISElements { IS_BACKGROUND_GP_ELEMENT = 0, IS_BACKGROUND_STUDIO_ELEMENT, IS_BACKGROUND_TEAM_ELEMENT, IS_BLACK_BACKGROUND_ELEMENT, IS_NUMBER_ELEMENTS };

class CIntroSplashState : public IState
{
private:
	CIntroSplashState( void );
	~CIntroSplashState( void ) { }

	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;
	float			m_Timer;
	float			m_FadeTimer;
	CUIElement		eISElements[IS_NUMBER_ELEMENTS];

	bool			m_bGPIn;
	bool			m_bGPOut;
	bool			m_bStudioIn;
	bool			m_bStudioOut;
	bool			m_TeamIn;
	bool			m_TeamOut;

	float			m_GPFadeTimerIn;
	float			m_TeamFadeTimerIn;
	float			m_StudioFadeTimerIn;
	float			m_GPFadeTimerOut;
	float			m_TeamFadeTimerOut;
	float			m_StudioFadeTimerOut;

	float			m_FadeTimerRedo;
	bool			m_bFadeOut;

public:
	//Singleton
	static CIntroSplashState* GetInstance( void );

	//IState Functions
	void Init(CGame* _game);
	void Shutdown( void );
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render( void );
	void Enter( void );
	void Exit( void ); 
};

#endif