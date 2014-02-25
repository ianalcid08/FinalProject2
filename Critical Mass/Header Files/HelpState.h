#pragma once

#include "IState.h"

class CRenderer;
class CUIElement;
class CGame;

class CHelpState : public IState
{
private:

	float			m_Timer;
	bool			m_FadeIn_Out;
	CRenderer*		m_Renderer;
	CUIElement		m_BackGround;
	CGame*			m_Game;

public:

	CHelpState(void);
	virtual ~CHelpState(void);

	virtual void Init(CGame* _game);
	virtual void Shutdown();
	virtual bool Input( CInput& _input );
	virtual void Update(float _fElapsedTime);
	virtual void Render();
	virtual void Enter();
	virtual void Exit();

	static CHelpState* GetInstance(void);

};

