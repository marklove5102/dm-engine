#include "LuaScript.h"
#include <vector>

CLuaScript g_LuaScript;

// include Lua libs and tolua++
extern "C"
{
#include "Global/include/lua.h"
#include "Global/include/lualib.h"
#include "Global/include/lauxlib.h"
}

#include "toluapp/tolua++.h"

// prototype for bindings initialisation function
int tolua_UI_open(lua_State* tolua_S);

CLuaScript::CLuaScript()
{
	// create a lua state
	d_ownsState = true;
	d_state = lua_open();

	// init all standard libraries
	luaopen_base(d_state);
	luaopen_io(d_state);
	luaopen_string(d_state);
	luaopen_table(d_state);
	luaopen_math(d_state);
#if defined(DEBUG) || defined (_DEBUG)
	luaopen_debug(d_state);
#endif

	tolua_UI_open(d_state);
}

CLuaScript::CLuaScript(lua_State* state)
{
	// just use the given state
	d_ownsState = false;
	d_state = state;

	tolua_UI_open(d_state);
}

CLuaScript::~CLuaScript()
{
	if ( d_ownsState && d_state )
	{
		lua_close( d_state );
	}
}

void CLuaScript::ExecuteScriptFile(const string& filename)
{
	// load code into lua
	int top = lua_gettop(d_state);
	int loaderr = luaL_loadfile(d_state,filename.c_str());

	if (loaderr)
	{
		string errMsg = lua_tostring(d_state,-1);
		lua_settop(d_state,top);

#ifdef LUA_DEBUG
		output_debug("执行脚本文件%s错误:%s\n",filename.c_str(),errMsg.c_str());
#endif
		return;
	}

	// call it
	if (lua_pcall(d_state,0,0,0))
	{
		string errMsg = lua_tostring(d_state,-1);
		lua_settop(d_state,top);

#ifdef LUA_DEBUG
		output_debug("执行脚本文件%s错误:%s\n",filename.c_str(),errMsg.c_str());
#endif
		return;
	}

	lua_settop(d_state,top); // just in case :P
}

//Execute global script function
int	CLuaScript::ExecuteScriptGlobal(const string& funcname,DWORD param)
{
	int top = lua_gettop(d_state);

	// get the function from lua
	lua_getglobal(d_state, funcname.c_str());

	// is it a function
	if (!lua_isfunction(d_state,-1))
	{
		lua_settop(d_state,top);
#ifdef LUA_DEBUG
		output_debug("执行脚本函数%s错误\n",funcname.c_str());
#endif
		return 0;
	}

	lua_pushnumber(d_state, param);

	// call it
	int error = lua_pcall(d_state,1,1,0);		

	// handle errors
	if (error)
	{
		string errMsg = lua_tostring(d_state,-1);
		lua_pop(d_state,1);
#ifdef LUA_DEBUG
		output_debug("执行脚本函数%s错误:%s\n",funcname.c_str(),errMsg.c_str());
#endif
		return 0;
	}

	// get return value
	if (!lua_isboolean(d_state,-1) && !lua_isnumber(d_state,-1))
	{
		// log that return value is invalid. return -1 and move on.
		lua_settop(d_state,top);

#ifdef LUA_DEBUG
		output_debug("执行脚本函数%s错误:返回类型不对\n",funcname.c_str());
#endif
		return -1;
	}

	int ret = (int)lua_tonumber(d_state,-1);
	lua_pop(d_state,1);

	// return it
	return ret;
}

//	Execute script code string
void CLuaScript::ExecuteString(const string& str)
{
	if(str.empty())
		return;

	int top = lua_gettop(d_state);

	// load code into lua and call it
	int error =	luaL_loadbuffer(d_state, str.c_str(), str.length(), str.c_str()) || lua_pcall(d_state,0,0,0);

	// handle errors
	if (error)
	{
		string errMsg = lua_tostring(d_state,-1);
		lua_settop(d_state,top);
#ifdef LUA_DEBUG
		output_debug("执行脚本%s错误%s\n",str.c_str(),errMsg.c_str());
#endif
		return;
	}
}