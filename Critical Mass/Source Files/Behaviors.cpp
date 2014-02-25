/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Behaviors.cpp
Author : Evan Wright

Description : Behaviors for the BehaviorTree to use. These can be anything, that effects
				an entity that has AI. (Enemy)

Created :  08/06/2013
Modified : 08/12/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/Behaviors.h"
#include "../Header Files/Enemy.h"

#include "../Header Files/MessageSystem.h"
#include "../Header Files/AssetManager.h"
//Sound includes...........................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Interface for all behaviors to derive from.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	Returns completed, if there is no error.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Movement_Chase::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if(_enemy != nullptr)
	{
		// Get a vector pointing TO the target.
		Vec3f toTarget;
		glsSubtractVectors3(toTarget,*_enemy->GetTarget()->GetPosition(),*_enemy->GetPosition());
		glsNormalizeVector3(toTarget);

		// Scale it to the max velocity.
		glsScaleVector3(toTarget,_enemy->GetMoveSpeed());
		_enemy->SetVelocity(toTarget);
		_enemy->SetState(Enemy_Chasing);
		
		if(_enemy->GetType() == ET_MEDIUMENEMY)
			_enemy->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_MEDIUMENEMY, MEDIUM_MOVE_FORWARD), 0.3f);

		return Success;
	}
	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Runs away from the entities current target!
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	Returns completed, if there is no error.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Movement_Flee::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if( _enemy != nullptr )
	{
		// Get a vector pointing AWAY from the target
		Vec3f toTarget;
		glsSubtractVectors3(toTarget,*_enemy->GetPosition(),*_enemy->GetTarget()->GetPosition());
		glsNormalizeVector3(toTarget);

		// Scale it to the max velocity.
		glsScaleVector3(toTarget,_enemy->GetMoveSpeed()/2.0f);
		_enemy->SetVelocity(toTarget);
		_enemy->SetState(Enemy_Chasing);
		
		if(_enemy->GetType() == ET_MEDIUMENEMY)
			_enemy->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_MEDIUMENEMY, MEDIUM_MOVE_BACKWARDS), 0.3f);
		return Success;
	}

	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Runs away from the entities current target!
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	Returns completed, if there is no error.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Movement_Strafe::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if( _enemy != nullptr )
	{
		// Get a vector pointing TOWARDS from the target
		Vec3f toTarget, strafe;
		glsSubtractVectors3(toTarget,*_enemy->GetTarget()->GetPosition(),*_enemy->GetPosition());
		glsNormalizeVector3(toTarget);

		// Get the vector perpendicular to the toTarget vector.
		glsCrossProduct3( strafe, _enemy->GetRenderNode().GetUpVector(), toTarget );
		if(glsGetVectorLength3(strafe) >= 0.01f)
			glsNormalizeVector3( strafe );

		// Scale it to the max velocity.
		glsScaleVector3(strafe,_enemy->GetMoveSpeed());
		_enemy->SetVelocity(strafe);
		_enemy->SetState(Enemy_Chasing);

		if(_enemy->GetType() == ET_MEDIUMENEMY)
		{
			if(_enemy->GetMoveSpeed() > 0.0f)
				_enemy->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_MEDIUMENEMY, MEDIUM_MOVE_LEFT), 0.3f);
			else
				_enemy->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_MEDIUMENEMY, MEDIUM_MOVE_RIGHT), 0.3f);
		}

		return Success;
	}

	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Adds an oscillation factor to the movement.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	Returns completed, if there is no error.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Movement_Oscillation::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if( _enemy != nullptr )
	{
		static float total_time = 0.0f;
		total_time += _elapsedTime;

		// Get the vector perpendicular to the toTarget vector.
		Vec3f strafe, finalVel;
		glsCopyVector3(strafe, _enemy->GetRenderNode().GetRightVector());

		float O = sin( (total_time+_enemy->GetID()) * frequency ) * wavelength;
		glsScaleVector3(strafe, O);

		// Scale it to the max velocity.
		glsScaleVector3(strafe,_enemy->GetMoveSpeed()/2.0f);

		glsAddVectors3(finalVel, *_enemy->GetVelocity(), strafe);
		_enemy->SetVelocity(finalVel);
		
		return Success;
	}

	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Orients the enemy towards it's target.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	Returns completed if the timer is complete, else it should return in progress.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Target_LookAt::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	// and that it has a target... -B
	if( _enemy != nullptr && _enemy->GetTarget() != nullptr )
	{
		Vec3f nFwd;
		glsSubtractVectors3(nFwd, *_enemy->GetTarget()->GetPosition(), *_enemy->GetPosition());
		glsNormalizeVector3(nFwd);

		_enemy->GetRenderNode().SetForwardVector(nFwd);
		_enemy->Reorient_UsingForward();

		return Success;
	}

	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Enemy becomes idle.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	Returns completed if it's idle, else returns ERROR.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Idle::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if( _enemy != nullptr )
	{
		_enemy->SetState(Enemy_Idle);
		return Success;
	}

	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Enemy prepares attack!
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	Returns completed if it's still preparing, else returns ERROR.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Attack_Prepare::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if( _enemy != nullptr )
	{
		_enemy->SetState(Enemy_Charging);
		return Success;
	}

	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Enemy charges forward, towards a location!
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	Returns completed the enemy has reached the rush location, else it should return in progress.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Attack_Rush::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if( _enemy != nullptr )
	{
		// If this is the first entry, start the attack!
		if( m_Start == true )
		{
			Vec3f scale, tPos;

			glsCopyVector3(tPos,*_enemy->GetPosition());
			glsCopyVector3(scale,_enemy->GetRenderNode().GetForwardVector());
			glsNormalizeVector3(scale);
			glsScaleVector3(scale,_enemy->GetMoveSpeed()+(_enemy->GetMoveSpeed()/2.0f));
			
			glsAddVectors3(m_RushTarget,tPos,scale);

			Vec3f _planet = {0.0f,75.0f,0.0f};
			Vec3f up;
			glsSubtractVectors3( up, _planet, m_RushTarget );
			float length = glsGetVectorLength3( up );
			glsNormalizeVector3( up );

			float off = 0.0f;
			if( length < 75.0f )
				off = length - 75;
			else if(length > 75.0f)
				off = length - 75;

			if(off != 0.0f)
			{
				Vec3f pos;
				Vec3f move;
				glsCopyVector3( pos, m_RushTarget );
				glsCopyVector3( move, up );
				glsScaleVector3( move, off );
				glsAddVectors3( m_RushTarget, pos, move );	
			}

			m_Start = false;
		}
		
		// Get a vector pointing TO the target.
		Vec3f toTarget;
		glsSubtractVectors3(toTarget,m_RushTarget,*_enemy->GetPosition());
		glsNormalizeVector3(toTarget);

		// Scale it to the max velocity.
		glsScaleVector3(toTarget,_enemy->GetMoveSpeed()+(_enemy->GetMoveSpeed()/2.0f));
		_enemy->SetVelocity(toTarget);
		_enemy->SetState(Enemy_Attacking);

		// Check to see if we've reached the target destination
		float D = glsGetDistance3(*_enemy->GetPosition(),m_RushTarget);
		bool result = ( D < 8.0f );
		if( result == true )	// If we reached the target return complete!
		{
			m_Start = true;
			return Success;
		}
		else					// If we haven't, return that we're still running.
			return Running;
	}

	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Fires a plasma cannon shot at the target.
