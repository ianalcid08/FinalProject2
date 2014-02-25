/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : ScriptManager.h
Author : Evan Wright

Description : The Script Manager is Responsible for binding to LUA to pass data back and 
			  forth between the main modules and the scripts. It also registers any 
			  functions that are needed for LUA to call. Scripting will be available to 
			  creating Waves of enemies, AI behaviors, and visual effects.

Created :  07/18/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef SCRIPTMANAGER_H_
#define SCRIPTMANAGER_H_

class CScriptManager
{

public:
	CScriptManager( void );
	~CScriptManager( void ) { Release(); }

	void Initialize( void );
	void Release( void );

	void Execute( const char* _fileName, void* _ptr );
	int GetLuaState( void );

};

#endif