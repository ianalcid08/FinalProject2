/*
File:		Renderer.cpp
Author:		Bob Pasekoff
Updated:	7/17/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/glsMath.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/Light.h"
#include "../Header Files/UIElement.h"

#include <algorithm>

#ifdef _DEBUG
CRenderer::CRenderer( void ) : screenWidth(1024), screenHeight(768), bFullScreen(false)
{
#elif defined(_BETA)
CRenderer::CRenderer(void) : screenWidth(1024), screenHeight(768), bFullScreen(true)
{
#else
CRenderer::CRenderer(void) : bFullScreen(true)
{
	// Get the default display device.
	DISPLAY_DEVICE dd;
	dd.cb = sizeof( DISPLAY_DEVICE );
	EnumDisplayDevices( NULL, 0, _Out_  &dd, 0 );

	// Get the current display resolution
	DEVMODE devmode;
	EnumDisplaySettings( dd.DeviceName, ENUM_CURRENT_SETTINGS, &devmode );

	screenWidth = devmode.dmPelsWidth;
	screenHeight = devmode.dmPelsHeight;
#endif
}

void CRenderer::Initialize( void )
{
	// Get the default display device.
	DISPLAY_DEVICE dd;
	dd.cb = sizeof( DISPLAY_DEVICE );
	EnumDisplayDevices( NULL, 0, _Out_  &dd, 0 );

	BOOL keepGoing = TRUE;

	for( DWORD i = 0; keepGoing; ++i )
	{
		DEVMODE tempDevMode;			// device mode
		POINT tempSize;					// screen dimensions
		bool alreadyGotThisOne = false;	// Did another device mode already cover this resolution?

		keepGoing = EnumDisplaySettings( dd.DeviceName, i, &tempDevMode );
		tempSize.x = tempDevMode.dmPelsWidth;
		tempSize.y = tempDevMode.dmPelsHeight;
		
		for( unsigned int j = 0, n = m_availableResolutions.size(); j < n; ++j )
		{
			if( tempSize.x == m_availableResolutions[j].x && tempSize.y == m_availableResolutions[j].y )
			{
				alreadyGotThisOne = true;
				break;
			}
		}

		if( !alreadyGotThisOne )
			m_availableResolutions.push_back( tempSize );
	}

	m_transparentRenderVector.reserve(MAX_DRAW_OBJECTS);
	// Make sure OpenGL entry points are set
	glewInit();

	// Initialze Shader Manager
	shaderManager.InitializeStockShaders();

	glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
	glsCheckGLErrors();

	//renderList.resize( someNumber );
	// We'll want to pre-size the renderList with the maximum number of nodes that
	// we plan on rendering if we can ever figure out what that number is.

	glEnable( GL_DEPTH_TEST );
	glsCheckGLErrors();
	glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
	glsCheckGLErrors();
	glPolygonMode( GL_FRONT, GL_FILL );
	glsCheckGLErrors();
	glEnableVertexAttribArray( GLS_ATTRIBUTE_VERTEX );
	glsCheckGLErrors();
	glEnableVertexAttribArray( GLS_ATTRIBUTE_COLOR );
	glsCheckGLErrors();
	glEnableVertexAttribArray( GLS_ATTRIBUTE_NORMAL );
	glsCheckGLErrors();
	glEnableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE0 );
	glsCheckGLErrors();
	glEnableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE1 );
	glsCheckGLErrors();
	glEnableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE2 );
	glsCheckGLErrors();

	//glEnable( GL_CULL_FACE );
	//glCullFace( GL_BACK );

	// reset the timer
	m_accumulatedTime = 0.0f;

	// Sky box stuff
	const char * szSkyFiles[6] = {
		"Assets/Textures/sky_pos_x.tga", "Assets/Textures/sky_neg_x.tga",
		"Assets/Textures/sky_pos_y.tga", "Assets/Textures/sky_neg_y.tga",
		"Assets/Textures/sky_pos_z.tga", "Assets/Textures/sky_neg_z.tga"
	};

	m_textureCubeMap = LoadTGAsAsCubeTexture( szSkyFiles );

	m_hShaderSkyBox = glsLoadShaderPairWithAttributes(
		"Assets/Shaders/skybox.vs", "Assets/Shaders/skybox.ps", 1,
		GLS_ATTRIBUTE_VERTEX, "vVertex" );

	m_hUniformMVPSkyBox = glGetUniformLocation( m_hShaderSkyBox, "mvpMatrix" );
	glsCheckGLErrors();

	glsCreateCube( m_batchSkyBox, 500.0f );

	m_nObjectsInRenderList = 0;
	m_nObjectsInTransparentRenderList = 0;
	//m_nObjectsInUIList = 0;

	/*
	Do not comment this back in!
	This may be THE GIVEN!
	*/
	//m_hCurrentShader = 0;
	//m_hCurrentDiffuse = 0;
	//m_hCurrentEmissive = 0;
	//m_hCurrentSpec = 0;

	//m_hCurrentTextureUnit0 = 0;



	//// Temporary setup for the G-Buffer shader program
	//hGBufferShader = glsLoadShaderPairWithAttributes(
	//	"Assets/Shaders/gBuffer.vs", "Assets/Shaders/gBuffer.ps", 3,
	//	GLS_ATTRIBUTE_VERTEX, "vVertex",
	//	GLS_ATTRIBUTE_NORMAL, "vNormal",
	//	GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	//glsCheckGLErrors();

	//hGBufferMMatrixUniform = glGetUniformLocation( hGBufferShader, "mMatrix" );
	//glsCheckGLErrors();
	//hGBufferMVPMatrixUniform = glGetUniformLocation( hGBufferShader, "mvpMatrix" );
	//glsCheckGLErrors();
	//hGBufferNormalMatrixUniform = glGetUniformLocation( hGBufferShader, "normalMatrix" );
	//glsCheckGLErrors();
	//hGBufferTextureUnit0 = glGetUniformLocation( hGBufferShader, "textureUnit0" );
	//glsCheckGLErrors();
	//hGBufferColorUniform = glGetUniformLocation( hGBufferShader, "vColor" );
	//glsCheckGLErrors();

	//// (Temporary?) setup for the deferred point light shader program
	//hDeferredPointLightShader = glsLoadShaderPairWithAttributes(
	//	"Assets/Shaders/screenSpaceQuad.vs", "Assets/Shaders/deferredPointLight.ps", 2,
	//	GLS_ATTRIBUTE_VERTEX, "vVertex",
	//	GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );
	//glsCheckGLErrors();

	//hUniformDeferredPointLightDiffuseTextureSampler = glGetUniformLocation( hDeferredPointLightShader, "diffuseTexture" );
	//glsCheckGLErrors();
	//hUniformDeferredPointLightDepthTextureSampler = glGetUniformLocation( hDeferredPointLightShader, "depthTexture" );
	//glsCheckGLErrors();
	//hUniformDeferredPointLightNormalTextureSampler = glGetUniformLocation( hDeferredPointLightShader, "normalTexture" );
	//glsCheckGLErrors();
	//hUniformDeferredPointLightPositionTextureSampler = glGetUniformLocation( hDeferredPointLightShader, "positionTexture" );
	//glsCheckGLErrors();
	//hUniformDeferredPointLightCameraMatrix = glGetUniformLocation( hDeferredPointLightShader, "cameraMatrix" );
	//glsCheckGLErrors();
	//hUniformDeferredPointLightPositions = glGetUniformLocation( hDeferredPointLightShader, "lightPositions" );
	//glsCheckGLErrors();
	//hUniformDeferredPointLightNumLights = glGetUniformLocation( hDeferredPointLightShader, "numLights" );



	InitializeRenderBuffers();
}

