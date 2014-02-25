#include "../Header Files/stdafx.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/BaseEntity.h"

// ComputePlane
//
// Calculate the plane normal and plane offset from the input points
void ComputePlane(Plane &plane, const Vec3f& pointA, const Vec3f& pointB, const Vec3f &pointC)
{
	Vec3f vecAB;
	Vec3f vecBC;
	glsSubtractVectors3(vecAB, pointB, pointA);
	glsSubtractVectors3(vecBC, pointC, pointB);
	glsCrossProduct3(plane.normal,vecAB,vecBC);
	glsNormalizeVector3(plane.normal);
	plane.offset = glsDotProduct3(plane.normal,pointA);
}

// ClassifyPointToPlane
//
// Perform a half-space test. Returns 1 if the point is on or in front of the plane.
// Returns 2 if the point is behind the plane.
int ClassifyPointToPlane(const Plane& plane, const Vec3f& point)
{
	if(glsDotProduct3(plane.normal,point) > plane.offset)
		return 1;
	else
		return 2;
}

// ClassifySphereToPlane
//
// Perform a sphere-to-plane test. 
// Returns 1 if the sphere is in front of the plane.
// Returns 2 if the sphere is behind the plane.
// Returns 3 if the sphere straddles the plane.
int ClassifySphereToPlane(const Plane& plane, const Sphere& sphere)
{
	if((glsDotProduct3(plane.normal,sphere.m_Center) - plane.offset) > sphere.m_Radius)
		return 1;
	else if ((glsDotProduct3(plane.normal,sphere.m_Center) - plane.offset) < -sphere.m_Radius)
		return 2;
	else
		return 3;
}

// ClassifyAabbToPlane
//
// Performs a AABB-to-plane test.
// Returns 1 if the aabb is in front of the plane.
// Returns 2 if the aabb is behind the plane.
// Returns 3 if the aabb straddles the plane.
int ClassifyAabbToPlane(const Plane& plane, const AABB& aabb)
{
	Sphere mySphere;
	glsAddVectors3(mySphere.m_Center, aabb.min, aabb.max);
	glsScaleVector3(mySphere.m_Center, 0.5f);
	Vec3f extent;
	glsSubtractVectors3(extent, mySphere.m_Center, aabb.max);
	float XTemp,YTemp,ZTemp;
	XTemp = abs(plane.normal[0] * extent[0]);
	YTemp = abs(plane.normal[1] * extent[1]);
	ZTemp = abs(plane.normal[2] * extent[2]);
	mySphere.m_Radius = XTemp+YTemp+ZTemp;

	return ClassifySphereToPlane(plane,mySphere);
}

// ClassifyCapsuleToPlane
//
// Performs a Capsule-to-plane test.
// Returns 1 if the aabb is in front of the plane.
// Returns 2 if the aabb is behind the plane.
// Returns 3 if the aabb straddles the plane.
int ClassifyCapsuleToPlane(const Plane& plane, const Capsule& capsule)
{
	if((glsDotProduct3(plane.normal,capsule.m_Segment.m_Start) - plane.offset) > capsule.m_Radius &&
       (glsDotProduct3(plane.normal,capsule.m_Segment.m_End) - plane.offset) > capsule.m_Radius)
		return 1;
	else if((glsDotProduct3(plane.normal,capsule.m_Segment.m_Start) - plane.offset) < -capsule.m_Radius &&
            (glsDotProduct3(plane.normal,capsule.m_Segment.m_End) - plane.offset) < -capsule.m_Radius)
		return 2;
	else
		return 3;
}

