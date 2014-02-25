/*///////////////////////////////////////////////////////
/FileName:		Particle.h
/Author:		Thomas Stefl
/Date:			8/6/2013
/Last Modified: 8/7/2013
/Purpose:		This class holds all the relative 
/				information necessary for each particle.
/*///////////////////////////////////////////////////////
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "../Header Files/glsMath.h"
#include "../Header Files/RenderNode.h"

class Particle
{
private:
	Vec3f m_Position;
	Vec3f m_Velocity;
	//TO DO - maybe move to emitter
	Vec3f m_Acceleration;
	//Vec3f m_Color;
	float m_LifeSpan;
	float rotation;
	float m_Scale;
	

	CRenderNode m_RenderNode;
	GLuint m_hShader;
	GLuint m_hTexture;


public:
	Particle(void);
	~Particle(void);

	void Initialize(void);

	void Update(float _fElapsedTime);
	void Render();
	bool IsAlive();
	void BuildGeometry();

	//Mutators
	void SetPosition		(const Vec3f _newPosition);
	void SetVelocity		(const Vec3f _newVelocity);
	void SetAcceleration	(Vec3f _newAcceleration);
	void SetLifeSpan		(float _newLifeSpan) {m_LifeSpan = _newLifeSpan;}
	void SetScale		    (float _newFloat) {m_Scale = _newFloat;}

	void SetParticleColor(Vec3f _newColor);
	
	//Accessors
	Vec3f* GetPosition()		{return &m_Position;}
	Vec3f* GetVelocity()		{return &m_Velocity;}
	Vec3f* GetAcceleration()	{return &m_Acceleration;}
	float  GetCurrLifeSpan()	{return m_LifeSpan;}
	float  GetCurrScale()		{return m_Scale;}

	CRenderNode* GetRenderNode(){return &m_RenderNode;}
	void OrientToCamera();


};
#endif
