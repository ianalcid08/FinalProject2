//--------------------------------------------------------------------------------
// Entity3D
// Written By: Justin Murphy
//
// very simple entity class holding a transform to represent 
// position and orientation of an object in 3D space
//--------------------------------------------------------------------------------
#ifndef Entity3D_h
#define Entity3D_h
//--------------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "Common.h"
//--------------------------------------------------------------------------------
class Entity3D
{
public:
	Entity3D( );
	virtual ~Entity3D( );
	XMFLOAT4X4 m_mWorld; //initialized to identity

	XMFLOAT3 GetPosition() const { return XMFLOAT3( m_mWorld._41, m_mWorld._42, m_mWorld._43); }
	XMFLOAT3 GetYAxis() const { return XMFLOAT3( m_mWorld._21, m_mWorld._22, m_mWorld._23); }
	XMFLOAT3 GetZAxis() const { return XMFLOAT3( m_mWorld._31, m_mWorld._32, m_mWorld._33);  }

	void SetPosition(const XMFLOAT3 pos);
	void SetXAxis(const XMFLOAT3 xAxis);
	void SetYAxis(const XMFLOAT3 yAxis);
	void SetZAxis(const XMFLOAT3 zAxis);
};
//--------------------------------------------------------------------------------
#endif // Entity3D_h
//--------------------------------------------------------------------------------