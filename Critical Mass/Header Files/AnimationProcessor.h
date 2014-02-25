#ifndef _AnimSYSTEM
#define _AnimSYSTEM

#include "glsMath.h"
#include "Mesh.h"
#include <vector>

struct Mat4F
{
	Matrix4x4f m;

	Mat4F& operator=(const Mat4F mat)
	{
		for(int x = 0; x < 16; ++x)
		{
			m[x] = mat.m[x];
		}
		
		return *this;
	}
};

class CAnimation 
{
	friend class CInterpolator;

	unsigned int m_Key;
	bool m_Repeat;
	double m_Duration;
	std::vector<tKeyJoints> m_Joints;

public:

	CAnimation(void){}
	~CAnimation(void){}

	void LoadAnimFromFile(char* _fileName, unsigned int _key, bool _Repeat);

	std::vector<tKeyJoints> &GetJoints(void) { return m_Joints; }
	double GetDuration(void) { return m_Duration; }

	unsigned int GetKey(void) { return m_Key; }
	bool GetRepeat(void) { return m_Repeat; }
};

class CInterpolator
{
	CAnimation				*m_Animation;

	std::vector<Mat4F>	m_CurKeyFrames;
	float					m_CurTime;
	unsigned int			m_Keyframe;

public:

	CInterpolator(void) : m_Animation(0){}
	~CInterpolator(void){}

	void SetAnimation(CAnimation *inAnimation) { m_Animation = inAnimation; }
	void AddTime(float _t) {m_CurTime += _t;}
	void SetTime(float _t) {m_CurTime = _t;}
	void Process(float _ElapsedTime);
	void StepProcess(unsigned int _KeyFrame);
	std::vector<Mat4F>& GetCurrentKeyFrame(void){return m_CurKeyFrames;}
	float GetCurTime(void) { return m_CurTime; }
};

void Interpolate(float* _Out, float* _MatA, float* _MatB, float _lamda);

#endif