//--------------------------------------------------------------------------------
// Camera
// Written By: Justin Murphy
// basic debug camera to fly through scene, uses mouse move for rotation and wasd for movement
//--------------------------------------------------------------------------------
#ifndef Camera_h
#define Camera_h
//--------------------------------------------------------------------------------
#include "Entity3D.h"
//--------------------------------------------------------------------------------
class Camera : public Entity3D
{
public:
	void LookAt(const XMFLOAT3 eye,const XMFLOAT3 at,const XMFLOAT3 up);
	void SetSpeed(float s);
	virtual void SetLens(float fov, float aspect, float nearZ, float farZ);
	virtual void Update(const float fDeltaTime);
	
	const XMFLOAT4X4 GetView() const { return m_mtxView; }
	const XMFLOAT4X4 GetProjection() const { return m_mtxProj; }
	const XMFLOAT4X4 GetViewProjection() const { return m_mtxViewProj; }
	
	Camera();
	~Camera();
private:
	XMFLOAT4X4	m_mtxView;
	XMFLOAT4X4	m_mtxProj;
	XMFLOAT4X4	m_mtxViewProj;

	float m_fNear;
	float m_fFar;
	float m_fFOV;
	float m_fAspectRatio;

	XMFLOAT3 m_vec3RightW;
	XMFLOAT3 m_vec3UpW;
	XMFLOAT3 m_vec3LookW;
	XMFLOAT3 m_vec3Dir;
	
	float m_fMouseDX;
	float m_fMouseDY;
	float m_fSpeed;

	void BuildView();
};
//--------------------------------------------------------------------------------
#endif // Camera_h
//--------------------------------------------------------------------------------