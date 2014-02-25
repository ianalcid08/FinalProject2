#ifndef _ANIMSTATE
#define	_ANIMSTATE

#include "IState.h"
#include "RenderNode.h"
#include "AnimationProcessor.h"
#include "GLTriangleBatch.h"
#include "Camera.h"
#include <vector>

class CRenderer;
class CObjectFactory;
class IEntity;
class CObjectManager;
class StateManager;

class CAnimDebugState : public IState
{
	CRenderer					*m_Renderer;
	CObjectFactory				*m_OF;
	CObjectManager				*m_OM;
	StateManager				*m_SM;

	CCamera						 m_Camera;

	float						 m_ElapsedTime;
	bool						 m_RenderBones;
	unsigned int				 m_Keyframe;

	std::vector<CRenderNode*>	 m_JointNodes;
	IEntity*					 m_Animation;

	void ChangeKeyframe(unsigned int _Index);

public:

	CAnimDebugState();
	~CAnimDebugState();

	static CAnimDebugState* GetInstance(void);

	void Init(CGame* _game);
	void Shutdown();
	bool Input( CInput& _input );
	void Update(float _fElapsedTime);
	void Render();
	void Enter();
	void Exit(); 
};

#endif