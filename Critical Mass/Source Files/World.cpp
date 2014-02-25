#include "../Header Files/StdAfx.h"
#include "..\Header Files\World.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"

CWorld::CWorld(void)
{
	this->SetType(ET_WORLD);

	// Collision shit
	Vec3f posOffset;
	glsLoadVector3( posOffset, 0.0f, 0.0f, 0.0f );
	this->SetCollisionEntity(new Sphere(73.0f, posOffset));

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );
}
void CWorld::Init()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[PLANET_SHAPE_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_WORLD).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_WORLD).m_hTextureSpecular );
	GetRenderNode().SetEmissiveTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_WORLD).m_hTextureEmissive );
	GetRenderNode().SetNormalTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_WORLD).m_hTextureNormal );
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
// IEntity interface:
void CWorld::Release( void )
{

}
bool CWorld::Update( float _elapsedTime )
{
	glsCopyVector3( ((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition() );
	return true;
}

void CWorld::HandleReaction( void )
{

}
