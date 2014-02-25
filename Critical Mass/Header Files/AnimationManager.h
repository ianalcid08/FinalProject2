#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "AnimationProcessor.h"
#include "Definitions.h"

class CRenderNode;

class CAnimationBlender
{
	friend class CAnimationManager;

	float m_TransitionTime;
	float m_CurTime;

	CInterpolator m_CurProcess;
	CInterpolator m_NextProcess;

	std::vector<Mat4F> m_CurTransforms;

public:

	// Setters
	void SetTransitionTime( float _Time ) { m_TransitionTime = _Time; }
	void SetCurrentTime( float _Time ) { m_CurTime = _Time; }

	// Getters
	const std::vector<Mat4F> &GetTransforms(void) { return m_CurTransforms; }
	float GetDissolveFactor(void) { return m_TransitionTime / m_CurTime; }


	void AddCurrentTime( float _Time ) { m_CurTime += _Time; }

};

class CAnimationManager
{
	//CAnimation*		m_Animations[AM_MAX_ANIMS];
	CAnimation*			m_CurAnim;
	CAnimation*			m_NextAnim;
	Matrix4x4f			m_SkinningMatrix[30];

	CAnimationBlender	m_Process;

public:

	CAnimationManager(void);
	~CAnimationManager(void);
	
	// Sets the current animation if there isn't one already, otherwise it sets the next animation to the one that is passed in.
	void SwitchAnimation(CAnimation* _Value, float _Transition);
	void Update(float _ElapsedTime, CRenderNode* _RenderNode);
};


#endif