#include "../Header Files/stdafx.h"
#include "../Header Files/Frustum.h"

void CFrustum::ComputePlane( tPlane &_plane, const Vec3f& _PointA, const Vec3f& _PointB, const Vec3f &_PointC )
{
	_plane.m_vNormal[0] = (_PointB[1] - _PointA[1]) * (_PointC[2] - _PointB[2]) - (_PointC[1] - _PointB[1]) * (_PointB[2] - _PointA[2]);
	_plane.m_vNormal[1] = -(_PointB[0] - _PointA[0]) * (_PointC[2] - _PointB[2]) + (_PointC[0] - _PointB[0]) * (_PointB[2] - _PointA[2]);
	_plane.m_vNormal[2] = (_PointB[0] - _PointA[0]) * (_PointC[1] - _PointB[1]) - (_PointC[0] - _PointB[0]) * (_PointB[1] - _PointA[1]);

	float normMag = glsGetMagnitude3(_plane.m_vNormal);
	_plane.m_vNormal[0] /= normMag;
	_plane.m_vNormal[1]	/= normMag;
	_plane.m_vNormal[2]	/= normMag;

	_plane.m_fOffset = glsDotProduct3(_plane.m_vNormal, _PointA);
}

void CFrustum::BuildFrustum( float _fFOV, float _fNearDist, float _fFarDist, float _fRatio, Matrix4x4f& _Mat )
{
	m_fFOV = _fFOV;	m_fNearDistance = _fNearDist;	m_fFarDistance = _fFarDist; m_fRatio = _fRatio;

	Vec3f xAxis;	xAxis[0] = _Mat[0]; xAxis[1] = _Mat[2]; xAxis[2] = _Mat[2];
	Vec3f yAxis;	yAxis[0] = _Mat[4]; yAxis[1] = _Mat[5]; yAxis[2] = _Mat[6];
	Vec3f zAxis;	zAxis[0] = _Mat[8]; zAxis[1] = _Mat[9]; zAxis[2] = _Mat[10];
	Vec3f Pos;		Pos[0] = _Mat[12]; Pos[1] = _Mat[13]; Pos[2] = _Mat[14];

	Vec3f vNearClip; 
	vNearClip[0] = Pos[0] + zAxis[0] * _fNearDist;
	vNearClip[1] = Pos[1] + zAxis[1] * _fNearDist;
	vNearClip[2] = Pos[2] + zAxis[2] * _fNearDist;


	Vec3f vFarClip; 
	vFarClip[0] = Pos[0] + zAxis[0] * _fFarDist;
	vFarClip[1] = Pos[1] + zAxis[1] * _fFarDist;
	vFarClip[2] = Pos[2] + zAxis[2] * _fFarDist;


	float fHnear = 2.0f * tan(_fFOV * 0.5f) * _fNearDist;
	float fHfar = 2.0f * tan(_fFOV * 0.5f) * _fFarDist;
	float fWnear = fHnear * _fRatio;
	float fWfar = fHfar * _fRatio;


	// The next 8 lines will calculate the eight points that make up the frustum
#pragma region FAR TOP LEFT CORNER
	m_varrCorners[eFTL][0] = vFarClip[0] + yAxis[0] * (fHfar * 0.5f) - xAxis[0] * (fWfar * 0.5f);
	m_varrCorners[eFTL][1] = vFarClip[1] + yAxis[1] * (fHfar * 0.5f) - xAxis[1] * (fWfar * 0.5f);
	m_varrCorners[eFTL][2] = vFarClip[2] + yAxis[2] * (fHfar * 0.5f) - xAxis[2] * (fWfar * 0.5f);
#pragma endregion
#pragma region FAR BOTTOM LEFT CORNER
	m_varrCorners[eFBL][0] = vFarClip[0] - yAxis[0] * (fHfar * 0.5f) - xAxis[0] * (fWfar * 0.5f);
	m_varrCorners[eFBL][1] = vFarClip[1] - yAxis[1] * (fHfar * 0.5f) - xAxis[1] * (fWfar * 0.5f);
	m_varrCorners[eFBL][2] = vFarClip[2] - yAxis[2] * (fHfar * 0.5f) - xAxis[2] * (fWfar * 0.5f);
#pragma endregion
#pragma region FAR BOTTOM RIGHT CORNER
	m_varrCorners[eFBR][0] = vFarClip[0] - yAxis[0] * (fHfar * 0.5f) + xAxis[0] * (fWfar * 0.5f);
	m_varrCorners[eFBR][1] = vFarClip[1] - yAxis[1] * (fHfar * 0.5f) + xAxis[1] * (fWfar * 0.5f);
	m_varrCorners[eFBR][2] = vFarClip[2] - yAxis[2] * (fHfar * 0.5f) + xAxis[2] * (fWfar * 0.5f);
#pragma endregion
#pragma region FAR TOP RIGHT CORNER
	m_varrCorners[eFTR][0] = vFarClip[0] + yAxis[0] * (fHfar * 0.5f) + xAxis[0] * (fWfar * 0.5f);
	m_varrCorners[eFTR][1] = vFarClip[1] + yAxis[1] * (fHfar * 0.5f) + xAxis[1] * (fWfar * 0.5f);
	m_varrCorners[eFTR][2] = vFarClip[2] + yAxis[2] * (fHfar * 0.5f) + xAxis[2] * (fWfar * 0.5f);
#pragma endregion
#pragma region NEAR TOP LEFT CORNER
	m_varrCorners[eNTL][0] = vNearClip[0] + yAxis[0] * (fHnear * 0.5f) - xAxis[0] * (fWnear * 0.5f);
	m_varrCorners[eNTL][1] = vNearClip[1] + yAxis[1] * (fHnear * 0.5f) - xAxis[1] * (fWnear * 0.5f);
	m_varrCorners[eNTL][2] = vNearClip[2] + yAxis[2] * (fHnear * 0.5f) - xAxis[2] * (fWnear * 0.5f);
#pragma endregion
#pragma region NEAR TOP RIGHT CORNER
	m_varrCorners[eNTR][0] = vNearClip[0] + yAxis[0] * (fHnear * 0.5f) + xAxis[0] * (fWnear * 0.5f);
	m_varrCorners[eNTR][1] = vNearClip[1] + yAxis[1] * (fHnear * 0.5f) + xAxis[1] * (fWnear * 0.5f);
	m_varrCorners[eNTR][2] = vNearClip[2] + yAxis[2] * (fHnear * 0.5f) + xAxis[2] * (fWnear * 0.5f);
#pragma endregion
#pragma region NEAR BOTTOM RIGHT CORNER
	m_varrCorners[eNBR][0] = vNearClip[0] - yAxis[0] * (fHnear * 0.5f) + xAxis[0] * (fWnear * 0.5f);
	m_varrCorners[eNBR][1] = vNearClip[1] - yAxis[1] * (fHnear * 0.5f) + xAxis[1] * (fWnear * 0.5f);
	m_varrCorners[eNBR][2] = vNearClip[2] - yAxis[2] * (fHnear * 0.5f) + xAxis[2] * (fWnear * 0.5f);
#pragma endregion
#pragma region NEAR BOTTOM LEFT CORNER
	m_varrCorners[eNBL][0] = vNearClip[0] - yAxis[0] * (fHnear * 0.5f) - xAxis[0] * (fWnear * 0.5f);
	m_varrCorners[eNBL][1] = vNearClip[1] - yAxis[1] * (fHnear * 0.5f) - xAxis[1] * (fWnear * 0.5f);
	m_varrCorners[eNBL][2] = vNearClip[2] - yAxis[2] * (fHnear * 0.5f) - xAxis[2] * (fWnear * 0.5f);
#pragma endregion

	// the next six lines will take the eight points from above and
	// calculate the planes that will make up the six sides of the frustum
	/*ComputePlane(frustum.m_arrPlanes[eLEFT_PLANE]  , frustum.m_d3darrCorners[eNBL], frustum.m_d3darrCorners[eFBL], frustum.m_d3darrCorners[eFTL]);
	ComputePlane(frustum.m_arrPlanes[eRIGHT_PLANE] , frustum.m_d3darrCorners[eNTR], frustum.m_d3darrCorners[eFTR], frustum.m_d3darrCorners[eFBR]);	
	ComputePlane(frustum.m_arrPlanes[eTOP_PLANE]   , frustum.m_d3darrCorners[eNTL], frustum.m_d3darrCorners[eFTL], frustum.m_d3darrCorners[eFTR]);		
	ComputePlane(frustum.m_arrPlanes[eBOTTOM_PLANE], frustum.m_d3darrCorners[eNBR], frustum.m_d3darrCorners[eFBR], frustum.m_d3darrCorners[eFBL]);	
	ComputePlane(frustum.m_arrPlanes[eNEAR_PLANE]  , frustum.m_d3darrCorners[eNTR], frustum.m_d3darrCorners[eNBR], frustum.m_d3darrCorners[eNBL]);	
	ComputePlane(frustum.m_arrPlanes[eFAR_PLANE]   , frustum.m_d3darrCorners[eFBL], frustum.m_d3darrCorners[eFBR], frustum.m_d3darrCorners[eFTR]);		*/

	CFrustum::ComputePlane(m_arrPlanes[eNEAR_PLANE],	m_varrCorners[eNTR], m_varrCorners[eNBR], m_varrCorners[eNTL]);
	CFrustum::ComputePlane(m_arrPlanes[eFAR_PLANE],		m_varrCorners[eFTL], m_varrCorners[eFBL], m_varrCorners[eFTR]);
	CFrustum::ComputePlane(m_arrPlanes[eLEFT_PLANE],	m_varrCorners[eNTL], m_varrCorners[eNBL], m_varrCorners[eFTL]);
	CFrustum::ComputePlane(m_arrPlanes[eRIGHT_PLANE],	m_varrCorners[eFTR], m_varrCorners[eFBR], m_varrCorners[eNTR]);
	CFrustum::ComputePlane(m_arrPlanes[eTOP_PLANE],		m_varrCorners[eNTL], m_varrCorners[eFTL], m_varrCorners[eNTR]);
	CFrustum::ComputePlane(m_arrPlanes[eBOTTOM_PLANE],	m_varrCorners[eFBL], m_varrCorners[eNBL], m_varrCorners[eFBR]);
}

