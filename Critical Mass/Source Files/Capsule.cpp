#include "../Header Files/stdafx.h"
#include "../Header Files/Capsule.h"


CCapsule::CCapsule()
{
	this->m_eBVType = eCapsule;
}

CCapsule::~CCapsule()
{

}

void CCapsule::SetStart( Vec3f _newStart )
{
	glsCopyVector3(m_vStart, _newStart);
}

void CCapsule::SetEnd( Vec3f _newEnd )
{
	glsCopyVector3(m_vEnd, _newEnd);
}
