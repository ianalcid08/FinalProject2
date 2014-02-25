#include "../Header Files/stdafx.h"
#include "../Header Files/AnimationProcessor.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/BaseEntity.h"

#include <fstream>
using namespace std;

void CAnimation::LoadAnimFromFile(char* _fileName, unsigned int _key, bool _Repeat)
{
	fstream animFile(_fileName, ios_base::in | ios_base::binary);
	animFile.seekg(0, ios_base::beg);

	m_Key = _key;
	m_Repeat = _Repeat;

	if(!animFile.is_open())
		return;

	// Total duration of the animation
	animFile.read((char*)&m_Duration, sizeof(double));
	size_t boneCount = 0;
	// Used to parse through the extra data from the x64 binary file
	float extraData = 0.0f;

	animFile.read((char*)&boneCount, sizeof(size_t));
	animFile.read((char*)&extraData, sizeof(float));

	double data = 0.0;
	// Looping through the bone count
	for(size_t i = 0; i < boneCount; i++)
	{
		tKeyJoints tmpJoint;
		size_t keyCount = 0;
		animFile.read((char*)&keyCount, sizeof(size_t));
		animFile.read((char*)&extraData, sizeof(float));

 		for(size_t curKey = 0; curKey < keyCount; curKey++)
		{
			tKeyFrame tmpKeyframe;
			animFile.read((char*)&tmpKeyframe.m_TimeInSeconds, sizeof(float));
			//animFile.read((char*)&extraData, sizeof(float));

			// Reads the local transform

			//x axis
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[0] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[1] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[2] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[3] = (float)data;

			//y axis
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[4] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[5] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[6] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[7] = (float)data;

			//z axis
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[8] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[9] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[10] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[11] = (float)data;

			//w axis
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[12] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[13] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[14] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.localXForm[15] = (float)data;

			// Reads the world transform

			//x axis
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[0] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[1] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[2] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[3] = (float)data;

			//y axis
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[4] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[5] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[6] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[7] = (float)data;

			//z axis
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[8] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[9] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[10] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[11] = (float)data;

			//w axis
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[12] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[13] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[14] = (float)data;
			animFile.read((char*)&data, sizeof(double));
			tmpKeyframe.worldXForm[15] = (float)data;

			tmpJoint.m_KeyFrames.push_back(tmpKeyframe);
		}

		m_Joints.push_back(tmpJoint);
	}

	animFile.close();
}

void CInterpolator::Process(float _ElapsedTime)
{
	m_CurKeyFrames.clear();
	if(!m_Animation)
		return;

	for(unsigned int i = 0; i < m_Animation->GetJoints().size(); i++)
	{
		float duration = (float)m_Animation->GetDuration();
		float tweenTime;
		tKeyFrame* nextKeyframe, *curFrame;
		
		if( m_CurTime >= duration && m_Animation->GetRepeat() )
			m_CurTime -= duration;
		else if( m_CurTime >= duration && !m_Animation->GetRepeat() )
		{
			curFrame = &m_Animation->GetJoints()[i].m_KeyFrames[m_Animation->GetJoints()[i].m_KeyFrames.size()-1];
			Mat4F newMatrix;
			glsCopyMatrix4x4(newMatrix.m, curFrame->worldXForm);
			m_CurKeyFrames.push_back(newMatrix);
			continue;
		}

		if( m_CurTime < 0 )
			m_CurTime += duration;

		for(unsigned int x = 1; x < m_Animation->GetJoints()[i].m_KeyFrames.size(); x++)
		{
			curFrame = &m_Animation->GetJoints()[i].m_KeyFrames[x];
			if(x == m_Animation->GetJoints()[i].m_KeyFrames.size()-1)
			{
				tweenTime = duration - m_Animation->GetJoints()[i].m_KeyFrames[x].m_TimeInSeconds;
				nextKeyframe = &m_Animation->GetJoints()[i].m_KeyFrames[0];
			}
			else
			{
				tweenTime = m_Animation->GetJoints()[i].m_KeyFrames[x+1].m_TimeInSeconds - m_Animation->GetJoints()[i].m_KeyFrames[x].m_TimeInSeconds;
				nextKeyframe = &m_Animation->GetJoints()[i].m_KeyFrames[x+1];
			}

			if( m_CurTime < tweenTime + m_Animation->GetJoints()[i].m_KeyFrames[x].m_TimeInSeconds )
				break;
		}

		Mat4F newMatrix;
		Interpolate(newMatrix.m, curFrame->worldXForm, nextKeyframe->worldXForm, (m_CurTime - curFrame->m_TimeInSeconds) / tweenTime);
		m_CurKeyFrames.push_back(newMatrix);
	}

}

