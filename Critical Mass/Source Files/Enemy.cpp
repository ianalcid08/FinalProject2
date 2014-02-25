#include "../Header Files/stdafx.h"
#include "../Header Files/Enemy.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/CollisionLib.h"

CEnemy::CEnemy(void)
{
	m_Target = nullptr;
	m_State = Enemy_Idle;
	m_id = 1;
	m_IsPaused = false;
	m_LaserDeath = false;
}

void CEnemy::Release(void)
{
	m_Target = nullptr;
	m_Tree = nullptr;
}

bool CEnemy::Update(float _elapsedTime)
{
	CLivingEntity::Update(_elapsedTime);
	glsLoadVector3(*this->GetVelocity(), 0.0f, 0.0f, 0.0f);
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CEnemy::HandleReaction(void)
{

}

void CEnemy::Reorient_UsingForward(void)
{
	Vec3f forward, right;
	glsCopyVector3( forward, GetRenderNode().GetForwardVector() );
	glsNormalizeVector3( forward );
	GetRenderNode().SetForwardVector( forward );

	glsCrossProduct3( right, this->GetRenderNode().GetUpVector(), forward );
	glsNormalizeVector3( right );
	GetRenderNode().SetRightVector( right );
}