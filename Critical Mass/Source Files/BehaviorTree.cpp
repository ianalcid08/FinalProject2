/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : BehaviorTree.cpp
Author : Evan Wright

Description : Traverses its child nodes in order, left to right. The condition to continue is stored
			  inside of whatever the root is classifyed as.

Created :  08/06/2013
Modified : 08/09/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/BehaviorTree.h"
#include "../Header Files/BehaviorNode.h"
#include "../Header Files/Enemy.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
Parameters:
[in] _fileName - file to load to create the tree.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorTree::BehaviorTree(const char* _fileName)
	: m_Root(nullptr), m_TreeName(nullptr)
{
	if(_fileName != nullptr)
		LoadTree(_fileName);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorTree::~BehaviorTree(void)
{
	Release();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void BehaviorTree::Release(void)
{
	if(m_Root!=nullptr)
		m_Root->Release();
	SAFE_DELETE(m_Root);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Update the sent in enemy based on this behavior tree.
Parameters:
[in] enemy - the enemy to use the tree on.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void BehaviorTree::Update(CEnemy* _enemy, float _elapsedTime)
{
	// Set the enemy to the current enemy.
	// PARANOIA CHECK!
	if(m_Root != nullptr && _enemy != nullptr)
	{
		if(m_Root->Behave(_enemy,_elapsedTime) == Error)
			int x = 0; // ERROR RETURN MESSAGE HERE
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Loads in a tree, from the passed in file.
Parameters:
[in] _fileName - file to load to create the tree.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void BehaviorTree::LoadTree(const char* _fileName)
{

}

void BehaviorTree::Reset(void)
{
	if(m_Root != nullptr)
		m_Root->Reset();
}