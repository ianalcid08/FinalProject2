#include "../Header Files/StdAfx.h"
#include "../Header Files/AnimDebugState.h"
#include "../Header Files/Camera.h"
#include "../Header Files/Game.h"
#include "../Header Files/AssetManager.h"

CAnimDebugState::CAnimDebugState()
{
	this->SetClosing(false);
}

CAnimDebugState::~CAnimDebugState()
{
	//this->SetClosing(false);

}

CAnimDebugState* CAnimDebugState::GetInstance(void)
{
	static CAnimDebugState s_Instance;
	return &s_Instance;
}

void CAnimDebugState::Init(CGame* _game)
{
	m_Renderer = _game->GetRenderer();

	//CSoundManager::GetInstance()->Pause();
	m_RenderBones = false;
	
	m_Keyframe = 0;

	m_Camera.UnAttach();
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	m_Camera.SetPosition(pos);

	m_OM = _game->GetOM();
	m_OF = _game->GetOF();
	m_SM = _game->GetStateManager();
}

void CAnimDebugState::Enter()
{
	m_OF->Create( ET_MEDIUMENEMY, &m_Animation );
	

	((CMediumEnemy*)m_Animation)->SetState(Enemy_Chasing);
}

void CAnimDebugState::Exit()
{
	m_OM->RemoveAllEntities();
	m_OF->ProcessDestroy();
}

void CAnimDebugState::ChangeKeyframe(unsigned int _Index)
{
	/*for(unsigned int i = 0; i < m_JointNodes.size(); i++)
	{
		glsLoadVector3(m_JointNodes[i]->m_vPosition, m_PlayerProcess.GetCurrentKeyFrame()[i].worldXForm[12], 
						(-m_PlayerProcess.GetCurrentKeyFrame()[i].worldXForm[13])+2.0f,
						(m_PlayerProcess.GetCurrentKeyFrame()[i].worldXForm[14])-15.0f);

		glsLoadVector3(m_JointNodes[i]->m_color, 0.5f, 0.8f, 0.75f );
	}*/
}

void CAnimDebugState::Shutdown()
{
	// Nothing to shutdown.
	//for(unsigned int i = 0; i < m_JointNodes.size(); i++)
	//	delete m_JointNodes[i];

	m_JointNodes.clear();
}

bool CAnimDebugState::Input( CInput& _input )
{
	m_Camera.Input(_input);

	static bool buffer = false;
	if(_input.KeyDown(DIK_Q))
	{
		if(!buffer)
		{
			m_RenderBones = !m_RenderBones;
			buffer = true;
		}
	}
	else
		buffer = false;

	if(_input.KeyDown(DIK_RETURN))
		m_SM->ChangeState(MAINMENU_STATE);

	static bool Pressed1 = true;
	if(_input.KeyDown(DIK_1))
	{
		if(!Pressed1)
		{
			((CLivingEntity*)m_Animation)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.5f);
			Pressed1 = true;
		}
	}
	else
		Pressed1 = false;

	static bool Pressed2 = true;
	if(_input.KeyDown(DIK_2))
	{
		if(!Pressed2)
		{
			((CLivingEntity*)m_Animation)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK), 0.5f);
			Pressed2 = true;
		}
	}
	else
		Pressed2 = false;

	static bool Pressed3 = true;
	if(_input.KeyDown(DIK_3))
	{
		if(!Pressed3)
		{
			((CLivingEntity*)m_Animation)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_DEATH), 0.5f);
			Pressed3 = true;
		}
	}
	else
		Pressed3 = false;

	static bool buffer2 = false;
	if(_input.KeyDown(DIK_SPACE))
	{
		if(!buffer2)
		{
			/*m_Keyframe++;

			if(m_Keyframe > m_PlayerAnim.GetJoints()[0].m_KeyFrames.size()-1) 
				m_Keyframe = 0;

			ChangeKeyframe(m_Keyframe);*/
			buffer2 = true;
		}
	}
	else
		buffer2 = false;

	return true;
}

void CAnimDebugState::Update(float _fElapsedTime)
{
	m_OM->UpdateEntities(_fElapsedTime);
	m_OF->ProcessDestroy();
	
	m_Camera.Update(_fElapsedTime);
	m_ElapsedTime = _fElapsedTime;

	m_Renderer->Update(*m_Camera.GetPosition(), *m_Camera.GetOrientation(), m_ElapsedTime);
}

void CAnimDebugState::Render()
{
	m_OM->RenderEntities();
}
