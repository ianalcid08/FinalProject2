/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : CinematicSystem.cpp
Author :  Evan Wright

Description : A system that contains a vector of positions to be at and a places, or 
			  entities, to look at. This system will then lerp between those vectors 
			  allowing for, well, cinematics! This will make life a lot easier when trying 
			  to create a cinematic in the game.

Created :  10/09/2013
Modified : 10/09/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/CinematicSystem.h"
#include "../Header Files/BaseEntity.h"
#include "../Header Files/Camera.h"

void CCinematicSystem::CinemaNode::GetLookAt(float* _lookat)
{
	if(lookat_entity)
	{
		float look[3] = {0.0f,0.0f,0.0f};
		glsSubtractVectors3(look,lookat_entity->GetRenderNode().GetPosition(),position);
		glsNormalizeVector3(look);
		memcpy_s(_lookat,sizeof(float)*3,look,sizeof(float)*3);
	}
	else
		memcpy_s(_lookat,sizeof(float)*3,lookat_pos,sizeof(float)*3);
}

void CCinematicSystem::GetOrientation(float* _orient, float* _up, float* _right, float* _forward)
{
	glsCrossProduct3( _forward, _right, _up );
	glsNormalizeVector3( _forward );

	glsCrossProduct3( _right, _up, _forward );
	glsNormalizeVector3( _right );


	for(int i = 0; i < 3; ++i)
		_orient[i] = _right[i];
	for(int i = 0; i < 3; ++i)
		_orient[i+3] = _up[i];
	for(int i = 0; i < 3; ++i)
		_orient[i+6] = _forward[i];
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CCinematicSystem::CCinematicSystem(CCamera* _camera, bool _loop)
{
	m_Camera = _camera;
	m_Looping = _loop;
	m_CurIndex = 0;
	m_CurTime = 0.0f;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CCinematicSystem::~CCinematicSystem(void)
{
	m_Camera = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the cinematic frames!
Parameters:
[in] _elapsedTime - the time passed since the last frame.
[ret] bool - true if the cinematic has completed. (never true if looping)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool CCinematicSystem::Update(float _deltaTime)
{
	if(m_Nodes.size() == 0 || m_Nodes.size() <= m_CurIndex)
		return true;

	m_CurTime += _deltaTime;
	CinemaNode currNode = m_Nodes[m_CurIndex];
	if(currNode.time <= m_CurTime)
	{
		m_CurTime = 0.0f;
		m_CurIndex++;
		if(m_Nodes.size() <= m_CurIndex && m_Looping)
			m_CurIndex = 0;
		else if(m_Nodes.size() <= m_CurIndex)
			return true;

		currNode = m_Nodes[m_CurIndex];
	}
	
	unsigned int nextIndex = m_CurIndex + 1;
	if(m_Nodes.size() <= nextIndex && m_Looping)
		nextIndex = 0;
	else if(m_Nodes.size() <= nextIndex)
		nextIndex = m_CurIndex;

	CinemaNode nextNode = m_Nodes[nextIndex];
	float newPos[3] = {0.0f,0.0f,0.0f};
	Lerp(newPos,currNode.position,nextNode.position,m_CurTime/currNode.time);

	float currLookAt[3], nextLookAt[3], newLookat[3] = {0.0f,0.0f,0.0f};
	currNode.GetLookAt(currLookAt);
	nextNode.GetLookAt(nextLookAt);

	Lerp(newLookat,	currLookAt,		nextLookAt,		m_CurTime/currNode.time);	
	glsNormalizeVector3(newLookat);

	m_Camera->SetPosition(newPos);
	m_Camera->SetLookAt(newLookat);

	return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Adds a cinema node to the nodes list. This function is if you want to streamline the
		 add function, to not have to create and init a cinema node. Usefull for MANY hand made 
		 nodes.
Parameters:
[in] _pos - the position the node should be at.
[in] _time - how long that node should last.
[in] _look_pos - direction to look towards
[in] _look_entity - the entity to lookat. (OPTIONAL)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCinematicSystem::AddNode(float _pos[3], float _time, float _look_pos[3], CBaseEntity* _look_entity)
{
	Matrix3x3f orient;
	memcpy_s(orient,sizeof(Matrix3x3f),m_Camera->GetOrientation(),sizeof(Matrix3x3f));

	Vec3f right, up;
	for(int i = 0; i < 3; ++i)
		right[i] = orient[i];
	for(int i = 0; i < 3; ++i)
		up[i] = orient[i+3];

	glsCrossProduct3( up, _look_pos, right );
	glsNormalizeVector3( up );

	glsCrossProduct3( right, up, _look_pos );
	glsNormalizeVector3( right );

	CinemaNode addNode;
	addNode.Init(_pos,_time,_look_pos,up,right,_look_entity);
	m_Nodes.push_back(addNode);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Adds a cinema node to the nodes list.
Parameters:
[in] _elapsedTime - the time passed since the last frame.
[ret] bool - true if the cinematic has completed. (never true if looping)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCinematicSystem::AddNode(CinemaNode _node)
{
	m_Nodes.push_back(_node);
}

void CCinematicSystem::Clear(void)
{
	m_Nodes.clear();
	m_CurTime = 0.0f;
	m_CurIndex = 0;
}