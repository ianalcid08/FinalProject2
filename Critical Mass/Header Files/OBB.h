#ifndef _OOBB
#define _OOBB

#include "glsMath.h"
#include "../header files/CollisionEntity.h"

class COBB : public CCollisionEntity
{
	Vec3f m_vCenter;
	Matrix3x3f m_mOrintation;
	Vec3f m_HalfwayExtents;

public:

	COBB();
	~COBB();

	Vec3f* GetCenter() {return &m_vCenter;}
	Matrix3x3f* GetOrientation() {return &m_mOrintation;}
	Vec3f* GetHalfwayExtents() {return &m_HalfwayExtents;}
	float GetHalfwayExtents(int _Index) {return m_HalfwayExtents[_Index];}
	void SetCenter(Vec3f _newCenter);
	void ReOrient(Vec3f _right, Vec3f _up, Vec3f _forward);
	void ReOrient(Matrix3x3f _newO);
	void SetExtents(Vec3f _newExtents);
	void SetExtents(float _x, float _Y, float _Z);
};

#endif