Parameters:
[in] enemy - the enemy to use the tree on.
[in] _elapsedTime - time since last frame.
Returns:
	Returns completed the if there is an enemy, else it returns error.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Attack_Shoot_PlasmaCannon::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if(_enemy != nullptr)
	{
		// Get direction to fire.
		if(m_NumShots == 1)
		{
			Vec3f fireDirection;
			glsSubtractVectors3(fireDirection, *_enemy->GetPosition(), *_enemy->GetTarget()->GetPosition());
			glsNormalizeVector3(fireDirection);

			_enemy->GetRenderNode().SetForwardVector(fireDirection);
			// Create a bullet heading in that direction.
			Vec3f dir = { 0.0f, 0.0f, 1.0f };
			
			// TODO:: Call create bullet message
			CMessageSystem::GetInstance()->SendMessage(new CCreateBulletMessage(_enemy,true,false,true));
			CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_QUAD_MED_ENEMY,_enemy,0.0f,0.0f,0.0f));


			if(_enemy->GetType() == ET_MEDIUMENEMY)
			{
				_enemy->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_MEDIUMENEMY, MEDIUM_FIREPLASMA), 0.1f);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MEDIUM_ATTACK);
			}
		}
		else if(m_NumShots > 1)
		{
			Vec3f fireDirection;
			glsSubtractVectors3(fireDirection, *_enemy->GetPosition(), *_enemy->GetTarget()->GetPosition());
			glsNormalizeVector3(fireDirection);

			_enemy->GetRenderNode().SetForwardVector(fireDirection);

			for(float i = -m_Angle/(m_NumShots-1); i < m_Angle; i+= m_Angle/(m_NumShots-1))
			{
				CMessageSystem::GetInstance()->SendMessage(new CCreateBulletMessage(_enemy,true,false,true,i));
			}
		}

		return Success;
	}
	// If it gets here... There is a problem. o_o  //Hi Evan, I got here... where is your God now?
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Places three airstrike markers around the target, calling for an EMP!!
Parameters:
[in] enemy - the enemy to use the tree on.
[in] _elapsedTime - time since last frame.
Returns:
	Returns completed the if there is an enemy, else it returns error.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Attack_Shoot_EMP::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if(_enemy != nullptr)
	{
		// Find out how many EMP's to fire, based on the min and max number possible.
		unsigned int numShots = (rand()%(m_MaxShots-m_MinShots))+m_MinShots;
		for(unsigned int i = 0; i < numShots; ++i)
		{
			float m_Area = m_Radius;

			float angle = 360.0f * float((float(i)+1.0f)/ (float)numShots);
			float randAngle = (float)glsDegToRad((float)angle);

			Vec3f point;
			point[0] = (*_enemy->GetTarget()->GetPosition())[0] + m_Area * cos(randAngle) * _enemy->GetRenderNode().GetForwardVector()[0] + m_Area * sin(randAngle) * _enemy->GetRenderNode().GetRightVector()[0];
			point[1] = (*_enemy->GetTarget()->GetPosition())[1] + m_Area * cos(randAngle) * _enemy->GetRenderNode().GetForwardVector()[1] + m_Area * sin(randAngle) * _enemy->GetRenderNode().GetRightVector()[1];
			point[2] = (*_enemy->GetTarget()->GetPosition())[2] + m_Area * cos(randAngle) * _enemy->GetRenderNode().GetForwardVector()[2] + m_Area * sin(randAngle) * _enemy->GetRenderNode().GetRightVector()[2];
			
			// TODO:: Call create EMP Message
			CMessageSystem::GetInstance()->SendMessage(new CCreateEMPMessage(point[0],point[1],point[2]));

			//if(_enemy->GetType() == ET_MEDIUMENEMY)
		}
		return Success;
	}
	// If it gets here... There is a problem. o_o
	return Error;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Tracks the enemies movement. At set intervals the enemies previous position will
		 EXPLODE!
