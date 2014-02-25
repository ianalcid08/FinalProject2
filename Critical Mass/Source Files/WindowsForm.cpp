#include "../Header Files/stdafx.h"
#include "..\Header Files\WindowsForm.h"
//#include "Renderer.h"
#include "..\Header Files\IState.h"
#include "..\Header Files\GamePlayState.h"
#include "..\Header Files\MainMenuState.h"
#include "..\Header Files\AssetManager.h"
#include "..\Header Files\Input.h"
#include <iostream>
#include <fstream>

using namespace std;

CWindowsForm CWindowsForm::m_Instance;
//IState* ProcessEnum(eGameStates _eIn);

CWindowsForm::CWindowsForm()
{

}
CWindowsForm::~CWindowsForm()
{

}
CWindowsForm* CWindowsForm::GetInstance()
{
	return &m_Instance;
}
void CWindowsForm::Init( HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	m_ScreenWidth	= nScreenWidth;
	m_ScreenHeight = nScreenHeight;
	m_IsWindowed	= bIsWindowed;
	m_hWnd = hWnd;
	m_hInstance = hInstance;

	//get the screen width and height

	// init the renderer

	// Get the asset manager ready

	// timer
	//m_cTimer.Reset();

	// object factory
	
	// input getinstance
	// init the input


	// init the sound engine

	//push the intro / mainmenu / gameplay state

}
bool CWindowsForm::Input()
{
	if( !m_CurrState.size() )
		return false;

	//return m_CurrState.back()->Input();
	return true;
}
void CWindowsForm::Update()
{
	if( !m_CurrState.size() )
		return;

	//m_pInput->Frame();
	//m_pInput->ReadDevices();
	

	/*float fTime = m_cTimer.GetDeltaTime();
	m_cTimer.Update();

	if(fTime > .125f)
		fTime = .125f;

	m_fDelta = fTime;*/
	
	//m_CurrState.back()->Update(fTime);

	//update sound
	
}
void CWindowsForm::Render()
{
	//stuff
}
bool CWindowsForm::Play()
{
	if(Input() == false || m_IsRunning == false)
		return false;

	Update();
	Render();

	return true;
}
void CWindowsForm::Shutdown()
{
	// shutdown the states
	/*for(unsigned int i = 0; i < m_pCurrState.size(); ++i)
		m_pCurrState[i]->Shutdown();*/

	// shutdown asset manager
	//CAssetManager::GetInstance()->Shutdown();

	// shutdown renderer
	/*if(m_Renderer != nullptr)
	{
		m_Renderer = nullptr;
	}*/

	// shutdown input 
	/*if(m_Input)
		m_Input->Shutdown();*/

	// shutdown the sound engine
	//WwiseNS::TermSoundEngine();
}
//void CWindowsForm::ChangeState(eGameStates _eNewState)
//{
	// Exit the current state (if any)
	//if (m_CurrState.size() != 0)
	//{
	//	for (auto iter = m_CurrState.rbegin(); iter != m_CurrState.rend(); ++iter)
	//		(*iter)->Shutdown();
	//	m_CurrState.clear();
	//}

	//IState* pNewState = ProcessEnum(_eNewState);

	//// Assign the current state
	//m_CurrState.push_back(pNewState);

	//// Enter the new state (if any)
	//if (m_CurrState.back() != nullptr)
	//	m_CurrState.back()->Initialize();
//}

//void CWindowsForm::PushState(eGameStates _eNewState) 
//{
//	IState* pNewState = ProcessEnum(_eNewState);
//
//	for (auto iter = m_CurrState.begin(); iter != m_CurrState.end(); ++iter) 
//	{
//		if ((*iter) == pNewState) 
//		{
//			m_CurrState.erase(iter);
//			m_CurrState.push_back(pNewState);
//			return;
//		}
//	}
//	m_CurrState.push_back(pNewState);
//
//	// Enter the new state (if any)
//	if (m_CurrState.back() != nullptr)
//	{
//		//m_CurrState.back()->Initialize();
//	}
//}

//void CWindowsForm::PopState() 
//{
//	// If only 1 State is on the Stack dont remove it
//	if (m_CurrState.size() < 2)
//		return;
//
//	// Exit the current state
//	//m_CurrState.back()->Shutdown();
//
//	// Remove the Current state
//	m_CurrState.pop_back();
//}
