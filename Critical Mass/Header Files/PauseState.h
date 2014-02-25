#ifndef _PAUSESTATE_H_
#define _PAUSESTATE_H_

#include "istate.h"

class CGame;
class CRenderer;
class CAssetManager;

enum ePauseChoices { P_PLAY, P_OPTIONS, P_EXIT, PAUSE_NUM_CHOICES};
enum ePauseElements {	PAUSE_BACKGROUND_ELEMENTS = 0,
						PAUSE_RESUME_ELEMENTS, PAUSE_OPTIONS_ELEMENTS, PAUSE_EXIT_ELEMENTS, 
						PAUSE_PLAY_HIGHLIGHTED_ELEMENTS, PAUSE_OPTIONS_HIGHLIGHTED_ELEMENTS,
						PAUSE_EXIT_HIGHLIGHTED_ELEMENTS, PAUSE_BLACK_BACKGROUND_ELEMENT,
						PAUSE_HIGHLIGHTED_ELEMENTS, PAUSE_NUM_ELEMENTS};

class CPauseState :	public IState
{
	CGame			*m_Game;
	CRenderer		*m_Renderer;
	CAssetManager	*m_AM;
	int				m_Choice;
	float			m_Timer;
	CUIElement		m_PauseElements[PAUSE_NUM_ELEMENTS];
	bool			m_bExit;
	bool			m_bPlay;
	bool			m_bOptions;

	float			m_FadeTimer;
	bool			m_bFadeOut;

	bool			m_bMouseActive;
	int				m_PrevMousePosX;
	int				m_PrevMousePosY;
	bool			m_Esc;

	bool			m_bSoundPlay;
	bool			m_bSoundExit;
	bool			m_bSoundOptions;

	void ProcessStates(ePauseChoices _choice);
	CPauseState(void);
	virtual ~CPauseState(void);
	CUIElement		m_reticle;

public:
	static CPauseState* GetInstance();
	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render();
	void Enter();
	void Exit(); 
	void FadeIn(float _fTime);
	void FadeOut(float _fTime);
};

#endif