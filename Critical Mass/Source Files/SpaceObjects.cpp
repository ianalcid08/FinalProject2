//////////////////////////////////////////////////////////////////////
//Filename: SpaceObjects.cpp
//Author:	Thomas Stefl
//Date:		9-13-2013
//Desc:		This file will detail how the objects in space such as
//			the sun and the asteroids will update around the moon.
/////////////////////////////////////////////////////////////////////
#include "../Header Files/StdAfx.h"
#include "../Header Files/SpaceObjects.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/AssetManager.h"


SpaceObjects::SpaceObjects(void)
{

}

SpaceObjects::~SpaceObjects(void)
{

}

void SpaceObjects::Initialize(int _IDType, Vec3f _pos, Vec3f _velocity, Vec3f _acceleration)
{
	
	this->SetType(_IDType);
	GetRenderNode().SetPosition(_pos[0],_pos[1],_pos[2]);
	//glsCopyVector3(GetRenderNode().GetPosition(),_pos);
	glsCopyVector3(m_Velocity,_velocity);
	glsCopyVector3(m_Acceleration,_acceleration);

	//glsLoadVector3(m_FocusPoint,0.0f,75.0f,0.0f);


	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );

	switch(GetType())
	{
		case ET_ASTEROID:
			{
				glsLoadVector3(m_FocusPoint,0.0f,75.0f,0.0f);
				//Determine which Triangle Batch (shape) and Texture you want them to be
				GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_ASTEROID).m_DiffuseHandle );
				GetRenderNode().SetEmissiveTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_ASTEROID).m_hTextureEmissive );
				GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[ASTEROID_TB] );

				GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] );
				GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEX_POINT_LIGHT] );
				GetRenderNode().SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[TEX_POINT_LIGHT] );
				GetRenderNode().SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_POINT_LIGHT] );
				GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEX_POINT_LIGHT] );
				GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEX_POINT_LIGHT] );
				GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_POINT_LIGHT] );
				GetRenderNode().SetTextureUnitHandle2( CAssetManager::GetInstance()->m_vTextureUnit2Handles[TEX_POINT_LIGHT] );
				GetRenderNode().SetLightPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT], 0 );

				break;
			}
		case ET_PLANET:
			{
				glsLoadVector3(m_FocusPoint,0.0f,75.0f,0.0f);
				//glsLoadVector3(m_FocusPoint,_pos[0],_pos[1],_pos[2]);
				//Determine which Triangle Batch (shape) and Texture you want them to be
				GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_PLANET).m_DiffuseHandle );
				GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[PLANET_SHAPE_TB] );

				break;
			}
		case ET_SUN:
			{
				glsLoadVector3(m_FocusPoint,_pos[0],_pos[1],_pos[2]);
				//Determine which Triangle Batch (shape) and Texture you want them to be
				GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SUN).m_DiffuseHandle );
				GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[PLANET_SHAPE_TB] );
				break;
			}
	};

	m_rotationDeg = 0.0f;

	SaveOrientation();

	//ReOrient();

}

