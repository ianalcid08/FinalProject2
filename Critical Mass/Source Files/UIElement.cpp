/*////////////////////////////////////////////////////////////////////////////
/Filename:		UIElement.h
/Author:		Thomas Stefl
/Date Modified: 7/25/2013
/Purpose:		This class is to hold information to hold orthographc information
/				for building and displaying 2d geometry to represent HUD and UI type
/				objects for our game.  This will be used for things like Menus and Health Bars...
/*/////////////////////////////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/UIElement.h"
#include "../Header Files/Renderer.h"


void RECTANGLE::SetWidth(float _width)
{ 
	this->right = this->left + _width; 
}
void RECTANGLE::SetHeight(float _height)
{ 
	this->bottom = this->top + _height; 
}

/*////////////////////////////////////////////////////////////////
/This constructor will initialze all member variables to 0 and the 
/handles to -1.
/
/*////////////////////////////////////////////////////////////////
CUIElement::CUIElement()
{
	m_hShader = -1;
	m_hTexture= -1;

	m_PosX = 0.0f;
	m_PosY = 0.0f;
	m_Rotation = 0.0f;
	m_Scale = 0.0f;
	m_GLBatch = nullptr;
	glsLoadVector4(m_Color, 1.0f, 1.0f, 1.0f, 1.0f);
}

CUIElement::~CUIElement()
{
	ReleaseGLBatch();
}

void CUIElement::ReleaseGLBatch( void )
{
	if( m_GLBatch )
	{
		m_GLBatch->ReleaseVBOs();
		delete m_GLBatch;
		m_GLBatch = nullptr;
	}
}

/*////////////////////////////////////////////////////////////////
/This function is used to set the parameters sent in to the
/respective data member.  Please call this function before you call
/BuildGeometry. 
/*////////////////////////////////////////////////////////////////
void CUIElement::Setup(CRenderer *_renderer, GLuint _texture, RECTANGLE _rect, float _xPos, float _yPos, float _rotation, float _scale, Vec4f _color)
{
	m_hTexture = _texture;
	m_Rect = _rect;
	m_PosX = _xPos;
	m_PosY = _yPos;
	m_Rotation = _rotation;
	m_Scale = _scale * _renderer->GetScreenHeight() / 720.0f;
	//glsCopyVector4(m_Color, _color);
	m_Color[0] = _color[0];
	m_Color[1] = _color[1];
	m_Color[2] = _color[2];
	m_Color[3] = _color[3];
	BuildGeometry(_renderer->GetScreenWidth(), _renderer->GetScreenHeight());
}