Parameters:
[in] enemy - the enemy to use the tree on.
[in] _elapsedTime - time since last frame.
Returns:
	Returns completed the if there is an enemy, else it returns error.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Attack_ExplosiveWake::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if(_enemy != nullptr)
	{
		// TODO:: FOR LARGE ENEMY
		if(m_Start)
		{
			for(float i = -m_Angle/(m_NumWakes-1); i < m_Angle; i+= m_Angle/(m_NumWakes-1))
			{
				Path path(i,m_StartLength,m_EndLength,m_WaveLength,m_Rate,m_ExplodeTime);
				m_Paths.push_back(path);
			}

			m_Start = false;

			/*if(_enemy->GetType() == ET_BOSSENEMY)
				_enemy->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_BOSSENEMY, LARGE_ALPHA), 0.3f);
			else
				_enemy->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_LARGEENEMY, LARGE_ALPHA), 0.3f);*/
		}

		std::vector<Path> stillPathing;
		for(unsigned int i = 0; i < m_Paths.size(); ++i )
		{
			if(m_Paths[i].m_Start)
			{
				glsCopyVector3(m_Paths[i].m_Pos,		*_enemy->GetPosition());
				glsCopyVector3(m_Paths[i].m_PrevPos,	*_enemy->GetPosition());
				glsCopyVector3(m_Paths[i].m_F,			_enemy->GetRenderNode().GetForwardVector());
				glsCopyVector3(m_Paths[i].m_R,			_enemy->GetRenderNode().GetRightVector());
				glsCopyVector3(m_Paths[i].m_U,			_enemy->GetRenderNode().GetUpVector());

				m_Paths[i].m_Start			= false;
				m_Paths[i].m_CurrLength		= m_Paths[i].m_WaveLength*2.0f;
				m_Paths[i].m_ExplodeTime	= 1.0f;
			}

			// Update the pos;
			float D = glsGetDistance3(m_Paths[i].m_Pos, m_Paths[i].m_PrevPos);
			if( m_Paths[i].m_CurrLength+D < m_Paths[i].m_EndLength )
			{
				if( D > m_Paths[i].m_WaveLength )
				{
					m_Paths[i].m_Wake.push_back(Wake(m_Paths[i].m_ExplodeTime,m_Paths[i].m_Pos[0],m_Paths[i].m_Pos[1],m_Paths[i].m_Pos[2]));
					m_Paths[i].m_CurrLength+=m_Paths[i].m_WaveLength;
					glsCopyVector3(m_Paths[i].m_PrevPos,m_Paths[i].m_Pos);
				}
				
				Vec3f scale;
				glsCopyVector3(scale, m_Paths[i].m_F);
				if(fabs(m_Paths[i].m_Angle) > 0.0f)
				{
					Vec3f rotationBullet;
					Matrix3x3f rotBulletMat;
					glsRotationMatrix3x3(rotBulletMat, (float)glsDegToRad(m_Paths[i].m_Angle), m_Paths[i].m_U[0], m_Paths[i].m_U[1], m_Paths[i].m_U[2]); 
					glsRotateVector(rotationBullet, scale, rotBulletMat);
					glsCopyVector3(scale,rotationBullet);
				}
				glsScaleVector3(scale, _elapsedTime*m_Paths[i].m_Rate);

				glsAddVectors3(m_Paths[i].m_Pos,m_Paths[i].m_Pos,scale);

				// Clamp the position onto the planet.
				Clamp(m_Paths[i]);

				float alpha = (m_Paths[i].m_EndLength-m_Paths[i].m_CurrLength) / m_Paths[i].m_EndLength;
				Vec4f posColor = {1.0f,0.0f,0.0f,alpha};
				CMessageSystem::GetInstance()->SendMessage(new CCreateDebugRenderNodeMessage(m_Paths[i].m_Pos,m_Paths[i].m_U,m_Paths[i].m_R,m_Paths[i].m_F,posColor));		
			}
			
			if( m_Paths[i].m_Wake.size() > 0 )
			{
				for(unsigned int x = 0; x < m_Paths[i].m_Wake.size(); ++x)
				{
					// TODO:: Call create EMP Message
					CMessageSystem::GetInstance()->SendMessage(new CCreateEMPMessage(m_Paths[i].m_Wake[x].pos[0],m_Paths[i].m_Wake[x].pos[1],m_Paths[i].m_Wake[x].pos[2],m_Paths[i].m_ExplodeTime));
					m_Paths[i].m_ExplodeTime+=0.05f;
				}
				m_Paths[i].m_Wake.clear();
			}
			else if( m_Paths[i].m_CurrLength+D > m_Paths[i].m_EndLength )
			{
				continue;
			}

			stillPathing.push_back(m_Paths[i]);
		}
		m_Paths = stillPathing;

		if(m_Paths.size() == 0)
		{
			m_Start = true;
			return Success;
		}
		else
			return Running;
	}
	// If it gets here... There is a problem. o_o
	return Error;
}

