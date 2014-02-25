/*
	File:		RenderNode.h
	Author:		Bob Pasekoff
	Updated:	7/17/2013

*/
#ifndef _RENDERNODE_
#define _RENDERNODE_

#include "../Header Files/glsMath.h"
#include "../Header Files/GLTriangleBatch.h"
#include "../Header Files/Mesh.h"

class CRenderNode
{
	// Replace these with handles in the asset manager later
	GLTriangleBatch		*triangleBatch;
	CMesh				*m_Mesh;
	
	// World position, rotation, and scale
	Vec3f				m_vPosition;
	Vec3f				m_vUp;
	Vec3f				m_vRight;
	Vec3f				m_vForward;
	float				m_fScale;

	// Texture and shader handles
	GLuint				hShader;
	GLuint				hTextureDiffuse;
	GLuint				hTextureNormal;		// currently unused, but may be used later
	GLuint				hTextureSpecular;
	GLuint				hTextureEmissive;
	GLuint				hTextureUnit0;
	GLuint				hTextureUnit1;
	GLuint				hTextureUnit2;
	GLuint				hTextureUnit3;

	// Uniform handles - These will likely change
	GLuint				m_numLights;
	GLuint				hUniformLightPos[8];	// Vec3f		 // forward rendering	
	GLuint				hUniformMVP;			// Matrix4x4f							
	GLuint				hUniformMV;				// Matrix4x4f	
	GLuint				hUniformProj;
	GLuint				hUniformNormal;			// matrix?
	GLuint				hUniformInverseCamera;	// Matrix4x4f
	GLuint				hUniformColor;			// Vec4f
	GLuint				hUniformTime;			// float
	GLuint				hUniformCamera;			// Matrix4x4f
	GLuint				hUniformHitPosition;	// Vec3f
	GLuint				hUniformDissolveFactor;
	GLuint				hUniformSkinningMatrix;
	GLuint				hUniformScaleMatrix;

	Vec3f				m_hitPosition;
	GLfloat				m_dissolveFactor;

	Matrix4x4f*			m_SkinningMatrix;

	Vec4f				m_color;

	// Special considerations
	bool				renderBackFacing;
	bool				enableBlending;
	bool				renderWireFrame;

	//Tom
	float m_cameraDist;

public:
	inline void SetColor( float _red = 1.0f, float _green = 0.0f, float _blue = 1.0f, float _alpha = 1.0f )
	{
		m_color[0] = _red;
		m_color[1] = _green;
		m_color[2] = _blue;
		m_color[3] = _alpha;
	};

	CRenderNode( void );
	~CRenderNode( void ){}

	GLTriangleBatch*		GetTriangleBatch( void )														{ return triangleBatch; }
	void					SetTriangleBatch( GLTriangleBatch* _triangleBatch )								{ triangleBatch = _triangleBatch; }

	const CMesh*			GetMesh( void )																	{ return m_Mesh; }
	void					SetMesh( CMesh* _mesh )															{ m_Mesh = _mesh; }

	const float*			GetPosition( void )																{ return m_vPosition; }
	void					SetPosition( const Vec3f _position )											{ glsCopyVector3( m_vPosition, _position ); }
	void					SetPosition( const float _x, const float _y, const float _z )					{ glsLoadVector3( m_vPosition, _x, _y, _z ); }
	
	const float*			GetUpVector( void )																{ return m_vUp; }
	void					SetUpVector( const Vec3f _up )													{ glsCopyVector3( m_vUp, _up ); }
	void					SetUpVector( const float _x, const float _y, const float _z )					{ glsLoadVector3( m_vUp, _x, _y, _z ); }

	const float*			GetRightVector( void )															{ return m_vRight; }
	void					SetRightVector( const Vec3f _right )											{ glsCopyVector3( m_vRight, _right ); }
	void					SetRightVector( const float _x, const float _y, const float _z )				{ glsLoadVector3( m_vRight, _x, _y, _z ); }

	const float*			GetForwardVector( void )														{ return m_vForward; }
	void					SetForwardVector( const Vec3f _forward )										{ glsCopyVector3( m_vForward, _forward ); }
	void					SetForwardVector( const float _x, const float _y, const float _z )				{ glsLoadVector3( m_vForward, _x, _y, _z ); }

	const GLuint&			GetShaderHandle( void )															{ return hShader; }
	void					SetShaderHandle( GLuint _hShader )												{ hShader = _hShader; }

	const GLuint&			GetDiffuseTextureHandle( void )													{ return hTextureDiffuse; }
	void					SetDiffuseTextureHandle( GLuint _hTextureDiffuse )								{ hTextureDiffuse = _hTextureDiffuse; }

	const GLuint&			GetNormalTextureHandle( void )													{ return hTextureNormal; }
	void					SetNormalTextureHandle( GLuint _hTextureNormal )								{ hTextureNormal = _hTextureNormal; }

	const GLuint&			GetSpecularTextureHandle( void )												{ return hTextureSpecular; }
	void					SetSpecularTextureHandle( GLuint _hTextureSpecular )							{ hTextureSpecular = _hTextureSpecular; }

	const GLuint&			GetEmissiveTextureHandle( void )												{ return hTextureEmissive; }
	void					SetEmissiveTextureHandle( GLuint _hTextureEmissive )							{ hTextureEmissive = _hTextureEmissive; }

	const GLuint&			GetTextureUnitHandle0( void )													{ return hTextureUnit0; }
	void					SetTextureUnitHandle0( GLuint _hTextureUnit0 )									{ hTextureUnit0 = _hTextureUnit0; }

