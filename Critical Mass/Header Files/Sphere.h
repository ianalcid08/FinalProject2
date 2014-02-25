#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "glsMath.h"
#include "../header files/CollisionEntity.h"

class CSphere :
	public CCollisionEntity
{
	
	float m_fRadius;
	Vec3f m_vPositionOffset;

public:

	CSphere();
	~CSphere();
	float GetPositionOffset(int _Index)  { return m_vPositionOffset[_Index]; }
	void GetPositionOffset(Vec3f& _ReturnParam) {_ReturnParam[0] = m_vPositionOffset[0]; _ReturnParam[1] = m_vPositionOffset[1]; _ReturnParam[2] = m_vPositionOffset[2];}
	//void SetPositionOffset(Vec3f _val) { m_vPositionOffset = _val; }
	void SetPositionOffset(float _X, float _Y, float _Z) {m_vPositionOffset[0] = _X; m_vPositionOffset[1] = _Y; m_vPositionOffset[2] = _Z;}
	float GetRadius()  { return m_fRadius; }
	void SetRadius(float _val) { m_fRadius = _val; }

};

#endif
