/*///////////////////////////////////////////////////////
/Filename      : GeometryEffect.h
/Author 	   : Tom Stefl
/Date:		   : 8/8/2013
/Last Modified : 8/8/2013
/Purpose	   : This class will hold the necessary data
/				 for specific geometry effects in the game
/*////////////////////////////////////////////////////////

#ifndef _GEOMETRY_EFFECT_H_
#define _GEOMETRY_EFFECT_H_
#include "glsMath.h"
#include "../Header Files/AnimationManager.h"
#include "../Header Files/RenderNode.h"

class IEntity;

class GeometryEffect
{
private:
	Vec3f m_Position;
	Vec3f m_Velocity;
	Vec3f m_Acceleration;
	CAnimationManager m_AnimManager;
	CRenderNode m_RenderNode;
	bool m_IsActive;
	int m_fxType;
	int m_animType;
	bool m_FlashIncrease;
	float m_FXOpacity;
	IEntity* m_SourceEntity;

	float m_BloodTimer;


public:
	GeometryEffect(void);
	~GeometryEffect(void);
	void Update(float _fElapsedTime);
	void Render();
	void InitializeEffect(int _enumFXType, int _enumAnimType, float _lifeSpan, const Vec3f _postion, IEntity* _entity);
	void BuildGeoFXGeometry();

	//Accessors
	Vec3f* GetPosition()	 {return &m_Position;}
	Vec3f* GetVelocity()	 {return &m_Velocity;}
	Vec3f* GetAcceleration() {return &m_Acceleration;}
	CRenderNode* GetRenderNode() {return &m_RenderNode;}
	bool IsActive() {return m_IsActive;}

	//Mutators
	void SetPosition(const Vec3f _newPos);
	void SetVelocity(Vec3f _newVelocity);
	void SetAcceleration(Vec3f _newAcceleration);
	void SetIsActive(bool _newBool) {m_IsActive = _newBool;}
	void SetOpacity(float _newFloat) {m_FXOpacity = _newFloat;}
	void Scale( float size );

	void FindClosestCrate(IEntity* _crate);
	Vec3f				m_ClosestCratePos;


};
#endif