CFrustum::CFrustum()
{
	//BuildFrustum(80, 0, 1, 1, 0)
	this->m_eBVType = eFrustum;
}

CFrustum::~CFrustum()
{

}

void CFrustum::RebuildFrustum( Matrix4x4f& _Mat )
{
	Vec3f xAxis;	xAxis[0] = _Mat[0]; xAxis[1] = _Mat[2]; xAxis[2] = _Mat[2];
	Vec3f yAxis;	yAxis[0] = _Mat[4]; yAxis[1] = _Mat[5]; yAxis[2] = _Mat[6];
	Vec3f zAxis;	zAxis[0] = _Mat[8]; zAxis[1] = _Mat[9]; zAxis[2] = _Mat[10];
	Vec3f Pos;		Pos[0] = _Mat[12]; Pos[1] = _Mat[13]; Pos[2] = _Mat[14];

	Vec3f vNearClip; 
	vNearClip[0] = Pos[0] + zAxis[0] * m_fNearDistance;
	vNearClip[1] = Pos[1] + zAxis[1] * m_fNearDistance;
	vNearClip[2] = Pos[2] + zAxis[2] * m_fNearDistance;


	Vec3f vFarClip; 
	vFarClip[0] = Pos[0] + zAxis[0] * m_fFarDistance;
	vFarClip[1] = Pos[1] + zAxis[1] * m_fFarDistance;
	vFarClip[2] = Pos[2] + zAxis[2] * m_fFarDistance;


	float fHnear = 2.0f * tan(m_fFOV * 0.5f) * m_fNearDistance;
	float fHfar = 2.0f * tan(m_fFOV * 0.5f) *   m_fFarDistance;
	float fWnear = fHnear * m_fRatio;
	float fWfar = fHfar * m_fRatio;


	// The next 8 lines will calculate the eight points that make up the frustum
#pragma region FAR TOP LEFT CORNER
	m_varrCorners[eFTL][0] = vFarClip[0] + yAxis[0] * (fHfar * 0.5f) - xAxis[0] * (fWfar * 0.5f);
	m_varrCorners[eFTL][1] = vFarClip[1] + yAxis[1] * (fHfar * 0.5f) - xAxis[1] * (fWfar * 0.5f);
	m_varrCorners[eFTL][2] = vFarClip[2] + yAxis[2] * (fHfar * 0.5f) - xAxis[2] * (fWfar * 0.5f);
#pragma endregion
#pragma region FAR BOTTOM LEFT CORNER
	m_varrCorners[eFBL][0] = vFarClip[0] - yAxis[0] * (fHfar * 0.5f) - xAxis[0] * (fWfar * 0.5f);
	m_varrCorners[eFBL][1] = vFarClip[1] - yAxis[1] * (fHfar * 0.5f) - xAxis[1] * (fWfar * 0.5f);
	m_varrCorners[eFBL][2] = vFarClip[2] - yAxis[2] * (fHfar * 0.5f) - xAxis[2] * (fWfar * 0.5f);
#pragma endregion
#pragma region FAR BOTTOM RIGHT CORNER
	m_varrCorners[eFBR][0] = vFarClip[0] - yAxis[0] * (fHfar * 0.5f) + xAxis[0] * (fWfar * 0.5f);
	m_varrCorners[eFBR][1] = vFarClip[1] - yAxis[1] * (fHfar * 0.5f) + xAxis[1] * (fWfar * 0.5f);
	m_varrCorners[eFBR][2] = vFarClip[2] - yAxis[2] * (fHfar * 0.5f) + xAxis[2] * (fWfar * 0.5f);
#pragma endregion
#pragma region FAR TOP RIGHT CORNER
	m_varrCorners[eFTR][0] = vFarClip[0] + yAxis[0] * (fHfar * 0.5f) + xAxis[0] * (fWfar * 0.5f);
	m_varrCorners[eFTR][1] = vFarClip[1] + yAxis[1] * (fHfar * 0.5f) + xAxis[1] * (fWfar * 0.5f);
	m_varrCorners[eFTR][2] = vFarClip[2] + yAxis[2] * (fHfar * 0.5f) + xAxis[2] * (fWfar * 0.5f);
#pragma endregion
#pragma region NEAR TOP LEFT CORNER
	m_varrCorners[eNTL][0] = vNearClip[0] + yAxis[0] * (fHnear * 0.5f) - xAxis[0] * (fWnear * 0.5f);
	m_varrCorners[eNTL][1] = vNearClip[1] + yAxis[1] * (fHnear * 0.5f) - xAxis[1] * (fWnear * 0.5f);
	m_varrCorners[eNTL][2] = vNearClip[2] + yAxis[2] * (fHnear * 0.5f) - xAxis[2] * (fWnear * 0.5f);
#pragma endregion
#pragma region NEAR TOP RIGHT CORNER
	m_varrCorners[eNTR][0] = vNearClip[0] + yAxis[0] * (fHnear * 0.5f) + xAxis[0] * (fWnear * 0.5f);
	m_varrCorners[eNTR][1] = vNearClip[1] + yAxis[1] * (fHnear * 0.5f) + xAxis[1] * (fWnear * 0.5f);
	m_varrCorners[eNTR][2] = vNearClip[2] + yAxis[2] * (fHnear * 0.5f) + xAxis[2] * (fWnear * 0.5f);
#pragma endregion
#pragma region NEAR BOTTOM RIGHT CORNER
	m_varrCorners[eNBR][0] = vNearClip[0] - yAxis[0] * (fHnear * 0.5f) + xAxis[0] * (fWnear * 0.5f);
	m_varrCorners[eNBR][1] = vNearClip[1] - yAxis[1] * (fHnear * 0.5f) + xAxis[1] * (fWnear * 0.5f);
	m_varrCorners[eNBR][2] = vNearClip[2] - yAxis[2] * (fHnear * 0.5f) + xAxis[2] * (fWnear * 0.5f);
#pragma endregion
#pragma region NEAR BOTTOM LEFT CORNER
	m_varrCorners[eNBL][0] = vNearClip[0] - yAxis[0] * (fHnear * 0.5f) - xAxis[0] * (fWnear * 0.5f);
	m_varrCorners[eNBL][1] = vNearClip[1] - yAxis[1] * (fHnear * 0.5f) - xAxis[1] * (fWnear * 0.5f);
	m_varrCorners[eNBL][2] = vNearClip[2] - yAxis[2] * (fHnear * 0.5f) - xAxis[2] * (fWnear * 0.5f);
#pragma endregion

	// the next six lines will take the eight points from above and
	// calculate the planes that will make up the six sides of the frustum
	/*ComputePlane(frustum.m_arrPlanes[eLEFT_PLANE]  , frustum.m_d3darrCorners[eNBL], frustum.m_d3darrCorners[eFBL], frustum.m_d3darrCorners[eFTL]);
	ComputePlane(frustum.m_arrPlanes[eRIGHT_PLANE] , frustum.m_d3darrCorners[eNTR], frustum.m_d3darrCorners[eFTR], frustum.m_d3darrCorners[eFBR]);	
	ComputePlane(frustum.m_arrPlanes[eTOP_PLANE]   , frustum.m_d3darrCorners[eNTL], frustum.m_d3darrCorners[eFTL], frustum.m_d3darrCorners[eFTR]);		
	ComputePlane(frustum.m_arrPlanes[eBOTTOM_PLANE], frustum.m_d3darrCorners[eNBR], frustum.m_d3darrCorners[eFBR], frustum.m_d3darrCorners[eFBL]);	
	ComputePlane(frustum.m_arrPlanes[eNEAR_PLANE]  , frustum.m_d3darrCorners[eNTR], frustum.m_d3darrCorners[eNBR], frustum.m_d3darrCorners[eNBL]);	
	ComputePlane(frustum.m_arrPlanes[eFAR_PLANE]   , frustum.m_d3darrCorners[eFBL], frustum.m_d3darrCorners[eFBR], frustum.m_d3darrCorners[eFTR]);		*/

	CFrustum::ComputePlane(m_arrPlanes[eNEAR_PLANE],	m_varrCorners[eNTR], m_varrCorners[eNBR], m_varrCorners[eNTL]);
	CFrustum::ComputePlane(m_arrPlanes[eFAR_PLANE],		m_varrCorners[eFTL], m_varrCorners[eFBL], m_varrCorners[eFTR]);
	CFrustum::ComputePlane(m_arrPlanes[eLEFT_PLANE],	m_varrCorners[eNTL], m_varrCorners[eNBL], m_varrCorners[eFTL]);
	CFrustum::ComputePlane(m_arrPlanes[eRIGHT_PLANE],	m_varrCorners[eFTR], m_varrCorners[eFBR], m_varrCorners[eNTR]);
	CFrustum::ComputePlane(m_arrPlanes[eTOP_PLANE],		m_varrCorners[eNTL], m_varrCorners[eFTL], m_varrCorners[eNTR]);
	CFrustum::ComputePlane(m_arrPlanes[eBOTTOM_PLANE],	m_varrCorners[eFBL], m_varrCorners[eNBL], m_varrCorners[eFBR]);
}