void CRenderer::InitializeRenderBuffers( void )
{
	// By default, do not debug draw any render buffers
	debug_render_flags = 0;

	// Set up an FBO for demonstrating off-screen rendering
	//glGenFramebuffers( 1, &hFBO );
	//glsCheckGLErrors();
	//glBindFramebuffer( GL_FRAMEBUFFER, hFBO );
	//glsCheckGLErrors();

	// Depth Buffer, off-screen rendering
	//glGenRenderbuffers( 1, &hDepthBuffer );
	//glsCheckGLErrors();
	//glBindRenderbuffer( GL_RENDERBUFFER, hDepthBuffer );
	//glsCheckGLErrors();
	//glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, screenWidth, screenHeight );
	//glsCheckGLErrors();

	// Create the color buffer, off-screen rendering
	//glGenTextures( 1, &hColorBuffer );
	//glsCheckGLErrors();
	//glBindTexture( GL_TEXTURE_2D, hColorBuffer );
	//glsCheckGLErrors();
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL );
	//glsCheckGLErrors();

	// Attach the color and depth buffers to the FBO, off-screen rendering
	//glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hColorBuffer, 0 );
	//glsCheckGLErrors();
	//glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hDepthBuffer );
	//glsCheckGLErrors();

	// Check the FBO's status
	//bool result = CheckFBOStatus( GL_DRAW_FRAMEBUFFER );
	//glsCheckGLErrors();



	//// Set up our FBO for deferred shenanigans
	//glGenFramebuffers( 1, &hGBuffer );
	//glsCheckGLErrors();
	//glBindFramebuffer( GL_FRAMEBUFFER, hGBuffer );
	//glsCheckGLErrors();

	//// Depth Buffer, G-Buffer
	//glGenTextures( 1, &hGDepthBuffer );
	//glsCheckGLErrors();
	//glBindTexture( GL_TEXTURE_2D, hGDepthBuffer );
	//glsCheckGLErrors();
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
	//glsCheckGLErrors();

	//// Diffuse buffer, G-Buffer
	//glGenTextures( 1, &hGDiffuseBuffer );
	//glsCheckGLErrors();
	//glBindTexture( GL_TEXTURE_2D, hGDiffuseBuffer );
	//glsCheckGLErrors();
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL );
	//glsCheckGLErrors();

	//// Normal buffer, G-Buffer
	//glGenTextures( 1, &hGNormalBuffer );
	//glsCheckGLErrors();
	//glBindTexture( GL_TEXTURE_2D, hGNormalBuffer );
	//glsCheckGLErrors();
	////glTexImage2D( GL_TEXTURE_2D, 0, GL_RG16, screenWidth, screenHeight, 0, GL_RG, GL_FLOAT, NULL );
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL );
	//glsCheckGLErrors();

	//// Position buffer, G-Buffer
	//glGenTextures( 1, &hGPositionBuffer );
	//glsCheckGLErrors();
	//glBindTexture( GL_TEXTURE_2D, hGPositionBuffer );
	//glsCheckGLErrors();
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL );
	//glsCheckGLErrors();

	//// Attach individual buffers to the G-Buffer
	//glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGDiffuseBuffer, 0 );
	//glsCheckGLErrors();
	//glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, hGNormalBuffer, 0 );
	//glsCheckGLErrors();
	//glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, hGPositionBuffer, 0 );
	//glsCheckGLErrors();
	//glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, hGDepthBuffer, 0 );
	//glsCheckGLErrors();

	//// Set the G-Buffer attachments
	//GBufferAttachments[0] = GL_COLOR_ATTACHMENT0;
	//GBufferAttachments[1] = GL_COLOR_ATTACHMENT1;
	//GBufferAttachments[2] = GL_COLOR_ATTACHMENT2;

	//// Check the FBO's status
	//result = CheckFBOStatus( GL_DRAW_FRAMEBUFFER );
	//glsCheckGLErrors();

	// Set up the (temporary?) screen-space quad for rendering to
	Vec3f pos[3];
	Vec3f norm[3];
	Vec2f UV[3];

	screenSpaceQuad.BeginMesh( 4, 6 );

	glsLoadVector3( pos[0], -1.0f, 1.0f, 0.0f );
	glsLoadVector3( norm[0], 0.0f, 0.0f, 1.0f );
	glsLoadVector2( UV[0], 0.0f, 1.0f );

	glsLoadVector3( pos[1], -1.0f, -1.0f, 0.0f );
	glsLoadVector3( norm[1], 0.0f, 0.0f, 1.0f );
	glsLoadVector2( UV[1], 0.0f, 0.0f );

	glsLoadVector3( pos[2], 1.0f, 1.0f, 0.0f );
	glsLoadVector3( norm[2], 0.0f, 0.0f, 1.0f );
	glsLoadVector2( UV[2], 1.0f, 1.0f );

	screenSpaceQuad.AddTriangle( pos, norm, UV );

	glsLoadVector3( pos[2], -1.0f, -1.0f, 0.0f );
	glsLoadVector3( norm[2], 0.0f, 0.0f, 1.0f );
	glsLoadVector2( UV[2], 0.0f, 0.0f );

	glsLoadVector3( pos[1], 1.0f, 1.0f, 0.0f );
	glsLoadVector3( norm[1], 0.0f, 0.0f, 1.0f );
	glsLoadVector2( UV[1], 1.0f, 1.0f );

	glsLoadVector3( pos[0], 1.0f, -1.0f, 0.0f );
	glsLoadVector3( norm[0], 0.0f, 0.0f, 1.0f );
	glsLoadVector2( UV[0], 1.0f, 0.0f );

	screenSpaceQuad.AddTriangle( pos, norm, UV );

	screenSpaceQuad.End();
}

void CRenderer::ReleaseRenderBuffers( void )
{
	// Deactivate all shaders
	glUseProgram( 0 );

	// Deactivate all textures
	glBindTexture( GL_TEXTURE_2D, 0 );

	// Delete the color buffer
	//glDeleteTextures( 1, &hColorBuffer );
	//glsCheckGLErrors();
	//glDeleteTextures( 1, &hGDiffuseBuffer );
	//glsCheckGLErrors();
	//glDeleteTextures( 1, &hGNormalBuffer );
	//glsCheckGLErrors();
	//glDeleteTextures( 1, &hGPositionBuffer );
	//glsCheckGLErrors();

	// Delete the depth buffers
	//glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	//glsCheckGLErrors();
	//glDeleteRenderbuffers( 1, &hDepthBuffer );
	//glsCheckGLErrors();
	//glDeleteTextures( 1, &hGDepthBuffer); // G-Buffer depth is a texture, not a RBO
	//glsCheckGLErrors();

	// Delete the FBO and the G-Buffer
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	//glsCheckGLErrors();
	//glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
	//glsCheckGLErrors();
	//glDeleteFramebuffers( 1, &hFBO );
	//glsCheckGLErrors();
	//glDeleteFramebuffers( 1, &hGBuffer );
	//glsCheckGLErrors();

	// Delete the screen-space quad
	screenSpaceQuad.ReleaseVBOs();
}

void CRenderer::Shutdown( void )
{
	// Don't need to bind a texture(?)
	//glActiveTexture( GL_TEXTURE0 );
	//glsCheckGLErrors();
	//glBindTexture( GL_TEXTURE_2D, 0 );
	//glsCheckGLErrors();

	// Is the error BEFORE shutdown?
	glsCheckGLErrors();

	// Clean up the sky box texture
	glDeleteTextures( 1, &m_textureCubeMap );

	// Clean up the sky box shader
	glUseProgram( 0 );
	glDeleteProgram( m_hShaderSkyBox );

	// Clean up the sky box VBOs
	m_batchSkyBox.ReleaseVBOs();

	// Delete the G-buffer shaders
	//glDeleteProgram( hGBufferShader );
	//glDeleteProgram( hDeferredPointLightShader );

	// Clean up the stock shaders
	shaderManager.ReleaseStockShaders();

	ReleaseRenderBuffers();
}

void CRenderer::ResizeWindow( GLsizei _width, GLsizei _height )
{
	screenWidth = _width;
	screenHeight = _height;
	glViewport(0, 0, _width, _height);
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);

	viewFrustum.SetPerspective( 60.0f, float(_width)/float(_height), 0.1f, 2000.0f );
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelViewMatrix.LoadIdentity();

	ReleaseRenderBuffers();
	InitializeRenderBuffers();
}

