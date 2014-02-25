#ifndef _ISTATE_H_
#define _ISTATE_H_

/*****************************************************************
File name:			IState.h
Date:				7/17/13
Modify Date:		7/17/13
Modify Initials:	IA
Author:				Ian Alcid
Purpose:			Interface for all game states.
******************************************************************/
#include "UIElement.h"

class CGame;
class CInput;

class IState
{
private:
	char m_ChangeState;
	bool m_closing;
	short m_ID;

public:
	virtual ~IState() = 0 {};
	virtual void Init(CGame* _game) = 0;
	virtual void Shutdown() = 0;
	virtual bool Input( CInput& _input ) = 0;
	virtual void Update(float _fElapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;

	//Accessors
	char GetChangeState() {return m_ChangeState;}
	inline bool IsClosing(){return m_closing;}
	inline short GetID(){return m_ID;}
	//mutators
	inline void SetClosing(bool _bool){m_closing = _bool;}
	inline void SetID(short _newShort){m_ID = _newShort;}


};

#endif