bool SpaceObjects::Update(float _fElapsedTime)
{

	RestoreOrientation();

	//m_rotationDeg += 0.5f * _fElapsedTime;

	switch(GetType())
	{
		case ET_ASTEROID:
			{
				m_rotationDeg += 0.5f * _fElapsedTime;

				//move_orientUpRightForward

				// MOVE
				Vec3f position;
				glsCopyVector3( position, GetRenderNode().GetPosition() );
				position[0] += GetRenderNode().GetForwardVector()[0] * 6.0f *_fElapsedTime;
				position[1] += GetRenderNode().GetForwardVector()[1] * 6.0f *_fElapsedTime;
				position[2] += GetRenderNode().GetForwardVector()[2] * 6.0f *_fElapsedTime;
				GetRenderNode().SetPosition( position );
				
				// ORIENT
				ReOrient();

				//SaveOrientation();
				//
				//Matrix3x3f rotMat;
				//glsRotationMatrix3x3(rotMat,m_rotationDeg,GetRenderNode().GetUpVector()[0],GetRenderNode().GetUpVector()[1],GetRenderNode().GetUpVector()[2]);
				//
				//Matrix3x3f objectMatrix;
				//
				//objectMatrix[0] = GetRenderNode().GetRightVector()[0];
				//objectMatrix[1] = GetRenderNode().GetRightVector()[1];
				//objectMatrix[2] = GetRenderNode().GetRightVector()[2];
				//objectMatrix[3] = GetRenderNode().GetUpVector()[0];
				//objectMatrix[4] = GetRenderNode().GetUpVector()[1];
				//objectMatrix[5] = GetRenderNode().GetUpVector()[2];
				//objectMatrix[6] = GetRenderNode().GetForwardVector()[0];
				//objectMatrix[7] = GetRenderNode().GetForwardVector()[1];
				//objectMatrix[8] = GetRenderNode().GetForwardVector()[2];
				//
				//glsMatrixMultiply3x3(objectMatrix,objectMatrix,rotMat);
				//
				//GetRenderNode().SetRightVector( &objectMatrix[0] );
				//GetRenderNode().SetUpVector( &objectMatrix[3] );
				//GetRenderNode().SetForwardVector( &objectMatrix[6] );

				Scale(2.0f);
				
				break;
			}
		case ET_PLANET:
			{

				m_rotationDeg += 0.0167f * _fElapsedTime;

				Vec3f position;
				glsCopyVector3( position, GetRenderNode().GetPosition() );
				position[0] += GetRenderNode().GetForwardVector()[0] * 6.0f *_fElapsedTime;
				position[1] += GetRenderNode().GetForwardVector()[1] * 6.0f *_fElapsedTime;
				position[2] += GetRenderNode().GetForwardVector()[2] * 6.0f *_fElapsedTime;
				GetRenderNode().SetPosition( position );

				ReOrient();

				SaveOrientation();

				Matrix3x3f rotMat;
				glsRotationMatrix3x3(rotMat,m_rotationDeg,GetRenderNode().GetForwardVector()[0],GetRenderNode().GetForwardVector()[1],GetRenderNode().GetForwardVector()[2]);

				Matrix3x3f objectMatrix;

				objectMatrix[0] = GetRenderNode().GetRightVector()[0];
				objectMatrix[1] = GetRenderNode().GetRightVector()[1];
				objectMatrix[2] = GetRenderNode().GetRightVector()[2];
				objectMatrix[3] = GetRenderNode().GetUpVector()[0];
				objectMatrix[4] = GetRenderNode().GetUpVector()[1];
				objectMatrix[5] = GetRenderNode().GetUpVector()[2];
				objectMatrix[6] = GetRenderNode().GetForwardVector()[0];
				objectMatrix[7] = GetRenderNode().GetForwardVector()[1];
				objectMatrix[8] = GetRenderNode().GetForwardVector()[2];

				glsMatrixMultiply3x3(objectMatrix,objectMatrix,rotMat);

				GetRenderNode().SetRightVector( &objectMatrix[0] );
				GetRenderNode().SetUpVector( &objectMatrix[3] );
				GetRenderNode().SetForwardVector( &objectMatrix[6] );

				Scale(2.5f);


				break;
			}
		case ET_SUN:
			{

				m_rotationDeg += 0.0167f * _fElapsedTime;

				Matrix3x3f rotMat;
				glsRotationMatrix3x3(rotMat,m_rotationDeg,GetRenderNode().GetRightVector()[0],GetRenderNode().GetRightVector()[1],GetRenderNode().GetRightVector()[2]);

				Matrix3x3f objectMatrix;

				objectMatrix[0] = GetRenderNode().GetRightVector()[0];
				objectMatrix[1] = GetRenderNode().GetRightVector()[1];
				objectMatrix[2] = GetRenderNode().GetRightVector()[2];
				objectMatrix[3] = GetRenderNode().GetUpVector()[0];
				objectMatrix[4] = GetRenderNode().GetUpVector()[1];
				objectMatrix[5] = GetRenderNode().GetUpVector()[2];
				objectMatrix[6] = GetRenderNode().GetForwardVector()[0];
				objectMatrix[7] = GetRenderNode().GetForwardVector()[1];
				objectMatrix[8] = GetRenderNode().GetForwardVector()[2];

				glsMatrixMultiply3x3(objectMatrix,objectMatrix,rotMat);

				GetRenderNode().SetRightVector( &objectMatrix[0] );
				GetRenderNode().SetUpVector( &objectMatrix[3] );
				GetRenderNode().SetForwardVector( &objectMatrix[6] );

				break;
			}
	};

	return true;
}