// BuildFrustum
//
// Calculates the corner points and planes of the frustum based upon input values.
// Should call ComputePlane.
void BuildFrustum( Frustum& frustum, float fov, float nearDist, float farDist, float ratio, const Matrix4x4f& camXform )
{
	Vec3f nearCenterPoint;
	Vec3f wAxis = {camXform[12], camXform[13], camXform[14]};
	Vec3f zAxis = {camXform[8], camXform[9], camXform[10]};
	Vec3f yAxis = {camXform[4], camXform[5], camXform[6]};
	Vec3f xAxis = {camXform[0], camXform[1], camXform[2]};
	glsScaleVector3(zAxis, nearDist);
	glsSubtractVectors3(nearCenterPoint, wAxis, zAxis);
	Vec3f farCenterPoint;
	zAxis[0] = camXform[0];
	zAxis[1] = camXform[1];
	zAxis[2] = camXform[2];
	glsScaleVector3(zAxis, farDist);
	glsSubtractVectors3(nearCenterPoint, wAxis, zAxis);

	float Hnear = 2* tan(fov/2) * nearDist;
	float Hfar = 2* tan(fov/2) * farDist;

	float Wnear = Hnear * ratio;
	float Wfar = Hfar * ratio;

#pragma region constructs the frustum
	// Sets teh FTL corner of the view frustum
	glsScaleVector3(yAxis, (Hfar*0.5f));
	glsAddVectors3(yAxis, yAxis, farCenterPoint);
	glsScaleVector3(xAxis, (Wfar*0.5f));
	glsSubtractVectors3(frustum.corners[eFTL], yAxis, xAxis);


	// Resets memory back to cam transform
	yAxis[0] = camXform[4];
	yAxis[1] = camXform[5];
	yAxis[2] = camXform[6];

	xAxis[0] = camXform[8];
	xAxis[1] = camXform[9];
	xAxis[2] = camXform[10];

	// Sets teh FTR corner of the view frustum
	glsScaleVector3(yAxis, (Hfar*0.5f));
	glsAddVectors3(yAxis, yAxis, farCenterPoint);
	glsScaleVector3(xAxis, (Wfar*0.5f));
	glsAddVectors3(frustum.corners[eFTR], yAxis, xAxis);


	// Resets memory back to cam transform
	yAxis[0] = camXform[4];
	yAxis[1] = camXform[5];
	yAxis[2] = camXform[6];

	xAxis[0] = camXform[8];
	xAxis[1] = camXform[9];
	xAxis[2] = camXform[10];

	// Sets teh FBL corner of the view frustum
	glsScaleVector3(yAxis, (Hfar*0.5f));
	glsSubtractVectors3(yAxis, yAxis, farCenterPoint);
	glsScaleVector3(xAxis, (Wfar*0.5f));
	glsSubtractVectors3(frustum.corners[eFBL], yAxis, xAxis);


	// Resets memory back to cam transform
	yAxis[0] = camXform[4];
	yAxis[1] = camXform[5];
	yAxis[2] = camXform[6];

	xAxis[0] = camXform[8];
	xAxis[1] = camXform[9];
	xAxis[2] = camXform[10];

	// Sets teh FBR corner of the view frustum
	glsScaleVector3(yAxis, (Hfar*0.5f));
	glsSubtractVectors3(yAxis, yAxis, farCenterPoint);
	glsScaleVector3(xAxis, (Wfar*0.5f));
	glsAddVectors3(frustum.corners[eFBR], yAxis, xAxis);


	// Resets memory back to cam transform
	yAxis[0] = camXform[4];
	yAxis[1] = camXform[5];
	yAxis[2] = camXform[6];

	xAxis[0] = camXform[8];
	xAxis[1] = camXform[9];
	xAxis[2] = camXform[10];

	// Sets teh NTL corner of the view frustum
	glsScaleVector3(yAxis, (Hnear*0.5f));
	glsAddVectors3(yAxis, yAxis, nearCenterPoint);
	glsScaleVector3(xAxis, (Wnear*0.5f));
	glsSubtractVectors3(frustum.corners[eNTL], yAxis, xAxis);

	// Resets memory back to cam transform
	yAxis[0] = camXform[4];
	yAxis[1] = camXform[5];
	yAxis[2] = camXform[6];

	xAxis[0] = camXform[8];
	xAxis[1] = camXform[9];
	xAxis[2] = camXform[10];

	// Sets teh NTR corner of the view frustum
	glsScaleVector3(yAxis, (Hnear*0.5f));
	glsAddVectors3(yAxis, yAxis, nearCenterPoint);
	glsScaleVector3(xAxis, (Wnear*0.5f));
	glsAddVectors3(frustum.corners[eNTR], yAxis, xAxis);

	// Resets memory back to cam transform
	yAxis[0] = camXform[4];
	yAxis[1] = camXform[5];
	yAxis[2] = camXform[6];

	xAxis[0] = camXform[8];
	xAxis[1] = camXform[9];
	xAxis[2] = camXform[10];

	// Sets teh NBL corner of the view frustum
	glsScaleVector3(yAxis, (Hnear*0.5f));
	glsSubtractVectors3(yAxis, yAxis, nearCenterPoint);
	glsScaleVector3(xAxis, (Wnear*0.5f));
	glsSubtractVectors3(frustum.corners[eNBL], yAxis, xAxis);

	// Resets memory back to cam transform
	yAxis[0] = camXform[4];
	yAxis[1] = camXform[5];
	yAxis[2] = camXform[6];

	xAxis[0] = camXform[8];
	xAxis[1] = camXform[9];
	xAxis[2] = camXform[10];

	// Sets teh NBR corner of the view frustum
	glsScaleVector3(yAxis, (Hnear*0.5f));
	glsSubtractVectors3(yAxis, yAxis, nearCenterPoint);
	glsScaleVector3(xAxis, (Wnear*0.5f));
	glsAddVectors3(frustum.corners[eNBR], yAxis, xAxis);

	// Resets memory back to cam transform
	yAxis[0] = camXform[4];
	yAxis[1] = camXform[5];
	yAxis[2] = camXform[6];

	xAxis[0] = camXform[8];
	xAxis[1] = camXform[9];
	xAxis[2] = camXform[10];
#pragma endregion

	ComputePlane(frustum.planes[eNEAR_PLANE],  frustum.corners[eNTL],frustum.corners[eNTR],frustum.corners[eNBL]);
	ComputePlane(frustum.planes[eFAR_PLANE],   frustum.corners[eFTR],frustum.corners[eFTL],frustum.corners[eFBR]);
	ComputePlane(frustum.planes[eTOP_PLANE],   frustum.corners[eFTL],frustum.corners[eFTR],frustum.corners[eNTR]);
	ComputePlane(frustum.planes[eBOTTOM_PLANE],frustum.corners[eNBL],frustum.corners[eNBR],frustum.corners[eFBR]);
	ComputePlane(frustum.planes[eLEFT_PLANE],  frustum.corners[eFTL],frustum.corners[eNTL],frustum.corners[eNBL]);
	ComputePlane(frustum.planes[eRIGHT_PLANE], frustum.corners[eNTR],frustum.corners[eFTR],frustum.corners[eFBR]);

}

