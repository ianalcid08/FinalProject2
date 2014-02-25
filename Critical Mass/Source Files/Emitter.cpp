/*///////////////////////////////////////////////////////
/FileName:		Emitter.cpp
/Author:		Thomas Stefl
/Date:			8/6/2013
/Last Modified: 8/24/2013
/Purpose:		The purpose of the emittier is to
/				hold an array of particles, update them
/				and initialize them.
/*///////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/Emitter.h"
#include "../Header Files/GLSetup.h"
#include "../Header Files/glsMath.h"
#include <random>
#include <ctime>
#include "../Header Files/AssetManager.h"
#include "../../Includes/tinyxml/tinyxml.h"
#include <sstream>
#include <iostream>
#include "../Header Files/BaseEntity.h"
#include "../Header Files/Laser.h"


CEmitter::CEmitter()
{
	//Seed the random generator
	srand(unsigned(time(0)));

	Vec3f empty;
	empty[0] = 0.0f;
	empty[1] = 0.0f;
	empty[2] = 0.0f;

	memcpy(m_Position,empty,sizeof(Vec3f));
	memcpy(m_InitialParticleVelocity,empty,sizeof(Vec3f));
	memcpy(m_InitialParticleAcceleration,empty,sizeof(Vec3f));

	m_InitialParticleLifeSpan = 0.0f;
	m_DeltaTime = 0.0f;
	m_Radius = 1.0f;
	m_Duration = 0.0f;
	m_RightOffset = 0.0f;
	m_ForwardOffset = 0.0f;
	m_UpOffset = 0.0f;

	m_IsRandomPos = false;
	m_IsActive = false;
	m_IsLooping = false;
	m_IsRandomParticleLifeSpans = false;
	m_IsTargetted = false;
	m_Beam = false;
	m_IsPull = false;
	m_IsSmoke = false;
	m_IsPush = false;
	m_IsEnemyDamage = false;

	m_SourceEntity = nullptr;

	m_AllAtOnce = false;
	m_ParticleCap = MAX_PARTICLES;  // set to max until otherwise set elsewhere

	//BuildGeometry();
	//m_hTexture = 1;
	//m_hShader = CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY];
	//m_ParticleArr = nullptr;
}

////////////////////////////////////////////////////////
//This is an overloaded constructor that will set up the 
//emitter for you based on the input you pass for the 
//properties.
//Some notes:
//If the emitter loops (i.e. infinite playback until manually made to stop)
//duration can be set to whatever value as it is not used.
//However if looping is false a timer will be set for how long the
//emitter will remain active
////////////////////////////////////////////////////////
void CEmitter::InitializeEmitter(bool _isActive, float _particleLifeSpan,
								 float _spawnRate,Vec3f _particleVelocity, Vec3f _particleAcceleration,
								 bool _isLooping, bool _isRandomLife, bool _isRandPos, bool _isTargetted, float _radius, 
								 float _forwardOffset,float _rightOffset, float _upOffset, float _duration,  bool _isRandVel)
{
	//Seed the random generator
	srand(unsigned(time(0)));


	//Set all the properties for the emitter
	m_DeltaTime = _spawnRate;						
	m_IsLooping = _isLooping;
	m_IsTargetted = _isTargetted;
	m_IsRandomParticleLifeSpans = _isRandomLife;
	m_InitialParticleLifeSpan = _particleLifeSpan;
	m_SpawnRate = _spawnRate;
	m_IsRandomPos = _isRandPos;
	m_Radius = _radius;

	m_ForwardOffset = _forwardOffset;
	m_RightOffset = _rightOffset;
	m_UpOffset = _upOffset;
	m_Duration = _duration;

	m_IsRandomVelocities = _isRandVel;


	memcpy(m_InitialParticleVelocity,_particleVelocity,sizeof(Vec3f));
	memcpy(m_InitialParticleAcceleration,_particleAcceleration,sizeof(Vec3f));

	m_IsActive = _isActive;

	//Particle properties
	for(int i = 0; i < MAX_PARTICLES; i ++)										//Loop through the Emitter's particles and 
	{																			// set their propeties

		if(m_IsRandomPos)														// check to see if the particles follow a line or spawn sporadically
		{
			if(m_IsTargetted)		                                            // check to see if the particles need to be shot at a position
			{                                                                
				SetRandCircPoint(*m_ParticleArr[i].GetPosition(), m_Position);
			}
			else
			{
				SetRandCircPoint(*m_ParticleArr[i].GetPosition());	
			}
		}
		else
		{
			m_ParticleArr[i].SetPosition(m_Position);							 //Set the particles starting position to that of the emitter's
		}

		m_ParticleArr[i].SetVelocity(_particleVelocity);
		m_ParticleArr[i].SetAcceleration(_particleAcceleration);

		if(m_IsRandomParticleLifeSpans)
		{
			m_ParticleArr[i].SetLifeSpan(rand() % PARTICLE_LIFESPAN_VARIATION + _particleLifeSpan);
		}
		else
		{
			m_ParticleArr[i].SetLifeSpan(0.0f);
		}
	}


}

