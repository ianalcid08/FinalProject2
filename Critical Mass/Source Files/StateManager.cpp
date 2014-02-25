#include "../Header Files/stdafx.h"
#include "StateManager.h"
#include "../Header Files/Input.h"
#include "../Header Files/IState.h"
#include "../Header Files/AnimDebugState.h"


//include all the states
#include "../Header Files/GamePlayState.h"
#include "../Header Files/WinState.h"
#include "../Header Files/LoseState.h"
#include "../Header Files/MainMenuState.h"
#include "../Header Files/OptionsMenu.h"
#include "../Header Files/CreditsMenu.h"
#include "../Header Files/PauseState.h"
#include "../Header Files/IntroState.h"
#include "../Header Files/DifficultyMenu.h"
#include "../Header Files/IntroSplashState.h"
#include "../Header Files/TutorialMenu.h"
#include "../Header Files/TutorialState.h"
#include "../Header Files/TutorialPause.h"
#include "../Header Files/HelpState.h"
#include "../Header Files/QuitGameState.h"


StateManager::StateManager(void)
{

}

StateManager::~StateManager(void)
{

}

void StateManager::Initialize(CGame * _game)
{
	m_Game = _game;
	
	//TO DO - Game Starts, push intro state

	//Until menus work, push the game play state
	CMainMenuState::GetInstance()->Init(_game);
	CGameplayState::GetInstance()->Init(_game);
	COptionsState::GetInstance()->Init(_game);
	CPauseState::GetInstance()->Init(_game);
	LoseState::GetInstance()->Init(_game);
	CIntroState::GetInstance()->Init(_game);
	CDifficultyMenu::GetInstance()->Init(_game);
	CCreditsState::GetInstance()->Init(_game);
	WinState::GetInstance()->Init(_game);
	CAnimDebugState::GetInstance()->Init(_game);
	CIntroSplashState::GetInstance()->Init(_game);
	CTutorialMenu::GetInstance()->Init(_game);
	CTutorialState::GetInstance()->Init(_game);
	CTutorialPause::GetInstance()->Init(_game);
	CHelpState::GetInstance()->Init(_game);
	CQuitGameState::GetInstance()->Init(_game);

#ifdef _DEBUG
	ChangeState(MAINMENU_STATE);
#else
	ChangeState(INTRO_SPLASH_STATE); 
#endif
	//PushState(GAMEPLAY_STATE);
}

void StateManager::Shutdown()
{
	CGameplayState::GetInstance()->Shutdown();
	CMainMenuState::GetInstance()->Shutdown();;
	COptionsState::GetInstance()->Shutdown();;
	CPauseState::GetInstance()->Shutdown();;
	LoseState::GetInstance()->Shutdown();;
	CIntroState::GetInstance()->Shutdown();;
	CDifficultyMenu::GetInstance()->Shutdown();;
	CCreditsState::GetInstance()->Shutdown();;
	WinState::GetInstance()->Shutdown();;
	CAnimDebugState::GetInstance()->Shutdown();;
	CIntroSplashState::GetInstance()->Shutdown();;
	CTutorialMenu::GetInstance()->Shutdown();;
	CTutorialState::GetInstance()->Shutdown();;
	CTutorialPause::GetInstance()->Shutdown();;
	CHelpState::GetInstance()->Shutdown();
	CQuitGameState::GetInstance()->Shutdown();
}

void StateManager::Input(CInput& _input)
{
	if(m_currStates.size() && !(m_currStates.back()->IsClosing()))
	{
		m_currStates.back()->Input(_input);
	}
}

void StateManager::Update(float _fElapsedTime)
{
	for(unsigned int i = 0; i < m_currStates.size(); i++)
	{
		if(m_currStates[i]->IsClosing() == true && m_currStates.size() > 1)    // check if the state wants to close and make sure its not the only one
		{
			m_currStates[i]->SetClosing(false);
			PopState();
			i--;
			m_currStates.back()->Enter();
		}
	}
	m_currStates.back()->Update(_fElapsedTime);
}

