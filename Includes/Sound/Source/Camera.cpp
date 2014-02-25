//--------------------------------------------------------------------------------
// Written by Justin Murphy
//--------------------------------------------------------------------------------
#include "Camera.h"
#include "InputManager.h"
//--------------------------------------------------------------------------------
Camera::Camera() :
	m_vec3RightW(1.0f, 0.0f, 0.0f),
	m_vec3UpW(0.0f, 1.0f, 0.0f),
	m_vec3LookW(0.0f, 0.0f, 1.0f),
	m_vec3Dir(0.0f, 0.0f, 0.0f),
	m_fSpeed(5),
	m_fMouseDX(0.f),
	m_fMouseDY(0.f),
	m_fNear(0.f),
	m_fFar(0.f),
	m_fFOV(0.f),
	m_fAspectRatio(0.f)

{
	XMStoreFloat4x4(&m_mtxView,XMMatrixIdentity());
	XMStoreFloat4x4(&m_mtxProj,XMMatrixIdentity());
	XMStoreFloat4x4(&m_mtxViewProj,XMMatrixIdentity());

	// WASD input registering	
	InputManager()->ActivateKey('W');
	InputManager()->ActivateKey('S');
	InputManager()->ActivateKey('A');
	InputManager()->ActivateKey('D');
	InputManager()->ActivateKey(VK_LBUTTON);
	InputManager()->ActivateKey(VK_SHIFT);
}
//--------------------------------------------------------------------------------
Camera::~Camera()
{
	InputManager()->DeActivateKey('W');
	InputManager()->DeActivateKey('S');
	InputManager()->DeActivateKey('A');
	InputManager()->DeActivateKey('D');
	InputManager()->DeActivateKey(VK_LBUTTON);
	InputManager()->DeActivateKey(VK_SHIFT);
}
//--------------------------------------------------------------------------------
void Camera::SetLens(float fov, float aspect, float nearZ, float farZ)
{
	m_fNear = nearZ;
	m_fFar	= farZ;
	m_fFOV	= fov;
	m_fAspectRatio = aspect;
	XMStoreFloat4x4(&m_mtxProj,XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ));
	XMStoreFloat4x4(&m_mtxViewProj,XMLoadFloat4x4(&m_mtxView)*XMLoadFloat4x4(&m_mtxProj));
}
//--------------------------------------------------------------------------------
void Camera::SetSpeed(float s)
{
	m_fSpeed = s;
}
//--------------------------------------------------------------------------------
void Camera::Update(const float fDeltaTime)
{
	if(InputManager()->GetKeyDown('W'))
		XMStoreFloat3(&m_vec3Dir, XMLoadFloat3(&m_vec3Dir) + XMLoadFloat3(&m_vec3LookW));

	if(InputManager()->GetKeyDown('S'))
		XMStoreFloat3(&m_vec3Dir, XMLoadFloat3(&m_vec3Dir) - XMLoadFloat3(&m_vec3LookW));

	if(InputManager()->GetKeyDown('A'))
		XMStoreFloat3(&m_vec3Dir, XMLoadFloat3(&m_vec3Dir) - XMLoadFloat3(&m_vec3RightW));

	if(InputManager()->GetKeyDown('D'))
		XMStoreFloat3(&m_vec3Dir, XMLoadFloat3(&m_vec3Dir) + XMLoadFloat3(&m_vec3RightW));

	if(InputManager()->GetKeyDown(VK_LBUTTON))
	{
		float pitch  = ((float)InputManager()->GetMouseInput().y - InputManager()->GetMouseInput().prevY) / 250.3f;
		float yAngle = ((float)InputManager()->GetMouseInput().x - InputManager()->GetMouseInput().prevX) / 250.3f;

		// Rotate camera's look and up vectors around the camera's right vector.
		XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&m_vec3RightW),pitch);
		XMStoreFloat3(&m_vec3LookW,XMVector3TransformNormal(XMLoadFloat3(&m_vec3LookW),r));
		XMStoreFloat3(&m_vec3UpW,XMVector3TransformNormal(XMLoadFloat3(&m_vec3UpW),r));

		// Rotate camera axis about the world's y-axis.
		r = XMMatrixRotationY(yAngle);
		XMStoreFloat3(&m_vec3RightW,XMVector3TransformNormal(XMLoadFloat3(&m_vec3RightW),r));
		XMStoreFloat3(&m_vec3UpW,XMVector3TransformNormal(XMLoadFloat3(&m_vec3UpW),r));
		XMStoreFloat3(&m_vec3LookW,XMVector3TransformNormal(XMLoadFloat3(&m_vec3LookW),r));
	}
	
	// move at the set speed along net direction.
	XMStoreFloat3(&m_vec3Dir,XMVector3Normalize(XMLoadFloat3(&m_vec3Dir)));
	
	XMFLOAT3 pos(0.f,0.f,0.f);
	if(InputManager()->GetKeyDown(VK_SHIFT))
		XMStoreFloat3(&pos,XMLoadFloat3(&GetPosition())+XMLoadFloat3(&m_vec3Dir)*m_fSpeed*10*fDeltaTime);
	else
		XMStoreFloat3(&pos,XMLoadFloat3(&GetPosition())+XMLoadFloat3(&m_vec3Dir)*m_fSpeed*fDeltaTime);
	SetPosition(pos);

	// reset direction and mouse distance changes
	m_vec3Dir = XMFLOAT3(0,0,0);
	m_fMouseDX = m_fMouseDY = 0.0f;
		
	BuildView();
}
//--------------------------------------------------------------------------------
void Camera::BuildView()
{
	// Keep camera's axes orthogonal to each other and of unit length.
	XMStoreFloat3(&m_vec3LookW,XMVector3Normalize(XMLoadFloat3(&m_vec3LookW)));

	XMStoreFloat3(&m_vec3UpW,XMVector3Cross(XMLoadFloat3(&m_vec3LookW),XMLoadFloat3(&m_vec3RightW)));
	XMStoreFloat3(&m_vec3UpW,XMVector3Normalize(XMLoadFloat3(&m_vec3UpW)));

	XMStoreFloat3(&m_vec3RightW,XMVector3Cross(XMLoadFloat3(&m_vec3UpW),XMLoadFloat3(&m_vec3LookW)));
	XMStoreFloat3(&m_vec3RightW,XMVector3Normalize(XMLoadFloat3(&m_vec3RightW)));

	float x,y,z;
	XMStoreFloat(&x,-XMVector3Dot(XMLoadFloat3(&GetPosition()),XMLoadFloat3(&m_vec3RightW)));
	XMStoreFloat(&y,-XMVector3Dot(XMLoadFloat3(&GetPosition()),XMLoadFloat3(&m_vec3UpW)));
	XMStoreFloat(&z,-XMVector3Dot(XMLoadFloat3(&GetPosition()),XMLoadFloat3(&m_vec3LookW)));

	m_mtxView(0,0) = m_vec3RightW.x; 
	m_mtxView(1,0) = m_vec3RightW.y; 
	m_mtxView(2,0) = m_vec3RightW.z; 
	m_mtxView(3,0) = x;   

	m_mtxView(0,1) = m_vec3UpW.x;
	m_mtxView(1,1) = m_vec3UpW.y;
	m_mtxView(2,1) = m_vec3UpW.z;
	m_mtxView(3,1) = y;  

	m_mtxView(0,2) = m_vec3LookW.x; 
	m_mtxView(1,2) = m_vec3LookW.y; 
	m_mtxView(2,2) = m_vec3LookW.z; 
	m_mtxView(3,2) = z;   

	m_mtxView(0,3) = 0.0f;
	m_mtxView(1,3) = 0.0f;
	m_mtxView(2,3) = 0.0f;
	m_mtxView(3,3) = 1.0f;

	//Update the local transform
	SetXAxis(m_vec3RightW);
	SetYAxis(m_vec3UpW);
	SetZAxis(m_vec3LookW);

	XMStoreFloat4x4(&m_mtxViewProj,XMLoadFloat4x4(&m_mtxView)*XMLoadFloat4x4(&m_mtxProj));
}
//--------------------------------------------------------------------------------
void Camera::LookAt(const XMFLOAT3 pos,const XMFLOAT3 target,const XMFLOAT3 up)
{
	XMVECTOR L = XMLoadFloat3(&target) - XMLoadFloat3(&pos);
	L = XMVector3Normalize(L);

	XMVECTOR R = XMVector3Cross(XMLoadFloat3(&up),L);
	R = XMVector3Normalize(R);

	XMVECTOR U = XMVector3Cross(L,R);
	U = XMVector3Normalize(U);

	XMStoreFloat3(&m_vec3RightW, R);
	XMStoreFloat3(&m_vec3UpW, U);
	XMStoreFloat3(&m_vec3LookW, L);
	SetPosition(pos);

	BuildView();
}
//--------------------------------------------------------------------------------