//////////////////////////////////////////////
//The destructor will kill all of the 
//particles
/////////////////////////////////////////////
CEmitter::~CEmitter()
{
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		m_ParticleArr[i].SetLifeSpan(0.0f);
	}
	m_IsLooping = false;
}

void CEmitter::Update(float _fElapsedTime)
{
	if(m_ParticleArr == nullptr)
	{
		return;
	}

	if(m_Duration > 0.0f && !m_IsLooping)						//if not looping indefinitely substract time from the duration of the emitter 
	{
		m_Duration -= _fElapsedTime;
	}

	//Turn off the emitter is the source is gone or if it is a non looping emitter and the duration is up.

	if(!m_IsSmoke)
	{
		if(((m_SourceEntity == nullptr || !m_SourceEntity->GetIsActive())) || (!m_IsLooping && m_Duration <= 0.0f)  )// || (!m_IsLooping && m_Duration <= 0.0f))  // if the source entity no longer exists, stop the emitter
		{
			m_IsActive = false;
			ResetEmitter();
		}
	}
	else
	{
		if(m_Duration <= 0.0f)
		{
			m_IsActive = false;
			ResetEmitter();

		}
	}

	if(m_IsActive)	//The emitter wont spawn new particles unless its active...
	{
		m_DeltaTime += _fElapsedTime;		//update the spawning timer

		if(m_SourceEntity != nullptr) //if there is a source entity, update the position of the emitter based on an offset because entities move.
		{
			glsCopyVector3(m_Position,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition());

			Vec3f tempForward;
			glsCopyVector3(tempForward,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector()); //tempForward = entity forward
			glsScaleVector3(tempForward,m_ForwardOffset);											//tempForward scaled by the forward offset
			glsAddVectors3(m_Position,m_Position,tempForward);										//add that to the position

			Vec3f tempRight;
			glsCopyVector3(tempRight,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetRightVector()); //tempRight = entity right
			glsScaleVector3(tempRight,m_RightOffset);											//tempRight scaled by the forward offset
			glsAddVectors3(m_Position,m_Position,tempRight);									//add that to the position

			Vec3f tempUp;
			glsCopyVector3(tempUp,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector());	//tempUp = entity up
			glsScaleVector3(tempUp,m_UpOffset);												//tempUp scaled by the forward offset
			glsAddVectors3(m_Position,m_Position,tempUp);									//add that to the position
		}

		//Time to spawn a new particle.  make sure either there is still time on the emitter or it is looping
		if(m_DeltaTime >= m_SpawnRate && (m_IsLooping || (m_Duration >= 0.0f && !m_IsLooping)))
		{
			//reset the timer
			for(int i = 0; i < m_ParticleCap; i++)  //loop and find a dead particle
			{
				if(!(m_ParticleArr[i].IsAlive())) //if the particle is dead reinit    "I cast Raise..."
				{
					ReInitParticle(i);
					m_DeltaTime = 0.0f;
					if(m_IsLooping || !m_AllAtOnce)
						break;
				}
			}
		}
	}

	for(int i = 0; i < MAX_PARTICLES; i++)      // ok now update all the particles calling their update7
	{
		if(m_ParticleArr[i].IsAlive())
		{
			if(m_IsTargetted && m_SourceEntity != nullptr)  // if the source is targetted accelerate the individual particles towards the source (player?)
			{
				Vec3f towardsEntity;
				glsSubtractVectors3(towardsEntity,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition(),m_ParticleArr[i].GetRenderNode()->GetPosition());
				glsNormalizeVector3(towardsEntity);
				m_ParticleArr[i].SetAcceleration(towardsEntity);
			}
	
			//m_ParticleArr[i].SetScale(50.0f);
			if(m_IsSmoke)
			{
				m_ParticleArr[i].SetScale(m_ParticleArr[i].GetCurrScale() + _fElapsedTime * 30.0f);

				Vec4f tempColor = {0.0f,0.0f,0.0f,0.0f};
				glsCopyVector4(tempColor, m_ParticleArr[i].GetRenderNode()->GetColor());
				//float * derp = &tempColor[0];
				tempColor[3] -=  _fElapsedTime;
				m_ParticleArr[i].GetRenderNode()->SetColor(tempColor);
			}

			if(m_IsSmoke && m_ParticleArr[i].GetRenderNode()->GetDiffuseTextureHandle() != CAssetManager::GetInstance()->GetParticleTexture(DUST_TEXTURE) )
			{
				m_ParticleArr[i].GetRenderNode()->SetDiffuseTextureHandle(CAssetManager::GetInstance()->GetParticleTexture(DUST_TEXTURE));
			}
			else if(m_IsEnemyDamage)
			{
				m_ParticleArr[i].GetRenderNode()->SetDiffuseTextureHandle(CAssetManager::GetInstance()->GetTexture(SMALL_ENEMY_DAMAGE));
				Vec4f yellow = {1.0f,1.0f,1.0f,1.0f};
				m_ParticleArr[i].GetRenderNode()->SetColor(yellow);
			}
			else if(!m_IsSmoke && !m_IsEnemyDamage)
			{
				m_ParticleArr[i].GetRenderNode()->SetDiffuseTextureHandle(CAssetManager::GetInstance()->GetParticleTexture(WHITE_PARTICLE));
			}

#pragma region Laser_Beam_Sizing
			if(m_Beam)
			{
				switch(((CLaser*)m_SourceEntity)->GetCurrLevel())
				{
				case 1:
					{
						m_ParticleArr[i].SetScale(1.0f);
						this->m_Radius = 2.0f;
						break;
					}
				case 2:
					{
						m_ParticleArr[i].SetScale(2.0f);
						this->m_Radius = 4.0f;

						break;
					}
				case 3:
					{
						m_ParticleArr[i].SetScale(3.5f);
						this->m_Radius = 8.0f;
						break;
					}
				};
			}
#pragma endregion

			m_ParticleArr[i].Update(_fElapsedTime);
		}
		m_ParticleArr[i].Initialize();
	}
}

