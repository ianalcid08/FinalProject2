/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : LaserBall.cpp
Author : Evan Wright

Description : The laser ball used in laser field. It spawns a ball that when started spawns
			  on the ground. It gradually moves up into the air, to a set distance, shooting
			  a laser downward, towards the ground.

Created :  09/21/2013
Modified : 09/21/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/LaserBall.h"

#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/CollisionLib.h"

CLaserBall::CLaserBall(void)
{
	this->SetType(ET_LASER_BALL);

	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector(	1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// COLLISION DATA
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(3.0f, pos));
}

void CLaserBall::Init(float* _position,float* _forward)
{
	this->GetRenderNode().SetPosition(_position);
	Adjust();

}

void CLaserBall::Adjust(void)
{
	Vec3f moon = {0.0f,75.0f,0.0f};

	Vec3f up, right, forward;
	glsSubtractVectors3(up,moon,*this->GetPosition());
	float length = glsGetVectorLength3(up);
	glsNormalizeVector3(up);
	GetRenderNode().SetUpVector(up);

	float rad = 75.0f;

	float off = 0.0f;
	if(length < rad) off=length-rad;
	else if(length > rad) off=length-rad;

	if(off != 0.0f)
	{
		Vec3f pos;
		Vec3f move;
		glsCopyVector3(pos,*this->GetPosition());
		glsCopyVector3(move,this->GetRenderNode().GetUpVector());
		glsScaleVector3(move,off);
		glsAddVectors3(*this->GetPosition(),pos,move);	
	}

	glsCrossProduct3(forward,this->GetRenderNode().GetRightVector(),up);
	glsNormalizeVector3(forward);
	GetRenderNode().SetForwardVector(forward);

	glsCrossProduct3(right,up,forward);
	glsNormalizeVector3(right);
	GetRenderNode().SetRightVector(right);
}

void CLaserBall::Release(void)
{

}

bool CLaserBall::Update(float _elapsedTime)
{

	return true;
}

void CLaserBall::HandleReaction(void)
{

}
