//--------------------------------------------------------------------------------
// Written by Justin Murphy
//--------------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "Entity3D.h"
//--------------------------------------------------------------------------------
Entity3D::Entity3D()
{
	XMStoreFloat4x4(&m_mWorld,XMMatrixIdentity());
}
//--------------------------------------------------------------------------------
Entity3D::~Entity3D()
{
}
//--------------------------------------------------------------------------------
void Entity3D::SetPosition(const XMFLOAT3 pos)
{
	m_mWorld._41 = pos.x;
	m_mWorld._42 = pos.y;
	m_mWorld._43 = pos.z;
}
//--------------------------------------------------------------------------------
void Entity3D::SetXAxis(const XMFLOAT3 xAxis)
{
	m_mWorld._11 = xAxis.x;
	m_mWorld._12 = xAxis.y;
	m_mWorld._13 = xAxis.z;
}
//--------------------------------------------------------------------------------
void Entity3D::SetYAxis(const XMFLOAT3 yAxis)
{
	m_mWorld._21 = yAxis.x;
	m_mWorld._22 = yAxis.y;
	m_mWorld._23 = yAxis.z;
}
//--------------------------------------------------------------------------------
void Entity3D::SetZAxis(const XMFLOAT3 zAxis)
{
	m_mWorld._31 = zAxis.x;
	m_mWorld._32 = zAxis.y;
	m_mWorld._33 = zAxis.z;
}
//--------------------------------------------------------------------------------