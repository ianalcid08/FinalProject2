#ifndef _OPTIONSMENU_H_
#define _OPTIONSMENU_H_
/*****************************************************************
File name:			OptionsMenu.h
Date:				7/17/13
Modify Date:		08/07/13
Modify Initials:	CM
Author:				Ian Alcid
Purpose:			Class for options state.
******************************************************************/

#include "IState.h"
#include "Game.h"

class CInput;
class CGame;
class CRenderer;
class CAssetManager;
class CPauseState;
struct tGamma;

enum eOMChoices { OM_MUSIC = 0, OM_SFX, OM_GAMMA, OM_GAMEPAD, OM_RESOLUTION, OM_LEFTARROW, OM_RIGHTARROW, OM_ACCEPT, OM_DEFAULT, OM_CANCEL, OM_NUM_CHOICES };

enum eOMElements {	OM_BACKGROUND_ELEMENT = 0,
					OM_MUSIC_ELEMENT, OM_SFX_ELEMENT, OM_GAMMA_ELEMENT, OM_MOUSE_ELEMENT, /*OM_EXIT_ELEMENT,*/
					OM_BACK_HIGHLIGHTED_ELEMENT, OM_GAMMA_HIGHLIGHTED_ELEMENT, OM_MUSIC_HIGHLIGHTED_ELEMENT,
					OM_SFX_HIGHLIGHTED_ELEMENT, OM_DEFAULT_ELEMENT, OM_DEFAULT_HIGHLIGHTED_ELEMENT,
					OM_MOUSE_HIGHLIGHTED_ELEMENT, OM_RESOLUTION_ELEMENT, OM_RESOLUTION_HIGHLIGHT_ELEMENT,
					OM_ACCEPT_ELEMENT, OM_ACCEPT_HIGHLIGHTED_ELEMENT, OM_CANCEL_ELEMENT, OM_CANCEL_HIGHLIGHTED_ELEMENT,
					OM_GAMEPAD_ELEMENT, OM_GAMEPAD_HIGHLIGHTED_ELEMENT, OM_ENABLE_ELEMENT, OM_DISABLE_ELEMENT,
					//OM_MOVE_ELEMENT, OM_OK_ELEMENT, OM_BACK_ELEMENT, OM_MOVE_CONTROLLER_ELEMENT, OM_OK_CONTROLLER_ELEMENT, OM_BACK_CONTROLLER_ELEMENT,
					OM_ALL3_ELEMENT, OM_ALL3_CONTROLLER_ELEMENT,
					OM_BAR_ELEMENT1, OM_SLIDER_ELEMENT1,
					OM_BAR_ELEMENT2, OM_SLIDER_ELEMENT2,
					OM_BAR_ELEMENT3, OM_SLIDER_ELEMENT3,
					OM_BAR_ELEMENT4, OM_SLIDER_ELEMENT4,
					OM_HIGHLIGHTED_ELEMENT, OM_BLACK_BACKGROUND_ELEMENT,
					OM_LEFTARROW_ELEMENT, OM_RIGHTARROW_ELEMENT,
					OM_LEFTARROW_HIGHLIGHTED_ELEMENT, OM_RIGHTARROW_HIGHLIGHTED_ELEMENT,
					OM_NUM_ELEMENTS };

class COptionsState : public IState
{
	COptionsState( void );
	virtual ~COptionsState( void );

public:
	// Singleton
	static	COptionsState*	GetInstance( void );

	// IState 
	void	Init(CGame* _game);
	void	Shutdown( void );
	bool	Input( CInput& _input );
	void	Update(float _fElapsedTime);
	void	Render( void );
	void	Enter( void );
	void	Exit( void );
	void	FadeIn (float _fTime);
	void	FadeOut (float _fTime);
	
	// Load & Save
	bool LoadOptions( const WCHAR* szFilename);
	void SaveOptions (const WCHAR* szFilename);

	void ProcessStates( eOMChoices _choice );

	void ResetDefault ( CGame* _game );

	bool GetEnable () { return m_bEnable; }

	//inline float Clamp( float _x, float _a, float _b) { return (_x < _a) ? _a : ( _x > _b) ? _b : _x ; }
	// if (value < floor) return floor, else if (value > ceiling) return ceiling, else return value

	inline double ClampGamma ( double _x) { return ((_x / 9.0) -.5) * 2.0; }

	//struct tGamma
	//{
	//	unsigned short	m_GammaColorRed[256];		//LPVOID
	//	unsigned short	m_GammaColorGreen[256];		//LPVOID
	//	unsigned short	m_GammaColorBlue[256];		//LPVOID
	//};
	
private:
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;
	CPauseState		*m_P;
	HDC				m_HDC;
	int				m_Choice;
	float			m_Timer;
	float			m_ResolutionChangeTimer;
	bool			m_Switch;
	CUIElement		m_OMElements[OM_NUM_ELEMENTS];

	std::vector<CUIElement*> m_UIResolution;
	int				m_nResolutionIndex;
	bool			m_bCheckResolution;

	int				m_MusicVolume;
	int				m_SFXVolume;
	int				m_Gamma;
	int				m_MouseSen;
	int				m_Enable;

	float			m_SliderX1;
	float			m_SliderY1;
	float			m_SliderX2;
	float			m_SliderY2;
	float			m_SliderX3;
	float			m_SliderY3;
	float			m_SliderX4;
	float			m_SliderY4;

	//bool			m_bExit;
	bool			m_bGamma;
	bool			m_bMusic;
	bool			m_bSFX;
	bool			m_bMouse;
	bool			m_bDefault;
	bool			m_bResolution;
	bool			m_bLeftArrow;
	bool			m_bRightArrow;
	bool			m_bAccept;
	bool			m_bCancel;
	bool			m_bGamepad;
	bool			m_bEnable;

	float			m_FadeTimer;
	bool			m_bFadeOut;

	bool			m_bMouseActive;
	int				m_PrevMousePosX;
	int				m_PrevMousePosY;

	tGamma			m_GammaArray;

	bool			m_bSoundMusic;
	bool			m_bSoundSFX;
	bool			m_bSoundMouse;
	bool			m_bSoundDefault;
	bool			m_bSoundGamma;
	bool			m_bSoundResolution;
	bool			m_bSoundAccept;
	bool			m_bSoundCancel;
	bool			m_bSoundGamePad;
	bool			m_bSoundClick;
	bool			m_bSoundEnable;

	int testCounter;
	bool playThisFrickinSoundOnce;
	CUIElement		m_reticle;
};

#endif
