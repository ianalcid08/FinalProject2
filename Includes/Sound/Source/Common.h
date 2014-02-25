//--------------------------------------------------------------------------------
// Common header to only include often used items in one place
//--------------------------------------------------------------------------------
#ifndef Common_h
#define Common_h
//--------------------------------------------------------------------------------
//#include <vld.h>
#include "../Header Files/stdafx.h"
#define WIN32_LEAN_AND_MEAN // Exclude rarely used stuff
#include <windows.h>
#include <vector>
#include <map>
#include <assert.h>
#ifdef _WIN32_WINNT_WIN8
#include <directxmath.h>
using namespace DirectX;
#else
#include <xnamath.h>
#endif
//--------------------------------------------------------------------------------
//void LogWrite( const wchar_t *cTextString );
//void LogWrite( std::wstring& TextString );
//--------------------------------------------------------------------------------
#endif Common_h
//--------------------------------------------------------------------------------