/*////////////////////////////////////////////////////////////////
/This function will build geometry based on what information was passed 
/into the Setup function call.  The built Geometry goes into the
/ GLBatch member.
/*////////////////////////////////////////////////////////////////
void CUIElement::BuildGeometry(int _screenWidth, int _screenHeight)
{
	// DYNAMIC ALLOCATION
	ReleaseGLBatch();

	m_GLBatch = new GLBatch();

	m_GLBatch->Begin( GL_TRIANGLES, 6, 1 );

	// find width and height in clip space
	double width = m_Rect.GetWidth() * m_Scale / (double)_screenWidth * 2.0;
	double height = m_Rect.GetHeight() * m_Scale/ (double)_screenHeight * 2.0;

	// find the left, right, top and bottom in clip space with no translation
	double left = -width * 0.5f;
	double right = width * 0.5f;
	double top = height * 0.5f;
	double bottom = -height * 0.5f;
	
	// find the corners in clip space with no rotation
	Vec3d topLeft = { left, top, 0.0 };
	Vec3d topRight = { right, top, 0.0 };
	Vec3d bottomLeft = { left, bottom, 0.0 };
	Vec3d bottomRight = { right, bottom, 0.0 };
	Vec3d origin = { 0.0, 0.0, 0.0 };
	
	if(m_Rotation)
	{
		Matrix3x3d rotationMat;
		glsRotationMatrix3x3(rotationMat, glsDegToRad(m_Rotation), 0.0f, 0.0f, 1.0f);

		//Vec3d UpVec = { rotationMat[3], rotationMat[4], rotationMat[5] };
		//Vec3d RightVec = { rotationMat[0], rotationMat[1], rotationMat[2] };

		//glsNormalizeVector3(UpVec);
		//glsNormalizeVector3(RightVec);

		//glsScaleVector3(UpVec, height);
		//glsScaleVector3(RightVec, width);
		//
		//Vec3d concat;
		//glsAddVectors3(concat, UpVec, RightVec);
		//glsLoadVector3(topRight, concat[0], concat[1], concat[2]);

		//glsScaleVector3(UpVec, -1.0f);
		//glsAddVectors3(concat, UpVec, RightVec);
		//glsLoadVector3(bottomRight, concat[0], concat[1], concat[2]);

		//glsScaleVector3(RightVec, -1.0f);
		//glsAddVectors3(concat, UpVec, RightVec);
		//glsLoadVector3(bottomLeft, concat[0], concat[1], concat[2]);

		//glsScaleVector3(RightVec, -1.0f);
		//glsAddVectors3(concat, UpVec, RightVec);
		//glsLoadVector3(bottomLeft, concat[0], concat[1], concat[2]);

		//glsScaleVector3(UpVec, -1.0f);
		//glsAddVectors3(concat, UpVec, RightVec);
		//glsLoadVector3(topLeft, concat[0], concat[1], concat[2]);

		//rotate the top left into position
		Vec3d rotation;
		glsRotateVector(rotation, topLeft, rotationMat);
		glsCopyVector3(topLeft, rotation);

		//rotate the top right into position
		glsRotateVector(rotation, topRight, rotationMat);
		glsCopyVector3(topRight, rotation);

		// rotate the bottom lef into position
		glsRotateVector(rotation, bottomLeft, rotationMat);
		glsCopyVector3(bottomLeft, rotation);

		// rotate the bottom right into position
		glsRotateVector(rotation, bottomRight, rotationMat);
		glsCopyVector3(bottomRight, rotation);
	}

	Vec3d pos;
	Vec3d translation;
	pos[0] = (m_PosX / (float)_screenWidth * 2.0f - 1.0f) - left;
	pos[1] = (((float)_screenHeight - m_PosY) / (float)_screenHeight * 2.0f - 1.0f) - top;
	pos[2] = 0.0f;
	
	glsSubtractVectors3(translation, pos, origin);

	// translate top left
	glsAddVectors3(topLeft, topLeft, translation);

	// translate top right
	glsAddVectors3(topRight, topRight, translation);

	// translate bottom left
	glsAddVectors3(bottomLeft, bottomLeft, translation);

	// translate bottom right
	glsAddVectors3(bottomRight, bottomRight, translation);

	// Left Triangle
	m_GLBatch->Normal3f( 0.0f, 0.0f, 1.0f );
	m_GLBatch->MultiTexCoord2f( 0, 0.0f, (GLclampf)m_Rect.GetHeight() );
	m_GLBatch->Vertex3f((float)topLeft[0], (float)topLeft[1], 0.0f);

	m_GLBatch->Normal3f( 0.0f, 0.0f, 1.0f );
	m_GLBatch->MultiTexCoord2f( 0, 0.0f, 0.0f );
	m_GLBatch->Vertex3f((float)bottomLeft[0], (float)bottomLeft[1], 0.0f);

	m_GLBatch->Normal3f( 0.0f, 0.0f, 1.0f );
	m_GLBatch->MultiTexCoord2f( 0, (GLclampf)m_Rect.GetWidth(), (GLclampf)m_Rect.GetHeight() );
	m_GLBatch->Vertex3f((float)topRight[0], (float)topRight[1], 0.0f);

	// Right Triangle
	m_GLBatch->Normal3f( 0.0f, 0.0f, 1.0f );
	m_GLBatch->MultiTexCoord2f( 0, (GLclampf)m_Rect.GetWidth(), (GLclampf)m_Rect.GetHeight() );
	m_GLBatch->Vertex3f((float)topRight[0], (float)topRight[1], 0.0f);

	m_GLBatch->Normal3f( 0.0f, 0.0f, 1.0f );
	m_GLBatch->MultiTexCoord2f( 0, 0.0f, 0.0f );
	m_GLBatch->Vertex3f((float)bottomLeft[0], (float)bottomLeft[1], 0.0f);

	m_GLBatch->Normal3f( 0.0f, 0.0f, 1.0f );
	m_GLBatch->MultiTexCoord2f( 0, (GLclampf)m_Rect.GetWidth(), 0.0f);
	m_GLBatch->Vertex3f((float)bottomRight[0], (float)bottomRight[1], 0.0f);

	m_GLBatch->End();
}

void CUIElement::LoadTGAAsTextureRect( const char *szFileName, GLenum wrapMode, GLenum filter )
{
	glGenTextures( 1, &m_hTexture );
	glBindTexture( GL_TEXTURE_RECTANGLE, m_hTexture );

	GLbyte *pColorBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	pColorBits = glsReadTGABits( szFileName, &nWidth, &nHeight, &nComponents, &eFormat );
	if( !pColorBits )
	{
		MessageBoxA( 0, "Failed to load targa texture.", szFileName, MB_OK );
		return;
	}

	glTexImage2D( GL_TEXTURE_RECTANGLE, 0, nComponents, nWidth, nHeight, 0 /*no border ever*/,
		eFormat, GL_UNSIGNED_BYTE, pColorBits );

	free( pColorBits ); // one of our function calls called malloc

	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, filter );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, filter );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, wrapMode );
}
