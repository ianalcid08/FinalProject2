/*///////////////////////////////////////////////////////
/FileName:		Particle.cpp
/Author:		Thomas Stefl
/Date:			8/6/2013
/Last Modified: 8/6/2013
/Purpose:		This class holds all the relative 
/				information necessary for each particle.
/*///////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/Particle.h"
#include "../Header Files/AssetManager.h"


Particle::Particle(void)
{
	m_Position[0]	  = 0.0f;
	m_Position[1]	  = 0.0f;
	m_Position[2]	  = 0.0f;
					  
	m_Velocity[0]	  = 0.0f;
	m_Velocity[1]	  = 0.0f;
	m_Velocity[2]	  = 0.0f;
	
	m_Acceleration[0] = 0.0f;
	m_Acceleration[1] = 0.0f;
	m_Acceleration[2] = 0.0f;

	m_LifeSpan = 0.0f;
	m_Scale = 1.0f;


	//m_RenderNode.hShader = CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY];
	//m_RenderNode.hUniformMVP = CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY];
	//m_RenderNode.hTextureUnit0 = CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY];

	//BuildGeometry();
	//PS shader handles
	Initialize();
	this->BuildGeometry();
}

Particle::~Particle(void)
{

}

void Particle::Initialize(void)
{
	m_RenderNode.SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[BILLBOARD] );
	m_RenderNode.SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[BILLBOARD] );
	//m_RenderNode.SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetParticleTexture(WHITE_PARTICLE) );
	//VS shader handles
	m_RenderNode.SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[BILLBOARD] );
	m_RenderNode.SetScaleMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformScaleMatrixHandles[BILLBOARD] );
	m_RenderNode.SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[BILLBOARD] );

	m_RenderNode.SetEnableBlending( true );

	m_RenderNode.SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[PARTICLE_TB] );

	//m_Scale = 1.0f;
}

void Particle::SetPosition(const Vec3f _newPosition)
{
	memcpy(m_Position,_newPosition,sizeof(Vec3f));
}

void Particle::SetVelocity(const Vec3f _newVelocity)
{
	memcpy(m_Velocity,_newVelocity,sizeof(Vec3f));
}

void Particle::SetAcceleration(Vec3f _newAcceleration)
{
	memcpy(m_Acceleration,_newAcceleration,sizeof(Vec3f));
}

///////////////////////////////////////////////////////
//This function updates the particle's lifespan,
//position based on velocity, and velocity based on
//acceleration by time.
///////////////////////////////////////////////////////
void Particle::Update(float _fElapsedTime)
{
	//Test if the particle is even alive... why update?
	if(m_LifeSpan > 0.0f)
	{
		//update Lifespan
		m_LifeSpan -= _fElapsedTime;

		//Update velocity based on acceleration
		m_Velocity[0] += m_Acceleration[0] * _fElapsedTime;
		m_Velocity[1] += m_Acceleration[1] * _fElapsedTime;
		m_Velocity[2] += m_Acceleration[2] * _fElapsedTime;

		//Update Position based on Velocity
		m_Position[0] += m_Velocity[0] * _fElapsedTime * 50;
		m_Position[1] += m_Velocity[1] * _fElapsedTime * 50;
		m_Position[2] += m_Velocity[2] * _fElapsedTime * 50;

		//put the particles position into the render node.
		m_RenderNode.SetPosition( m_Position );
	
		m_RenderNode.SetScale(m_Scale);
	}
	//cam - you = forward; norm adjust right and up
}

void Particle::Render()
{

}

bool Particle::IsAlive()
{
	return (m_LifeSpan > 0.0f);
}

//////////////////////////////////////////
//This function will build the geometry
//for the quad inside the tri batch 
//of each particle's render node. It builds
//two triangles and adds them to the 
//rendernode's batch
/////////////////////////////////////////
void Particle::BuildGeometry()
{
	m_RenderNode.GetTriangleBatch()->BeginMesh(6, 6);
#pragma region Build the verts norms and uvs

	//first triangle
	Vec3f vec[3];
	Vec3f norm[3];
	Vec2f UV[3];

	//second triangle
	Vec3f vec2[3];
	Vec3f norm2[3];
	Vec2f UV2[3];

	//Top Left
	vec[0][0] = 0.0f;
	vec[0][1] = 1.0f;
	vec[0][2] = 0.0f;

	norm[0][0] = 0.0f;
	norm[0][1] = 0.0f;
	norm[0][2] = 1.0f;

	UV[0][0] = 0.0f;
	UV[0][1] = 0.0f;

	//Bottom Left
	vec[1][0] = 0.0f;
	vec[1][1] = 0.0f;
	vec[1][2] = 0.0f;

	norm[1][0] = 0.0f;
	norm[1][1] = 0.0f;
	norm[1][2] = 1.0f;

	UV[1][0] = 0.0f;
	UV[1][1] = 1.0f;

	//Top Right
	vec[2][0] = 1.0f;
	vec[2][1] = 1.0f;
	vec[2][2] = 0.0f;

	norm[2][0] = 0.0f;
	norm[2][1] = 0.0f;
	norm[2][2] = 1.0f;

	UV[2][0] = 1.0f;
	UV[2][1] = 0.0f;

	m_RenderNode.GetTriangleBatch()->AddTriangle(vec,norm,UV);

	//Top Right
	vec2[0][0] = 1.0f;
	vec2[0][1] = 1.0f;
	vec2[0][2] = 0.0f;

	norm2[0][0] = 0.0f;
	norm2[0][1] = 0.0f;
	norm2[0][2] = 1.0f;

	UV2[0][0] = 1.0f;
	UV2[0][1] = 0.0f;

	//Bottom Left
	vec2[1][0] = 0.0f;
	vec2[1][1] = 0.0f;
	vec2[1][2] = 0.0f;

	norm2[1][0] = 0.0f;
	norm2[1][1] = 0.0f;
	norm2[1][2] = 1.0f;

	UV2[1][0] = 0.0f;
	UV2[1][1] = 1.0f;

	//Bottom Right
	vec2[2][0] = 1.0f;
	vec2[2][1] = 0.0f;
	vec2[2][2] = 0.0f;

	norm2[2][0] = 0.0f;
	norm2[2][1] = 0.0f;
	norm2[2][2] = 1.0f;

	UV2[2][0] = 1.0f;
	UV2[2][1] = 1.0f;

	m_RenderNode.GetTriangleBatch()->AddTriangle(vec2,norm2,UV2);

#pragma endregion
	m_RenderNode.GetTriangleBatch()->End();
}

void Particle::SetParticleColor(Vec3f _newColor)
{
	//glsCopyVector3(m_Color,_newColor);
	m_RenderNode.SetColor(_newColor[0],_newColor[1],_newColor[2]);
}

