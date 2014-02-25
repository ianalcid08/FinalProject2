/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : BehaviorTree.h
Author : Evan Wright

Description : Traverses its child nodes in order, left to right. The condition to continue is stored
			  inside of whatever the root is classifyed as.

Created :  08/06/2013
Modified : 08/09/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _BEHAVIOR_TREE_H_
#define _BEHAVIOR_TREE_H_

#include "Definitions.h"

class CEnemy;
class BehaviorNode;
class BehaviorTree
{
	BehaviorNode*	m_Root;
	char*			m_TreeName;

	void LoadTree(const char* _fileName);
public:
	BehaviorTree(const char* _fileName = nullptr);
	~BehaviorTree(void);

	void Release(void);
	void Update(CEnemy* _enemy, float _elapsedTime);

	void Reset(void);
	void SetRoot(BehaviorNode* _root) { m_Root = _root; }
};

#endif