#pragma once

#include "Global/Global.h"
#include "BaseClass/Control/Control.h"

struct lua_State;

class CLuaScript
{
public:
	CLuaScript();
	CLuaScript(lua_State* state);
	~CLuaScript();

	//º¯Êý
	void ExecuteScriptFile(const string& filename);
	int  ExecuteScriptGlobal(const string& funcname,DWORD param);

	void ExecuteString(const string& str);
private:
    bool d_ownsState;      //!< true when the attached lua_State was created by this script module
    lua_State* d_state;    //!< The lua_State that this script module uses.
};

extern CLuaScript g_LuaScript;