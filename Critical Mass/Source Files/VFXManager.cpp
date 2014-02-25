/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : VFXManager.h
Author : Evan Wright

Description : Handles all visual effects in the game. On startup allocates a pool of 
			  memory to use for all emitters in the game. Uses particles and meshes to 
			  create unique and appealing effects for the game, such as; explosions, 
			  sparks, gaining power-ups, etc. When an emitter is finished it’s memory 
			  is recycled back into the VFXs memory pool for use when another emitter 
			  is needed.

Created :  07/18/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h" 
#include "../Header Files/VFXManager.h"

#include "../Header Files/Definitions.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/IEntity.h"
#include "../Header Files/BaseEntity.h"
#include "../Header Files/Player.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CVFXManager::CVFXManager( void )
{


}

void CVFXManager::Initialize( CRenderer* _renderer )
{

	m_renderer = _renderer;

	for (int i = 0; i < MAX_EMITTERS; i ++)
	{
		m_EmitterArray[i].SetLifeSpan(0.0f);
		m_EmitterArray[i].SetIsActive(false);
	}

	for(int i = 0; i < MAX_GEO_EMITTERS; i++)
	{
		m_GeoEmitterArray[i].SetDuration(0.0f);
	}

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the emitters
Parameters:
[in] _elapsedTime - the time passed since the last frame.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CVFXManager::Update( float _elapsedTime )
{
	for (int index = 0; index < MAX_EMITTERS; index ++)  //call update for all the active emitters
	{
		//if(m_EmitterArray[index].IsActive())
			m_EmitterArray[index].Update(_elapsedTime);
	}

	
	//Update all of the geometry emitters that are active
	for(int i = 0; i < MAX_GEO_EMITTERS; i++)
	{
		if(m_GeoEmitterArray[i].IsActive())
			m_GeoEmitterArray[i].Update(_elapsedTime);
	}

	m_MuzzleFlash.Update(_elapsedTime);
	m_HPArrow.Update(_elapsedTime);


}

void CVFXManager::Render()
{
	//Add all the active particles of the emitter to the render list.
	for(int i = 0; i < MAX_EMITTERS; i++) // loop through each emmiter
	{
		//if(m_EmitterArray[i].IsActive())
		//{
			for(int j = 0; j < MAX_PARTICLES; j++)  // loop through each particle
			{			
				if(m_EmitterArray[i].GetParticleArr()[j].IsAlive())
				{
					//add each particle's render node to the render list
					m_renderer->AddToRenderList(m_EmitterArray[i].GetParticleArr()[j].GetRenderNode());
				}
			}
		//}
	}

	//Add all the active geo effects of the emitter to the render list.
	for(int i = 0; i < MAX_GEO_EMITTERS; i++) // loop through each emmiter
	{
		if( m_GeoEmitterArray[i].IsActive())
		{
				//add of the geo effects to the list
				if(m_GeoEmitterArray[i].IsActive())
				{
					m_renderer->AddToRenderList(m_GeoEmitterArray[i].GetGeoEffect()->GetRenderNode());
				}
			
		}
	}

	m_renderer->AddToRenderList(m_MuzzleFlash.GetGeoEffect()->GetRenderNode());
	if(m_HPArrow.GetGeoEffect()->IsActive())
	{
		m_renderer->AddToRenderList(m_HPArrow.GetGeoEffect()->GetRenderNode());
	}

}

void CVFXManager::StartEmitter(Vec3f _position, float _duration)
{
	for(int i = 0; i < MAX_EMITTERS; i++)				// Loop until you find an inactive emitter, set its position and turn it on.
	{
		if(m_EmitterArray[i].IsActive() == false)
		{
			m_EmitterArray[i].SetPosition(_position);
			m_EmitterArray[i].SetIsActive(true);
			break;
		}
	}
}

void CVFXManager::StartGeoEmmiter(Vec3f _position,float _duration, unsigned int _enumEffectType, unsigned int _enumFXAnimType)
{
	for(int i = 0; i < MAX_GEO_EMITTERS; i++)							
	{
		if(!m_GeoEmitterArray[i].IsActive())							
		{
			//turn on the first emitter;
			//m_GeoEmitterArray[i].InitializeEmitter(_position,_duration,_enumEffectType,_enumFXAnimType);
			break;
		}
	}
}


///////////////////////////////////////////////////////////////
//This function is for when VFX needs to start an emitter.
//It will take in a _enumFXTYpe which will dictate where the 
//returned pointer will be.
///////////////////////////////////////////////////////////////
unsigned int CVFXManager::RequestEmitter(unsigned int _enumFXType, Vec3f _position, IEntity* _entity)
{
	unsigned int activatedEmitter = -1;

	switch(_enumFXType)
	{
	case EMITTER_PULL:													//LOAD ALL OF PULLS PREFERENCES
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						//find the first open Pull Emitters taht can be used inside the arr
			{
				if(!m_EmitterArray[i].IsActive())						//return the first inactive Pull Emitter you can find
				{
					activatedEmitter = i;
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/Pull.xml");
					if(_entity != nullptr)
					{
						m_EmitterArray[i].SetEntity(_entity);
						m_EmitterArray[i].SetIsPull(true);
						//m_EmitterArray[i]
					}
					break;
				}
			}
			break;
		}
	case EMITTER_PUSH:												
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						
			{
				if(!m_EmitterArray[i].IsActive())						
				{
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/Push.xml");
					if(_entity != nullptr)
					{
						m_EmitterArray[i].SetEntity(_entity);
						Vec3f forward;
						glsCopyVector3(forward,((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector());
						glsScaleVector3(forward,-1.0f);
						m_EmitterArray[i].SetInitPaticleVelocity(forward);
						m_EmitterArray[i].SetInitPaticleAcceleration(forward);
						m_EmitterArray[i].SetAllAtOnce(true);
						m_EmitterArray[i].SetIsPush(true);
					}
					break;
				}
			}
			break;
		}
	case EMITTER_POWERUP:												
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						
			{
				if(!m_EmitterArray[i].IsActive())						
				{
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/PowerUp.xml");
					if(_entity != nullptr)
					{
						m_EmitterArray[i].SetEntity(_entity);
						Vec3f Up;
						glsCopyVector3(Up,((CBaseEntity*)_entity)->GetRenderNode().GetUpVector());
						glsScaleVector3(Up,-0.85f);
						m_EmitterArray[i].SetInitPaticleVelocity(Up);
						m_EmitterArray[i].SetInitPaticleAcceleration(Up);
						m_EmitterArray[i].SetAllAtOnce(false);
					}
					break;
				}
			}
			break;
		}
	case EMITTER_PLAYER_DAMAGE:													
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						
			{
				if(!m_EmitterArray[i].IsActive())						
				{
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/PlayerTakeDamage.xml");
					if(_entity != nullptr)
					{
						m_EmitterArray[i].SetEntity(_entity);
						Vec3f forward;
						glsSubtractVectors3(forward,_position,((CBaseEntity*)_entity)->GetRenderNode().GetPosition());
						glsNormalizeVector3(forward);
						glsScaleVector3(forward,0.25f);
						m_EmitterArray[i].SetInitPaticleVelocity(forward);
						//m_EmitterArray[i].SetInitPaticleAcceleration(forward);
						m_EmitterArray[i].SetDuration(0.5f);
						m_EmitterArray[i].SetAllAtOnce(true);
					}
					break;
				}
			}
			break;
		}
	case EMITTER_ENEMY_DAMAGE:													
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						
			{
				if(!m_EmitterArray[i].IsActive())						
				{
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/EnemyTakeDamage.xml");
					if(_entity != nullptr)
					{
						m_EmitterArray[i].SetEntity(_entity);
						Vec3f forward;
						glsSubtractVectors3(forward,_position,((CBaseEntity*)_entity)->GetRenderNode().GetPosition());
						glsNormalizeVector3(forward);
						glsScaleVector3(forward,0.05f);
						m_EmitterArray[i].SetInitPaticleVelocity(forward);
						//m_EmitterArray[i].SetInitPaticleAcceleration(forward);
						m_EmitterArray[i].SetDuration(2.5f);
						m_EmitterArray[i].SetAllAtOnce(true);
						m_EmitterArray[i].SetIsEnemyDamage(true);
					}
					break;
				}
			}
			break;
		}
	case EMITTER_EXPLOSION:											
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						
			{
				if(!m_EmitterArray[i].IsActive())						
				{
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/ExplosionTom.xml");
					if(_entity != nullptr)
					{
						m_EmitterArray[i].SetEntity(_entity);
						Vec3f Up;
						Vec3f worldcenter = {0.0f,75.0f,0.0f};
						glsCopyVector3(Up,((CBaseEntity*)_entity)->GetRenderNode().GetPosition());
						glsSubtractVectors3(Up,Up,worldcenter);
						glsNormalizeVector3(Up);
						//glsScaleVector3(forward,5.0f);
						m_EmitterArray[i].SetInitPaticleVelocity(Up);
						glsScaleVector3(Up,-2.0f);
						m_EmitterArray[i].SetInitPaticleAcceleration(Up);
						m_EmitterArray[i].SetAllAtOnce(true);
					}
					break;
				}
			}
			break;
		}
	case EMITTER_BEACON:											
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						
			{
				if(!m_EmitterArray[i].IsActive())						
				{
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/Beacon.xml");
					if(_entity != nullptr && _entity->GetIsAlive() && _entity->GetIsActive())
					{
						m_EmitterArray[i].SetEntity(_entity);
						Vec3f Up;
						Vec3f worldcenter = {0.0f,75.0f,0.0f};
						glsCopyVector3(Up,((CBaseEntity*)_entity)->GetRenderNode().GetPosition());
						glsSubtractVectors3(Up,Up,worldcenter);
						glsNormalizeVector3(Up);
						//glsScaleVector3(forward,5.0f);
						m_EmitterArray[i].SetInitPaticleVelocity(Up);
					}
					break;
				}
			}
			break;
		}

	case EMITTER_LASER:
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						
			{
				if(!m_EmitterArray[i].IsActive())						
				{
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/Laser.xml");
					if(_entity != nullptr && _entity->GetIsAlive() && _entity->GetIsActive())
					{
						m_EmitterArray[i].SetEntity(_entity);
						Vec3f temp;
						glsCopyVector3(temp,((CBaseEntity*)_entity)->GetRenderNode().GetForwardVector());
						glsScaleVector3(temp,-1.0f);
						m_EmitterArray[i].SetInitPaticleVelocity(temp);
						m_EmitterArray[i].SetBeam(true);
						m_EmitterArray[i].SetIsRandomVelocities(true);
						switch(((CPlayer*)_entity)->GetGun().GetWeaponLevel())
						{
						case 1:
							{
								m_EmitterArray[i].SetSpawnRadius(2.0f);
								break;
							}
						case 2:
							{
								m_EmitterArray[i].SetSpawnRadius(3.0f);
								break;
							}
						case 3:
							{
								m_EmitterArray[i].SetSpawnRadius(4.0f);
								break;
							}
						};
						m_EmitterArray[i].SetIsRandomVelocities(false);
					}
					break;
				}
			}
			break;
		}
	case EMITTER_SMOKE:											
		{
			for(int i = 0; i < MAX_EMITTERS; i++)						
			{
				if(!m_EmitterArray[i].IsActive())						
				{
					m_EmitterArray[i].ResetEmitter();
					m_EmitterArray[i].LoadEmitter("Assets/Scripts/Particles/Smoke.xml");
					if(_entity != nullptr)
					{
						m_EmitterArray[i].SetPosition(((CBaseEntity*)_entity)->GetRenderNode().GetPosition());
						m_EmitterArray[i].SetForwardOffset(_position[2]);
						m_EmitterArray[i].SetRightOffset(_position[0]);
						m_EmitterArray[i].SetUpOffset(_position[1]);
						m_EmitterArray[i].SetEntity(_entity);
						Vec3f Up;
						Vec3f worldcenter = {0.0f,75.0f,0.0f};
						glsCopyVector3(Up,((CBaseEntity*)_entity)->GetRenderNode().GetPosition());
						glsSubtractVectors3(Up,Up,worldcenter);
						glsNormalizeVector3(Up);
						glsScaleVector3(Up,0.5f);
						m_EmitterArray[i].SetInitPaticleVelocity(Up);
						glsScaleVector3(Up,-0.85f);
						m_EmitterArray[i].SetInitPaticleAcceleration(Up);
						m_EmitterArray[i].SetAllAtOnce(false);
						//m_EmitterArray[i].SetIsLooping(true);
						m_EmitterArray[i].SetIsSmoke(true);
					}
					break;
				}
			}
			break;
		}
	case GEO_EMITTER_SCALING_BOX:
		{
			for(int i = 0; i < MAX_GEO_EMITTERS; i++)
			{
				if(!m_GeoEmitterArray[i].IsActive())
				{
					Vec3f zero = {0.0f,0.0f,0.0f};
					m_GeoEmitterArray[i].InitializeEmitter(i,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),5.0f,GEO_SCALING_BOX,GEO_SCALING_BOX_ANIM);
				}
				break;
			}
			break;
		}
	case GEO_EMITTER_BEACON:
		{
			for(int i = 0; i < MAX_GEO_EMITTERS; i++)
			{
				if(!m_GeoEmitterArray[i].IsActive())		// Find the first inactive Emitter.
				{
					m_GeoEmitterArray[i].InitializeEmitter(i,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),50.0f,GEO_BEACON,GEO_BEACON_ANIM);
					break;
				}
			}
			break;
		}
		break;
	case EMITTER_CRATE_BEACON:
		{
			for(int i = 0; i < MAX_GEO_EMITTERS; i++)
			{
				if(!m_GeoEmitterArray[i].IsActive())		// Find the first inactive Emitter.
				{
					m_GeoEmitterArray[i].InitializeEmitter(i,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),50.0f,GEO_BEACON,GEO_BEACON_ANIM);
					break;
				}
			}
			break;
		}
	case GEO_EMITTER_QUAD_PLAYER:
		{
			m_MuzzleFlash.InitializeEmitter(0,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),50.0f,GEO_QUAD_PLAYER_MUZZLE,-1);
			break;
		}
	case GEO_EMITTER_QUAD_MED_ENEMY:
		{
			for(int i = 0; i < MAX_GEO_EMITTERS; i++)
			{
				if(!m_GeoEmitterArray[i].IsActive())		// Find the first inactive Emitter.
				{
					m_GeoEmitterArray[i].InitializeEmitter(0,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),5.0f,GEO_QUAD_MED_MUZZLE,-1);
					break;
				}
			}
			break;
		}
	case GEO_EMITTER_BULLET_TRAIL:
		{
			for(int i = 0; i < MAX_GEO_EMITTERS; i++)
			{
				if(!m_GeoEmitterArray[i].IsActive())		// Find the first inactive Emitter.
				{
					m_GeoEmitterArray[i].InitializeEmitter(0,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),5.0f,GEO_QUAD_BULLET_TRAIL,-1);
					break;
				}
			}
			break;
		}
	case GEO_EMITTER_BLOOD_POOL:
		{
			for(int i = 0; i < MAX_GEO_EMITTERS; i++)
			{
				if(!m_GeoEmitterArray[i].IsActive())		// Find the first inactive Emitter.
				{
					m_GeoEmitterArray[i].InitializeEmitter(0,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),5.0f,GEO_QUAD_BLOOD_POOL,-1);
					break;
				}
			}
			break;
		}
	case GEO_EMITTER_HP_ARROW:
		{
			for(int i = 0; i < MAX_GEO_EMITTERS; i++)
			{
				if(!m_GeoEmitterArray[i].IsActive())		// Find the first inactive Emitter.
				{
					m_HPArrow.InitializeEmitter(0,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),5.0f,GEO_QUAD_HP_ARROW,-1);
					break;
				}
			}
			break;
		}
	case GEO_EMITTER_ENERGY_RING:
		{
			for(int i = 0; i < MAX_GEO_EMITTERS; i++)
			{
				if(!m_GeoEmitterArray[i].IsActive())		// Find the first inactive Emitter.
				{
					m_GeoEmitterArray[i].InitializeEmitter(0,_entity,((CBaseEntity*)_entity)->GetRenderNode().GetPosition(),5.0f,GEO_QUAD_ENERGY_RING,-1);
					break;
				}
			}
			break;
		}

	};
	return activatedEmitter;
}