void SpaceObjects::Render()
{

}

void SpaceObjects::SetVelocity(Vec3f _newVel)
{
	glsCopyVector3(m_Velocity,_newVel);
}

void SpaceObjects::SetAcceleration(Vec3f _newAcc)
{
	glsCopyVector3(m_Acceleration,_newAcc);
}

void SpaceObjects::ReOrient()
{

	//reOrient based on the focus point

	Vec3f up, right, forward;
	//set up  to the vector connecting the object to the focus point.
	glsSubtractVectors3(up,m_FocusPoint,GetRenderNode().GetPosition());
	//find the magnitude of this vector.  This should be the distance from the focus point
	float dist = glsGetVectorLength3(up);
	//Normalize the up
	glsNormalizeVector3(up);
	GetRenderNode().SetUpVector( up );

	//Error check to make sure that the distance the object is from your point is equivelent to what you want.
	float distanceWanted;
	switch(this->GetType())
	{
	case ET_ASTEROID:
		{
			distanceWanted = 150.0f;
			break;
		}
	case ET_PLANET:
		{
			distanceWanted = 500.0f;
			break;
		}

	};

	//The difference in your distances
	float difference = 0.0f;

	//find the differences
	if( dist < distanceWanted )
	{
		difference = dist - distanceWanted;	  //100 - 130 = -30
	}
	else if(dist > distanceWanted)
	{
		difference = dist - distanceWanted;   //130 - 100 = 30
	}

	//correct if the values are not the same.
	if(difference != 0)
	{
		Vec3f pos, newPos;
		Vec3f move;
		glsCopyVector3( pos, GetRenderNode().GetPosition() );
		glsCopyVector3( move, up );
		glsScaleVector3( move, difference );
		glsAddVectors3( newPos, pos, move );
		GetRenderNode().SetPosition( newPos );
	}

	glsCrossProduct3(forward, GetRenderNode().GetRightVector(), up);
	glsNormalizeVector3(forward);
	GetRenderNode().SetForwardVector( forward );

	glsCrossProduct3(right, up, forward);
	glsNormalizeVector3(right);
	GetRenderNode().SetRightVector( right );
}

void SpaceObjects::Scale(float _size)
{
	Vec3f tempUp, tempRight, tempForward;
	glsCopyVector3(tempUp, GetRenderNode().GetUpVector());
	glsCopyVector3(tempRight, GetRenderNode().GetRightVector());
	glsCopyVector3(tempForward, GetRenderNode().GetForwardVector());

	// Default Orientation.
	glsScaleVector3(tempUp, _size);
	glsScaleVector3(tempRight, _size);
	glsScaleVector3(tempForward, _size);

	GetRenderNode().SetUpVector(tempUp);
	GetRenderNode().SetRightVector(tempRight);
	GetRenderNode().SetForwardVector(tempForward);
}

void SpaceObjects::SaveOrientation()
{
	glsCopyVector3(m_prevForward,GetRenderNode().GetForwardVector());
	glsCopyVector3(m_prevUp,GetRenderNode().GetUpVector());
	glsCopyVector3(m_prevRight,GetRenderNode().GetRightVector());
}

void SpaceObjects::RestoreOrientation()
{
	GetRenderNode().SetForwardVector(m_prevForward);
	GetRenderNode().SetUpVector(m_prevUp);
	GetRenderNode().SetRightVector(m_prevRight);
}

void SpaceObjects::ReOrientRotation()
{
	Vec3f up, right;
	glsSubtractVectors3(up,m_FocusPoint,GetRenderNode().GetPosition());
	glsNormalizeVector3(up);
	GetRenderNode().SetUpVector( up );

	glsCrossProduct3(right, up, GetRenderNode().GetForwardVector());
	glsNormalizeVector3(right);
	GetRenderNode().SetRightVector( right );
}

