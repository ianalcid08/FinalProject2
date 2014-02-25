#ifndef _AABB_H_
#define _AABB_H_

#include "glsMath.h"
#include "CollisionEntity.h"

class CAABB :
	public CCollisionEntity
{
	Vec3f m_vMin,
		m_vMax;

public:

	CAABB();
	~CAABB();

	void GetMin(Vec3f _OutParam) { _OutParam[0] = m_vMin[0]; _OutParam[1] = m_vMin[1]; _OutParam[2] = m_vMin[2]; }
	void GetMax(Vec3f _OutParam) { _OutParam[0] = m_vMax[0]; _OutParam[1] = m_vMax[1]; _OutParam[2] = m_vMax[2]; }

	void SetMin(Vec3f _NewMin) {m_vMin[0] = _NewMin[0]; m_vMin[1] = _NewMin[1]; m_vMin[2] = _NewMin[2];}
	void SetMax(Vec3f _NewMax) {m_vMax[0] = _NewMax[0]; m_vMax[1] = _NewMax[1]; m_vMax[2] = _NewMax[2];}

	void SetMin(float _X, float _Y, float _Z) {m_vMin[0] = _X; m_vMin[1] = _Y; m_vMin[2] = _Z;}
	void SetMax(float _X, float _Y, float _Z) {m_vMax[0] = _X; m_vMax[1] = _Y; m_vMax[2] = _Z;}
};

#endif