void CRenderer::AddToRenderList( CRenderNode* _pRenderNode )
{
	// NB1: Eventually, we are going to need to add spatial sorting.

	// NB2: This really doesn't fulfill the purpose for which it was intended.
	// We probably need to implement some sort of render set system.

	Vec3f toCamera, toObject, world;
	float distFromWorldCenterSquared, CDotO;
	glsLoadVector3( world, 0.0f, 75.0f, 0.0f );
	glsSubtractVectors3( toObject, _pRenderNode->GetPosition(), world );
	glsSubtractVectors3( toCamera, m_cameraPos, world );
	distFromWorldCenterSquared = glsGetVectorLengthSquared3( toObject );
	glsNormalizeVector3( toObject );
	glsNormalizeVector3( toCamera );
	CDotO = glsDotProduct3( toCamera, toObject );

	// Don't add the object to the render list if it is directly opposit the moon
	// from the camera.
	if( CDotO < -0.95f )
		return;

	// Don't add the object to the render list if it is close enough to the moon's
	// surface that it is probably obscured even if it is not directly opposite the
	// moon from the camera.
	if( CDotO < -0.6f && distFromWorldCenterSquared < 5929.0f ) // 77^2
		return;

	// Perform the same exclusion but more aggresively for opaque objects, since
	// none of them are as tall as the beacon.
	if( CDotO < -0.2f && distFromWorldCenterSquared < 5929.0f && !_pRenderNode->GetEnableBlending() ) // 77^2
		return;

	if( _pRenderNode->GetEnableBlending() == false )
	{
		// This really doesn't fulfill the purpose for which it was intended.
		// We probably need to implement some sort of render set system.
		if( m_nObjectsInRenderList >= MAX_DRAW_OBJECTS )
		{
			cout << "Too many objects in the render list.  Increase MAX_DRAW_OBJECTS.\n";
			return;
		}
		m_renderList[m_nObjectsInRenderList] = _pRenderNode;
		++m_nObjectsInRenderList;
	}
	else
	{
		// This is currently completely unsorted.  Obviously, that needs to be fixed.
		if( m_nObjectsInTransparentRenderList >= MAX_DRAW_OBJECTS )
		{
			cout << "Too many objects in the transparent render list.  Increase MAX_DRAW_OBJECTS.\n";
			return;
		}
		//m_transparentRenderList[m_nObjectsInTransparentRenderList] = _pRenderNode;
		
		Vec3f temp;
		glsSubtractVectors3(temp,_pRenderNode->GetPosition(), m_cameraPos);
		float dist = glsGetMagnitudeSquared3(temp);
		_pRenderNode->SetCameraDist(dist);

		//int midPoint = m_transparentRenderVector.size()*0.5f;
		//
		//while(true)
		//{
		//	if(_pRenderNode->GetCameraDist() > m_transparentRenderVector[midPoint]->GetCameraDist())
		//	{
		//		
		//	}

		//}
		//if(m_transparentRenderVector.size() > 1)
		//{
		//	for(std::vector<CRenderNode*>::iterator i; i < m_transparentRenderVector.end(); i++)
		//	{
		//		if((*i)->GetCameraDist() < _pRenderNode->GetCameraDist())
		//		{
		//			continue;
		//		}
		//		else
		//		{
		//			m_transparentRenderVector.emplace(i,_pRenderNode);
		//		}
		//	}
		//}
		//else
		m_transparentRenderVector.push_back(_pRenderNode);
		//m_transparentRenderVector.emplace(

		++m_nObjectsInTransparentRenderList;
	}
}

void CRenderer::ClearRenderList( void )
{
	m_transparentRenderVector.clear();
	m_nObjectsInRenderList = 0;
	m_nObjectsInTransparentRenderList = 0;
}

void CRenderer::DrawRenderList( void )
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	//////////////////////////////////////////////////////////////////////////////////
	//// HAX
	//// Test-render to the diffuse G-Buffer
	//////////////////////////////////////////////////////////////////////////////////

	//// Switch rendering to the G-Buffer
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, hGBuffer );
	//glsCheckGLErrors();
	//glDrawBuffers( 3, GBufferAttachments ); // must change the size as we add buffer attachments. 
	//glsCheckGLErrors();
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glsCheckGLErrors();
	//glDisable( GL_BLEND );
	//glsCheckGLErrors();

	//for( unsigned int i = 0; i < m_nObjectsInRenderList; ++i )
	//{
	//	// If this object has no geometry, don't draw it.
	//	if( !m_renderList[i]->GetTriangleBatch() )
	//		continue;

	//	modelViewMatrix.PushMatrix();

	//	cameraFrame.GetCameraMatrix( m_camera );
	//	modelViewMatrix.MultMatrix( m_camera );

	//	modelViewMatrix.Translate( -m_renderList[i]->GetPosition()[0], -m_renderList[i]->GetPosition()[1], -m_renderList[i]->GetPosition()[2] );

	//	Matrix3x3f cameraRotation;
	//	glsExtractRotationMatrix3x3( cameraRotation, m_camera );

	//	Vec3f right, up, forward;
	//	glsRotateVector( right, m_renderList[i]->GetRightVector(), cameraRotation );
	//	glsRotateVector( up, m_renderList[i]->GetUpVector(), cameraRotation );
	//	glsRotateVector( forward, m_renderList[i]->GetForwardVector(), cameraRotation );

	//	Matrix4x4f mModelView;
	//	modelViewMatrix.GetMatrix( mModelView );

	//	mModelView[0] = right[0];
	//	mModelView[1] = right[1];
	//	mModelView[2] = right[2];

	//	mModelView[4] = up[0];
	//	mModelView[5] = up[1];
	//	mModelView[6] = up[2];

	//	mModelView[8] = forward[0];
	//	mModelView[9] = forward[1];
	//	mModelView[10] = forward[2];

	//	modelViewMatrix.LoadMatrix( mModelView );

	//	Matrix4x4f MVP, MV, M, V, cam;
	//	Matrix3x3f NORM;

	//	glsLoadIdentity4x4( M );
	//	glsCopyVector3( M, m_renderList[i]->GetRightVector() );
	//	glsCopyVector3( &M[4], m_renderList[i]->GetUpVector() );
	//	glsCopyVector3( &M[8], m_renderList[i]->GetForwardVector() );
	//	glsCopyVector3( &M[12], m_renderList[i]->GetPosition() );

	//	glsCopyMatrix4x4( cam, m_camera );
	//	glsScaleVector3( &cam[8], -1.0f );
	//	glsScaleVector3( &cam[12], -1.0f );
	//	glsCrossProduct3( cam, &cam[8], &cam[4] );
	//	glsNormalizeVector3( cam );

	//	glsInvertMatrix4x4( V, cam );

	//	glsMatrixMultiply4x4( MV, V, M );
	//	glsMatrixMultiply4x4( MVP, transformPipeline.GetProjectionMatrix(), MV );

	//	memcpy( &MVP, &transformPipeline.GetModelViewProjectionMatrix(), sizeof( Matrix4x4f ) );
	//	//memcpy( &MV, &transformPipeline.GetModelViewMatrix(), sizeof( Matrix4x4f ) );
	//	memcpy( &NORM, &transformPipeline.GetNormalMatrix(), sizeof( Matrix3x3f ) );

	//	glDepthFunc( GL_LESS );
	//	glsCheckGLErrors();

	//	// Use the G-Buffer Shader
	//	glUseProgram( hGBufferShader );
	//	glsCheckGLErrors();

	//	// Diffuse Texture
	//	glActiveTexture( GL_TEXTURE0 );
	//	glsCheckGLErrors();
	//	glBindTexture( GL_TEXTURE_2D, m_renderList[i]->GetDiffuseTextureHandle() );
	//	glsCheckGLErrors();
	//	glUniform1i( hGBufferTextureUnit0, 0 );
	//	glsCheckGLErrors();

	//	glUniformMatrix4fv( hGBufferMMatrixUniform, 1, GL_FALSE, M );
	//	glsCheckGLErrors();

	//	// Model View Projection Matrix
	//	glUniformMatrix4fv( hGBufferMVPMatrixUniform, 1, GL_FALSE, MVP );
	//	glsCheckGLErrors();

	//	// Normal Matrix
	//	glUniformMatrix3fv( hGBufferNormalMatrixUniform, 1, GL_FALSE, NORM );
	//	glsCheckGLErrors();

	//	// Uniform Color
	//	glUniform4fv( hGBufferColorUniform, 1, m_renderList[i]->GetColor() );

	//	// Check for OpenGL Errors
	//	if( glsCheckGLErrors() )
	//	{
	//		int IAmABreakPoint = 0;
	//	}

	//	// Actually draw the current object
	//	m_renderList[i]->GetTriangleBatch()->Draw();

	//	// Reset the model view matrix for the next object
	//	modelViewMatrix.PopMatrix();
	//}

	//// Switch the frame buffers back
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, hFBO );
	//glsCheckGLErrors();

	//////////////////////////////////////////////////////////////////////////////////


	// May want to move out of DrawRenderList, because it clears the entire backbuffer.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glsCheckGLErrors();

	//////////////////////////////////////////////////
	// Draw all of the opaque stuff first.

