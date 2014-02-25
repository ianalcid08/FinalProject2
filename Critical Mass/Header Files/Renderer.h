/*
	File:		Renderer.h
	Author:		Bob Pasekoff
	Updated:	7/17/2013

*/

#ifndef _RENDERER_
#define _RENDERER_

#include "GLTriangleBatch.h"
#include "GLBatch.h"
#include "GLSetup.h"
#include "GLShaderManager.h"
#include "GLFrustum.h"
#include "GLMatrixStack.h"
#include "GLGeometryTransform.h"
#include <vector>

// for now.
class CRenderNode;
class CRenderState;
class CPostEffect;
class CUIElement;
struct CPointLight;

#define MAX_DRAW_OBJECTS 3000
//#define	MAX_UI_ELEMENTS 50

// These are for debug rendering all off-screen buffers
#define OFF_SCREEN_RENDER_BUFFER_FLAG 0x01
#define DIFFUSE_BUFFER_FLAG 0x02
#define NORMAL_BUFFER_FLAG 0x04

class CRenderer
{
	//std::list< CRenderNode* > m_renderList;
	//std::list< CRenderNode* > m_transparentRenderList;
	//std::vector< CUIElement* > m_UIList;

	std::vector<POINT> m_availableResolutions;

	unsigned int m_nObjectsInRenderList;
	CRenderNode* m_renderList[MAX_DRAW_OBJECTS];
	unsigned int m_nObjectsInTransparentRenderList;
	//CRenderNode* m_transparentRenderList[MAX_DRAW_OBJECTS];
	std::vector< CPointLight* > m_pointLightList;
	//unsigned int m_nObjectsInUIList;
	//CUIElement* m_UIList[MAX_UI_ELEMENTS];
	std::vector<CRenderNode*> m_transparentRenderVector;

	Matrix4x4f			m_modelView;
	Matrix4x4f			m_camera;
	Vec3f				m_cameraPos;

	float				m_accumulatedTime;

	// HAX
	// This is a temporary variable used to track which lighting
	// pass we're currently on.  This will be removed when
	// DoShading is implemented.
	GLuint				m_currentLight;

	/*
		Do not comment this back in!
		This may be THE GIVEN!
	*/
	//GLuint				m_hCurrentShader;
	//GLuint				m_hCurrentDiffuse;
	//GLuint				m_hCurrentSpec;
	//GLuint				m_hCurrentEmissive;

	//GLuint				m_hCurrentTextureUnit0;

	// Sky box stuff
	GLBatch				m_batchSkyBox;
	GLuint				m_textureCubeMap;

	GLuint				m_hShaderSkyBox;
	GLuint				m_hUniformMVPSkyBox;

	GLuint				m_hShaderCubeMap;
	GLuint				m_hUniformColorCubeMap;
	GLuint				m_hUniformMVPCubeMap;
	GLuint				m_hUniformMVCubeMap;
	GLuint				m_hUniformNormalCubeMap;
	GLuint				m_hUniformICCubeMap;

	// textureOnly shader stuff
	GLuint				hShader;
	GLuint				hTextureDiffuse;

	// Uniform handles - These will likely change
	GLuint				hUniformMVP;			// Matrix4x4f							
	GLuint				hUniformMV;				// Matrix4x4f
	GLuint				hTextureUnit;

	// Frame buffer to demonstrate our ability to render off-screen
	//GLuint				hFBO;
	//GLuint				hDepthBuffer;
	//GLuint				hColorBuffer;
	
	// Frame buffer object to hold all our deferred shenanigans
	//GLuint				hGBuffer;
	//GLuint				hGDepthBuffer;
	//GLuint				hGDiffuseBuffer;
	//GLuint				hGNormalBuffer;
	//GLuint				hGPositionBuffer;
	//GLenum				GBufferAttachments[3]; // 1 for each color buffer

	// Temporary handles to the stuff we need for the G-Buffer shader program
	//GLuint				hGBufferShader;
	//GLuint				hGBufferMMatrixUniform;
	//GLuint				hGBufferMVPMatrixUniform;
	//GLuint				hGBufferNormalMatrixUniform;
	//GLuint				hGBufferTextureUnit0;
	//GLuint				hGBufferColorUniform;

