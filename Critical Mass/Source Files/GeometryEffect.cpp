/*///////////////////////////////////////////////////////
/Filename      : GeometryEffect.cpp
/Author 	   : Tom Stefl
/Date:		   : 8/8/2013
/Last Modified : 8/8/2013
/Purpose	   : This class will hold the necessary data
/				 for specific geometry effects in the game
/*////////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/GeometryEffect.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/BaseEntity.h"
#include "../Header Files/MovingEntity.h"


GeometryEffect::GeometryEffect(void)
{
	m_Position[0] = 0.0f;
	m_Position[1] = 0.0f;
	m_Position[2] = 0.0f;

	m_Velocity[0] = 0.0f;
	m_Velocity[1] = 0.0f;
	m_Velocity[2] = 0.0f;

	m_Acceleration[0] = 0.0f;
	m_Acceleration[1] = 0.0f;
	m_Acceleration[2] = 0.0f;

	//m_RenderNode.hShader = CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY];
	//m_RenderNode.hUniformMVP = CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY];
	//m_RenderNode.hUniformColor = CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY];
	//m_RenderNode.hTextureUnit0 = CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY];

	m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
	m_RenderNode.SetDiffuseTextureHandle(CAssetManager::GetInstance()->m_vTextureDiffuseHandles[ANIMATED_ONLY]);
	m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_ONLY] );
	m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_ONLY] );
	m_RenderNode.SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_ONLY] );
	m_RenderNode.SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_ONLY] );

	m_fxType = 0;
	m_animType = 0;
	m_IsActive = false;
	m_FXOpacity = 1.0f;
	//m_RenderNode = nullptr;
	glsLoadVector3(m_ClosestCratePos,1000.0f,1000.0f,1000.0f); //arbitrary high value for first time test

}


GeometryEffect::~GeometryEffect(void)
{

}

