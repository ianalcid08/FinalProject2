//--------------------------------------------------------------------------------
// Written by Justin Murphy
//--------------------------------------------------------------------------------
#include "Renderer.h"
#include "FileLoader.h"
#include "Entity3D.h"
#include "Camera.h"
#include "Common.h"
//--------------------------------------------------------------------------------
//include the needed libraries for rendering
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
//--------------------------------------------------------------------------------
namespace Renderer
{
	//--------------------------------------------------------------------------------
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           g_pd3dDevice = NULL;
	ID3D11DeviceContext*    g_pImmediateContext = NULL;
	IDXGISwapChain*         g_pSwapChain = NULL;
	ID3D11RenderTargetView* g_pRenderTargetView = NULL;
	ID3D11Texture2D*        g_pDepthStencil = NULL;
	ID3D11DepthStencilView* g_pDepthStencilView = NULL;
	ID3D11VertexShader*     g_pVertexShader = NULL;
	ID3D11PixelShader*      g_pPixelShader = NULL;
	ID3D11InputLayout*      g_pVertexLayout = NULL;
	ID3D11Buffer*           g_pVertexBuffer = NULL;
	ID3D11Buffer*           g_pIndexBuffer = NULL;
	ID3D11Buffer*           g_pConstantBuffer = NULL;
	//--------------------------------------------------------------------------------
	HRESULT InitDevice(HWND hWnd)
	{
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect( hWnd, &rc );
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			g_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
			if( SUCCEEDED( hr ) )
				break;
		}
		if( FAILED( hr ) )
			return hr;
		LogWrite(L"Created Device and SwapChain Successfully");

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		if( FAILED( hr ) )
			return hr;

		hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
		pBackBuffer->Release();
		if( FAILED( hr ) )
			return hr;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
		if( FAILED( hr ) )
			return hr;

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
		if( FAILED( hr ) )
			return hr;

		g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_pImmediateContext->RSSetViewports( 1, &vp );
		
		FileLoader vertexShader;
		if(!vertexShader.Open(L"../Resources/Shaders/PosColorVS.cso"))
		{
			MessageBox( NULL,
				L"The vertex hlsl file couldn't be found.  Please ensure the path is correct.", L"Error", MB_OK );
			return -1;
		}

		hr = g_pd3dDevice->CreateVertexShader(
			vertexShader.GetDataPtr(),
			vertexShader.GetDataSize(),
			0, &g_pVertexShader );
		if( FAILED( hr ) )
		{
			MessageBox( NULL,
				L"Failed to create vertex shader from compiled file", L"Error", MB_OK );
			return hr;
		}

		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE( layout );

		// Create the input layout
		hr = g_pd3dDevice->CreateInputLayout( layout, numElements, vertexShader.GetDataPtr(),
			vertexShader.GetDataSize(), &g_pVertexLayout );
		vertexShader.Close();
		if( FAILED( hr ) )
			return hr;

		// Set the input layout
		g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

		FileLoader pixelShader;
		if(!pixelShader.Open(L"../Resources/Shaders/PosColorPS.cso"))
		{
			MessageBox( NULL,
				L"The pixel hlsl file couldn't be found.  Please ensure the path is correct.", L"Error", MB_OK );
			return -1;
		}

		hr = g_pd3dDevice->CreatePixelShader(
			pixelShader.GetDataPtr(),
			pixelShader.GetDataSize(),
			0, &g_pPixelShader );
		if( FAILED( hr ) )
		{
			MessageBox( NULL,
				L"Failed to create pixel shader from compiled file", L"Error", MB_OK );
			return hr;
		}
		pixelShader.Close();

		// Create vertex buffer
		SimpleVertex vertices[] =
		{
			{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
			{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
			{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) },
			{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
			{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
			{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
			{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
			{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) },
		};
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( SimpleVertex ) * 8;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = vertices;
		hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
		if( FAILED( hr ) )
			return hr;

		// Set vertex buffer
		UINT stride = sizeof( SimpleVertex );
		UINT offset = 0;
		g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

		// Create index buffer
		WORD indices[] =
		{
			3,1,0,
			2,1,3,

			0,5,4,
			1,5,0,

			3,4,7,
			0,4,3,

			1,6,5,
			2,6,1,

			2,7,6,
			3,7,2,

			6,4,5,
			7,4,6,
		};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( WORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = indices;
		hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
		if( FAILED( hr ) )
			return hr;

		// Set index buffer
		g_pImmediateContext->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

		// Set primitive topology
		g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		// Create the constant buffer
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pConstantBuffer );
		if( FAILED( hr ) )
			return hr;
		
		LogWrite(L"Renderer Initialized");
		return S_OK;
	}
	//--------------------------------------------------------------------------------
	void CleanupDevice()
	{
		if( g_pImmediateContext ) g_pImmediateContext->ClearState();

		if( g_pConstantBuffer ) g_pConstantBuffer->Release();
		if( g_pVertexBuffer ) g_pVertexBuffer->Release();
		if( g_pIndexBuffer ) g_pIndexBuffer->Release();
		if( g_pVertexLayout ) g_pVertexLayout->Release();
		if( g_pVertexShader ) g_pVertexShader->Release();
		if( g_pPixelShader ) g_pPixelShader->Release();
		if( g_pDepthStencil ) g_pDepthStencil->Release();
		if( g_pDepthStencilView ) g_pDepthStencilView->Release();
		if( g_pRenderTargetView ) g_pRenderTargetView->Release();
		if( g_pSwapChain ) g_pSwapChain->Release();
		if( g_pImmediateContext ) g_pImmediateContext->Release();
		if( g_pd3dDevice ) g_pd3dDevice->Release();
	}
	//--------------------------------------------------------------------------------
	void BeginRender()
	{
		// Clear the back buffer
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red, green, blue, alpha
		g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );

		// Clear the depth buffer to 1.0 (max depth)
		g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

		g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
		g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pConstantBuffer );
		g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
	}
	//--------------------------------------------------------------------------------
	void RenderCube(const Entity3D* pObj,const Camera* pCamera)
	{
		ConstantBuffer cb;
		cb.mWorld = XMLoadFloat4x4(&pObj->m_mWorld);
		cb.mView = XMLoadFloat4x4(&pCamera->GetView());
		cb.mProjection = XMLoadFloat4x4(&pCamera->GetProjection());
		g_pImmediateContext->UpdateSubresource( g_pConstantBuffer, 0, NULL, &cb, 0, 0 );

		g_pImmediateContext->DrawIndexed( 36, 0, 0 );
	}
	//--------------------------------------------------------------------------------
	void Present()
	{
		// Present our back buffer to our front buffer
		g_pSwapChain->Present( 0, 0 );
	}
	//--------------------------------------------------------------------------------
};
//--------------------------------------------------------------------------------