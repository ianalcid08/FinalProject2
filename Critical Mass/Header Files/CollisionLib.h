
#ifndef _COLLSIONLIB_H_
#define _COLLSIONLIB_H_

#include "glsMath.h"
#include "../Header Files/CollisionEntity.h"

struct Plane  : public CCollisionEntity
{
	Plane() {this->m_eBVType = ePlane;}
	Plane(Vec3f _Normal, float _Offset)
	{
		glsCopyVector3(normal, _Normal);
		offset = _Offset;
		this->m_eBVType = ePlane;
	}

	Vec3f normal;
	float offset;
};

struct AABB  : public CCollisionEntity
{
	AABB() {this->m_eBVType = eAABB;}

	AABB(Vec3f _Min, Vec3f _Max)
	{
		glsCopyVector3(min, _Min);
		glsCopyVector3(max, _Max);
		this->m_eBVType = eAABB;
	}

	Vec3f min;
	Vec3f max;
};

struct Frustum  : public CCollisionEntity
{	
	Frustum() {this->m_eBVType = eFrustum;}

	Plane planes[6];
	Vec3f corners[8];
};

struct Segment  : public CCollisionEntity
{
	Segment() {this->m_eBVType = eSegment;}

	Segment(Vec3f _Start, Vec3f _End)
	{
		glsCopyVector3(m_Start, _Start);
		glsCopyVector3(m_End, _End);
		this->m_eBVType = eSegment;
	}

	Vec3f m_Start;
	Vec3f m_End;
};

struct Sphere : public CCollisionEntity
{
	Sphere() { m_eBVType = eSphere; }

	Sphere(float _Radius, const Vec3f _Center)
	{
		m_Radius = _Radius;
		glsCopyVector3(m_Center, _Center);
		m_eBVType = eSphere;
	}

	Vec3f m_Center;
	float m_Radius;
};

struct Capsule : public CCollisionEntity
{
	Capsule() {this->m_eBVType = eCapsule;}

	Capsule(Segment _Segment, float _Radius)
	{
		m_Segment = _Segment;
		m_Radius = _Radius;
		this->m_eBVType = eCapsule;
	}

	Segment m_Segment;
	float m_Radius;
};

class CBaseEntity;

void ComputePlane(Plane &plane, const Vec3f& pointA, const Vec3f& pointB, const Vec3f &pointC);

int ClassifyPointToPlane(const Plane& plane, const Vec3f& point);

int ClassifySphereToPlane(const Plane& plane, const Sphere& sphere);

int ClassifyAabbToPlane(const Plane& plane, const AABB& aabb);

int ClassifyCapsuleToPlane(const Plane& plane, const Capsule& capsule);

void BuildFrustum( Frustum& frustum, float fov, float nearDist, float farDist, float ratio, const Matrix4x4f& camXform );

bool FrustumToSphere(const Frustum& frustum, const Sphere& sphere);

bool FrustumToAABB(const Frustum& frustum, const AABB& aabb);

bool FrustumToCapsule(const Frustum& frustum, const Capsule& capsule);

bool AABBtoAABB(const AABB& lhs, const AABB& rhs);

bool SphereToSphere(const Sphere& lhs, const Sphere& rhs);

bool SphereToAABB(const Sphere& lhs, const AABB& rhs);

bool CapsuleToSphere(const Capsule& capsule, const Sphere& sphere);

bool CheckCollision(CBaseEntity* _entity1, CBaseEntity* _entity2);

#endif
