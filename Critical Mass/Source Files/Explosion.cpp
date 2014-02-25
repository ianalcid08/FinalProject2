#include "../Header Files/StdAfx.h"
#include "../Header Files/Explosion.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/GLSetup.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"

//Sound----------------------------------------------------
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Wwise_IDs.h"
//--------------------------------------------------------

#define EXPLOSION_TIMER_LIFE	0.5f
#define EXPLOSION_TIMER_FADE	0.2f
#define EXPLOSION_TIMER_EXPAND	0.18f

CExplosion::CExplosion(void)
{
	CBaseEntity::SetType(ET_EXPLOSION);

	//Collision Check
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(10.0f, pos));

	// Default Orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	m_Size = 1.0f;
}

// IEntity interface:
void CExplosion::Release ( void )
{
	CBaseEntity::Release();
}

bool CExplosion::Update (float _elapsedTime )
{
	CBaseEntity::Update(_elapsedTime);

	// Transparency for explosion, over time.
	Vec4f color;
	glsCopyVector4( color, GetRenderNode().GetColor() );
	color[3] = m_Life/(EXPLOSION_TIMER_LIFE-EXPLOSION_TIMER_FADE);
	GetRenderNode().SetColor( color );

	// Scaling for explosion, using the expand timer.
	if( !m_FinishedExpansion )
	{
		m_ExpandTimer -= _elapsedTime;
		if( m_ExpandTimer <= 0.0f)
		{
			m_FinishedExpansion = true;
			m_ExpandTimer = 0.0f;
		}
		Scale( ((float(EXPLOSION_TIMER_EXPAND)-m_ExpandTimer)/float(EXPLOSION_TIMER_EXPAND))*m_Size );
		((Sphere*)this->GetCollisionEntity())->m_Radius = m_Size*10.0f;
	}

	m_Life -= _elapsedTime;
	if (m_Life <= 0.0f)
	{
//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MORTAR_WHISTLE);
		this->SetIsActive(false);
		this->SetIsAlive(false);
		return false;
	}
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CExplosion::Scale( float size )
{
	// Default Orientation.
	GetRenderNode().SetUpVector( 0.0f, size, 0.0f );
	GetRenderNode().SetRightVector( size, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, size );
}

void CExplosion::HandleReaction ( void )
{

}

void CExplosion::Initialize ( Vec3f _position, Matrix4x4f _worldPosition )
{
	this->SetIsActive(true);
	this->SetIsAlive(true);
	this->SetPosition(_position);
	this->SetWorldPosition(_worldPosition);
	this->m_Life = EXPLOSION_TIMER_LIFE;
	this->m_ExpandTimer = EXPLOSION_TIMER_EXPAND;
	this->m_FinishedExpansion = false;
	std::cout << "SPLASH DAMAGE CREATED" << std::endl;
	
	int derp = rand()%3 +1;
	switch(derp)
	{
	case 1:
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_EXPLOSION_INTENSITY1);
		}
		break;
	case 2:
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_EXPLOSION_INTENSITY2);
		}
		break;
	case 3:
		{
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_EXPLOSION_INTENSITY3);
		}
		break;

	};
	//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_EXPLOSION);


	GetRenderNode().SetPosition( _position );

	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[EXPLOSION_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_EXPLOSION).m_DiffuseHandle );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
	GetRenderNode().SetEnableBlending( true );
}