#if 1
	for( unsigned int i = 0; i < m_nObjectsInRenderList; ++i )
	{
		// If this object has no geometry, don't draw it.
		if( !m_renderList[i]->GetTriangleBatch() )
			continue;

		modelViewMatrix.PushMatrix();

		cameraFrame.GetCameraMatrix( m_camera );
		modelViewMatrix.MultMatrix( m_camera );

		modelViewMatrix.Translate( -m_renderList[i]->GetPosition()[0], -m_renderList[i]->GetPosition()[1], -m_renderList[i]->GetPosition()[2] );

		Matrix3x3f cameraRotation;
		glsExtractRotationMatrix3x3( cameraRotation, m_camera );

		Vec3f right, up, forward;
		glsRotateVector( right, m_renderList[i]->GetRightVector(), cameraRotation );
		glsRotateVector( up, m_renderList[i]->GetUpVector(), cameraRotation );
		glsRotateVector( forward, m_renderList[i]->GetForwardVector(), cameraRotation );

		Matrix4x4f mModelView;
		modelViewMatrix.GetMatrix( mModelView );

		mModelView[0] = right[0];
		mModelView[1] = right[1];
		mModelView[2] = right[2];

		mModelView[4] = up[0];
		mModelView[5] = up[1];
		mModelView[6] = up[2];

		mModelView[8] = forward[0];
		mModelView[9] = forward[1];
		mModelView[10] = forward[2];

		modelViewMatrix.LoadMatrix( mModelView );

		// HAX - This functionality will eventually be moved in to DoShading.
		if( m_renderList[i]->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] )
		{
			SwapUniforms( m_renderList[i], CAssetManager::GetInstance()->m_vShaderHandles[TEX_DIR_LIGHT] );
			ChangeRenderState( m_renderList[i], m_camera );
			m_renderList[i]->GetTriangleBatch()->Draw();
			SwapUniforms( m_renderList[i], CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] );
			ChangeRenderState( m_renderList[i], m_camera );
			GLuint hUniformLightPos = CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT];

			for( unsigned int m = 0, n = m_pointLightList.size(); m < n; ++m )
			{
				m_currentLight = m;
				glUniform3fv( hUniformLightPos, 1, m_pointLightList[m_currentLight]->m_Position );
				m_renderList[i]->GetTriangleBatch()->Draw();
			}
		}
		else if( m_renderList[i]->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] )
		{
			SwapUniforms( m_renderList[i], CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_DIR] );
			ChangeRenderState( m_renderList[i], m_camera );
			m_renderList[i]->GetTriangleBatch()->Draw();
			SwapUniforms( m_renderList[i], CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_POINT] );
			ChangeRenderState( m_renderList[i], m_camera );
			GLuint hUniformLightPos = CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][ANIMATED_POINT];

			for( unsigned int m = 0, n = m_pointLightList.size(); m < n; ++m )
			{
				m_currentLight = m;
				glUniform3fv( hUniformLightPos, 1, m_pointLightList[m_currentLight]->m_Position );
				m_renderList[i]->GetTriangleBatch()->Draw();
			}

			SwapUniforms( m_renderList[i], CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
		}
		else
		{
			ChangeRenderState( m_renderList[i], m_camera );	
			m_renderList[i]->GetTriangleBatch()->Draw();
		}

		modelViewMatrix.PopMatrix();
	}
#endif

	////////////////////////////////////////////////////////////////////////////////
	// Draw the G-Buffer to a screen-space quad
	////////////////////////////////////////////////////////////////////////////////
#if 0
	glsCheckGLErrors();
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, hFBO );
	//glsCheckGLErrors();
	//glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
	//glsCheckGLErrors();
	//glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
	//glClear( GL_COLOR_BUFFER_BIT );
	//glClearColor(0.3f, 0.3f, 0.5f, 1.0f);

	//glDepthFunc( GL_ALWAYS );

	// Use the deferred point light shader
	glUseProgram( hDeferredPointLightShader );
	glsCheckGLErrors();

	// Diffuse Texture
	glActiveTexture( GL_TEXTURE0 );
	glsCheckGLErrors();
	glBindTexture( GL_TEXTURE_2D, hGDiffuseBuffer );
	glsCheckGLErrors();
	glUniform1i( hUniformDeferredPointLightDiffuseTextureSampler, 0 );
	glsCheckGLErrors();

	// Normal Texture
	glActiveTexture( GL_TEXTURE1 );
	glsCheckGLErrors();
	glBindTexture( GL_TEXTURE_2D, hGNormalBuffer );
	glsCheckGLErrors();
	glUniform1i( hUniformDeferredPointLightNormalTextureSampler, 1 );
	glsCheckGLErrors();

	// Depth Texture
	glActiveTexture( GL_TEXTURE2 );
	glsCheckGLErrors();
	glBindTexture( GL_TEXTURE_2D, hGDepthBuffer );
	glsCheckGLErrors();
	glUniform1i( hUniformDeferredPointLightDepthTextureSampler, 2 );
	glsCheckGLErrors();

	// Position Texture
	glActiveTexture( GL_TEXTURE3 );
	glsCheckGLErrors();
	glBindTexture( GL_TEXTURE_2D, hGPositionBuffer );
	glsCheckGLErrors();
	glUniform1i( hUniformDeferredPointLightPositionTextureSampler, 3 );
	glsCheckGLErrors();

	// VP Matrix
	Matrix4x4f VP;
	glsMatrixMultiply4x4( VP, m_camera, transformPipeline.GetProjectionMatrix() );
	glUniformMatrix4fv( hUniformDeferredPointLightCameraMatrix, 1, GL_FALSE, m_camera );
	glsCheckGLErrors();

	// Light Positions
	Vec3f lightPositions[100];
	int numLights = m_pointLightList.size();
	if( numLights > 100 )
		numLights = 100;

	for( int i = 0; i < numLights; ++i )
		glsCopyVector3( lightPositions[i], m_pointLightList[i]->m_Position );

	glUniform3fv( hUniformDeferredPointLightPositions, numLights, *lightPositions );
	glUniform1i( hUniformDeferredPointLightNumLights, numLights );
	glsCheckGLErrors();

	// Actually draw the current object
	screenSpaceQuad.Draw();
#endif


	////////////////////////////////////////////////////////////////////////////////
	// Draw the sky box
	////////////////////////////////////////////////////////////////////////////////

	modelViewMatrix.PushMatrix();

	cameraFrame.GetCameraMatrix( m_camera );
	modelViewMatrix.MultMatrix( m_camera );

	// Draw the sky box
	glUseProgram( m_hShaderSkyBox );
	glsCheckGLErrors();
	glUniformMatrix4fv( m_hUniformMVPSkyBox, 1, GL_FALSE,
		transformPipeline.GetModelViewProjectionMatrix() );
	glsCheckGLErrors();

	glCullFace( GL_FRONT );
	glsCheckGLErrors();

	glBindTexture( GL_TEXTURE_CUBE_MAP, m_textureCubeMap );
	glsCheckGLErrors();

	glDisable( GL_BLEND );
	glsCheckGLErrors();

	glsCheckGLErrors();
	glDepthFunc( GL_LESS );
	glsCheckGLErrors();

	m_batchSkyBox.Draw();	

	glCullFace( GL_BACK );
	glsCheckGLErrors();

	modelViewMatrix.PopMatrix();
	////////////////////////////////////////////////////////////////////////////////



	// Draw all of the transparent stuff.  This will need to be updated to 
	// correctly display transparent objects.
	glDepthMask( GL_FALSE );
	for( unsigned int i = 0; i < m_nObjectsInTransparentRenderList; ++i )
	{
		// If this object has no geometry, don't draw it.
		if( !m_transparentRenderVector[i]->GetTriangleBatch() )
			continue;

		modelViewMatrix.PushMatrix();

		cameraFrame.GetCameraMatrix( m_camera );
		modelViewMatrix.MultMatrix( m_camera );

		modelViewMatrix.Translate( -m_transparentRenderVector[i]->GetPosition()[0], -m_transparentRenderVector[i]->GetPosition()[1], -m_transparentRenderVector[i]->GetPosition()[2] );

		Matrix3x3f cameraRotation;
		glsExtractRotationMatrix3x3( cameraRotation, m_camera );

		Vec3f right, up, forward;
		glsRotateVector( right, m_transparentRenderVector[i]->GetRightVector(), cameraRotation );
		glsRotateVector( up, m_transparentRenderVector[i]->GetUpVector(), cameraRotation );
		glsRotateVector( forward, m_transparentRenderVector[i]->GetForwardVector(), cameraRotation );

		Matrix4x4f mModelView;
		modelViewMatrix.GetMatrix( mModelView );

		mModelView[0] = right[0];
		mModelView[1] = right[1];
		mModelView[2] = right[2];

		mModelView[4] = up[0];
		mModelView[5] = up[1];
		mModelView[6] = up[2];

		mModelView[8] = forward[0];
		mModelView[9] = forward[1];
		mModelView[10] = forward[2];

		modelViewMatrix.LoadMatrix( mModelView );

		//std::sort(m_transparentRenderVector.begin(),m_transparentRenderVector.end(),[](CRenderNode* lhs,CRenderNode* rhs){return lhs->GetCameraDist() < rhs->GetCameraDist();});

		// HAX - This functionality needs to be removed.  There should be no illuminated transparent objects.
		if( m_transparentRenderVector[i]->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] )
		{
			SwapUniforms( m_transparentRenderVector[i], CAssetManager::GetInstance()->m_vShaderHandles[TEX_DIR_LIGHT] );
			ChangeRenderState( m_transparentRenderVector[i], m_camera );
			m_transparentRenderVector[i]->GetTriangleBatch()->Draw();
			SwapUniforms( m_transparentRenderVector[i], CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] );
			ChangeRenderState( m_transparentRenderVector[i], m_camera );
			GLuint hUniformLightPos = CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT];

			for( unsigned int m = 0, n = m_pointLightList.size(); m < n; ++m )
			{
				m_currentLight = m;
				glUniform3fv( hUniformLightPos, 1, m_pointLightList[m_currentLight]->m_Position );
				m_transparentRenderVector[i]->GetTriangleBatch()->Draw();
			}
		}
		else
		{
			ChangeRenderState( m_transparentRenderVector[i], m_camera );

			// HAX
			//glDepthFunc( GL_LEQUAL );

			m_transparentRenderVector[i]->GetTriangleBatch()->Draw();
		}

		modelViewMatrix.PopMatrix();
	}
	glDepthMask( GL_TRUE );
	////////////////////////////////////////////////////

	ClearRenderList();
	ClearLightList();

	// Check for OpenGL Errors
	if( glsCheckGLErrors() )
	{
		int IAmABreakPoint = 0;
	}
}

