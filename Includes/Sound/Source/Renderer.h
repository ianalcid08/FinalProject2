//--------------------------------------------------------------------------------
// Renderer
// Written by Justin Murphy
// 
// Super basic rendering functionality wrapped in a namespace
// For tutorial purposes only - was insanely lazy and there is no
// good structure to this code
//--------------------------------------------------------------------------------
#ifndef Renderer_h
#define Renderer_h
//--------------------------------------------------------------------------------
#include <d3d11.h>
#include "Common.h"
//--------------------------------------------------------------------------------
class Entity3D;
class Camera;
//--------------------------------------------------------------------------------
namespace Renderer
{
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
	};
	
	HRESULT InitDevice(HWND hWnd);
	void CleanupDevice();
	void BeginRender();
	void RenderCube(const Entity3D* pObj,const Camera* pCamera);
	void Present();
};
//--------------------------------------------------------------------------------
#endif // Renderer_h
//--------------------------------------------------------------------------------