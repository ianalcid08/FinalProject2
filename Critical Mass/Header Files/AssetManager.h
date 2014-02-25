/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : AssetManager.h
Author : Evan Wright

Description : This module will be responsible for loading and containing texture and 
			  shaders assets as well as animations and meshes.  It will contain an array 
			  of data for every game object where assets must be loaded in. All of the 
			  assets are loaded at the beginning of the game in order to cut down or even 
			  eliminate long loading times during gameplay.  Accessors will be available 
			  to access all the elements in an array of object data and will use an int 
			  ID to get the correct index for data.

Created :  07/18/2013
Modified : 07/19/2013
Modified by: IA

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

#include "Definitions.h"
#include <vector>
#include <map>
#include <fstream>
#include "Mesh.h"
#include "AnimationProcessor.h"
#include "GLSetup.h"
#include "GLShaderManager.h"
#include "GeometryEffect.h"
using namespace std;

// Singleton
class CAssetManager
{
	struct ObjData
	{
		GLuint					m_hShader;
		GLuint					m_DiffuseHandle;
		GLuint					m_hTextureNormal;
		GLuint					m_hTextureSpecular;
		GLuint					m_hTextureEmissive;
		CAnimation				m_Animations[AM_MAX_ANIMS];
		CMesh					m_ModelData;
	};

	// SINGLETON!
	CAssetManager( void );
	CAssetManager( const CAssetManager& );
	CAssetManager& operator=( const CAssetManager& );
	virtual ~CAssetManager( void );


	// Holds the data needed to make each obj in game
	ObjData				m_ObjectDataTotal[ET_NUMTYPES];	
	ObjData				m_SmallEnemyData[4];
	ObjData				m_ShieldData[NUM_SHIELDS];

	GLuint				m_Textures[MAX_TEXTURES];

	//Array for textures used for Effects
	GLuint				m_FXTexturesArr[MAX_TEXTURES];



	// black, red, yellow, blue.

	ObjData m_GeoEffectDataArray[AM_MAX_EFFECTS_ANIMS];

	//HAX
	//Author: Tom
	//This is a dynamically allocated batch must be deleted every movement
	//GLTriangleBatch * m_


public:
	//TODO: Write accessors / mutators for these and make them private
	vector<GLuint>	m_vShaderHandles;
	vector<GLuint>	m_vTextureDiffuseHandles;
	vector<GLuint>	m_vTextureNormalHandles;
	vector<GLuint>	m_vTextureSpecularHandles;
	vector<GLuint>	m_vTextureUnit0Handles;
	vector<GLuint>	m_vTextureUnit1Handles;
	vector<GLuint>	m_vTextureUnit2Handles;

	vector<GLuint>	m_vUniformLightPosHandles[8];
	vector<GLuint>	m_vUniformLightDirHandles;
	vector<GLuint>	m_vUniformLinearAttenuationHandles;
	vector<GLuint>	m_vUniformQuadraticAttenuationHandles;
	vector<GLuint>	m_vUniformAmbientIntensityHandles;
	vector<GLuint>	m_vUniformDiffuseIntensityHandles;
	vector<GLuint>	m_vUniformMVPHandles;			
	vector<GLuint>	m_vUniformMVHandles;
	vector<GLuint>	m_vUniformProjHandles;				
	vector<GLuint>	m_vUniformCameraHandles;				
	vector<GLuint>	m_vUniformNormalHandles;			
	vector<GLuint>	m_vUniformInverseCameraHandles;	
	vector<GLuint>	m_vUniformScaleMatrixHandles;
	vector<GLuint>	m_vUniformColorHandles;
	vector<GLuint>	m_vUniformJointHandles;
	vector<GLuint>	m_vUniformBindHandles;
	vector<GLuint>	m_vUniformTimeHandles;
	vector<GLuint>	m_vUniformDissolveFactorHandles;
	vector<GLuint>	m_vUniformHitPositionHandles;

	// Eventually, we will want to get rid of Triangle batch.  In the mean time...
	GLTriangleBatch m_vTriangleBatches[MAX_TB];

	// HAX until we set this up as a map or something
	CMesh					m_playerMesh;
	//CMesh					m_geoEffectMesh;

	static CAssetManager* GetInstance( void );

	void Release( void );

	void Init();
	void LoadShaders( void );
	void LoadTriangleBatches( void );
	void BuildTriangleBatch( CMesh& _mesh, GLTriangleBatch& _triangleBatch, bool _Animated = false );
	GLuint LoadTGAAsTexture( const char* _fileName, GLenum _wrapMode = GL_CLAMP_TO_EDGE, GLenum _filter = GL_LINEAR );
	GLuint LoadTGAsAsCubeTexture( const char *_fileName[6] );
	GLuint LoadTGAAsTextureRect( const char *szFileName, GLenum wrapMode = GL_CLAMP_TO_EDGE, GLenum filter = GL_LINEAR );
	void UnloadTexture( GLuint _hTexture );

	unsigned int LoadAnimation( char* _fileName );

	unsigned int LoadMesh( char* _fileName, int _objID );

	unsigned int GetTexture( unsigned int _index );
	//Returns a texture for particle FX
	GLuint GetParticleTexture(unsigned int _index) {return m_FXTexturesArr[_index];}

	CAnimation*		GetAnimation( unsigned int _ObjType, unsigned int _Anim ) { return &m_ObjectDataTotal[_ObjType].m_Animations[_Anim]; }
	CAnimation*		GetFXAnimation( unsigned int _FXType, unsigned int _FXAnim ) { return &m_GeoEffectDataArray[_FXType].m_Animations[_FXAnim]; }

	ObjData*		GetFXData(unsigned int _FXType) {return &m_GeoEffectDataArray[_FXType];}

	ObjData&		GetObjData(unsigned int _objIndex)  {return m_ObjectDataTotal[_objIndex];}
	ObjData&		GetSmallEnemyData(unsigned int _objIndex) {return m_SmallEnemyData[_objIndex];}
	ObjData&		GetShieldData(unsigned int _objIndex) {return m_ShieldData[_objIndex];}

	//CMesh			GetMeshData(unsigned int _objIndex) { return m_ObjectDataTotal[_objIndex].m_ModelData;} 
	//unsigned int	GetTextureData(unsigned int _objIndex) { return m_ObjectDataTotal[_objIndex].m_TextureHandle;} 
};

#endif