	// (Temporary?) handles to the stuff we need for the deferred point light shader program
	//GLuint				hDeferredPointLightShader;
	//GLuint				hUniformDeferredPointLightCameraMatrix;
	//GLuint				hUniformDeferredPointLightDiffuseTextureSampler;
	//GLuint				hUniformDeferredPointLightDepthTextureSampler;
	//GLuint				hUniformDeferredPointLightPositionTextureSampler;
	//GLuint				hUniformDeferredPointLightNormalTextureSampler;
	//GLuint				hUniformDeferredPointLightPositions;
	//GLuint				hUniformDeferredPointLightNumLights;

	// (Temporary?) screen-space quad for rendering to
	GLTriangleBatch		screenSpaceQuad;

	// This variable holds the flags for which buffers to debug render
	GLuint				debug_render_flags;

protected:
	GLsizei	 screenWidth;			// Desired window or desktop width
	GLsizei  screenHeight;			// Desired window or desktop height

	GLboolean bFullScreen;			// Request to run full screen

public:
	GLShaderManager		shaderManager;			// Shader Manager
	GLMatrixStack		modelViewMatrix;		// Modelview Matrix
	GLMatrixStack		projectionMatrix;		// Projection Matrix
	GLFrustum			viewFrustum;			// View Frustum
	GLGeometryTransform	transformPipeline;		// Geometry Transform Pipeline
	GLFrame				cameraFrame;			// Camera frame

	CRenderer( void );
	~CRenderer( void ) {};

	inline const GLuint GetScreenWidth( void )							{ return screenWidth; }
	inline const GLuint GetScreenHeight( void )							{ return screenHeight; }
	inline const GLboolean GetFullScreen( void )						{ return bFullScreen; }
	inline void SetFullScreen( bool _fullscreen )						{ bFullScreen = _fullscreen; }
	inline const std::vector<POINT>& GetAvailableResolutions( void )	{ return m_availableResolutions; }

	void ResizeWindow( GLsizei _width, GLsizei _height );
	
	void Initialize( void );
	void InitializeRenderBuffers( void );
	void Shutdown( void );
	void ReleaseRenderBuffers( void );
	void AddToRenderList( CRenderNode* _pRenderNode );
	void ClearRenderList( void );
	void DrawRenderList( void );
	void ChangeRenderState( CRenderNode* _pRenderNode, const Matrix4x4f& _mCamera );
	void AddToPointLightList( CPointLight* _light );
	void ClearLightList( void );
	void DoShading( void );
	void DoPostEffect( CPostEffect* _pPostEffect );
	//void AddToUIList( CUIElement* _pUIElement );
	//void ClearUIList( void );
	void EnableDraw(void);
	void DrawTexture( CUIElement* _pUIElement );
	void DisableDraw(void);
	void Update( Vec3f _cameraPos, Matrix3x3f _cameraOrientation, float _elapsedTime );

	// For debugging
	void ChangeClearColor( float _red, float _green, float _blue, float _alpha )
	{
		glClearColor( _red, _green, _blue, _alpha );
	}

	void ChangeClearColor( Vec4f _color )
	{
		glClearColor( _color[0], _color[1], _color[2], _color[3] );
	}

	GLuint LoadTGAsAsCubeTexture( const char *_fileName[6] );

	// HAX
	// Swap the uniform handles when we swap lighting shaders
	void SwapUniforms( CRenderNode* _pRenderNode, GLuint _shaderHandle );

	// Switch to debug rendering the off-screen render buffer
	inline void ToggleOffScreenRenderBufferDisplay( void ){ debug_render_flags ^= OFF_SCREEN_RENDER_BUFFER_FLAG; }
	inline void ToggleDiffuseBufferDisplay( void ){ debug_render_flags ^= DIFFUSE_BUFFER_FLAG; }
	inline void ToggleNormalBufferDisplay( void ){ debug_render_flags ^= NORMAL_BUFFER_FLAG; }

	bool _CheckFBOStatus( GLenum _readOrWrite );

#ifdef _DEBUG
	// IN _readOrWrite	- values are GL_DRAW_FRAMEBUFFER or GL_READ_FRAMEBUFFER
	// Returns true if the FBO is good to go
#define CheckFBOStatus _CheckFBOStatus
#else
#define CheckFBOStatus __noop
#endif
};


#endif // _RENDERER_