//////////////////////////////////////////////////
//This function sets the propeties for the effect
//ENUMFXTYPE: Which effect you want. See Defs.h
//ENUMANIMTYEP: what kind of animation you want.
void GeometryEffect::InitializeEffect(int _enumFXType, int _enumAnimType, float _lifeSpan, const Vec3f _postion, IEntity* _entity)
{
	m_Velocity[0] = 0.0f;
	m_Velocity[1] = 0.0f;
	m_Velocity[2] = 0.0f;

	m_Acceleration[0] = 0.0f;
	m_Acceleration[1] = 0.0f;
	m_Acceleration[2] = 0.0f;

	m_FXOpacity = 1.0f;

	m_IsActive = true;
	//((CPlayer*)m_Player)->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_WALK));

	//SET THE EFFECTS POSITION
	glsCopyVector3(m_Position,_postion);
	//THE THE EFFECTS RENDER NODE'S POSITIONS
	m_RenderNode.SetPosition( _postion );


	m_fxType = _enumFXType;
	m_animType = _enumAnimType;
	m_SourceEntity = _entity;

	//Orient the render node to the passed in entity;
	m_RenderNode.SetUpVector( ((CBaseEntity*)_entity)->GetRenderNode().GetUpVector() );
	m_RenderNode.SetForwardVector( ((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector() );
	m_RenderNode.SetRightVector( ((CBaseEntity*)_entity)->GetRenderNode().GetRightVector() );


	//grab the information from the the asset manager
	m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetFXData(_enumFXType)->m_DiffuseHandle );


	switch(m_fxType)
	{
	case GEO_BEACON:
		{
			m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
			m_RenderNode.SetColorUniformHandle(CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_ONLY]);
			m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_ONLY] );
			m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_ONLY] );
			m_RenderNode.SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_ONLY] );

			//m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetFXData(GEO_BEACON)->m_DiffuseHandle );

			m_RenderNode.SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[GEO_BEACON_TB] );
			
			if(m_SourceEntity->GetType() == ET_CRATE)
				m_RenderNode.SetColor(0.0f,1.0f,0.0f,0.8f);
			else
				m_RenderNode.SetColor(1.0f,0.0f,0.0f,0.8f);

			m_RenderNode.SetEnableBlending( true );
		}
		break;
	case GEO_QUAD_PLAYER_MUZZLE:
		{
			m_RenderNode.SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[GEO_QUAD_TB] );
			m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
			m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
			m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
			m_RenderNode.SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
			m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetTexture(MUZZLEFLASH1) );

			m_RenderNode.SetUpVector( ((CBaseEntity*)_entity)->GetRenderNode().GetUpVector() );
			m_RenderNode.SetForwardVector( ((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector() );
			m_RenderNode.SetRightVector( ((CBaseEntity*)_entity)->GetRenderNode().GetRightVector() );

			m_FXOpacity = 1.0f;
			m_RenderNode.SetEnableBlending( true );
			m_RenderNode.SetColor(1.0f,1.0f,1.0f,m_FXOpacity);
		}
		break;
	case GEO_QUAD_MED_MUZZLE:
		{
			m_RenderNode.SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[GEO_QUAD_TB] );
			m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
			m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
			m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
			m_RenderNode.SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
			m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetTexture(MUZZLEFLASH1) );

			m_RenderNode.SetUpVector( ((CBaseEntity*)_entity)->GetRenderNode().GetUpVector() );
			m_RenderNode.SetForwardVector( ((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector() );
			m_RenderNode.SetRightVector( ((CBaseEntity*)_entity)->GetRenderNode().GetRightVector() );

			m_FXOpacity = 1.0f;
			m_RenderNode.SetEnableBlending( true );
			m_RenderNode.SetColor(1.0f,1.0f,1.0f,m_FXOpacity);
		}
		break;
	case GEO_QUAD_BULLET_TRAIL:
		{
			m_RenderNode.SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[GEO_QUAD_TB] );
			m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
			m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
			m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
			m_RenderNode.SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
			m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetTexture(BULLET_TRAIL) );

			m_RenderNode.SetUpVector( ((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector() );
			m_RenderNode.SetForwardVector( ((CBaseEntity*)_entity)->GetRenderNode().GetUpVector() );
			m_RenderNode.SetRightVector( ((CBaseEntity*)_entity)->GetRenderNode().GetRightVector() );

			m_FXOpacity = 1.0f;
			m_RenderNode.SetEnableBlending( true );
			m_RenderNode.SetColor(1.0f,1.0f,1.0f,m_FXOpacity);

			Vec3f temp;
			glsCopyVector3(temp,m_RenderNode.GetUpVector());
			glsScaleVector3(temp,0.35f);
			m_RenderNode.SetUpVector(temp);

			glsCopyVector3(temp,m_RenderNode.GetForwardVector());
			glsScaleVector3(temp,0.35f);
			m_RenderNode.SetForwardVector(temp);

			glsCopyVector3(temp,m_RenderNode.GetRightVector());
			glsScaleVector3(temp,0.35f);
			m_RenderNode.SetRightVector(temp);
		}
		break;
	case GEO_QUAD_BLOOD_POOL:
		{
			m_FXOpacity = 1.0f;
			m_RenderNode.SetEnableBlending( true );
			m_RenderNode.SetColor(1.0f,1.0f,1.0f,0.8f);

			m_RenderNode.SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[GEO_QUAD_TB] );
			m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
			m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
			m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
			m_RenderNode.SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
			m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetTexture(BLOOD_POOL) );

			m_RenderNode.SetUpVector( ((CBaseEntity*)_entity)->GetRenderNode().GetUpVector() );
			m_RenderNode.SetForwardVector( ((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector() );
			m_RenderNode.SetRightVector( ((CBaseEntity*)_entity)->GetRenderNode().GetRightVector() );

			m_BloodTimer = 2.0f;
		}
		break;

		case GEO_QUAD_HP_ARROW:
		{
			m_FXOpacity = 0.0f;
			m_FlashIncrease = true;
			m_RenderNode.SetEnableBlending( true );
			m_RenderNode.SetColor(1.0f,1.0f,1.0f,0.8f);

			m_RenderNode.SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[GEO_QUAD_TB] );
			m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
			m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
			m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
			m_RenderNode.SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
			m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetTexture(HP_ARROW) );

			m_RenderNode.SetUpVector( ((CBaseEntity*)_entity)->GetRenderNode().GetUpVector() );
			m_RenderNode.SetForwardVector( ((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector() );
			m_RenderNode.SetRightVector( ((CBaseEntity*)_entity)->GetRenderNode().GetRightVector() );

			//m_BloodTimer = 2.0f;
		}
		break;
		case GEO_QUAD_ENERGY_RING:
		{
			m_FXOpacity = 1.0f;
			//m_FlashIncrease = true;
			m_RenderNode.SetEnableBlending( true );
			m_RenderNode.SetColor(1.0f,1.0f,1.0f,1.0f);

			m_RenderNode.SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[GEO_QUAD_TB] );
			m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
			m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
			m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
			m_RenderNode.SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
			m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetTexture(ENERGY_RING) );

			m_RenderNode.SetUpVector( ((CBaseEntity*)_entity)->GetRenderNode().GetUpVector() );
			m_RenderNode.SetForwardVector( ((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector() );
			m_RenderNode.SetRightVector( ((CBaseEntity*)_entity)->GetRenderNode().GetRightVector() );

			Vec3f temp1;
			Vec3f worldCenter = {0.0f,75.0f,0.0f};
			glsSubtractVectors3(temp1,m_RenderNode.GetPosition(), worldCenter);
			glsNormalizeVector3(temp1);
			glsScaleVector3(temp1,2.0f);
			glsAddVectors3(temp1,temp1, m_RenderNode.GetPosition());
			m_RenderNode.SetPosition(temp1);

			Vec3f vel;
			glsSubtractVectors3(vel,m_RenderNode.GetPosition(), worldCenter);
			glsNormalizeVector3(vel);
			glsScaleVector3(vel,10.0f);
			SetVelocity(vel);


			m_BloodTimer = 1.55f;
		}
		break;
	};


	if(m_animType != -1)
	{
		m_AnimManager.SwitchAnimation(CAssetManager::GetInstance()->GetFXAnimation(_enumFXType,_enumAnimType), 0.0f);
	}
}