void CEmitter::Render()
{
	if(m_ParticleArr == nullptr)
		return;
	//for(int i = 0; i < MAX_PARTICLES; i++)
	//{
	//	if(m_ParticleArr[i].IsAlive())
	//	{
	//		//TO DO - this can be optimized through further enhancing instancing
	//		memcpy(m_RenderNode.m_vPosition, m_ParticleArr[i].GetPosition(), sizeof(Vec3f));
	//		
	//	}

	//}
}

///////////////////////////////////////////
//When a particle is dead it will stay dead.
//If, however, the emitter is looping, it will
//recycle the particles and set them back to
//how they would spawn initially.
///////////////////////////////////////////
void CEmitter::ReInitParticle(int _index)
{	
	if(m_ParticleArr == nullptr)
		return;

	m_ParticleArr[_index].SetScale(1.0f);

	//m_ParticleArr[_index].Initialize();


	//Sets the specific particle's lifespan back to the initial lifespan
	if(m_IsRandomParticleLifeSpans)
	{                                              // 0 -> #define + initial Life
		m_ParticleArr[_index].SetLifeSpan(rand() % PARTICLE_LIFESPAN_VARIATION + m_InitialParticleLifeSpan);
	}
	else
	{
		m_ParticleArr[_index].SetLifeSpan(m_InitialParticleLifeSpan);
	}

	//Sets the specific particle's acceleration to its initial acc
	//m_ParticleArr[_index].SetAcceleration(m_InitialParticleAcceleration);

	//Sets the specific particle's velocity back to the initial velocity

	//if(m_Beam)
	//{
	//	int derp = 0;
	//}
	if(m_IsRandomVelocities)
	{
		//gives each particle a random velocity
		if(m_IsPush)
		{
			Vec3f tempPos;
			float randDeg = float(rand()%360);

			Matrix3x3f rotMat;
			glsRotationMatrix3x3(rotMat,randDeg,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector()[0],((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector()[1],((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetUpVector()[2]);

			glsRotateVector(tempPos,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector(),rotMat);

			glsNormalizeVector3(tempPos);

			m_ParticleArr[_index].SetVelocity(tempPos);
		}
		else if(!m_Beam)
		{
			Vec3f temp;
			glsCopyVector3(temp, m_InitialParticleVelocity);
			for(int i = 0; i < 3; i ++)
			{
				int tempInt = rand() % 2;
				if(tempInt)
				{
					//add 0.1 - 0.3
					temp[i] += (float)(rand()%9 - 4) * (0.1f);
					//if the original vel was positive, keep it so
					if(m_InitialParticleVelocity[i] >= 0)
						temp[i] = abs(temp[i]);
				}
			}
			glsNormalizeVector3(temp);
			m_ParticleArr[_index].SetVelocity(temp);
		}
		else
		{
			Vec3f temp;
			glsCopyVector3(temp, ((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector());
			for(int i = 0; i < 3; i ++)
			{
				int tempInt = rand() % 2;
				if(tempInt)
				{
					//add 0.1 - 0.3
					temp[i] += (float)(rand()%9 - 4) * (0.1f);
					//if the original vel was positive, keep it so
					if(m_InitialParticleVelocity[i] >= 0)
						temp[i] = abs(temp[i]);
				}

			}
			glsNormalizeVector3(temp);
			m_ParticleArr[_index].SetVelocity(temp);
		}
	}
	else if(m_IsTargetted)
	{
		Vec3f towards;
		glsSubtractVectors3(towards,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition(),m_Position);
		glsNormalizeVector3(towards);
		m_ParticleArr[_index].SetVelocity(m_InitialParticleVelocity);

	}
	else if(m_Beam)
	{
		Vec3f temp;
		glsCopyVector3(temp,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector());
		//glsScaleVector3(temp,2.0f);
		glsNormalizeVector3(temp);
		m_ParticleArr[_index].SetVelocity(temp);
		
	}
	else
	{
		m_ParticleArr[_index].SetVelocity(m_InitialParticleVelocity);
	}


	if(m_IsEnemyDamage)
	{
		//Matrix3x3f rotMat;
		//glsRotationMatrix3x3(rotMat,90.0f,m_ParticleArr[_index].GetRenderNode()->GetUpVector()[0],m_ParticleArr[_index].GetRenderNode()->GetUpVector()[1],m_ParticleArr[_index].GetRenderNode()->GetUpVector()[2]);

		//Vec3f tempPos;
		//glsCopyVector3(tempPos,m_ParticleArr[_index].GetRenderNode()->GetUpVector());
		//glsRotateVector(tempPos,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetForwardVector(),rotMat);
		//m_ParticleArr[_index].GetRenderNode()->SetUpVector(tempPos[0],tempPos[1],tempPos[2]);

		m_ParticleArr[_index].GetRenderNode()->SetUpVector(*m_ParticleArr[_index].GetVelocity());
	}
	
	//Setting Acceleration
	if(m_IsTargetted)
	{
		Vec3f towards;
		glsSubtractVectors3(towards,((CBaseEntity*)m_SourceEntity)->GetRenderNode().GetPosition(),m_Position);
		glsNormalizeVector3(towards);
		m_ParticleArr[_index].SetAcceleration(m_InitialParticleAcceleration);
	}
	else
	{
		m_ParticleArr[_index].SetAcceleration(m_InitialParticleAcceleration);
	}

	//Sets the specific particle's position back to the emitter's position
	if(m_IsRandomPos)
	{
		SetRandCircPoint(*m_ParticleArr[_index].GetPosition());
	}
	else
	{
		m_ParticleArr[_index].SetPosition(m_Position);
	}

	//m_ParticleArr[_index].BuildGeometry();


}

void CEmitter::SetPosition(const Vec3f _newPosition)
{
	memcpy(m_Position,_newPosition,sizeof(Vec3f));
}

////////////////////////////////////////////////////////
//  This function takes the emitter's postion and
//  builds a circle tangential to the surface of
//  the sphere.  Then it will put _POSITION at 
//  that point in space.
//  Alternatively a "_targetPos"ition may be given.
//  If not provided it will be set to null in which
//  case the particles will start from then emitter's
//  position and travel according to V and Acc.
//  If a target position is provided then the emitter
//  will face that direction
/////////////////////////////////////////////////////////
void CEmitter::SetRandCircPoint(Vec3f _emitterPos, Vec3f _targetPos)
{
	//Procedure
	//1. Choose any point (arbitraryPos) randomly which doesn't lie on the line through the up vector
	//2. Calculate the vector "fakeForward" as the cross product between the vectors arbitraryPos - m_position and up. This vector "fakeForward" is now perpendicular to up. (If "right" is 0 then 1. wasn't satisfied) 
	//3. Calculate the vector "right" as the cross product between the vectors fakeForard and up.
	//4. The unit vectors ||fakeForward|| and ||right|| are two orthonormal vectors in the plane perpendicular to UP.

	Vec3f arbitraryPos = {(float)rand(),(float)rand(),(float)rand()};
	Vec3f up,fakeForward,right;

	//HAX
	//Author : Tom Stefl
	//Should adjust for any facing direction
	//if non are given use the worlds center pos to draw an up
	Vec3f worldCenter = {0.0,75.0f,0.0f};


	if(_targetPos == nullptr)
	{
		glsSubtractVectors3(up,m_Position,worldCenter);
		glsNormalizeVector3(up);
	}
	else
	{
		glsSubtractVectors3(up,_targetPos,m_Position);
		glsNormalizeVector3(up);
	}

	Vec3f temp;
	glsSubtractVectors3(temp,arbitraryPos,m_Position);
	glsNormalizeVector3(temp);

	glsCrossProduct3(fakeForward,temp,up);
	glsNormalizeVector3(fakeForward);

	glsCrossProduct3(right,fakeForward,up);
	glsNormalizeVector3(right);

	double randAngle = glsDegToRad(rand() % 360);

	_emitterPos[0] = m_Position[0] + m_Radius * (float)cos(randAngle) * fakeForward[0] + m_Radius * (float)sin(randAngle) * right[0];
	_emitterPos[1] = m_Position[1] + m_Radius * (float)cos(randAngle) * fakeForward[1] + m_Radius * (float)sin(randAngle) * right[1];
	_emitterPos[2] = m_Position[2] + m_Radius * (float)cos(randAngle) * fakeForward[2] + m_Radius * (float)sin(randAngle) * right[2];

}

void CEmitter::SetTarget(Vec3f _newTarget)
{
	//glsLoadVector3(m_Target,_newTarget[0],_newTarget[1],_newTarget[2]);
}

void CEmitter::SetInitPaticleVelocity(Vec3f _newVelocity)
{
	glsLoadVector3(m_InitialParticleVelocity,_newVelocity[0],_newVelocity[1],_newVelocity[2]);
}

void CEmitter::SetInitPaticleAcceleration(Vec3f _newAcceleration)
{
	glsLoadVector3(m_InitialParticleAcceleration,_newAcceleration[0],_newAcceleration[1],_newAcceleration[2]);
}

////////////////////////////////////////////////////
//This function will load in properties for an
//emitter from an xml file using tinyXML.
////////////////////////////////////////////////////
void CEmitter::LoadEmitter(char* _szFile)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(_szFile))
	{
		MessageBoxA(0,"Cannot load XML file for the Emitter, please check the Load Emitter function.",0,0);
		return;
	}

	TiXmlElement* root = doc.FirstChildElement();	
	if (root == NULL)
		return;

	TiXmlElement* child = root->FirstChildElement();   //load in bool for looping
	int Looping = atoi(child->GetText());

	child = child->NextSiblingElement();			   //load in bool to see if the particles have randomized lifespans
	int randLife = atoi(child->GetText());

	child = child->NextSiblingElement();			   //load in bool to see if the particles have randomized spawning locations
	int randPos = atoi(child->GetText());

	child = child->NextSiblingElement();			   //load in bool to see if the particles have randomized spawning locations
	int randVel = atoi(child->GetText());

	child = child->NextSiblingElement();			   
	int isTargetted = atoi(child->GetText());

	child = child->NextSiblingElement();			   //load in radius of the spawn rate
	float radius = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	float lifeSpan = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	float spawnRate = (float)atof(child->GetText());

	Vec3f velocity;
	Vec3f acceleration;
	Vec3f newColor;

	child = child->NextSiblingElement();
	velocity[0] = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	velocity[1] = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	velocity[2] =(float)atof(child->GetText());

	child = child->NextSiblingElement();
	acceleration[0] = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	acceleration[1] = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	acceleration[2] = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	float rightOffset = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	float forwardOffset = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	float upOffset = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	float emitterDuration = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	int particleCap = atoi(child->GetText());

	child = child->NextSiblingElement();
	newColor[0] = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	newColor[1] = (float)atof(child->GetText());

	child = child->NextSiblingElement();
	newColor[2] = (float)atof(child->GetText());

	//This true false set up using the int is to avoid type casting which triggers a performance warning - Tom
	bool isLooping = false;
	bool isRandLife = false;
	bool isRandPos = false;
	bool bisTargetted = false;
	bool isRandVel = false;

	if(Looping)
		isLooping = true;
	if(randLife)
		isRandLife = true;
	if(randPos)
		isRandPos = true;
	if(isTargetted)
		bisTargetted = true;
	if(randVel)
		isRandVel = true;

	ResetEmitter(); //sets everything to false and 0;
	InitializeEmitter(true,lifeSpan,spawnRate,velocity,acceleration,isLooping,isRandLife,isRandPos,bisTargetted,radius,forwardOffset,rightOffset,upOffset,emitterDuration, isRandVel);
	this->SetParticleCap(particleCap);
	this->SetParticleColor(newColor);
}

void CEmitter::ResetEmitter()
{

	//Particle  m_ParticleArr[MAX_PARTICLES];
	//Vec3f empty = {0.0f,0.0f,0.0f};
	glsLoadVector3(m_Position,0.0f,0.0f,0.0f);							//The emitter's position in the world
	m_SourceEntity = nullptr;		

	//offsets
	m_RightOffset	= 0.0f;
	m_ForwardOffset	= 0.0f;
	m_UpOffset		= 0.0f;

	m_SpawnRate	= 0.0f;
	m_DeltaTime	= 0.0f;   //a timer that is checked against spawn rate to determine whether or not a new particle should be spawned
	m_Duration	= 0.0f;
	m_Radius	= 0.0f;
	m_InitialParticleLifeSpan = 0.0f;

	//Preferences/Settings
	//HAX
	//Author: Tom Stefl
	//Desc: Using bools instead of bit flags, not a huge deal but would like to change in the future
	m_IsRandomPos				= false;
	m_IsActive					= false;
	m_IsLooping					= false;
	m_IsRandomParticleLifeSpans = false;
	m_IsTargetted				= false;
	m_IsRandomVelocities		= false;
	m_AllAtOnce					= false;
	m_Beam						= false;
	m_IsPull					= false;
	m_IsSmoke					= false;
	m_IsPush					= false;
	m_IsEnemyDamage				= false;

	//Initial particle information
	glsLoadVector3(m_InitialParticleVelocity,0.0f,0.0f,0.0f);	
	glsLoadVector3(m_InitialParticleAcceleration,0.0f,0.0f,0.0f);	

	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		m_ParticleArr[i].SetLifeSpan(0.0f);
		m_ParticleArr[i].SetScale(1.0f);
	}
}

bool CEmitter::IsActive()
{
	bool aliveParticles = false;
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		if(m_ParticleArr[i].IsAlive())
		{
			aliveParticles = true;
			break;
		}
	}

	if(m_IsActive || (!m_IsActive && aliveParticles))   //if the emitter is active or if it is not BUT THERE ARE STILL LIVE PARTICLES say its alive
	{
		return true;
	}
	else
	{
		return false;
	}

}

void CEmitter::SetParticleColor(Vec3f _newColor)
{
	//glsCopyVector3(m_ParticleColor,_newColor);
	for(int i = 0; i < MAX_PARTICLES; i ++)
	{
		m_ParticleArr[i].SetParticleColor(_newColor);
	}
}
