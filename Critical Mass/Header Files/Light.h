/*
	File:		Light.h
	Author:		Bob Pasekoff
	Created:	8/6/2013
*/

#ifndef _LIGHT_
#define _LIGHT_
#include "glsMath.h"

struct CLight
{
	Vec4f m_Color;
	float m_AmbientIntensity;
	float m_DiffuseIntensity;

	virtual ~CLight( void ) = 0;
};

struct CDirectionalLight : public CLight
{
	Vec3f m_Direction;
};

struct CPointLight : public CLight
{
	Vec3f m_Position;

	struct
	{
		float Constant;
		float Linear;
		float Exponential;
	} m_Attenuation;

	void Init( Vec3f _position, Vec4f _color, float _ambientInensity = 0.1f, float _diffuseIntensity = 7.5f )
	{
		memcpy( m_Position, _position, sizeof( Vec3f ) );
		memcpy( m_Color, _color, sizeof( Vec4f ) );
		m_AmbientIntensity = _ambientInensity;
		m_DiffuseIntensity = _diffuseIntensity;
	}
};

#endif //_LIGHT_