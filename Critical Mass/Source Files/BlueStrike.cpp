#include "../Header Files/stdafx.h"
#include "../Header Files/BlueStrike.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"

CBlueStrike::CBlueStrike( void )
{
	this->SetType(ET_BLUESTRIKE);

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(2.0f, pos));

	Reset();
}

void CBlueStrike::Enter( const Vec3f position , bool _isCrate )
{
	this->m_IsCrate = _isCrate;

	GetRenderNode().SetPosition(position);

	// Orient based on planets surface, so it's RESTING on the surface of the planetoid.
	Vec3f planet = { 0.0f, 75.0f, 0.0f };
	Vec3f newRight, newUp, newForward;
	glsSubtractVectors3( newUp, planet, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( newUp );
	glsNormalizeVector3( newUp );

	// CLAMP TO THE PLANET!
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
		glsCopyVector3( move, newUp );
		glsScaleVector3( move, off );
		glsAddVectors3( newPos, pos, move );
		GetRenderNode().SetPosition( newPos );
	}

	glsCrossProduct3( newForward, this->GetRenderNode().GetRightVector(), newUp );
	glsNormalizeVector3( newForward );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, newUp, newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );

	glsScaleVector3( newUp, 10.0f );
	GetRenderNode().SetUpVector( newUp );
		
	if(_isCrate)
	{
		Vec3f right;
		GetRenderNode().SetColor(0.0f,1.0f,0.0f,1.0f);
		glsCopyVector3( right, GetRenderNode().GetRightVector() );
		glsScaleVector3(right,10.0f);

		GetRenderNode().SetRightVector( right );

		Vec3f forward;
		glsCopyVector3( forward, GetRenderNode().GetForwardVector() );
		glsScaleVector3(forward, 10.0f);

		GetRenderNode().SetForwardVector( forward );

		glsNormalizeVector3(right);
		Vec3f newPos;
		glsCopyVector3(newPos, GetRenderNode().GetPosition());
		glsScaleVector3(right, -3.0f);
		glsAddVectors3(newPos, newPos, right);
		GetRenderNode().SetPosition(newPos);
	}
	else
	{
		Vec3f right;
		GetRenderNode().SetColor(1.0f,1.0f,0.0f,1.0f);
		glsCopyVector3( right, GetRenderNode().GetRightVector() );
		glsScaleVector3(right,2.0f);
		GetRenderNode().SetRightVector( right );
	}
}

void CBlueStrike::Init( void )
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[BLUE_STRIKE_TB] );

	//Tom is changing this to be random
	//GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetObjData(ET_SHIELD).m_DiffuseHandle );
	int random = rand()%3+1;
	switch(random)
	{
	case 1:
		{
			GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetTexture(BOLT_1) );
		}
		break;
	case 2:
		{
			GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetTexture(BOLT_2) );
		}
		break;
	case 3:
		{
			GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetTexture(BOLT_2) );
		}
		break;
	};



	GetRenderNode().SetColor(1.0f,1.0f,0.0f,1.0f);

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	//GetRenderNode().SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[TEX_POINT_LIGHT] );
	//GetRenderNode().SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
	//GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_POINT_LIGHT] );
	//GetRenderNode().SetTextureUnitHandle2( CAssetManager::GetInstance()->m_vTextureUnit2Handles[TEX_POINT_LIGHT] );
	//GetRenderNode().SetLightPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT], 0 );
	GetRenderNode().SetEnableBlending( true );

	Reset();
}

void CBlueStrike::Reset( void )
{
	m_FadeTimer = BLUE_STRIKE_TIMER_FADE;
}

void CBlueStrike::Release( void )
{
	CBaseEntity::Release();
}

bool CBlueStrike::Update( float _elapsedTime )
{
	if( m_FadeTimer > 0.0f )
	{
		m_FadeTimer -= _elapsedTime;
		Vec4f color;
		glsCopyVector4( color, GetRenderNode().GetColor() );
		color[3] = m_FadeTimer/BLUE_STRIKE_TIMER_FADE;
		GetRenderNode().SetColor( color );
	}
	else
	{
		this->SetIsActive(false);
		this->SetIsAlive(false);

		Reset();
		return false;
	}
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CBlueStrike::HandleReaction( void )
{

}