// FrustumToSphere
//
// Perform a Sphere-to-Frustum check. Returns true if the sphere is inside. False if not.
bool FrustumToSphere(const Frustum& frustum, const Sphere& sphere)
{
	for(int i = 0; i < 6; i++)
	{
		if(ClassifySphereToPlane(frustum.planes[i],sphere) == 2)
			return false;
	}
	return true;
}

// FrustumToAABB
//
// Perform a Aabb-to-Frustum check. Returns true if the aabb is inside. False if not.
bool FrustumToAABB(const Frustum& frustum, const AABB& aabb)
{
	
	for(int i = 0; i < 6; i++)
	{
		if(ClassifyAabbToPlane(frustum.planes[i],aabb) == 2)
			return false;
	}
	return true;
}

// FrustumToCapsule
//
// Perform a Capsule-to-Frustum check. Returns true if the Capsule is inside. False if not.
bool FrustumToCapsule(const Frustum& frustum, const Capsule& capsule)
{
	for(int i = 0; i < 6; i++)
	{
		if(ClassifyCapsuleToPlane(frustum.planes[i],capsule) == 2)
			return false;
	}
	return true;
}

// AABBtoAABB
//
// Returns true if the AABBs collide. False if not.
bool AABBtoAABB(const AABB& lhs, const AABB& rhs)
{
	if( lhs.max[0] < rhs.min[0] || lhs.min[0] > rhs.max[0] ) 
		return false;		
	if( lhs.max[1] < rhs.min[1] || lhs.min[1] > rhs.max[1] ) 
		return false; 		
	if( lhs.max[2] < rhs.min[2] || lhs.min[2] > rhs.max[2] ) 
		return false;
	return true;
}

// SphereToSphere
//
// Returns true if the Spheres collide. False if not.
bool SphereToSphere(const Sphere& lhs, const Sphere& rhs)
{
	Vec3f v;
	glsSubtractVectors3(v, lhs.m_Center, rhs.m_Center);
	if(glsGetMagnitudeSquared3(v) < (lhs.m_Radius + rhs.m_Radius) * (lhs.m_Radius + rhs.m_Radius))
		return true;
	else
		return false;
}

