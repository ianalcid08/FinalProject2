#include "../Header Files/StdAfx.h"
#include "../Header Files/AnimationManager.h"
#include "../Header Files/RenderNode.h"

CAnimationManager::CAnimationManager(void)
{
	m_NextAnim = NULL;
	m_CurAnim = NULL;
	m_Process.m_CurTime = 0.0f;
}

CAnimationManager::~CAnimationManager(void)
{
}

void CAnimationManager::Update(float _ElapsedTime, CRenderNode* _RenderNode)
{
	if(m_CurAnim == NULL)
		return; 

	int x = 0;

	float ElapsedTime = _ElapsedTime;// / 10.0f;
	if(m_NextAnim != NULL)
	{
		if(m_Process.m_TransitionTime <= m_Process.m_CurTime)
		{
			m_CurAnim = m_NextAnim;
			m_Process.m_CurProcess = m_Process.m_NextProcess;
			m_NextAnim = NULL;
		}
		
		m_Process.m_CurProcess.AddTime(ElapsedTime);
		m_Process.m_CurProcess.Process(ElapsedTime);

		m_Process.m_NextProcess.AddTime(ElapsedTime);
		m_Process.m_NextProcess.Process(ElapsedTime);

		m_Process.m_CurTransforms.resize(m_Process.m_CurProcess.GetCurrentKeyFrame().size());
		m_Process.m_CurTime += ElapsedTime;

		for(unsigned int i = 0; i < m_Process.m_CurTransforms.size(); i++)
			Interpolate(m_Process.m_CurTransforms[i].m, m_Process.m_CurProcess.GetCurrentKeyFrame()[i].m, m_Process.m_NextProcess.GetCurrentKeyFrame()[i].m, m_Process.m_CurTime / m_Process.m_TransitionTime);
	}
	else
	{
		
		m_Process.m_CurProcess.AddTime(ElapsedTime);
		m_Process.m_CurProcess.Process(ElapsedTime);

		if( m_Process.m_CurTransforms.size() != m_Process.m_CurProcess.GetCurrentKeyFrame().size() )
			m_Process.m_CurTransforms.resize( m_Process.m_CurProcess.GetCurrentKeyFrame().size() );

		for(unsigned int i = 0; i < m_Process.m_CurProcess.GetCurrentKeyFrame().size(); ++i)
			m_Process.m_CurTransforms[i] = m_Process.m_CurProcess.GetCurrentKeyFrame()[i];
	}

	//_RenderNode->m_Mesh->ApplySkinning(m_Process.m_BlendedFrames, m_CurAnim->GetJoints());

	for(unsigned int i = 0; i < 30; i++)
	{
		if(i <= m_Process.m_CurTransforms.size()-1)
		{
			Matrix4x4f mInv;
			glsInvertMatrix4x4(mInv, m_CurAnim->GetJoints()[i].m_KeyFrames[0].worldXForm);
			glsMatrixMultiply4x4(m_SkinningMatrix[i], m_Process.m_CurTransforms[i].m, mInv);
		}
		else
			glsLoadIdentity4x4(m_SkinningMatrix[i]);
	}

	_RenderNode->SetSkinningMatrix( m_SkinningMatrix );


	////Adding triangles to renderNode	
	//_RenderNode->triangleBatch->BeginMesh(_RenderNode->m_Mesh->GetTriangles().size() * 3);
	//for(size_t i = 0; i < _RenderNode->m_Mesh->GetTriangles().size(); i++)
	//{
	//	Vec3f vec[3];
	//	Vec3f norm[3];
	//	Vec2f UV[3];

	//	// loops throught the three verts in the triangle
	//	for(int curVert = 0; curVert < 3; curVert++)
	//	{
	//		// loops through for the size of the vector
	//		for(int curPos = 0; curPos < 3; curPos++)
	//		{
	//			vec[curVert][curPos] = _RenderNode->m_Mesh->GetSkinVerts()[_RenderNode->m_Mesh->GetTriangles()[i].Indicies[curVert]].v[curPos];
	//			norm[curVert][curPos] = _RenderNode->m_Mesh->GetNormals()[_RenderNode->m_Mesh->GetTriangles()[i].Indicies[curVert]].v[curPos];
	//			
	//			if( curPos < 2 )
	//				UV[curVert][curPos] = _RenderNode->m_Mesh->GetUVs()[_RenderNode->m_Mesh->GetTriangles()[i].Indicies[curVert]].v[curPos];
	//		}
	//	}

	//	// adds the triangle to the batch
	//	_RenderNode->triangleBatch->AddTriangle(vec, norm, UV);
	//}
	//_RenderNode->triangleBatch->End();
}

void CAnimationManager::SwitchAnimation(CAnimation* _Value, float _Transition)
{
	if(m_CurAnim == NULL)
	{
		m_CurAnim = _Value;
		m_Process.m_CurProcess.SetAnimation(_Value);
		//m_Process.m_NextProcess.SetAnimation(m_CurAnim);
	}
	else
	{	
		if( m_NextAnim != NULL )
		{
			if( m_CurAnim->GetKey() == _Value->GetKey() || m_NextAnim->GetKey() == _Value->GetKey())
				return;
		}

		m_NextAnim = _Value; 
		m_Process.m_TransitionTime = _Transition;
		//m_Process.m_CurProcess.SetAnimation(m_CurAnim);
		m_Process.m_NextProcess.SetAnimation(m_NextAnim);
		m_Process.m_CurTime = 0.0f;
	}
}