void CRenderer::ChangeRenderState( CRenderNode* _pRenderNode, const Matrix4x4f& _mCamera )
{
	// Only calculate the matrices once.
	Matrix4x4f MVP, MV;
	Matrix3x3f NORM;
	memcpy( &MVP, &transformPipeline.GetModelViewProjectionMatrix(), sizeof( Matrix4x4f ) );
	memcpy( &MV, &transformPipeline.GetModelViewMatrix(), sizeof( Matrix4x4f ) );
	memcpy( &NORM, &transformPipeline.GetNormalMatrix(), sizeof( Matrix3x3f ) );

	if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY])
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}
		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}

		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[RIPPLE_EFFECT] )
	{
		// Depth test
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		// Activate shader
		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		// Bind the texture
		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}
		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}

		// Model View Projection Matrix
		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();

		// Model View Matrix
		glUniformMatrix4fv( _pRenderNode->GetMVUniformHandle(), 1, GL_FALSE,
			transformPipeline.GetModelViewMatrix() );
		glsCheckGLErrors();

		// Camera Matrix
		glUniformMatrix4fv( _pRenderNode->GetCameraUniformHandle(), 1, GL_FALSE, _mCamera );
		glsCheckGLErrors();

		// Time
		glUniform1f( _pRenderNode->GetTimeUniformHandle(), m_accumulatedTime );

		// Color
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );

		glsCheckGLErrors();

		// Hit Position
		glUniform3fv( _pRenderNode->GetHitPositionUniformHandle(), 1, _pRenderNode->GetHitPosition() );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[NO_TEX_DIR_LIGHT] )
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		glUniformMatrix4fv( CAssetManager::GetInstance()->m_vUniformMVPHandles[NO_TEX_DIR_LIGHT],
			1, GL_FALSE, MVP );
		glsCheckGLErrors();

		glUniformMatrix3fv( CAssetManager::GetInstance()->m_vUniformNormalHandles[NO_TEX_DIR_LIGHT],
			1, GL_FALSE, NORM );
		glsCheckGLErrors();

		Vec3f globalLightDir = { -1.0f, 1.0f, 0.5f }, LightDir;
		Matrix3x3f lightMatrix;
		glsExtractRotationMatrix3x3( lightMatrix, _mCamera );
		glsRotateVector( LightDir, globalLightDir, lightMatrix );
		glUniform3fv( CAssetManager::GetInstance()->m_vUniformLightDirHandles[NO_TEX_DIR_LIGHT], 1, LightDir );
		glsCheckGLErrors();
		glUniform4fv( CAssetManager::GetInstance()->m_vUniformColorHandles[NO_TEX_DIR_LIGHT], 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformAmbientIntensityHandles[NO_TEX_DIR_LIGHT], 0.15f );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformDiffuseIntensityHandles[NO_TEX_DIR_LIGHT], 1.0f );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[TEX_DIR_LIGHT] )
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}

		//if( _pRenderNode->hTextureSpecular != m_hCurrentSpec )
		//{
		glActiveTexture( GL_TEXTURE1 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetSpecularTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentSpec = _pRenderNode->hTextureSpecular;
		//}

		//if( _pRenderNode->hTextureEmissive != m_hCurrentEmissive )
		//{
		glActiveTexture( GL_TEXTURE2 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetEmissiveTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentEmissive = _pRenderNode->hTextureEmissive;
		//}

		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}

		glUniform1i( _pRenderNode->GetTextureUnitHandle1(), 1 );
		glsCheckGLErrors();
		// Don't currently have an emissive texture
		//glUniform1i( _pRenderNode->hTextureUnit2, 2 );
		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();

		glUniformMatrix3fv( _pRenderNode->GetNormalMatrixUniformHandle(),
			1, GL_FALSE, NORM );
		glsCheckGLErrors();

		Vec3f globalLightDir = { -1.0f, 1.0f, 0.5f }, LightDir;
		Matrix3x3f lightMatrix;
		glsExtractRotationMatrix3x3( lightMatrix, _mCamera );
		glsRotateVector( LightDir, globalLightDir, lightMatrix );

		glUniform3fv( CAssetManager::GetInstance()->m_vUniformLightDirHandles[TEX_DIR_LIGHT], 1, LightDir );
		glsCheckGLErrors();
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformAmbientIntensityHandles[TEX_DIR_LIGHT], 0.1f );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformDiffuseIntensityHandles[TEX_DIR_LIGHT], 1.0f );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT])
	{
		// Because we're drawing multiple light passes, an equal depth
		// doesn't fail the depth test.
		glDepthFunc( GL_LESS | GL_EQUAL );
		glsCheckGLErrors();

		// Shader to use
		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		// Diffuse Texture
		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}
		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}

		// Specular Texture
		//if( _pRenderNode->hTextureSpecular != m_hCurrentSpec )
		//{
		glActiveTexture( GL_TEXTURE1 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetSpecularTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentSpec = _pRenderNode->hTextureSpecular;
		//}
		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}

		// Normal Texture - Don't currently have one
		//glActiveTexture( GL_TEXTURE2 );
		//glBindTexture( GL_TEXTURE_2D, _pRenderNode->hTextureNormal );
		//glUniform1i( _pRenderNode->hTextureUnit2, 2 );

		// Emissive TExture
		glActiveTexture( GL_TEXTURE2 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetEmissiveTextureHandle() );
		glsCheckGLErrors();
		glUniform1i( _pRenderNode->GetTextureUnitHandle2(), 2 );
		glsCheckGLErrors();

		// Modelview Projection Matrix
		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();

		// Modelview Matrix
		glUniformMatrix4fv( _pRenderNode->GetMVUniformHandle(), 1, GL_FALSE,
			transformPipeline.GetModelViewMatrix() );
		glsCheckGLErrors();

		// camera matrix
		glUniformMatrix4fv( CAssetManager::GetInstance()->m_vUniformCameraHandles[TEX_POINT_LIGHT],
			1, GL_FALSE, _mCamera );
		glsCheckGLErrors();

		// Normal Matrix
		glUniformMatrix3fv( _pRenderNode->GetNormalMatrixUniformHandle(),
			1, GL_FALSE, NORM );

		if( m_currentLight >= 0 && m_currentLight < m_pointLightList.size() )
		{
			glUniform3fv( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT], 1, m_pointLightList[m_currentLight]->m_Position );
			glsCheckGLErrors();
		}

		// Multiply Color
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();

		// Light Intensity
		glUniform1f( CAssetManager::GetInstance()->m_vUniformAmbientIntensityHandles[TEX_POINT_LIGHT], 0.005f );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformDiffuseIntensityHandles[TEX_POINT_LIGHT], 5.0f );
		glsCheckGLErrors();

		// Light Falloff
		glUniform1f( CAssetManager::GetInstance()->m_vUniformLinearAttenuationHandles[TEX_POINT_LIGHT], 2.0f );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformQuadraticAttenuationHandles[TEX_POINT_LIGHT], 0.001f );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		glEnable( GL_BLEND );
		if( _pRenderNode->GetEnableBlending() )
			glBlendFuncSeparate( GL_ONE, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		else
			glBlendFuncSeparate( GL_ONE, GL_ONE, GL_ONE, GL_ONE );

		glsCheckGLErrors();
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[SKYBOX] )
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();
		glCullFace( GL_FRONT );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_CUBE_MAP, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[CUBE_MAP] )
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		//glEnable( GL_BLEND );
		//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();
		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();
		glUniformMatrix4fv( _pRenderNode->GetMVUniformHandle(), 1, GL_FALSE,
			transformPipeline.GetModelViewMatrix() );
		glsCheckGLErrors();
		glUniformMatrix3fv( _pRenderNode->GetNormalMatrixUniformHandle(), 1, GL_FALSE, NORM );
		glsCheckGLErrors();
		glUniformMatrix4fv( _pRenderNode->GetInverseCameraUniformHandle(), 1, GL_TRUE,
			_mCamera );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_CUBE_MAP, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] )
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}

		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}

		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();

		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();

		glUniformMatrix4fv( _pRenderNode->GetSkinningMatrixUniformHandle(), 30, GL_FALSE, *_pRenderNode->GetSkinningMatrix() );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	else if(_pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_POINT])
	{
		glDepthFunc( GL_LESS | GL_EQUAL );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}

		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}

		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();

		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();

		glUniformMatrix4fv( _pRenderNode->GetSkinningMatrixUniformHandle(), 30, GL_FALSE, *_pRenderNode->GetSkinningMatrix() );
		glsCheckGLErrors();

		// Modelview Matrix
		glUniformMatrix4fv( _pRenderNode->GetMVUniformHandle(), 1, GL_FALSE,
			transformPipeline.GetModelViewMatrix() );
		glsCheckGLErrors();

		// camera matrix
		glUniformMatrix4fv( CAssetManager::GetInstance()->m_vUniformCameraHandles[ANIMATED_POINT],
			1, GL_FALSE, _mCamera );
		glsCheckGLErrors();

		// Normal Matrix
		glUniformMatrix3fv( _pRenderNode->GetNormalMatrixUniformHandle(),
			1, GL_FALSE, NORM );

		if( m_currentLight >= 0 && m_currentLight < m_pointLightList.size() )
		{
			glUniform3fv( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][ANIMATED_POINT], 1, m_pointLightList[m_currentLight]->m_Position );
			glsCheckGLErrors();
		}

		// Multiply Color
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();

		// Light Intensity
		glUniform1f( CAssetManager::GetInstance()->m_vUniformAmbientIntensityHandles[ANIMATED_POINT], 0.005f );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformDiffuseIntensityHandles[ANIMATED_POINT], 5.0f );
		glsCheckGLErrors();

		// Light Falloff
		glUniform1f( CAssetManager::GetInstance()->m_vUniformLinearAttenuationHandles[ANIMATED_POINT], 2.0f );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformQuadraticAttenuationHandles[ANIMATED_POINT], 0.001f );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		glEnable( GL_BLEND );/*
		if( _pRenderNode->GetEnableBlending() )
			glBlendFuncSeparate( GL_ONE, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		else*/
			glBlendFuncSeparate( GL_ONE, GL_ONE, GL_ONE, GL_ONE );

		glsCheckGLErrors();
	}
	else if(_pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_DIR])
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}

		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}
		// Don't currently have an emissive texture
		//glUniform1i( _pRenderNode->hTextureUnit2, 2 );
		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();

		glUniformMatrix3fv( _pRenderNode->GetNormalMatrixUniformHandle(),
			1, GL_FALSE, NORM );
		glsCheckGLErrors();

		Vec3f globalLightDir = { -1.0f, 1.0f, 0.5f }, LightDir;
		Matrix3x3f lightMatrix;
		glsExtractRotationMatrix3x3( lightMatrix, _mCamera );
		glsRotateVector( LightDir, globalLightDir, lightMatrix );

		glUniform3fv( CAssetManager::GetInstance()->m_vUniformLightDirHandles[ANIMATED_DIR], 1, LightDir );
		glsCheckGLErrors();
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformAmbientIntensityHandles[ANIMATED_DIR], 0.1f );
		glsCheckGLErrors();
		glUniform1f( CAssetManager::GetInstance()->m_vUniformDiffuseIntensityHandles[ANIMATED_DIR], 1.0f );
		glsCheckGLErrors();

		glUniformMatrix4fv( _pRenderNode->GetSkinningMatrixUniformHandle(), 30, GL_FALSE, *_pRenderNode->GetSkinningMatrix() );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[BILLBOARD])
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		// Set the shader handle
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{

		// Activate the diffuse texture
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}

		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}


		Vec3f right, up, forward;
		glsLoadVector3( right, 1.0f, 0.0f, 0.0f );
		glsLoadVector3( up, 0.0f, 1.0f, 0.0f );
		glsLoadVector3( forward, 0.0f, 0.0f, 1.0f );

		Matrix3x3f cameraRotation;
		glsExtractRotationMatrix3x3( cameraRotation, _mCamera );

		Vec3f newRight, newUp, newForward;
		glsRotateVector( newRight, right, cameraRotation );
		glsRotateVector( newUp, up, cameraRotation ),
			glsRotateVector( newForward, forward, cameraRotation );

		modelViewMatrix.PushMatrix();
		Matrix4x4f mMV;
		modelViewMatrix.GetMatrix( mMV );

		glsCopyVector3( &mMV[0], right );
		glsCopyVector3( &mMV[4], up );
		glsCopyVector3( &mMV[8], forward );

		modelViewMatrix.LoadMatrix( mMV );

		float scale = _pRenderNode->GetScale();
		Vec3f vScale = { scale, scale, scale };
		Matrix4x4f mScale;
		glsLoadIdentity4x4( mScale );
		glsScaleMatrix4x4( mScale, vScale );

		// Set the scale matrix handle
		glUniformMatrix4fv( _pRenderNode->GetScaleMatrixUniformHandle(), 1, GL_FALSE, mScale );

		// Set the MVP handle
		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix() );
		modelViewMatrix.PopMatrix();

		// Set the particle color
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[DISSOLVE])
	{
		glDepthFunc( GL_LESS );
		glsCheckGLErrors();

		/*
		Do not comment this back in!
		This may be THE GIVEN!
		*/
		//if( _pRenderNode->hShader != m_hCurrentShader )
		//{
		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		//	m_hCurrentShader = _pRenderNode->hShader;
		//}

		// Set the diffuse
		//if( _pRenderNode->hTextureDiffuse != m_hCurrentDiffuse )
		//{
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentDiffuse = _pRenderNode->hTextureDiffuse;
		//}

		//if( _pRenderNode->hTextureUnit0 != m_hCurrentTextureUnit0 )
		//{
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		//	m_hCurrentTextureUnit0 = _pRenderNode->hTextureUnit0;
		//}

		// Set the dissolve texture
		//if( _pRenderNode->hTextureSpecular != m_hCurrentSpec )
		//{
		glActiveTexture( GL_TEXTURE1 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetSpecularTextureHandle() );
		glsCheckGLErrors();
		//	m_hCurrentSpec = _pRenderNode->hTextureSpecular;
		//}

		glUniform1i( _pRenderNode->GetTextureUnitHandle1(), 1 );
		glsCheckGLErrors();

		// Set the dissolve factor
		glUniform1f( _pRenderNode->GetDissolveFactorUniformHandle(), _pRenderNode->GetDissolveFactor() );
		glsCheckGLErrors();

		// Set the MVP matrix
		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();

		// Set the color
		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		// This alone isn't sufficient for transparent objects.
		if( _pRenderNode->GetEnableBlending() )
		{
			glEnable( GL_BLEND );								
			glsCheckGLErrors();
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glsCheckGLErrors();
		}
		else
		{
			glDisable( GL_BLEND );
			glsCheckGLErrors();
		}
	}
	else if( _pRenderNode->GetShaderHandle() == CAssetManager::GetInstance()->m_vShaderHandles[SCROLLING_UV] )
	{
		glDepthFunc( GL_LESS | GL_EQUAL );
		glsCheckGLErrors();

		glUseProgram( _pRenderNode->GetShaderHandle() );
		glsCheckGLErrors();
		
		glActiveTexture( GL_TEXTURE0 );
		glsCheckGLErrors();
		glBindTexture( GL_TEXTURE_2D, _pRenderNode->GetDiffuseTextureHandle() );
		glsCheckGLErrors();
		
		glUniform1i( _pRenderNode->GetTextureUnitHandle0(), 0 );
		glsCheckGLErrors();
		
		glUniformMatrix4fv( _pRenderNode->GetMVPUniformHandle(), 1, GL_FALSE, MVP );
		glsCheckGLErrors();

		glUniform1f( _pRenderNode->GetTimeUniformHandle(), m_accumulatedTime );

		glUniform4fv( _pRenderNode->GetColorUniformHandle(), 1, _pRenderNode->GetColor() );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderBackFacing() )
			glCullFace( GL_FRONT );
		else
			glCullFace( GL_BACK );
		glsCheckGLErrors();

		if( _pRenderNode->GetRenderWireFrame() )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT, GL_FILL );
		glsCheckGLErrors();

		glsCheckGLErrors();
	}
}

