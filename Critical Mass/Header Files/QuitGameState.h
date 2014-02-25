#pragma once

#include "IState.h"

class CRenderer;
class CUIElement;
class CGame;

enum eQuitChoices {		QUIT_YES, QUIT_NO, QUIT_ESC, QUIT_NUM_CHOICES };
enum eQuitElements {	QUIT_BACKGROUND_ELEMENT = 0,
						QUIT_YES_ELEMENT, QUIT_NO_ELEMENT,
						QUIT_YES_HIGHLIGHTED_ELEMENT, QUIT_TEXT_ELEMENT,
						QUIT_NO_HIGHLIGHTED_ELEMENT, QUIT_NUM_ELEMENTS };

class CQuitGameState : public IState
{
private:

	float			m_Timer;
	float			m_FadeTimer;
	bool			m_bFadeOut;
	CRenderer*		m_Renderer;
	CUIElement		m_BackGround;
	CGame*			m_Game;
	bool			m_bMouseActive;
	int				m_PrevMousePosX;
	int				m_PrevMousePosY;
	int				m_Choice;
	bool			m_bSoundYes;
	bool			m_bSoundNo;
	bool			m_bNo;
	bool			m_bYes;
	CUIElement		m_QuitElements[QUIT_NUM_ELEMENTS];
	CUIElement		m_reticle;

	CQuitGameState(void);
	virtual ~CQuitGameState(void);

public:
	virtual void Init(CGame* _game);
	virtual void Shutdown();
	virtual bool Input( CInput& _input );
	virtual void Update(float _fElapsedTime);
	virtual void Render();
	virtual void Enter();
	virtual void Exit();

	static CQuitGameState* GetInstance(void);
	void FadeOut(float _fTime);
	void FadeIn(float _fTime);

};

