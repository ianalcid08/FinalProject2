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

#include "../Header Files/stdafx.h"
#include "../Header Files/SkyBox.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"

CSkyBox::CSkyBox( void )
{
	this->SetType(ET_SKYBOX);

	// Default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// Default position
	GetRenderNode().SetPosition(  0.0f, 0.0f, 0.0f );
	
	// Collision entity
	Vec3f posOffset;
	glsLoadVector3( posOffset, 0.0f, 0.0f, 0.0f );
	this->SetCollisionEntity( new Sphere(1.0f, posOffset) );
}

CSkyBox::~CSkyBox( void )
{

}

void CSkyBox::InitShaders()
{
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
}
/*///////////////////////////////////////////////////////////////
/This function winds a cube CCW.  This is then loaded into the 
/m_renderNode's GLTriangleBatch.
/*///////////////////////////////////////////////////////////////
void CSkyBox::BuildGeometry( float _size )
{
	//36 wound verts
#pragma region Front face
	//Vert 0
	boxGeometry[0].position[0] = -_size;
	boxGeometry[0].position[1] =  _size;
	boxGeometry[0].position[2] =  _size;
	boxGeometry[0].normal[0] =	0.0f;
	boxGeometry[0].normal[1] =  0.0f;
	boxGeometry[0].normal[2] =  1.0f;
	boxGeometry[0].UV[0] = 0.0f;
	boxGeometry[0].UV[1] = 0.0f;

	//Vert 1
	boxGeometry[1].position[0] = -_size;
	boxGeometry[1].position[1] = -_size;
	boxGeometry[1].position[2] =  _size;
	boxGeometry[1].normal[0] =	0.0f;
	boxGeometry[1].normal[1] =  0.0f;
	boxGeometry[1].normal[2] =  1.0f;
	boxGeometry[1].UV[0] = 0.0f;
	boxGeometry[1].UV[1] = 1.0f;

	//Vert 2
	boxGeometry[2].position[0] =  _size;
	boxGeometry[2].position[1] =  _size;
	boxGeometry[2].position[2] =  _size;
	boxGeometry[2].normal[0] =	0.0f;
	boxGeometry[2].normal[1] =  0.0f;
	boxGeometry[2].normal[2] =  1.0f;
	boxGeometry[2].UV[0] = 1.0f;
	boxGeometry[2].UV[1] = 0.0f;

	//Vert 3
	boxGeometry[3].position[0] =  _size;
	boxGeometry[3].position[1] =  _size;
	boxGeometry[3].position[2] =  _size;
	boxGeometry[3].normal[0] =	0.0f;
	boxGeometry[3].normal[1] =  0.0f;
	boxGeometry[3].normal[2] =  1.0f;
	boxGeometry[3].UV[0] = 1.0f;
	boxGeometry[3].UV[1] = 0.0f;

	//Vert 4
	boxGeometry[4].position[0] = -_size;
	boxGeometry[4].position[1] = -_size;
	boxGeometry[4].position[2] =  _size;
	boxGeometry[4].normal[0] =	0.0f;
	boxGeometry[4].normal[1] =  0.0f;
	boxGeometry[4].normal[2] =  1.0f;
	boxGeometry[4].UV[0] = 0.0f;
	boxGeometry[4].UV[1] = 1.0f;

	//Vert 5
	boxGeometry[5].position[0] =  _size;
	boxGeometry[5].position[1] = -_size;
	boxGeometry[5].position[2] =  _size;
	boxGeometry[5].normal[0] =	0.0f;
	boxGeometry[5].normal[1] =  0.0f;
	boxGeometry[5].normal[2] =  1.0f;
	boxGeometry[5].UV[0] = 1.0f;
	boxGeometry[5].UV[1] = 1.0f;

#pragma endregion

#pragma region Bottom Face
	//Vert 6
	boxGeometry[6].position[0] = -_size;
	boxGeometry[6].position[1] = -_size;
	boxGeometry[6].position[2] =  _size;
	boxGeometry[6].normal[0] =	0.0f;
	boxGeometry[6].normal[1] = -1.0f;
	boxGeometry[6].normal[2] =  0.0f;
	boxGeometry[6].UV[0] = 0.0f;
	boxGeometry[6].UV[1] = 0.0f;

	//Vert 7
	boxGeometry[7].position[0] = -_size;
	boxGeometry[7].position[1] = -_size;
	boxGeometry[7].position[2] = -_size;
	boxGeometry[7].normal[0] =	0.0f;
	boxGeometry[7].normal[1] = -1.0f;
	boxGeometry[7].normal[2] =  0.0f;
	boxGeometry[7].UV[0] = 0.0f;
	boxGeometry[7].UV[1] = 1.0f;

	//Vert 8
	boxGeometry[8].position[0] =  _size;
	boxGeometry[8].position[1] = -_size;
	boxGeometry[8].position[2] =  _size;
	boxGeometry[8].normal[0] =	0.0f;
	boxGeometry[8].normal[1] = -1.0f;
	boxGeometry[8].normal[2] =  0.0f;
	boxGeometry[8].UV[0] = 0.0f;
	boxGeometry[8].UV[1] = 1.0f;

	//Vert 9
	boxGeometry[9].position[0] =  _size;
	boxGeometry[9].position[1] = -_size;
	boxGeometry[9].position[2] =  _size;
	boxGeometry[9].normal[0] =	0.0f;
	boxGeometry[9].normal[1] = -1.0f;
	boxGeometry[9].normal[2] =  0.0f;
	boxGeometry[9].UV[0] = 0.0f;
	boxGeometry[9].UV[1] = 1.0f;

	//Vert 10
	boxGeometry[10].position[0] = -_size;
	boxGeometry[10].position[1] = -_size;
	boxGeometry[10].position[2] = -_size;
	boxGeometry[10].normal[0] =	 0.0f;
	boxGeometry[10].normal[1] = -1.0f;
	boxGeometry[10].normal[2] =  0.0f;
	boxGeometry[10].UV[0] = 0.0f;
	boxGeometry[10].UV[1] = 1.0f;

	//Vert 11
	boxGeometry[11].position[0] =  _size;
	boxGeometry[11].position[1] = -_size;
	boxGeometry[11].position[2] = -_size;
	boxGeometry[11].normal[0] =	 0.0f;
	boxGeometry[11].normal[1] = -1.0f;
	boxGeometry[11].normal[2] =  0.0f;
	boxGeometry[11].UV[0] = 1.0f;
	boxGeometry[11].UV[1] = 1.0f;

#pragma endregion

#pragma region Back Face

	//Vert 12
	boxGeometry[12].position[0] =  _size;
	boxGeometry[12].position[1] =  _size;
	boxGeometry[12].position[2] = -_size;
	boxGeometry[12].normal[0] =	 0.0f;
	boxGeometry[12].normal[1] =  0.0f;
	boxGeometry[12].normal[2] = -1.0f;
	boxGeometry[12].UV[0] = 0.0f;
	boxGeometry[12].UV[1] = 0.0f;

	//Vert 13
	boxGeometry[13].position[0] =  _size;
	boxGeometry[13].position[1] = -_size;
	boxGeometry[13].position[2] = -_size;
	boxGeometry[13].normal[0] =	 0.0f;
	boxGeometry[13].normal[1] =  0.0f;
	boxGeometry[13].normal[2] = -1.0f;
	boxGeometry[13].UV[0] = 0.0f;
	boxGeometry[13].UV[1] = 1.0f;

	//Vert 14
	boxGeometry[14].position[0] = -_size;
	boxGeometry[14].position[1] =  _size;
	boxGeometry[14].position[2] = -_size;
	boxGeometry[14].normal[0] =	 0.0f;
	boxGeometry[14].normal[1] =  0.0f;
	boxGeometry[14].normal[2] = -1.0f;
	boxGeometry[14].UV[0] = 1.0f;
	boxGeometry[14].UV[1] = 0.0f;

	//Vert 15
	boxGeometry[15].position[0] = -_size;
	boxGeometry[15].position[1] =  _size;
	boxGeometry[15].position[2] = -_size;
	boxGeometry[15].normal[0] =	 0.0f;
	boxGeometry[15].normal[1] =  0.0f;
	boxGeometry[15].normal[2] = -1.0f;
	boxGeometry[15].UV[0] = 1.0f;
	boxGeometry[15].UV[1] = 0.0f;

	//Vert 16
	boxGeometry[16].position[0] =  _size;
	boxGeometry[16].position[1] = -_size;
	boxGeometry[16].position[2] = -_size;
	boxGeometry[16].normal[0] =	 0.0f;
	boxGeometry[16].normal[1] =  0.0f;
	boxGeometry[16].normal[2] = -1.0f;
	boxGeometry[16].UV[0] = 0.0f;
	boxGeometry[16].UV[1] = 1.0f;

	//Vert 17
	boxGeometry[17].position[0] = -_size;
	boxGeometry[17].position[1] = -_size;
	boxGeometry[17].position[2] = -_size;
	boxGeometry[17].normal[0] =	 0.0f;
	boxGeometry[17].normal[1] =  0.0f;
	boxGeometry[17].normal[2] = -1.0f;
	boxGeometry[17].UV[0] = 1.0f;
	boxGeometry[17].UV[1] = 1.0f;

#pragma endregion

#pragma region Top Face

	//Vert 18
	boxGeometry[18].position[0] = -_size;
	boxGeometry[18].position[1] =  _size;
	boxGeometry[18].position[2] = -_size;
	boxGeometry[18].normal[0] =	 0.0f;
	boxGeometry[18].normal[1] =  1.0f;
	boxGeometry[18].normal[2] =  0.0f;
	boxGeometry[18].UV[0] = 0.0f;
	boxGeometry[18].UV[1] = 0.0f;

	//Vert 19
	boxGeometry[19].position[0] = -_size;
	boxGeometry[19].position[1] =  _size;
	boxGeometry[19].position[2] =  _size;
	boxGeometry[19].normal[0] =	 0.0f;
	boxGeometry[19].normal[1] =  1.0f;
	boxGeometry[19].normal[2] =  0.0f;
	boxGeometry[19].UV[0] = 0.0f;
	boxGeometry[19].UV[1] = 1.0f;

	//Vert 20
	boxGeometry[20].position[0] =  _size;
	boxGeometry[20].position[1] =  _size;
	boxGeometry[20].position[2] =  _size;
	boxGeometry[20].normal[0] =	 0.0f;
	boxGeometry[20].normal[1] =  1.0f;
	boxGeometry[20].normal[2] =  0.0f;
	boxGeometry[20].UV[0] = 1.0f;
	boxGeometry[20].UV[1] = 0.0f;

	//Vert 21
	boxGeometry[21].position[0] =  _size;
	boxGeometry[21].position[1] =  _size;
	boxGeometry[21].position[2] = -_size;
	boxGeometry[21].normal[0] =	 0.0f;
	boxGeometry[21].normal[1] =  1.0f;
	boxGeometry[21].normal[2] =  0.0f;
	boxGeometry[21].UV[0] = 1.0f;
	boxGeometry[21].UV[1] = 0.0f;

	//Vert 22
	boxGeometry[22].position[0] = -_size;
	boxGeometry[22].position[1] =  _size;
	boxGeometry[22].position[2] =  _size;
	boxGeometry[22].normal[0] =	 0.0f;
	boxGeometry[22].normal[1] =  1.0f;
	boxGeometry[22].normal[2] =  0.0f;
	boxGeometry[22].UV[0] = 0.0f;
	boxGeometry[22].UV[1] = 1.0f;

	//Vert 23
	boxGeometry[23].position[0] =  _size;
	boxGeometry[23].position[1] =  _size;
	boxGeometry[23].position[2] =  _size;
	boxGeometry[23].normal[0] =	 0.0f;
	boxGeometry[23].normal[1] =  1.0f;
	boxGeometry[23].normal[2] =  0.0f;
	boxGeometry[23].UV[0] = 1.0f;
	boxGeometry[23].UV[1] = 1.0f;

#pragma endregion

#pragma region Right Face
	//Vert 24
	boxGeometry[22].position[0] =  _size;
	boxGeometry[22].position[1] =  _size;
	boxGeometry[22].position[2] =  _size;
	boxGeometry[22].normal[0] =	 1.0f;
	boxGeometry[22].normal[1] =  0.0f;
	boxGeometry[22].normal[2] =  0.0f;
	boxGeometry[22].UV[0] = 0.0f;
	boxGeometry[22].UV[1] = 0.0f;

	//Vert 25
	boxGeometry[25].position[0] =  _size;
	boxGeometry[25].position[1] = -_size;
	boxGeometry[25].position[2] =  _size;
	boxGeometry[25].normal[0] =	 1.0f;
	boxGeometry[25].normal[1] =  0.0f;
	boxGeometry[25].normal[2] =  0.0f;
	boxGeometry[25].UV[0] = 0.0f;
	boxGeometry[25].UV[1] = 1.0f;

	//Vert 26
	boxGeometry[26].position[0] =  _size;
	boxGeometry[26].position[1] =  _size;
	boxGeometry[26].position[2] = -_size;
	boxGeometry[26].normal[0] =	 1.0f;
	boxGeometry[26].normal[1] =  0.0f;
	boxGeometry[26].normal[2] =  0.0f;
	boxGeometry[26].UV[0] = 1.0f;
	boxGeometry[26].UV[1] = 0.0f;

	//Vert 27
	boxGeometry[27].position[0] =  _size;
	boxGeometry[27].position[1] = -_size;
	boxGeometry[27].position[2] =  _size;
	boxGeometry[27].normal[0] =	 1.0f;
	boxGeometry[27].normal[1] =  0.0f;
	boxGeometry[27].normal[2] =  0.0f;
	boxGeometry[27].UV[0] = 0.0f;
	boxGeometry[27].UV[1] = 1.0f;

	//Vert 28
	boxGeometry[28].position[0] =  _size;
	boxGeometry[28].position[1] =  _size;
	boxGeometry[28].position[2] = -_size;
	boxGeometry[28].normal[0] =	 1.0f;
	boxGeometry[28].normal[1] =  0.0f;
	boxGeometry[28].normal[2] =  0.0f;
	boxGeometry[28].UV[0] = 0.0f;
	boxGeometry[28].UV[1] = 1.0f;

	//Vert 29
	boxGeometry[29].position[0] =  _size;
	boxGeometry[29].position[1] = -_size;
	boxGeometry[29].position[2] = -_size;
	boxGeometry[29].normal[0] =	 1.0f;
	boxGeometry[29].normal[1] =  0.0f;
	boxGeometry[29].normal[2] =  0.0f;
	boxGeometry[29].UV[0] = 1.0f;
	boxGeometry[29].UV[1] = 1.0f;
#pragma endregion

#pragma region Left Face
	//Vert 30
	boxGeometry[30].position[0] = -_size;
	boxGeometry[30].position[1] =  _size;
	boxGeometry[30].position[2] = -_size;
	boxGeometry[30].normal[0] =	-1.0f;
	boxGeometry[30].normal[1] =  0.0f;
	boxGeometry[30].normal[2] =  0.0f;
	boxGeometry[30].UV[0] = 0.0f;
	boxGeometry[30].UV[1] = 0.0f;

	//Vert 31
	boxGeometry[31].position[0] = -_size;
	boxGeometry[31].position[1] = -_size;
	boxGeometry[31].position[2] = -_size;
	boxGeometry[31].normal[0] =	-1.0f;
	boxGeometry[31].normal[1] =  0.0f;
	boxGeometry[31].normal[2] =  0.0f;
	boxGeometry[31].UV[0] = 0.0f;
	boxGeometry[31].UV[1] = 1.0f;

	//Vert 32
	boxGeometry[32].position[0] = -_size;
	boxGeometry[32].position[1] =  _size;
	boxGeometry[32].position[2] =  _size;
	boxGeometry[32].normal[0] =	-1.0f;
	boxGeometry[32].normal[1] =  0.0f;
	boxGeometry[32].normal[2] =  0.0f;
	boxGeometry[32].UV[0] = 1.0f;
	boxGeometry[32].UV[1] = 0.0f;

	//Vert 33
	boxGeometry[33].position[0] = -_size;
	boxGeometry[33].position[1] =  _size;
	boxGeometry[33].position[2] =  _size;
	boxGeometry[33].normal[0] =	-1.0f;
	boxGeometry[33].normal[1] =  0.0f;
	boxGeometry[33].normal[2] =  0.0f;
	boxGeometry[33].UV[0] = 1.0f;
	boxGeometry[33].UV[1] = 0.0f;

	//Vert 34
	boxGeometry[34].position[0] = -_size;
	boxGeometry[34].position[1] = -_size;
	boxGeometry[34].position[2] = -_size;
	boxGeometry[34].normal[0] =	-1.0f;
	boxGeometry[34].normal[1] =  0.0f;
	boxGeometry[34].normal[2] =  0.0f;
	boxGeometry[34].UV[0] = 0.0f;
	boxGeometry[34].UV[1] = 1.0f;

	//Vert 35
	boxGeometry[35].position[0] = -_size;
	boxGeometry[35].position[1] = -_size;
	boxGeometry[35].position[2] =  _size;
	boxGeometry[35].normal[0] =	-1.0f;
	boxGeometry[35].normal[1] =  0.0f;
	boxGeometry[35].normal[2] =  0.0f;
	boxGeometry[35].UV[0] = 1.0f;
	boxGeometry[35].UV[1] = 1.0f;
#pragma endregion


	this->GetRenderNode().GetTriangleBatch()->BeginMesh(36, 36);
	{
		for(size_t i = 0; i < 12; i++)   // loop through the triangles
		{
			Vec3f vec[3];
			Vec3f norm[3];
			Vec2f UV[3];
			for(int j = 0; j < 3; j++) //load in the normals and the pos 3 times for each tri...
			{
				glsLoadVector3(vec[j], boxGeometry[i*3 +j].position[0], boxGeometry[i*3 + j].position[1], boxGeometry[i*3 +j].position[2]);  //load the three verts 
				glsLoadVector3(vec[j], boxGeometry[i*3 +j].normal[0], boxGeometry[i*3 + j].normal[1], boxGeometry[i*3 +j].normal[2]);		 //load the three normals
			}	
			for(int j = 0; j < 2; j ++)
			{
				glsLoadVector2(UV[j],boxGeometry[i*3 +j].UV[0],boxGeometry[i*3 +j].UV[1]);													//load the two UVs
			}
			this->GetRenderNode().GetTriangleBatch()->AddTriangle(vec,norm,UV);  // add the triangle to the batch
		}
	}
	this->GetRenderNode().GetTriangleBatch()->End();
}



void CSkyBox::Release( void )
{


}
bool CSkyBox::Update( float _elapsedTime )
{

	return true;
}
void CSkyBox::HandleReaction( IEntity* _other )
{
	//empty COLLIDE WIFF TEH SKAH!?!?!?!
}
