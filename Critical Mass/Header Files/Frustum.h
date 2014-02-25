#ifndef _FRUSTUM
#define _FRUSTUM

#include "Definitions.h"
#include "glsMath.h"
#include "../Header Files/CollisionEntity.h"


class CFrustum : public CCollisionEntity
{
public:
	struct tPlane
	{
		float m_fOffset;
		Vec3f m_vNormal;
	};

private:
	Vec3f m_varrCorners[8];
	tPlane m_arrPlanes[6];
	float m_fFOV;
	//void FOV(float val) { m_fFOV = val; }
	float m_fNearDistance;
	//void NearDistance(float val) { m_fNearDistance = val; }
	float m_fFarDistance;
	//void FarDistance(float val) { m_fFarDistance = val; }
	float m_fRatio;

	//void Ratio(float val) { m_fRatio = val; }
public:
	CFrustum();
	~CFrustum();

	static void ComputePlane( tPlane &_plane, const Vec3f& _d3dPointA, const Vec3f& _d3dPointB, const Vec3f &_d3dPointC );
	void BuildFrustum(float _fFOV, float _fNearDist, float _fFarDist, float _fRatio, Matrix4x4f& _Mat );
	void RebuildFrustum(Matrix4x4f& _Mat);
	tPlane GetPlane(int _index) {return m_arrPlanes[_index];}
	float GetFarDistance() const { return m_fFarDistance; }
	float GetRatio() const { return m_fRatio; }
	float GetFOV() const { return m_fFOV; }
	float GetNearDistance() const { return m_fNearDistance; }

};

#endif