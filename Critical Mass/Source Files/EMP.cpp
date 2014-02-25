/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : EMP.h
Author : Evan Wright

Description : The mortar shot in the game. It should paint a target on the ground that will cause
			  an EMP explosion to happen after a set interval.

Created :  08/15/2013
Modified : 08/15/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/EMP.h"

#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/CollisionLib.h"

CEMP::CEMP(void)
{
	this->SetType(ET_EMP);

	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector(	1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// COLLISION DATA
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(3.0f, pos));

	glsLoadVector3(m_ColorStart,1.0f,0.0f,0.0f);
	glsLoadVector3(m_ColorEnd,0.0f,1.0f,0.0f);
}

CEMP::~CEMP(void)
{

}

void CEMP::Enter(float _impactTimer, Vec3f _position)
{
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector(	1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	m_ImpactTimer = _impactTimer;
	m_StartTimer = m_ImpactTimer;

	GetRenderNode().SetPosition( _position );

	// Orient based on planets surface, so it's RESTING on the surface of the planetoid.
	Vec3f planet = { 0.0f, 75.0f, 0.0f };

	Vec3f up, right, forward, pos;
	glsSubtractVectors3( up, planet, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( up );
	glsNormalizeVector3( up );
	GetRenderNode().SetUpVector( up );

	float off = 0.0f;
	if( length < 75.0f )
		off = length - 75;
	else if(length > 75.0f)
		off = length - 75;

	if(off != 0.0f)
	{
		Vec3f pos, newPos;
		Vec3f move;
		glsCopyVector3( pos, this->GetRenderNode().GetPosition() );
		glsCopyVector3( move, this->GetRenderNode().GetUpVector() );
		glsScaleVector3( move, off );
		glsAddVectors3( newPos, pos, move );
		GetRenderNode().SetPosition( newPos );
	}

	glsCrossProduct3( forward, this->GetRenderNode().GetRightVector(), up );
	glsNormalizeVector3( forward );
	GetRenderNode().SetForwardVector( forward );

	glsCrossProduct3( right, up, forward );
	glsCopyVector3( right, GetRenderNode().GetRightVector() );
	glsNormalizeVector3( right );
	GetRenderNode().SetRightVector( right );

	// Save start position as the position adjusted to the planets surface.
	glsCopyVector3(m_StartPos,this->GetRenderNode().GetPosition());

	// Now move it a bit off the ground, so the base of the mesh is touching the ground.
	Vec3f tempUp;
	glsCopyVector3(tempUp,up);
	glsScaleVector3(tempUp,-0.0f);
	glsAddVectors3(pos,this->GetRenderNode().GetPosition(),tempUp);
	GetRenderNode().SetPosition( pos );

	// Random spin direction, clockwise->counter-clockwise.
	if(rand()%2 == 0)
		m_RotDir = -1.0f;
	else
		m_RotDir = 1.0f;
}

void CEMP::Init(void)
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[EMP_TARGETER_TB] );
	GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetObjData(ET_SHIELD).m_DiffuseHandle );
	GetRenderNode().SetColor(1.0f,0.0f,0.0f,1.0f);

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	//GetRenderNode().SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[TEX_POINT_LIGHT] );
	//GetRenderNode().SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
	//GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_POINT_LIGHT] );
	//GetRenderNode().SetTextureUnitHandle2( CAssetManager::GetInstance()->m_vTextureUnit2Handles[TEX_POINT_LIGHT] );
	//GetRenderNode().SetLightPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT], 0 );
}

// IEntity interface:
void CEMP::Release( void )
{

}

bool CEMP::Update( float _elapsedTime )
{
	m_ImpactTimer -= _elapsedTime;
	if(m_ImpactTimer <= 0.0f)
	{	
		GetRenderNode().SetPosition(m_StartPos);
		CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_SMALLENEMY,true));
		this->SetIsActive(false);
		this->SetIsAlive(false);
		return false;
	}

	Vec3f color;
	//(start + percent*(end - start))
	Vec3f percent;
	glsSubtractVectors3(percent,m_ColorEnd, m_ColorStart);
	glsScaleVector3(percent, m_ImpactTimer/m_StartTimer);
	glsAddVectors3(color, m_ColorStart, percent);
	GetRenderNode().SetColor(color[0],color[1],color[2],1.0f);

	// Maybe make the reticle slowly rotate? That be cool.
	float rate = 1.0f*m_RotDir;
	Vec3f newForward, newRight;
	Matrix3x3f rotMatrix;
	glsRotationMatrix3x3( rotMatrix, _elapsedTime*rate, this->GetRenderNode().GetUpVector()[0], this->GetRenderNode().GetUpVector()[1], this->GetRenderNode().GetUpVector()[2] );
	glsRotateVector( newForward, this->GetRenderNode().GetForwardVector(), rotMatrix );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, this->GetRenderNode().GetUpVector(), newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CEMP::HandleReaction( void )
{

}