void StateManager::Render()
{
	for(std::vector<IState*>::iterator iter = m_currStates.begin(); iter != m_currStates.end(); iter++)
	{
		(*iter)->Render();  // renders from back most state to the most recently pushed (top most) state
	}

	//m_currStates.back()->Render();
}

void StateManager::ChangeState( eGameStates _NewState)
{
	if (m_currStates.size() != 0)
	{
		for (auto iter = m_currStates.rbegin(); iter != m_currStates.rend(); ++iter)
			(*iter)->Exit();
		m_currStates.clear();
	}

	IState* pNewState = ProcessEnum(_NewState);

	// Assign the current state
	m_currStates.push_back(pNewState);

	// Enter the new state (if any)
	if (m_currStates.back() != nullptr)
		m_currStates.back()->Enter();
	
}

void StateManager::PushState(eGameStates _eNewState)
{
	IState* pNewState = ProcessEnum(_eNewState);

	for (vector<IState*>::iterator iter = m_currStates.begin(); iter != m_currStates.end(); ++iter) 
	{
		if ((*iter) == pNewState) 
		{
			m_currStates.erase(iter);
			m_currStates.push_back(pNewState);
			return;
		}
	}
	m_currStates.push_back(pNewState);

	// Enter the new state (if any)
	if (m_currStates.size() != 0)
	{
		//m_currStates.back()->Init(m_Game);
		m_currStates.back()->Enter();
	}
	int x = 0;
}

void StateManager::PopState()
{
	// If only 1 State is on the Stack dont remove it
	if (m_currStates.size() < 2)
		return;

	// Exit the current state
	m_currStates.back()->Exit();

	// Remove the Current state
	//if(m_currStates.back()->GetID() == WIN_STATE)
	//{
	//	//m_currStates.front
	//	m_currStates.back()->Shutdown();
	//	m_currStates.pop_back();
	//	//m_currStates.front()->Init(m_Game);
	//	m_currStates.back()->Shutdown();
	//	m_currStates.pop_back();
	//	PushState(GAMEPLAY_STATE);
	//}
	m_currStates.pop_back();

}

IState* StateManager::ProcessEnum(eGameStates _eIn)
{
	IState* pOut = nullptr;

	switch(_eIn)
	{
	case INTRO_STATE:
		{
			pOut = CIntroState::GetInstance();
		}
		break;
	case MAINMENU_STATE:
		{
			pOut = CMainMenuState::GetInstance();
		}
		break;
	case OPTIONS_STATE:
		{
			pOut = COptionsState::GetInstance();
		}
		break;
	case CREDITS_STATE:
		{
			pOut = CCreditsState::GetInstance();
		}
		break;
	case PAUSE_STATE:
		{
			pOut = CPauseState::GetInstance();
		}
		break;
	case GAMEPLAY_STATE:
		{
			pOut = CGameplayState::GetInstance();
		}
		break;
	case ANIM_STATE:
		{
			pOut = CAnimDebugState::GetInstance();
		}
		break;
	case WIN_STATE:
		{
			pOut = WinState::GetInstance();
		}
		break;
	case LOSE_STATE:
		{
			pOut = LoseState::GetInstance();
		}
		break;
	case INTRO_SPLASH_STATE:
		{
			pOut = CIntroSplashState::GetInstance();
		}
		break;
	case TUTORIAL_STATE:
		{
			pOut = CTutorialMenu::GetInstance();
		}
		break;
	case TUTORIAL_GAMEPLAY_STATE:
		{
			pOut = CTutorialState::GetInstance();
		}
		break;
	case DIFFICULTY_STATE:
		{
			pOut = CDifficultyMenu::GetInstance();
		}
		break;
	case TUTORIAL_PAUSE_STATE:
		{
			pOut = CTutorialPause::GetInstance();
		}
		break;
	case TUTORIAL_MENU_STATE:
		{
			pOut = CTutorialMenu::GetInstance();
		}
		break;
	case HELP_STATE:
		{
			pOut = CHelpState::GetInstance();
		}
		break;
	case QUITGAME_STATE:
		{
			pOut = CQuitGameState::GetInstance();
		}
		break;
	}
	return pOut;
}