void CVFXManager::TurnOffEmitter(unsigned int _index)
{
	m_EmitterArray[_index].SetIsActive(false);
}

void CVFXManager::ResetGeoEmitters(void)
{
	for(int i = 0; i < MAX_GEO_EMITTERS; i++)
	{
		m_GeoEmitterArray[i].SetIsActive(false);
	}
}
void CVFXManager::SetHPArrowActive(bool _newBool)
{
	m_HPArrow.GetGeoEffect()->SetIsActive(_newBool);
}






#pragma region Old Code
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Starts an emitter
Parameters:
[in] _type - the type of emitter to start: fire, smoke, spark, etc.
[in] _position - the position to start the emitter at.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void CVFXManager::StartEmitter( int _type, Vec3f _position,Matrix4x4f _world, bool _loopable )
//{
//	//
//	//switch(_type)
//	//{
//	//case TEST:
//	//	{
//	//		if(m_nNextAvalable >= MAX_EMITTERS - 1)
//	//		{
//	//			m_nNextAvalable = 0;
//	//		}
//	//		for(int i = m_nNextAvalable; !m_EmitterArray[m_nNextAvalable].IsActive(); i++)
//	//		{
//	//			if(!m_EmitterArray[m_nNextAvalable].IsActive())
//	//		{
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//
//	//			m_EmitterArray[m_nNextAvalable].LoadEmitter( "Assets\\Scripts\\Particles\\test.xml" ,_position ,_world,TEST,true);
//	//			m_nNextAvalable += 1;
//
//	//		}
//
//	//		}
//	//	
//	//	};
//	//	break;
//	//case EXPLODE:
//
//	//	{
//
//
//	//		if(m_nNextAvalable >= MAX_EMITTERS - 1)
//	//	{
//	//		m_nNextAvalable = 0;
//	//	}
//
//
//	//	{
//	//		{
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//
//	//			m_EmitterArray[m_nNextAvalable].LoadEmitter( "Assets\\Scripts\\Particles\\explosion.xml" ,_position ,_world,EXPLODE,true);
//	//			m_nNextAvalable += 1;
//
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//
//	//			m_EmitterArray[m_nNextAvalable].LoadEmitter( "Assets\\Scripts\\Particles\\explosion2.xml" ,_position ,_world,EXPLODE,true);
//	//			m_nNextAvalable += 1;
//
//
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//
//	//			m_EmitterArray[m_nNextAvalable].LoadEmitter( "Assets\\Scripts\\Particles\\explosion3.xml" ,_position ,_world,EXPLODE,true);
//	//			m_nNextAvalable += 1;
//
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//
//	//			m_EmitterArray[m_nNextAvalable].LoadEmitter( "Assets\\Scripts\\Particles\\explosion4.xml" ,_position ,_world,EXPLODE,true);
//	//			m_nNextAvalable += 1;
//
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//
//	//			m_EmitterArray[m_nNextAvalable].LoadEmitter( "Assets\\Scripts\\Particles\\explosion5.xml" ,_position ,_world,EXPLODE,true);
//	//			m_nNextAvalable += 1;
//
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//	//			glsCopyVector3(m_EmitterArray[m_nNextAvalable].m_baseEmitter.m_vPosition,_position);
//
//	//			m_EmitterArray[m_nNextAvalable].LoadEmitter( "Assets\\Scripts\\Particles\\explosion6.xml" ,_position ,_world,EXPLODE,true);
//	//			m_nNextAvalable += 1;
//
//	//		}
//
//	//	}
//
//	//	};
//	//	break;
//
//	//}
//}
#pragma endregion

