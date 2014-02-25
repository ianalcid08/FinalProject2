/*////////////////////////////////////////////////////////////////////////////
/Filename:		UIElement.h
/Author:		Thomas Stefl
/Date Modified: 7/25/2013
/Purpose:		This class is to hold information to hold orthographc information
/				for building and displaying 2d geometry to represent HUD and UI type
/				objects for our game.  This will be used for things like Menus and Health Bars...
*/////////////////////////////////////////////////////////////////////////////



#ifndef _UI_ELEMENT_H_
#define _UI_ELEMENT_H_
/////////////#includes
#include "../Header Files/GLSetup.h"
#include "../Header Files/GLBatch.h"

struct RECTANGLE
{
	float left, right, top, bottom;

	RECTANGLE(void)
	{
		left = right = top = bottom = 0.0f;
	}

	RECTANGLE(float _l, float _r, float _t, float _b)
	{
		left = _l;
		right = _r;
		top = _t;
		bottom = _b;
	}

	float GetWidth() { return this->right - this->left; }
	float GetHeight() { return this->bottom - this->top; }
	void SetWidth(float _width);
	void SetHeight(float _height);
};

class CRenderer;

class CUIElement
{
private:
	//HANDLES
	GLuint m_hShader;
	GLuint m_hTexture;

	RECTANGLE		m_Rect;
	float			m_PosX;
	float			m_PosY;
	float			m_Rotation;
	float			m_Scale;

	Vec4f			m_Color;

	GLBatch* m_GLBatch;
	
public:
	CUIElement();
	~CUIElement();
	void ReleaseGLBatch( void );

	//void Setup(float _width, float _height, unsigned int _screenWidth, unsigned int _screenHeight, Vec3f _position, float _rotation);
	// TODO: find a better way to do this	
	void BuildGeometry(int _screenWidth, int _screenHeight);
	void Setup(CRenderer *_renderer, GLuint _texture, RECTANGLE _rect, float _xPos, float _yPos, float _rotation, float _scale, Vec4f _color);

	//Accessors
	GLuint& GetShaderHandle()	{return m_hShader;}
	GLuint& GetTextureHandle()	{return m_hTexture;}
	GLBatch& GetGlBatch()		{return *m_GLBatch;}
	RECTANGLE& GetRect(void)		{return m_Rect;}
	float GetPosX()				{return m_PosX;} 
	float GetPosY()				{return m_PosY;} 
	float* GetOpacity()			{return &m_Color[3];}
	float* GetColor(void)		{return m_Color;}
	float  GetScale(void)		{ return m_Scale; }
	float GetRotation(void) { return m_Rotation; }

	//Mutators
	void SetShaderHandle(GLuint _newShaderHandle)		{m_hShader = _newShaderHandle;}
	void SetTextureHandle(GLuint _newTextureHandle)	    {m_hTexture = _newTextureHandle;}
	void SetPosX(float _newPosX)						{m_PosX = _newPosX;}
	void SetPosY(float _newPosY)						{m_PosY = _newPosY;}
	void SetRect(RECTANGLE _r)							{m_Rect = _r;}
	void SetOpacity (float _opacity)					{ m_Color[3] = _opacity; }
	void SetColor(Vec3f _color)							{ glsCopyVector3(m_Color, _color); }
	void SetRotatation(float _rot)						{ m_Rotation = _rot; }
	void SetScale(float _newScale)						{m_Scale = _newScale;}
	void SetColor(float _r, float _g, float _b, float _a);




	void LoadTGAAsTextureRect( const char *szFileName, GLenum wrapMode = GL_CLAMP_TO_EDGE, GLenum filter = GL_LINEAR );
};

#endif