#include "../Header Files/stdafx.h"
#include "../header files/OBB.h"

COBB::COBB()
{
	this->m_eBVType = eOBB;
}

COBB::~COBB()
{

}

void COBB::SetCenter( Vec3f _newCenter )
{
	glsCopyVector3(m_vCenter, _newCenter);
}

void COBB::ReOrient( Vec3f _right, Vec3f _up, Vec3f _forward )
{
	m_mOrintation[0] = _right[0];	m_mOrintation[1] = _right[1];	m_mOrintation[2] = _right[2];
	m_mOrintation[3] = _up[0];		m_mOrintation[4] = _up[1];		m_mOrintation[5] = _up[2];
	m_mOrintation[6] = _forward[0];	m_mOrintation[7] = _forward[1];	m_mOrintation[8] = _forward[2];
}

void COBB::ReOrient( Matrix3x3f _newO )
{
	memcpy_s(&m_mOrintation, sizeof(Matrix3x3f), &_newO, sizeof(Matrix3x3f));
}

void COBB::SetExtents( Vec3f _newExtents )
{
	glsCopyVector3(m_HalfwayExtents, _newExtents);
}

void COBB::SetExtents( float _x, float _Y, float _Z )
{
	glsLoadVector3(m_HalfwayExtents, _x, _Y, _Z);
}

