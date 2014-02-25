/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : CinematicSystem.h
Author :  Evan Wright

Description : A system that contains a vector of positions to be at and a places, or 
			  entities, to look at. This system will then lerp between those vectors 
			  allowing for, well, cinematics! This will make life a lot easier when trying 
			  to create a cinematic in the game.

Created :  10/09/2013
Modified : 10/09/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _CINEMATIC_SYSTEM_H_
#define _CINEMATIC_SYSTEM_H_

#include <iostream>
#include <vector>
using std::vector;

class CBaseEntity;
class CCamera;
class CCinematicSystem
{
	struct CinemaNode
	{
		float position[3];
		float time;
		float lookat_pos[3];
		float right[3];
		float up[3];
		CBaseEntity* lookat_entity;
		CinemaNode(void) {}
		~CinemaNode(void) { lookat_entity = nullptr; }

		void Init(float _pos[3], float _time, float _look_pos[3], float _right[3], float _up[3], CBaseEntity* _look_entity = nullptr)
		{
			memcpy_s(position,sizeof(float)*3,_pos,sizeof(float)*3);
			time = _time;
			memcpy_s(lookat_pos,sizeof(float)*3,_look_pos,sizeof(float)*3);
			memcpy_s(right,sizeof(float)*3,_right,sizeof(float)*3);
			memcpy_s(up,sizeof(float)*3,_up,sizeof(float)*3);
			lookat_entity	= _look_entity;
		}
		CinemaNode& operator=(const CinemaNode _node)
		{
			memcpy_s(position,sizeof(float)*3,_node.position,sizeof(float)*3);
			time = _node.time;
			memcpy_s(lookat_pos,sizeof(float)*3,_node.lookat_pos,sizeof(float)*3);
			memcpy_s(right,sizeof(float)*3,_node.right,sizeof(float)*3);
			memcpy_s(up,sizeof(float)*3,_node.up,sizeof(float)*3);
			lookat_entity = _node.lookat_entity;
			
			return *this;
		}

		void GetLookAt(float* _lookat);
	};
	
	float				m_CurTime;
	vector<CinemaNode>	m_Nodes;
	CCamera*			m_Camera;
	unsigned int		m_CurIndex;

	bool				m_Looping;

	void Lerp(float* v0, float v1[3] , float v2[3], float step)
	{
		v0[0] = v1[0] + (v2[0] - v1[0]) * step;
		v0[1] = v1[1] + (v2[1] - v1[1]) * step;
		v0[2] = v1[2] + (v2[2] - v1[2]) * step;
	}
	void GetOrientation(float* _orient, float* _up, float* _right, float* _forward);
	
	bool Finished(void) {return (!m_Looping) ? m_CurIndex >= m_Nodes.size() : false;}

public:
	CCinematicSystem(CCamera* _camera, bool _loop = false);
	~CCinematicSystem(void);
	
	bool Update(float _deltaTime);

	void AddNode(float _pos[3], float _time, float _look_pos[3], CBaseEntity* _look_entity = nullptr);
	void AddNode(CinemaNode _node);

	void Clear(void);

	bool InProgress(void) {return (!m_Looping) ? m_CurIndex < m_Nodes.size() : true;}
};

#endif