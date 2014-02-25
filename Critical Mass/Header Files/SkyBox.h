/*//////////////////////////////////////////////////////////
/Filename:		Skybox.h
/Author:		Tom Stefl
/Last Modified: 7/30/2013
/Purpose:		This class holds a render node for the skybox 
/				in our game.  The function BuildGeometry winds a
/				CCW cube.  The remaining attributes such as
/				the shader specifics must be set manually as
/				these vary from object to object.
/*//////////////////////////////////////////////////////////


#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "GLSetup.h"
#include "GLShaderManager.h"
#include "../Header Files/RenderNode.h"
#include "../Header Files/BaseEntity.h"
struct vertex
{
	Vec3f position;
	Vec3f normal;
	Vec2f UV;
};

class CSkyBox : public CBaseEntity
{
private:

	vertex boxGeometry[36];
	//GLuint				m_textureCubeMap;

	//GLuint				m_hShaderSkyBox;
	//GLuint				m_hUniformMVPSkyBox;

	//GLuint				m_hShaderCubeMap;
	//GLuint				m_hUniformColorCubeMap;
	//GLuint				m_hUniformMVPCubeMap;
	//GLuint				m_hUniformMVCubeMap;
	//GLuint				m_hUniformNormalCubeMap;
	//GLuint				m_hUniformICCubeMap;

	//textureOnly shader stuff
	//GLuint				hShader;
	//GLuint				hTextureDiffuse;

	//// Uniform handles - These will likely change
	//GLuint				hUniformMVP;			// Matrix4x4f							
	//GLuint				hUniformMV;				// Matrix4x4f
	//GLuint				hTextureUnit;


public:
	CSkyBox( void );
	~CSkyBox( void );

	void BuildGeometry(float _size);
	void InitShaders();
	
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( IEntity* _other );

};
#endif