	const GLuint&			GetTextureUnitHandle1( void )													{ return hTextureUnit1; }
	void					SetTextureUnitHandle1( GLuint _hTextureUnit1 )									{ hTextureUnit1 = _hTextureUnit1; }

	const GLuint&			GetTextureUnitHandle2( void )													{ return hTextureUnit2; }
	void					SetTextureUnitHandle2( GLuint _hTextureUnit2 )									{ hTextureUnit2 = _hTextureUnit2; }

	const GLuint&			GetTextureUnitHandle3( void )													{ return hTextureUnit3; }
	void					SetTextureUnitHandle3( GLuint _hTextureUnit3 )									{ hTextureUnit3 = _hTextureUnit3; }

	const GLuint&			GetNumLights( void )															{ return m_numLights; }
	void					SetNumLights( GLuint _numLights )												{ m_numLights = _numLights; }

	const GLuint&			GetLightPositionUniformHandle( unsigned int _index )							{ return hUniformLightPos[_index]; }
	void					SetLightPositionUniformHandle( GLuint _hUniformLightPos, unsigned int _index )	{ hUniformLightPos[_index] = _hUniformLightPos; }

	const GLuint&			GetMVPUniformHandle( void )														{ return hUniformMVP; }
	void					SetMVPUniformHandle( GLuint _hUniformMVP )										{ hUniformMVP = _hUniformMVP; }

	const GLuint&			GetMVUniformHandle( void )														{ return hUniformMV; }
	void					SetMVUniformHandle( GLuint _hUniformMV )										{ hUniformMV = _hUniformMV; }

	const GLuint&			GetProjMatrixUniformHandle( void )												{ return hUniformProj; }
	void					SetProjMatrixUniformHandle( GLuint _hUniformProj )								{ hUniformProj = _hUniformProj; }

	const GLuint&			GetNormalMatrixUniformHandle( void )											{ return hUniformNormal; }
	void					SetNormalMatrixUniformHandle( GLuint _hUniformNormal )							{ hUniformNormal = _hUniformNormal; }

	const GLuint&			GetInverseCameraUniformHandle( void )											{ return hUniformInverseCamera; }
	void					SetInverseCameraUniformHandles( GLuint _hUniformInverseCamera )					{ hUniformInverseCamera = _hUniformInverseCamera; }

	const GLuint&			GetColorUniformHandle( void )													{ return hUniformColor; }
	void					SetColorUniformHandle( GLuint _hUniformColor )									{ hUniformColor = _hUniformColor; }

	const GLuint&			GetTimeUniformHandle( void )													{ return hUniformTime; }
	void					SetTimeUniformHandle( GLuint _hUniformTime )									{ hUniformTime = _hUniformTime; }

	const GLuint&			GetCameraUniformHandle( void )													{ return hUniformCamera; }
	void					SetCameraUniformHandle( GLuint _hUniformCamera )								{ hUniformCamera = _hUniformCamera; }

	const GLuint&			GetHitPositionUniformHandle( void )												{ return hUniformHitPosition; }
	void					SetHitPositionUniformHandle( GLuint _hUniformHitPosition )						{ hUniformHitPosition = _hUniformHitPosition; }

	const GLuint&			GetDissolveFactorUniformHandle( void )											{ return hUniformDissolveFactor; }
	void					SetDissolveFactorUniformHandle( GLuint _hUniformDissolveFactor )				{ hUniformDissolveFactor = _hUniformDissolveFactor; }

	const GLuint&			GetSkinningMatrixUniformHandle( void )											{ return hUniformSkinningMatrix; }
	void					SetSkinningMatrixUniformHandle( GLuint _hUniformSkinningMatrix )				{ hUniformSkinningMatrix = _hUniformSkinningMatrix; }

	const GLuint&			GetScaleMatrixUniformHandle( void )													{ return hUniformScaleMatrix; }
	void					SetScaleMatrixUniformHandle( GLuint _hUniformScaleMatrix )								{ hUniformScaleMatrix = _hUniformScaleMatrix; }

	const float*			GetHitPosition( void )															{ return m_hitPosition; }
	void					SetHitPosition( const Vec3f _hitPosition )										{ glsCopyVector3( m_hitPosition, _hitPosition ); }

	const GLfloat&			GetDissolveFactor( void )														{ return m_dissolveFactor; }
	void					SetDissolveFactor( GLfloat _dissolveFactor )									{ m_dissolveFactor = _dissolveFactor; }

	const Matrix4x4f*		GetSkinningMatrix( void )														{ return m_SkinningMatrix; }
	void					SetSkinningMatrix( Matrix4x4f* _skinningMatrix )								{ m_SkinningMatrix = _skinningMatrix; }

	const float*			GetColor( void )																{ return m_color; }
	void					SetColor( Vec4f _color )														{ glsCopyVector4( m_color, _color ); }

	const bool&				GetRenderBackFacing( void )														{ return renderBackFacing; }
	void					SetRenderBackFacing( bool _renderBackFacing )									{ renderBackFacing = _renderBackFacing; }

	const bool&				GetEnableBlending( void )														{ return enableBlending; }
	void					SetEnableBlending( bool _enableBlending )										{ enableBlending = _enableBlending; }

	const bool&				GetRenderWireFrame( void )														{ return renderWireFrame; }
	void					SetRenderWireframe( bool _renderWireFrame )										{ renderWireFrame = _renderWireFrame; }

	const float&			GetScale( void )																{ return m_fScale; }
	void					SetScale( float _scale )														{ m_fScale = _scale; }

	const float&			GetCameraDist( void )															{ return m_cameraDist; }
	void					SetCameraDist( float _camDst )													{ m_cameraDist = _camDst; }
};




#endif // _RENDERNODE_