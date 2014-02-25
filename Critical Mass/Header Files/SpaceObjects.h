//////////////////////////////////////////////////////////////////////
//Filename: SpaceObjects.h
//Author:	Thomas Stefl
//Date:		9-13-2013
//Desc:		This file will detail how the objects in space such as
//			the sun and the asteroids will update around the moon.
/////////////////////////////////////////////////////////////////////
#ifndef SPACE_OBJECTS_H_
#define SPACE_OBJECTS_H_



///////////////////////////////////////
//		INCLUDES
///////////////////////////////////////
#include "../Header Files/RenderNode.h"
#include "../Header Files/BaseEntity.h"
#include "glsMath.h"

class SpaceObjects : public CBaseEntity
{
private:

	Vec3f m_Velocity;
	Vec3f m_Acceleration;
	Vec3f m_RotationPoint;
	float m_RotationSpeed;
	Vec3f m_FocusPoint;

	float m_rotationDeg;

	Vec3f m_prevForward;
	Vec3f m_prevUp;
	Vec3f m_prevRight;



public:
	SpaceObjects(void);
	~SpaceObjects(void);
	void Initialize(int _IDType, Vec3f _pos, Vec3f _velocity, Vec3f _acceleration);
	bool Update( float _elapsedTime );
	void Render();
	void ReOrient();
	void ReOrientRotation();
	void Scale(float _size);
	void SaveOrientation();
	void RestoreOrientation();

	//Accessors 
	Vec3f* GetVelocity()			 {return &m_Velocity;}
	Vec3f* GetAcceleration()		 {return &m_Acceleration;}


	//Mutators
	void SetFocusPoint();
	void SetVelocity(Vec3f _newVel);
	void SetAcceleration(Vec3f _newAcc);

};
#endif
