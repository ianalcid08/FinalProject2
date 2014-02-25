#ifndef _COLLENT
#define _COLLENT

#include "Definitions.h"

class CCollisionEntity
{
protected:

	eCollisionEntityTypes m_eBVType;

public:

	CCollisionEntity() { }
	virtual ~CCollisionEntity() { }

	eCollisionEntityTypes GetBVType() {return m_eBVType;}

};

#endif