void Attack_ExplosiveWake::Clamp(Path& path)
{
	Vec3f _planet = {0.0f,75.0f,0.0f};

	glsSubtractVectors3( path.m_U, _planet, path.m_Pos );
	float length = glsGetVectorLength3( path.m_U );
	glsNormalizeVector3( path.m_U );

	float off = 0.0f;
	if( length < 75.0f )
		off = length - 75;
	else if(length > 75.0f)
		off = length - 75;

	if(off != 0.0f)
	{
		Vec3f pos;
		Vec3f move;
		glsCopyVector3( pos, path.m_Pos );
		glsCopyVector3( move, path.m_U );
		glsScaleVector3( move, off );
		glsAddVectors3( path.m_Pos, pos, move );	
	}

	glsCrossProduct3( path.m_F, path.m_R, path.m_U );
	glsNormalizeVector3( path.m_F );

	glsCrossProduct3( path.m_R, path.m_U, path.m_F );
	glsNormalizeVector3( path.m_R );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Fires mortars at a target, for a set period of time.
Parameters:
[in] enemy - the enemy to use the tree on.
[in] _elapsedTime - time since last frame.
Returns:
	Returns running while the timer is not completed, else returns success
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Attack_MortarField::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// Make sure the enemy exists...
	if(_enemy != nullptr)
	{
		if(m_Duration > m_CurrTime)
		{
			m_CurrTime += _elapsedTime;
			if(m_CurrTime > m_CurrRate)
			{
				unsigned int numShots = 3;
				for(unsigned int i = 0; i < numShots; ++i)
				{
						float m_Area = float(rand()%150)/150.0f+10.0f;

						float angle = 360.0f * float((float(i)+1.0f)/ (float)numShots);
						float randAngle = (float)glsDegToRad(float(rand()%(int)angle));

						Vec3f point;
						point[0] = (*_enemy->GetTarget()->GetPosition())[0] + m_Area * cos(randAngle) * _enemy->GetRenderNode().GetForwardVector()[0] + m_Area * sin(randAngle) * _enemy->GetRenderNode().GetRightVector()[0];
						point[1] = (*_enemy->GetTarget()->GetPosition())[1] + m_Area * cos(randAngle) * _enemy->GetRenderNode().GetForwardVector()[1] + m_Area * sin(randAngle) * _enemy->GetRenderNode().GetRightVector()[1];
						point[2] = (*_enemy->GetTarget()->GetPosition())[2] + m_Area * cos(randAngle) * _enemy->GetRenderNode().GetForwardVector()[2] + m_Area * sin(randAngle) * _enemy->GetRenderNode().GetRightVector()[2];
					
						// TODO:: Call create EMP Message
						CMessageSystem::GetInstance()->SendMessage(new CCreateEMPMessage(point[0],point[1],point[2],1.5f));
				}
				m_CurrRate += m_Rate;
			}
			return Running;
		}
		else
		{
			m_CurrRate = m_Rate;
			m_CurrTime = 0.0f;
			return Success;
		}
	}
	// If it gets here... There is a problem. o_o
	return Error;
}
