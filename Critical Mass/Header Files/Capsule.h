#ifndef _CAPSULE_H_
#define _CAPSULE_H_

#include "glsMath.h"
#include "../header files/CollisionEntity.h"

class CCapsule :
	public CCollisionEntity
{
	float m_fRadius;
	Vec3f m_vStart;
	Vec3f m_vEnd;

public:

	CCapsule();
	~CCapsule();

	float GetRadius() {return m_fRadius;}
	void SetRadius(float _newRadius) {m_fRadius = _newRadius;}

	Vec3f* GetStart() {return &m_vStart;}
	Vec3f* GetEnd() {return &m_vEnd;}

	void SetStart(Vec3f _newStart);
	void SetEnd(Vec3f _newEnd);
};

#endif