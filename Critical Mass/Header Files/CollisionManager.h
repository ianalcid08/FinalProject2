/*/////////////////////////////////////////////////////////
/Filename:		CollisionManager.h
/Author:		Tom Stefl & Ryan Cartier
/Date:			8/4/2013
/Last Modified: 8/4/2013
/*/////////////////////////////////////////////////////////
#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

//	Includes //
//#include <utility>
#include <vector>
class IEntity;

class CollisionManager
{
private:
	//std::map<IEntity*, IEntity*> collisionsMap;
	std::vector<IEntity*>       m_Collisions;

public:

	CollisionManager(void);
	~CollisionManager(void);
	void Execute();
	void AddCollision(IEntity* _a, IEntity* _b);
	bool IsContained(IEntity* _a);
};
#endif