// SphereToAABB
//
// Returns true if the sphere collides with the AABB. False if not.
bool SphereToAABB(const Sphere& lhs, const AABB& rhs)
{
	Vec3f closeVec;
	//finding the closest vec for ranges
	if(lhs.m_Center[0] < rhs.min[0])
		closeVec[0] = rhs.min[0];
	else if(lhs.m_Center[0] > rhs.max[0])
		closeVec[0] = rhs.max[0];
	else
		closeVec[0] = lhs.m_Center[0];

	if(lhs.m_Center[1] < rhs.min[1])
		closeVec[1] = rhs.min[1];
	else if(lhs.m_Center[1] > rhs.max[1])
		closeVec[1] = rhs.max[1];
	else
		closeVec[1] = lhs.m_Center[1];

	if(lhs.m_Center[2] < rhs.min[2])
		closeVec[2] = rhs.min[2];
	else if(lhs.m_Center[2] > rhs.max[2])
		closeVec[2] = rhs.max[2];
	else
		closeVec[2] = lhs.m_Center[2];

	float XTemp,YTemp,ZTemp, power;
	XTemp = pow(closeVec[0] - lhs.m_Center[0],2);
	YTemp = pow(closeVec[1] - lhs.m_Center[1],2);
	ZTemp = pow(closeVec[2] - lhs.m_Center[2],2);
	power = sqrt(XTemp+YTemp+ZTemp);

	if(power < lhs.m_Radius)
		return true;

	return false;

}

// CapsuleToSphere
//
// Returns true if the capsule collides with the sphere. False if not.
bool CapsuleToSphere(const Capsule& capsule, const Sphere& sphere)
{
	Vec3f lineSeg;
	glsSubtractVectors3(lineSeg, capsule.m_Segment.m_End, capsule.m_Segment.m_Start);
	//lineSeg.normalize();

	Vec3f vecV;
	glsSubtractVectors3(vecV, sphere.m_Center, capsule.m_Segment.m_Start);

	float fProjectionRatio = glsDotProduct3(lineSeg,vecV) / glsDotProduct3(lineSeg,lineSeg);
	float distD = max(0.0f, min(fProjectionRatio, 1.0f));

	glsScaleVector3(lineSeg, distD);

	//Closest point on the line segment
	Vec3f closestPoint;
	glsAddVectors3(closestPoint, capsule.m_Segment.m_Start, lineSeg);

	Sphere capusuleSphere;
	glsCopyVector3(capusuleSphere.m_Center, closestPoint);
	capusuleSphere.m_Radius = capsule.m_Radius;

	return SphereToSphere(sphere,capusuleSphere);
}

bool CheckCollision(CBaseEntity* _entity1, CBaseEntity* _entity2)
{
	if(_entity1 == nullptr || _entity2 == nullptr)
	{
		return false;
	}

	int type1 = _entity1->GetCollisionEntity()->GetBVType();
	int type2 = _entity2->GetCollisionEntity()->GetBVType();

	switch (type1)
	{
	case eSphere:
		{
			Sphere check1 = *(Sphere*)_entity1->GetCollisionEntity();
			if(type2 == eSphere)										//Both Entities are spheres
			{
				
				Sphere check2 = *(Sphere*)_entity2->GetCollisionEntity();

				return SphereToSphere(check1,check2);
			}
			else if(type2 == eAABB)											// Entity2 is an AABB
			{
				return SphereToAABB(check1,*(AABB*)_entity2->GetCollisionEntity());
			}
			else if(type2 == eCapsule)
			{
				return CapsuleToSphere( *((Capsule*)_entity2->GetCollisionEntity()), check1);
			}
		}

	case eCapsule:
		{
			Capsule check1 = *((Capsule*)_entity1->GetCollisionEntity());
			if(type2 == eSphere)
			{
				Sphere check2 = Sphere( ((Sphere*)_entity2->GetCollisionEntity())->m_Radius, _entity2->GetRenderNode().GetPosition() );
				return CapsuleToSphere( check1, check2);
			}
		}
	default:
		break;
	}

	return false;

	//if(type1 == 

	//ComputePlane(Plane &pl

	//	ClassifyPointToPlane(co
	//	ClassifySphereToPlane(c
	//	ClassifyAabbToPlane(con
	//	ClassifyCapsuleToPlane(
	//	BuildFrustum( Frustum&
	//	FrustumToSphere(const 
	//	FrustumToAABB(const Fr
	//	FrustumToCapsule(const
	//	AABBtoAABB(const AABB&
	//	SphereToSphere(const S
	//	SphereToAABB(const Sph
	//	CapsuleToSphere(const 
	//	CheckCollision(CBaseEn




}