void Interpolate(float* _Out, float* _MatA, float* _MatB, float _lamda)
{											
	/*glsCopyMatrix4x4(_Out, _MatA);
		return;*/

	//_MatA[12] + ((_MatB[12] -_MatA[12])*_lamda);
	for(unsigned int x = 0; x < 4; x++)
	{
		for(unsigned int y = 0; y < 4; y++)
		{
			_Out[x*4 + y] = (_MatA[x *4 + y] * (1 - _lamda)) + (_MatB[x*4 + y] *  _lamda);
		}
	}

	//Matrix4x4f iMat;
	//glsCopyMatrix4x4(iMat, _MatA);
	//Matrix4x4f result;
	//glsCopyMatrix4x4(result, _MatB);

	//// Inverse of MatrixA
	//glsInvertMatrix4x4(iMat, iMat);

	//// Remove MatrixA's transformation from MatrixB
	//glsMatrixMultiply4x4(result, result, iMat);

	//// iMat is now the intermediary transformation from MatrixA to MatrixB
	//// ie: iMat * MatrixA = MatrixB
	//glsCopyMatrix4x4(iMat, result);

	//// The trace of our matrix
	//float trace = 1.0f + iMat[0] + iMat[5] + iMat[10];

	//float quatResult[4];

	//// Calculate the quaternion of iMat
	//// If trace is greater than 0, but consider small values that
	//// might result in 0 when operated upon due to floating point error
	//if( trace > 0.00000001f )
	//{
	//	float S = (float)sqrt(trace)*2.0f;
	//	quatResult[0] = (iMat[9] - iMat[6]) / S;
	//	quatResult[1] = (iMat[2] - iMat[8]) / S;
	//	quatResult[2] = (iMat[4] - iMat[1]) / S;
	//	quatResult[3] = 0.25f * S;
	//}
	//else
	//{
	//	if( iMat[0] > iMat[5] && iMat[0] > iMat[10] )
	//	{
	//		float S = sqrt( 1.0f + iMat[0] - iMat[5] - iMat[10] ) * 2.0f;
	//		quatResult[0] = 0.25f * S;
	//		quatResult[1] = (iMat[4] + iMat[1]) / S;
	//		quatResult[2] = (iMat[2] + iMat[8]) / S;
	//		quatResult[3] = (iMat[9] - iMat[6]) / S;
	//	}
	//	else if( iMat[5] > iMat[10] )
	//	{
	//		float S = sqrt( 1.0f + iMat[5] - iMat[0] - iMat[10] ) * 2.0f;
	//		quatResult[0] = (iMat[4] + iMat[1]) / S;
	//		quatResult[1] = 0.25f * S;
	//		quatResult[2] = (iMat[9] + iMat[6]) / S;
	//		quatResult[3] = (iMat[2] - iMat[8]) / S;
	//	}
	//	else
	//	{
	//		float S = sqrt( 1.0f + iMat[10] - iMat[0] - iMat[5] ) * 2.0f;
	//		quatResult[0] = (iMat[2] + iMat[8]) / S;
	//		quatResult[1] = (iMat[9] + iMat[6]) / S;
	//		quatResult[2] = 0.25f * S;
	//		quatResult[3] = (iMat[4] - iMat[1]) / S;
	//	}
	//}

	//// Get the magnitude of our quaternion
	//float quatMagnitude = sqrt( quatResult[0]*quatResult[0] + quatResult[1]*quatResult[1] + quatResult[2]*quatResult[2] + quatResult[3]*quatResult[3] );

	//// Normalize our quaternion
	//float quatNormalized[4] = { quatResult[0]/quatMagnitude, quatResult[1]/quatMagnitude, quatResult[2]/quatMagnitude, quatResult[3]/quatMagnitude }; 

	//// Calculate the angles relevant to our quaternion
	//float cos_a = quatNormalized[3];
	//float angle = acos( cos_a ) * 2;
	//float sin_a = sqrt( 1.0f - cos_a * cos_a );
	//
	//// If there was no rotation between matrices, calculation
	//// of the rotation matrix will end badly. So just do the linear
	//// interpolation of the translation component and return
	//if( angle == 0.0 )
	//{
	//	glsCopyMatrix4x4(result, _MatA);

	//	result[12] = _MatA[12] + ((_MatB[12] -_MatA[12])*_lamda);
	//	result[13] = _MatA[13] + ((_MatB[13] -_MatA[13])*_lamda);
	//	result[14] = _MatA[14] + ((_MatB[14] -_MatA[14])*_lamda);
	//	glsCopyMatrix4x4(_Out, result);
	//	return;
	//}

	//// Our axis of abitrary rotation
	//Vec3f axis;

	//if( fabs( sin_a ) < 0.0005 )
	//	sin_a = 1;

	//axis[0] = quatNormalized[0] / sin_a;
	//axis[1] = quatNormalized[1] / sin_a;
	//axis[2] = quatNormalized[2] / sin_a;

	//// Get the portion of the angle to rotate by
	//angle *= _lamda;

	//glsNormalizeVector3(axis);

	//// Calculate the quaternion for our new (partial) angle of rotation
	//sin_a = sin( angle / 2 );
	//cos_a = cos( angle / 2 );
	//quatNormalized[0] = axis[0] * sin_a;
	//quatNormalized[1] = axis[1] * sin_a;
	//quatNormalized[2] = axis[2] * sin_a;
	//quatNormalized[3] = cos_a;

	//quatMagnitude = sqrt( quatNormalized[0]*quatNormalized[0] + quatNormalized[1]*quatNormalized[1] + quatNormalized[2]*quatNormalized[2] + quatNormalized[3]*quatNormalized[3] );		
	//quatNormalized[0] /= quatMagnitude;
	//quatNormalized[1] /= quatMagnitude;
	//quatNormalized[2] /= quatMagnitude;
	//quatNormalized[3] /= quatMagnitude;

	//// Calculate our partial rotation matrix
	//float xx      = quatNormalized[0] * quatNormalized[0];
	//float xy      = quatNormalized[0] * quatNormalized[1];
	//float xz      = quatNormalized[0] * quatNormalized[2];
	//float xw      = quatNormalized[0] * quatNormalized[3];
	//float yy      = quatNormalized[1] * quatNormalized[1];
	//float yz      = quatNormalized[1] * quatNormalized[2];
	//float yw      = quatNormalized[1] * quatNormalized[3];
	//float zz      = quatNormalized[2] * quatNormalized[2];
	//float zw      = quatNormalized[2] * quatNormalized[3];

	//result[0]  = 1 - 2 * ( yy + zz );
	//result[1]  =     2 * ( xy - zw );
	//result[2]  =     2 * ( xz + yw );
	//result[4]  =     2 * ( xy + zw );
	//result[5]  = 1 - 2 * ( xx + zz );
	//result[6]  =     2 * ( yz - xw );
	//result[8]  =     2 * ( xz - yw );
	//result[9]  =     2 * ( yz + xw );
	//result[10] = 1 - 2 * ( xx + yy );
	//result[3]  = result[7] = result[11] = result[12] = result[13] = result[14] = 0;
	//result[15] = 1;

	//// Combine our partial rotation with MatrixA
	//glsMatrixMultiply4x4(result, result, _MatA);

	//// Linear interpolation of the translation components of the matrices
	//result[12] = _MatA[12] + ((_MatB[12] -_MatA[12])*_lamda);
	//result[13] = _MatA[13] + ((_MatB[13] -_MatA[13])*_lamda);
	//result[14] = _MatA[14] + ((_MatB[14] -_MatA[14])*_lamda);
	//	
	//glsCopyMatrix4x4(_Out, result);
	//return;
}

void CInterpolator::StepProcess(unsigned int _KeyFrame)
{
	/*m_CurKeyFrames.clear();
	tKeyFrame *curFrame;
	for(unsigned int i = 0; i < m_Animation->GetJoints().size(); i++)
	{
		curFrame = &m_Animation->GetJoints()[i].m_KeyFrames[_KeyFrame];
		Mat4F m;
		m.m = curFrame->worldXForm;
		m_CurKeyFrames.push_back(m);
	}*/
}