void CRenderer::AddToPointLightList( CPointLight* _light )
{
	m_pointLightList.push_back( _light );
}

void CRenderer::ClearLightList( void )
{
	m_pointLightList.clear();
}

void CRenderer::DoShading( void )
{
	// We'll move the lighting passes in here soon.  For right now, this
	// functionality is in DrawRenderList.
}

void CRenderer::DoPostEffect( CPostEffect* _pPostEffect )
{

}

void CRenderer::EnableDraw(void)
{
	glActiveTexture( GL_TEXTURE0 );
	glEnable( GL_BLEND );
	glDepthFunc( GL_ALWAYS );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void CRenderer::DisableDraw(void)
{
	glDisable( GL_BLEND );

	//////////////////////////////////////////////////////////////////////////////////
	//// HAX
	//// This shouldn't live here.  However, for the time being, this simply a
	//// demonstration of off-screen rendering.
	//////////////////////////////////////////////////////////////////////////////////

	//// Switch rendering to the screen frame buffer
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	//// Read from the off-screen frame buffer
	//glBindFramebuffer( GL_READ_FRAMEBUFFER, hFBO );

	//// Blit the scene to the screen buffer
	//glBlitFramebuffer( 0, 0, screenWidth, screenHeight, 0, 0,
	//	screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	//// If this option is enabled, blit the scene to the bottom-left corner of the
	//// screen buffer
	//if( debug_render_flags & OFF_SCREEN_RENDER_BUFFER_FLAG )
	//{
	//	// Read from the G-Buffer
	//	glBindFramebuffer( GL_READ_FRAMEBUFFER, hGBuffer );

	//	// Switch the position buffer to be the one that draws
	//	glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGPositionBuffer, 0 );

	//	glBlitFramebuffer( 0, 0, screenWidth, screenHeight, 0, 0,
	//		screenWidth / 5, screenHeight / 5, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	//	// Switch the buffers back
	//	glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGDiffuseBuffer, 0 );
	//	glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, hGPositionBuffer, 0 );

	//	if( !CheckFBOStatus( GL_READ_FRAMEBUFFER ) || !CheckFBOStatus( GL_DRAW_FRAMEBUFFER ) )
	//	{
	//		int IAmABreakPoint = 0;
	//	}
	//}

	//// If this option is enabled, blit the diffuse component of the scene to the
	//// bottom-left corner of the screen buffer
	//if( debug_render_flags & DIFFUSE_BUFFER_FLAG )
	//{
	//	// Read from the G-Buffer
	//	glBindFramebuffer( GL_READ_FRAMEBUFFER, hGBuffer );

	//	glBlitFramebuffer( 0, 0, screenWidth, screenHeight, screenWidth / 5, 0,
	//		(screenWidth / 5) * 2, screenHeight / 5, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	//	if( !CheckFBOStatus( GL_READ_FRAMEBUFFER ) || !CheckFBOStatus( GL_DRAW_FRAMEBUFFER ) )
	//	{
	//		int IAmABreakPoint = 0;
	//	}
	//}

	//// If this option is enabled, blit the normal component of the scene to the
	//// bottom-center of the screen buffer
	//if( debug_render_flags & NORMAL_BUFFER_FLAG )
	//{
	//	// Read from the G-Buffer
	//	glBindFramebuffer( GL_READ_FRAMEBUFFER, hGBuffer );

	//	// Switch the normal buffer to be the one that draws
	//	glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGNormalBuffer, 0 );

	//	glBlitFramebuffer( 0, 0, screenWidth, screenHeight, (screenWidth / 5) * 2, 0,
	//		(screenWidth / 5) * 3, screenHeight / 5, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	//	// Switch the buffers back
	//	glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGDiffuseBuffer, 0 );
	//	glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, hGNormalBuffer, 0 );

	//	if( !CheckFBOStatus( GL_READ_FRAMEBUFFER ) || !CheckFBOStatus( GL_DRAW_FRAMEBUFFER ) )
	//	{
	//		int IAmABreakPoint = 0;
	//	}
	//}

	//// Switch the frame buffers back
	//glBindFramebuffer( GL_FRAMEBUFFER, hFBO );

}

void CRenderer::DrawTexture( CUIElement* _pUIElement )
{
	shaderManager.UseStockShader( GLS_SHADER_UI, 0, _pUIElement->GetColor() );
	glBindTexture( GL_TEXTURE_RECTANGLE, _pUIElement->GetTextureHandle() );
	_pUIElement->GetGlBatch().Draw();
}

#pragma region old way of drawing ui elements
//void CRenderer::AddToUIList( CUIElement* _pUIElement )
//{
//	if( m_nObjectsInUIList >= MAX_UI_ELEMENTS )
//	{
//		cout << "Uh oh, somethings wrong!\n";
//		cout << "One of two things is wrong:\n";
//		cout << "Most likely, somebody's trying to read or write from the wrong place in memory.\n";
//		cout << "Otherwise, there may be too many objects in the UI List.  Consider increasing MAX_UI_ELEMENTS.\n\n";
//		ClearUIList();
//		return;
//	}
//	m_UIList[m_nObjectsInUIList] = _pUIElement;
//	++m_nObjectsInUIList;
//}
//
//void CRenderer::ClearUIList( void )
//{
//	m_nObjectsInUIList = 0;
//}

//void CRenderer::DrawUI( void )
//{
//	glActiveTexture( GL_TEXTURE0 );
//
//	if( m_nObjectsInUIList )
//		shaderManager.UseStockShader( GLS_SHADER_UI, 0, m_UIList[0]->GetOpacity() );
//
//	glEnable( GL_BLEND );
//	glDepthFunc( GL_ALWAYS );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//
//	for( unsigned int i = 0; i < m_nObjectsInUIList; ++i )
//	{
//		if( *m_UIList[i]->GetOpacity() != *m_UIList[0]->GetOpacity() )
//			shaderManager.UseStockShader( GLS_SHADER_UI, 0, m_UIList[i]->GetOpacity() );
//
//		glBindTexture( GL_TEXTURE_RECTANGLE, m_UIList[i]->GetTextureHandle() );
//		m_UIList[i]->GetGlBatch().Draw();
//	}
//
//	glDisable( GL_BLEND );
//
//	////////////////////////////////////////////////////////////////////////////////
//	// HAX
//	// This shouldn't live here.  However, for the time being, this simply a
//	// demonstration of off-screen rendering.
//	////////////////////////////////////////////////////////////////////////////////
//
//	// Switch rendering to the screen frame buffer
//	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
//
//	// Read from the off-screen frame buffer
//	glBindFramebuffer( GL_READ_FRAMEBUFFER, hFBO );
//
//	// Blit the scene to the screen buffer
//	glBlitFramebuffer( 0, 0, screenWidth, screenHeight, 0, 0,
//		screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR );
//
//	// If this option is enabled, blit the scene to the bottom-left corner of the
//	// screen buffer
//	if( debug_render_flags & OFF_SCREEN_RENDER_BUFFER_FLAG )
//	{
//		//// Read from the G-Buffer
//		//glBindFramebuffer( GL_READ_FRAMEBUFFER, hGBuffer );
//
//		//// Switch the position buffer to be the one that draws
//		//glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGPositionBuffer, 0 );
//
//		glBlitFramebuffer( 0, 0, screenWidth, screenHeight, 0, 0,
//			screenWidth / 5, screenHeight / 5, GL_COLOR_BUFFER_BIT, GL_LINEAR );
//
//		//// Switch the buffers back
//		//glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGDiffuseBuffer, 0 );
//		//glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, hGPositionBuffer, 0 );
//
//		if( !CheckFBOStatus( GL_READ_FRAMEBUFFER ) || !CheckFBOStatus( GL_DRAW_FRAMEBUFFER ) )
//		{
//			int IAmABreakPoint = 0;
//		}
//	}
//
//	// If this option is enabled, blit the diffuse component of the scene to the
//	// bottom-left corner of the screen buffer
//	if( debug_render_flags & DIFFUSE_BUFFER_FLAG )
//	{
//		// Read from the G-Buffer
//		glBindFramebuffer( GL_READ_FRAMEBUFFER, hGBuffer );
//
//		glBlitFramebuffer( 0, 0, screenWidth, screenHeight, screenWidth / 5, 0,
//			(screenWidth / 5) * 2, screenHeight / 5, GL_COLOR_BUFFER_BIT, GL_LINEAR );
//
//		if( !CheckFBOStatus( GL_READ_FRAMEBUFFER ) || !CheckFBOStatus( GL_DRAW_FRAMEBUFFER ) )
//		{
//			int IAmABreakPoint = 0;
//		}
//	}
//
//	// If this option is enabled, blit the normal component of the scene to the
//	// bottom-center of the screen buffer
//	if( debug_render_flags & NORMAL_BUFFER_FLAG )
//	{
//		// Read from the G-Buffer
//		glBindFramebuffer( GL_READ_FRAMEBUFFER, hGBuffer );
//
//		// Switch the normal buffer to be the one that draws
//		glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGNormalBuffer, 0 );
//
//		glBlitFramebuffer( 0, 0, screenWidth, screenHeight, (screenWidth / 5) * 2, 0,
//			(screenWidth / 5) * 3, screenHeight / 5, GL_COLOR_BUFFER_BIT, GL_LINEAR );
//
//		// Switch the buffers back
//		glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hGDiffuseBuffer, 0 );
//		glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, hGNormalBuffer, 0 );
//
//		if( !CheckFBOStatus( GL_READ_FRAMEBUFFER ) || !CheckFBOStatus( GL_DRAW_FRAMEBUFFER ) )
//		{
//			int IAmABreakPoint = 0;
//		}
//	}
//
//	// Switch the frame buffers back
//	glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
//	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, hFBO );
//
//	////////////////////////////////////////////////////////////////////////////////
//
//	ClearUIList();
//
//	// Check for OpenGL errors
//}
#pragma endregion

void CRenderer::Update( Vec3f _cameraPos, Matrix3x3f _cameraOrientation, float _elapsedTime )
{
	cameraFrame.SetOrigin( -_cameraPos[0], -_cameraPos[1],- _cameraPos[2] );
	cameraFrame.SetForwardVector( -_cameraOrientation[6], -_cameraOrientation[7], -_cameraOrientation[8] );
	cameraFrame.SetUpVector( _cameraOrientation[3], _cameraOrientation[4], _cameraOrientation[5] );
	glsCopyVector3( m_cameraPos, _cameraPos );
	m_accumulatedTime += _elapsedTime;
}

GLuint CRenderer::LoadTGAsAsCubeTexture( const char *_fileName[6] )
{
	GLuint textureCubeMap;
	glGenTextures( 1, &textureCubeMap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, textureCubeMap );

	GLbyte *pColorBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	GLenum cube[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for( int i = 0; i < 6; ++i )
	{
		(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
		pColorBits = glsReadTGABits( _fileName[i], &nWidth, &nHeight, &nComponents, &eFormat );
		if( !pColorBits )
		{
			MessageBoxA( 0, "Failed to load targa texture.", 0, 0 );
			return 0;
		}

		//glTexImage2D( cube[i], 0, nComponents, nWidth, nHeight, 0 /*no border ever*/,
		//	eFormat, GL_UNSIGNED_BYTE, pColorBits );

		// We're enabling texture compression, because the cube maps use six large textures
		// apiece and we'd rather have bad load time than poor runtime performance.  We may
		// switch over to DDS textures later.
		glTexImage2D( cube[i], 0, GL_COMPRESSED_RGBA, nWidth, nHeight, 0 /*no border ever*/,
			eFormat, GL_UNSIGNED_BYTE, pColorBits );

		free( pColorBits ); // one of our function calls called malloc
	}

	glGenerateMipmap( GL_TEXTURE_CUBE_MAP );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	return textureCubeMap;
}

void CRenderer::SwapUniforms( CRenderNode* _pRenderNode, GLuint _shaderHandle )
{
	if( _shaderHandle == CAssetManager::GetInstance()->m_vShaderHandles[TEX_DIR_LIGHT] )
	{
		_pRenderNode->SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEX_DIR_LIGHT] );
		_pRenderNode->SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEX_DIR_LIGHT] );
		_pRenderNode->SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_DIR_LIGHT] );
		_pRenderNode->SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEX_DIR_LIGHT] );
		_pRenderNode->SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEX_DIR_LIGHT] );
		_pRenderNode->SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_DIR_LIGHT] );
	}
	else if( _shaderHandle == CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] )
	{
		_pRenderNode->SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] );
		_pRenderNode->SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEX_POINT_LIGHT] );
		_pRenderNode->SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[TEX_POINT_LIGHT] );
		_pRenderNode->SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_POINT_LIGHT] );
		_pRenderNode->SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEX_POINT_LIGHT] );
		_pRenderNode->SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEX_POINT_LIGHT] );
		_pRenderNode->SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_POINT_LIGHT] );
		_pRenderNode->SetLightPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT], 0 );
	}
	else if( _shaderHandle == CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] )
	{
		_pRenderNode->SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
		_pRenderNode->SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
		_pRenderNode->SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[TEXTURE_ONLY] );
		_pRenderNode->SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEXTURE_ONLY] );
		_pRenderNode->SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
		_pRenderNode->SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
		_pRenderNode->SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEXTURE_ONLY] );
	}
	else if( _shaderHandle == CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] )
	{
		_pRenderNode->SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
		_pRenderNode->SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_ONLY] );
		_pRenderNode->SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_ONLY] );
		_pRenderNode->SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_ONLY] );
		_pRenderNode->SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_ONLY] );
	}
	else if( _shaderHandle == CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_DIR] )
	{
		_pRenderNode->SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_DIR] );
		_pRenderNode->SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_DIR] );
		_pRenderNode->SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[ANIMATED_DIR] );
		_pRenderNode->SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_DIR] );
		_pRenderNode->SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_DIR] );
		_pRenderNode->SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[ANIMATED_DIR] );
		_pRenderNode->SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_DIR] );
	}
	else if( _shaderHandle == CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_POINT] )
	{
		_pRenderNode->SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_POINT] );
		_pRenderNode->SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_POINT] );
		_pRenderNode->SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[ANIMATED_POINT] );
		_pRenderNode->SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[ANIMATED_POINT] );
		_pRenderNode->SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_POINT] );
		_pRenderNode->SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_POINT] );
		_pRenderNode->SetLightPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][ANIMATED_POINT], 0 );
		_pRenderNode->SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_POINT] );
	}
}

bool CRenderer::_CheckFBOStatus( GLenum _readOrWrite )
{
	GLenum result = glCheckFramebufferStatus( _readOrWrite );

	switch( result )
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		cout << "Current FBO binding is 0, but no default frame buffer exists.\n";
		return false;
	case GL_FRAMEBUFFER_COMPLETE:
		// OK
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		cout << "One of the buffers enabled for rendering is incomplete.\n";
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		cout << "No buffers are attached to the FBO.\n";
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		cout << "One of the buffer attachments enabled for rendering does not have a buffer attached.\n";
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		cout << "One of the buffer attachments enabled for reading does not have a buffer attached.\n";
		return false;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		cout << "The attempted combination of internal buffer formats is unsupported.\n";
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		cout << "The number of samples or the value for GL_TEXTURE_FIXED_SAMPLE_LOCATIONS for all buffers does not match.\n";
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		cout << "Not all color attachments are layered textures or bound to the same target.\n";
		return false;
	};

	return true;
}