void GeometryEffect::Update(float _fElapsedTime)
{
		//Update velocity based on acceleration
		m_Velocity[0] += m_Acceleration[0] * _fElapsedTime;
		m_Velocity[1] += m_Acceleration[1] * _fElapsedTime;
		m_Velocity[2] += m_Acceleration[2] * _fElapsedTime;

		//Update Position based on Velocity
		m_Position[0] += m_Velocity[0];
		m_Position[1] += m_Velocity[1];
		m_Position[2] += m_Velocity[2];

		if(m_fxType != GEO_QUAD_ENERGY_RING)
		{
			m_RenderNode.SetPosition( m_Position );
		}

		switch(m_fxType)
		{
		case GEO_BEACON:
			{
				m_RenderNode.SetUpVector( ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector() );
				m_RenderNode.SetForwardVector( ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector() );
				m_RenderNode.SetRightVector( ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetRightVector() );
				//m_RenderNode.SetColor(0.0f,1.0f,0.0f,0.8f);
			}
			break;
		case GEO_QUAD_PLAYER_MUZZLE:
			{
				if(m_FXOpacity > 0.0f)
					m_FXOpacity -= 10.0f * _fElapsedTime;

				m_RenderNode.SetColor(1.0f,1.0f,1.0f,m_FXOpacity);
				//Scale(0.5f);

				m_RenderNode.SetUpVector( ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector() );			// up, up
				m_RenderNode.SetForwardVector( ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetRightVector() );	// forward, right
				Vec3f right;
				glsCopyVector3( right, ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector() );
				glsScaleVector3(right, -1.0f);
				m_RenderNode.SetRightVector( right );																// right, forward

				m_RenderNode.SetPosition(((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition());

				Vec3f temp;
				glsCopyVector3(temp,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector());
				glsScaleVector3(temp,-5.0f);
				glsAddVectors3(temp,m_RenderNode.GetPosition(),temp);
				m_RenderNode.SetPosition(temp);

				glsCopyVector3(temp,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector());
				glsScaleVector3(temp,8.5f);
				glsAddVectors3(temp,m_RenderNode.GetPosition(),temp);
				m_RenderNode.SetPosition(temp);

				glsCopyVector3(temp,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetRightVector());
				glsScaleVector3(temp,-1.5f);
				glsAddVectors3(temp,m_RenderNode.GetPosition(),temp);
				m_RenderNode.SetPosition(temp);


				//This is to scale the muzzle flash
				glsCopyVector3(temp,m_RenderNode.GetUpVector());
				glsScaleVector3(temp,0.5f);
				m_RenderNode.SetUpVector(temp);

				glsCopyVector3(temp,m_RenderNode.GetForwardVector());
				glsScaleVector3(temp,0.5f);
				m_RenderNode.SetForwardVector(temp);

				glsCopyVector3(temp,m_RenderNode.GetRightVector());
				glsScaleVector3(temp,0.5f);
				m_RenderNode.SetRightVector(temp);
			}
			break;
		case GEO_QUAD_MED_MUZZLE:
			{
				if(m_FXOpacity > 0.0f)
					m_FXOpacity -= 5.0f * _fElapsedTime;

				if(m_FXOpacity <= 0.0f)
					this->SetIsActive(false);

				m_RenderNode.SetColor(1.0f,1.0f,1.0f,m_FXOpacity);

				m_RenderNode.SetUpVector( ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector() );			// up, up
				m_RenderNode.SetForwardVector( ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetRightVector() );	// forward, right
				Vec3f right;
				glsCopyVector3( right, ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector() );
				glsScaleVector3(right, -1.0f);
				m_RenderNode.SetRightVector( right );																// right, forward

				m_RenderNode.SetPosition(((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition());

				Vec3f temp;
				glsCopyVector3(temp,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector());
				glsScaleVector3(temp,-6.0f);
				glsAddVectors3(temp,m_RenderNode.GetPosition(),temp);
				m_RenderNode.SetPosition(temp);

				glsCopyVector3(temp,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector());
				glsScaleVector3(temp,10.0f);
				glsAddVectors3(temp,m_RenderNode.GetPosition(),temp);
				m_RenderNode.SetPosition(temp);

				//This is to scale the muzzle flash
				glsCopyVector3(temp,m_RenderNode.GetUpVector());
				glsScaleVector3(temp,0.5f);
				m_RenderNode.SetUpVector(temp);

				glsCopyVector3(temp,m_RenderNode.GetForwardVector());
				glsScaleVector3(temp,0.5f);
				m_RenderNode.SetForwardVector(temp);

				glsCopyVector3(temp,m_RenderNode.GetRightVector());
				glsScaleVector3(temp,0.5f);
				m_RenderNode.SetRightVector(temp);
			}
			break;
		case GEO_QUAD_BULLET_TRAIL:
			{
				if(m_FXOpacity > 0.0f)
					m_FXOpacity -=  _fElapsedTime;

				if(m_FXOpacity <= 0.0f)
					this->SetIsActive(false);

				m_RenderNode.SetColor(1.0f,1.0f,1.0f,m_FXOpacity);

				Vec3f temp;
				glsCopyVector3(temp,m_RenderNode.GetUpVector());
				glsScaleVector3(temp,m_FXOpacity);
				m_RenderNode.SetUpVector(temp);

				glsCopyVector3(temp,m_RenderNode.GetForwardVector());
				glsScaleVector3(temp,m_FXOpacity);
				m_RenderNode.SetForwardVector(temp);

				glsCopyVector3(temp,m_RenderNode.GetRightVector());
				glsScaleVector3(temp,m_FXOpacity);
				m_RenderNode.SetRightVector(temp);

			}
			break;
		case GEO_QUAD_BLOOD_POOL:
			{		
				if(m_BloodTimer >= 0.0f)
				{
					m_BloodTimer-= _fElapsedTime;
					m_FXOpacity += 0.01f * _fElapsedTime;

					Vec3f temp;
					glsCopyVector3(temp,m_RenderNode.GetUpVector());
					glsScaleVector3(temp,m_FXOpacity);
					m_RenderNode.SetUpVector(temp);

					glsCopyVector3(temp,m_RenderNode.GetForwardVector());
					glsScaleVector3(temp,m_FXOpacity);
					m_RenderNode.SetForwardVector(temp);

					glsCopyVector3(temp,m_RenderNode.GetRightVector());
					glsScaleVector3(temp,m_FXOpacity);
					m_RenderNode.SetRightVector(temp);
				}
			}
			break;
		case GEO_QUAD_HP_ARROW:
			{
				if(IsActive())
				{
					if(m_FXOpacity < 1.0f && m_FlashIncrease )
					{
						m_FXOpacity += _fElapsedTime;
						Vec4f color;
						glsLoadVector4(color,1.0f,1.0f,1.0f,m_FXOpacity);
						m_RenderNode.SetColor(color);
					}
					else if(m_FXOpacity >= 1.0f && m_FlashIncrease)
					{
						m_FlashIncrease = false;
					}

					if(m_FXOpacity > 0.0f && !m_FlashIncrease )
					{
						m_FXOpacity -= _fElapsedTime;
						Vec4f color;
						glsLoadVector4(color,1.0f,1.0f,1.0f,m_FXOpacity);
						m_RenderNode.SetColor(color);
					}
					else if(m_FXOpacity <= 0.0f && !m_FlashIncrease)
					{
						m_FlashIncrease = true;
					}

					m_RenderNode.SetPosition(((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition());

					Vec3f planetCenter = {0.0f,75.0f,0.0f};
					Vec3f A;
					glsSubtractVectors3(A,m_ClosestCratePos,planetCenter);

					Vec3f N;
					glsCopyVector3(N,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector() );

					float AdotN = glsDotProduct3(A,N);
					Vec3f B;
					//Vec3f B = A - (AdotN) * N;
					glsScaleVector3(N,AdotN);

					glsSubtractVectors3(B,A,N);

					m_RenderNode.SetPosition(*((CBaseEntity*)m_SourceEntity)->GetPosition());

					m_RenderNode.SetUpVector( ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector() );

					glsNormalizeVector3(B);
					m_RenderNode.SetForwardVector( B );

					Vec3f tempRight;
					glsCrossProduct3(tempRight,m_RenderNode.GetUpVector(),B);				// Use the cross product in order to find the right vector.
					glsNormalizeVector3(tempRight);
					m_RenderNode.SetRightVector(tempRight);

					Vec3f temp;
					glsCopyVector3(temp,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector());
					glsScaleVector3(temp,-4.0f);
					glsAddVectors3(temp,m_RenderNode.GetPosition(),temp);
					m_RenderNode.SetPosition(temp);
				}
			}
			break;
		case GEO_QUAD_ENERGY_RING:
			{
				m_BloodTimer -= _fElapsedTime;

				if(m_BloodTimer <= 0.0f)
				{
					SetIsActive(false);
				}

				if(m_BloodTimer <= 1.0f)
				{
					m_RenderNode.SetColor(1.0f,1.0f,1.0f,m_BloodTimer);
				}
				//if(m_BloodTimer >= 0.775f)
				//{
					m_FXOpacity += 30.0f *  _fElapsedTime;
				//}
				//else
				//{
					//m_FXOpacity -=  30.0f * _fElapsedTime;
				//}

				//m_FXOpacity = 12.0f;
				//normalize all of the vectors
				Vec3f temp;
				glsCopyVector3(temp, m_RenderNode.GetUpVector());
				glsNormalizeVector3(temp);
				m_RenderNode.SetUpVector(temp);

				glsCopyVector3(temp, m_RenderNode.GetForwardVector());
				glsNormalizeVector3(temp);
				m_RenderNode.SetForwardVector(temp);

				glsCopyVector3(temp, m_RenderNode.GetRightVector());
				glsNormalizeVector3(temp);
				m_RenderNode.SetRightVector(temp);

				////adjust pos
				//Vec3f tempUp;
				//glsCopyVector3(tempUp, m_RenderNode.GetUpVector());
				//glsScaleVector3(tempUp,1.5f);
				//glsAddVectors3(tempUp,tempUp,m_RenderNode.GetPosition());
				//m_RenderNode.SetPosition(tempUp);


				// adjust scale
				Vec3f temp2;
				glsCopyVector3(temp2,m_RenderNode.GetUpVector());
				glsScaleVector3(temp2,m_FXOpacity);
				m_RenderNode.SetUpVector(temp2);

				glsCopyVector3(temp2,m_RenderNode.GetForwardVector());
				glsScaleVector3(temp2,m_FXOpacity);
				m_RenderNode.SetForwardVector(temp2);

				glsCopyVector3(temp2,m_RenderNode.GetRightVector());
				glsScaleVector3(temp2,m_FXOpacity);
				m_RenderNode.SetRightVector(temp2);
			}
			break;
		};


		if(m_animType != -1)
		{
			m_AnimManager.Update(_fElapsedTime,&m_RenderNode);
		}

}

void GeometryEffect::Render()
{
	//specific render calls will be made where when changes to Rendernodes behaviors have made
}

void GeometryEffect::SetPosition(const Vec3f _newPos)
{
	glsCopyVector3(m_Position,_newPos);
}

void GeometryEffect::SetVelocity(Vec3f _newVelocity)
{
	glsCopyVector3(m_Velocity,_newVelocity);
}

void GeometryEffect::SetAcceleration(Vec3f _newAcceleration)
{
	glsCopyVector3(m_Acceleration,_newAcceleration);
}

void GeometryEffect::BuildGeoFXGeometry()
{
	

}

void GeometryEffect::Scale( float size )
{
	Vec3f tempUp, tempRight, tempForward;
	glsCopyVector3(tempUp, ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector());
	glsCopyVector3(tempRight, ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetRightVector());
	glsCopyVector3(tempForward, ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector());

	// Default Orientation.
	//glsScaleVector3(tempUp, size/4.0f);
	glsScaleVector3(tempRight, size);
	glsScaleVector3(tempForward, size);

	GetRenderNode()->SetUpVector(tempUp);
	GetRenderNode()->SetRightVector(tempRight);
	GetRenderNode()->SetForwardVector(tempForward);
}

void GeometryEffect::FindClosestCrate(IEntity* _crate)
{
	if(((CMovingEntity*)_crate)->GetIsActive() && ((CMovingEntity*)_crate)->GetIsAlive())
	{
		Vec3f tempDistNew;
		glsSubtractVectors3(tempDistNew,((CMovingEntity*)_crate)->GetRenderNode().GetPosition(),((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition());
		Vec3f tempDistCurr;
		glsSubtractVectors3(tempDistCurr,m_ClosestCratePos,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition());


		//m_ClosestCratePos 

		float currentLen = glsGetVectorLength3(tempDistCurr);
		float newLen = glsGetVectorLength3(tempDistNew);

		if( newLen < currentLen )  // if the new found distance is shorter than the old one set it to the shorter distance
		{
			glsCopyVector3(m_ClosestCratePos,((CMovingEntity*)_crate)->GetRenderNode().GetPosition());
		}
	}
}