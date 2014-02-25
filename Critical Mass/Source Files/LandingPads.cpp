#include "../Header Files/StdAfx.h"
#include "..\Header Files\LandingPads.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/ObjectFactory.h"
#include "../Header Files/CollisionLib.h"

CLandingPads::CLandingPads(void)
{
	CBaseEntity::SetType(ET_LANDINGPAD);

	// COLLISION 
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(1.0f, pos));
}

void CLandingPads::Initialize ( double _X, double _Y )
{
	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, -1.0f );

	this->SetIsActive(true);
	this->SetIsAlive(true);

	m_rotX = (float)_X;
	m_rotY = (float)_Y;

	//Calculation for the rotation of the Depot
	Vec3f fwd, up, right, final;
	//m_Fwd, m_Up, m_Right, m_Final
	Matrix3x3f rotMatrixY, rotMatrixX;

	glsRotationMatrix3x3( rotMatrixY, (float)glsDegToRad(_Y), 0.0f, 1.0f, 0.0f );
	glsRotateVector(fwd, this->GetRenderNode().GetForwardVector(), rotMatrixY );
	glsNormalizeVector3(fwd);
	GetRenderNode().SetForwardVector( fwd );

	glsRotationMatrix3x3( rotMatrixX, (float)glsDegToRad(_X), fwd[0], fwd[1], fwd[2] );
	glsRotateVector(up, this->GetRenderNode().GetUpVector(), rotMatrixX );
	glsNormalizeVector3(up);
	GetRenderNode().SetUpVector( up );

	glsCrossProduct3( right, fwd, up );
	glsNormalizeVector3(right);
	GetRenderNode().SetRightVector( right );

	Vec3f _moon;
	glsLoadVector3( _moon, 0.0f, 75.0f, 0.0f );

	glsScaleVector3( up, -75 );
	glsAddVectors3( final, up, _moon );
	
	//glsCopyVector3(((Sphere*)(this->GetCollisionEntity()))->m_Center, final);
	
	Vec3f id;
	glsLoadVector3( id, 0.0f, 0.0f, 0.0f );

	// Reorient ze matrix
	Vec3f newUp, newRight, newForward;
	glsSubtractVectors3( newUp, _moon, final );
	float length = glsGetVectorLength3( newUp );
	glsNormalizeVector3( newUp );
	GetRenderNode().SetUpVector( newUp );

	glsCrossProduct3( newForward, right, newUp );
	glsNormalizeVector3( newForward );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, newUp, newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );

	Vec3f tempUp;
	glsCopyVector3(tempUp, newUp);
	glsScaleVector3(tempUp, 0.5f);
	glsAddVectors3(m_Lowest, tempUp, final);
	glsCopyVector3(tempUp, newUp);
	glsScaleVector3(tempUp, -1.0);
	glsAddVectors3(m_Highest, tempUp, final);

	GetRenderNode().SetPosition( m_Lowest );
	glsCopyVector3(*this->GetPosition(), m_Lowest);
}
void CLandingPads::InitShaders()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[LANDING_PAD_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_LANDINGPAD).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_LANDINGPAD).m_hTextureSpecular );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle2( CAssetManager::GetInstance()->m_vTextureUnit2Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetLightPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT], 0 );
}
void CLandingPads::Release()
{
	CBaseEntity::Release();
}
bool CLandingPads::Update(float _elapsedTime)
{
	CBaseEntity::Update(_elapsedTime);
	return true;
}
void CLandingPads::HandleReaction( void )
{

}