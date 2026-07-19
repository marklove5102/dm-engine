/*
** Lua binding: UI
** Generated automatically by tolua++-1.0.92 on 11/21/07 21:09:33.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_UI_open (lua_State* tolua_S);

#pragma warning(disable : 4800)
#include "Global/Global.h"
#include "Global/MathUtil.h"
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "BaseClass/Control/CtrlTip.h"
#include "BaseClass/Control/CtrlScroll.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Misc/Input.h"
#include "WndClass/MainWnd/MainWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "LuaGlobal.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_UINT (lua_State* tolua_S)
{
 UINT* self = (UINT*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_BOOL (lua_State* tolua_S)
{
 BOOL* self = (BOOL*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CCtrlEdit");
 tolua_usertype(tolua_S,"CCtrlWindow");
 tolua_usertype(tolua_S,"WORD");
 tolua_usertype(tolua_S,"UCHAR");
 tolua_usertype(tolua_S,"CCtrlMainWin");
 tolua_usertype(tolua_S,"CCtrlWindowX");
 tolua_usertype(tolua_S,"CGood");
 tolua_usertype(tolua_S,"CCtrlRadio");
 tolua_usertype(tolua_S,"CCtrlScroll");
 tolua_usertype(tolua_S,"BOOL");
 tolua_usertype(tolua_S,"UINT");
 tolua_usertype(tolua_S,"CInput");
 tolua_usertype(tolua_S,"CControlContainer");
 tolua_usertype(tolua_S,"CGoodGrid");
 tolua_usertype(tolua_S,"CCharacter");
 tolua_usertype(tolua_S,"eCtrlMode");
 tolua_usertype(tolua_S,"CCharacterAttr");
 tolua_usertype(tolua_S,"CDemonAttr");
 tolua_usertype(tolua_S,"CMainWnd");
 tolua_usertype(tolua_S,"CCtrlButton");
 tolua_usertype(tolua_S,"CControl");
 tolua_usertype(tolua_S,"CCtrlTip");
}

/* method: GetName of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetName00
static int tolua_UI_Control_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetName00
static int tolua_UI_Control_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'",NULL);
#endif
  {
   self->SetName(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetX of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetX00
static int tolua_UI_Control_GetX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetX'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetY of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetY00
static int tolua_UI_Control_GetY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetY'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWidth of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetWidth00
static int tolua_UI_Control_GetWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWidth'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeight of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetHeight00
static int tolua_UI_Control_GetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScreenX of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetScreenX00
static int tolua_UI_Control_GetScreenX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScreenX'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetScreenX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScreenX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScreenY of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetScreenY00
static int tolua_UI_Control_GetScreenY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScreenY'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetScreenY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScreenY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMX of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetMX00
static int tolua_UI_Control_GetMX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMX'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetMX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMY of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetMY00
static int tolua_UI_Control_GetMY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMY'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetMY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetParent of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetParent00
static int tolua_UI_Control_GetParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetParent'",NULL);
#endif
  {
   CControl* tolua_ret = (CControl*)  self->GetParent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CControl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CanFocus of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_CanFocus00
static int tolua_UI_Control_CanFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CanFocus'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CanFocus();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CanFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNoFocus of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetNoFocus00
static int tolua_UI_Control_SetNoFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNoFocus'",NULL);
#endif
  {
   self->SetNoFocus(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNoFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFrame of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetFrame00
static int tolua_UI_Control_SetFrame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  int iFrame = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned long dwFrameColor = (( unsigned long)  tolua_tonumber(tolua_S,3,0));
  bool bAmb = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFrame'",NULL);
#endif
  {
   self->SetFrame(iFrame,dwFrameColor,bAmb);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFrame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMarge of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetMarge00
static int tolua_UI_Control_SetMarge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  int iMarge = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMarge'",NULL);
#endif
  {
   self->SetMarge(iMarge);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMarge'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMarge of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetMarge00
static int tolua_UI_Control_GetMarge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMarge'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetMarge();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMarge'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetText00
static int tolua_UI_Control_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  const char* sText = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'",NULL);
#endif
  {
   self->SetText(sText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetText of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetText00
static int tolua_UI_Control_GetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetText'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetText();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

///* method: SetBack of class  CControl */
//#ifndef TOLUA_DISABLE_tolua_UI_Control_SetBack00
//static int tolua_UI_Control_SetBack00(lua_State* tolua_S)
//{
//#ifndef TOLUA_RELEASE
// tolua_Error tolua_err;
// if (
//     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
//     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
//     !tolua_isnoobj(tolua_S,3,&tolua_err)
// )
//  goto tolua_lerror;
// else
//#endif
// {
//  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
//  unsigned long dwBackcolor = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
//#ifndef TOLUA_RELEASE
//  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBack'",NULL);
//#endif
//  {
//   self->SetBack(dwBackcolor);
//  }
// }
// return 0;
//#ifndef TOLUA_RELEASE
// tolua_lerror:
// tolua_error(tolua_S,"#ferror in function 'SetBack'.",&tolua_err);
// return 0;
//#endif
//}
//#endif //#ifndef TOLUA_DISABLE

/* method: SetColor of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetColor00
static int tolua_UI_Control_SetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwColor = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetColor'",NULL);
#endif
  {
   self->SetColor(dwColor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetColor of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetColor00
static int tolua_UI_Control_GetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetColor'",NULL);
#endif
  {
   unsigned long tolua_ret = ( unsigned long)  self->GetColor();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Show of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_Show00
static int tolua_UI_Control_Show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Show'",NULL);
#endif
  {
   self->Show();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Show'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Hide of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_Hide00
static int tolua_UI_Control_Hide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Hide'",NULL);
#endif
  {
   self->Hide();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Hide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsShow of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_IsShow00
static int tolua_UI_Control_IsShow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsShow'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsShow();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsShow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Enable of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_Enable00
static int tolua_UI_Control_Enable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Enable'",NULL);
#endif
  {
   self->Enable();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Enable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Disable of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_Disable00
static int tolua_UI_Control_Disable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Disable'",NULL);
#endif
  {
   self->Disable();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Disable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsEnable of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_IsEnable00
static int tolua_UI_Control_IsEnable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsEnable'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsEnable();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsEnable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMode of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetMode00
static int tolua_UI_Control_SetMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  bool bMode = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMode'",NULL);
#endif
  {
   self->SetMode(bMode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMode of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetMode00
static int tolua_UI_Control_GetMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMode'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetMode();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsMouseOn of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_IsMouseOn00
static int tolua_UI_Control_IsMouseOn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  int iX = ((int)  tolua_tonumber(tolua_S,2,0));
  int iY = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsMouseOn'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsMouseOn(iX,iY);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsMouseOn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsClick of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_IsClick00
static int tolua_UI_Control_IsClick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsClick'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsClick();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsClick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsRBClick of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_IsRBClick00
static int tolua_UI_Control_IsRBClick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsRBClick'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsRBClick();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsRBClick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsParent of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_IsParent00
static int tolua_UI_Control_IsParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  CControl* pCtrl = ((CControl*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsParent'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsParent(pCtrl);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsNoChangeLevel of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_IsNoChangeLevel00
static int tolua_UI_Control_IsNoChangeLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsNoChangeLevel'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsNoChangeLevel();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsNoChangeLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetControlMode of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetControlMode00
static int tolua_UI_Control_GetControlMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetControlMode'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetControlMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetControlMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTips of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetTips00
static int tolua_UI_Control_SetTips00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTips'",NULL);
#endif
  {
   self->SetTips(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTips'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTips of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetTips00
static int tolua_UI_Control_GetTips00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTips'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetTips();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTips'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTipTime of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetTipTime00
static int tolua_UI_Control_SetTipTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTipTime'",NULL);
#endif
  {
   self->SetTipTime(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTipTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTipTime of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetTipTime00
static int tolua_UI_Control_GetTipTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTipTime'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTipTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTipTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMask of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetMask00
static int tolua_UI_Control_SetMask00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  int wMask = ((int)  tolua_tonumber(tolua_S,2,0));
  int w = ((int)  tolua_tonumber(tolua_S,3,0));
  int h = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMask'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SetMask(wMask,w,h);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMask'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Move of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_Move00
static int tolua_UI_Control_Move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  int iX = ((int)  tolua_tonumber(tolua_S,2,0));
  int iY = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Move'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Move(iX,iY);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReSize of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_ReSize00
static int tolua_UI_Control_ReSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  int iW = ((int)  tolua_tonumber(tolua_S,2,0));
  int iH = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReSize'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ReSize(iW,iH);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFocus of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetFocus00
static int tolua_UI_Control_SetFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFocus'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SetFocus();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: KillFocus of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_KillFocus00
static int tolua_UI_Control_KillFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  CControl* pNewFocus = ((CControl*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'KillFocus'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->KillFocus(pNewFocus);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'KillFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetColorIndex of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetColorIndex00
static int tolua_UI_Control_SetColorIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  int c = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetColorIndex'",NULL);
#endif
  {
   self->SetColorIndex(c);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetColorIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetColorByIndex of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetColorByIndex00
static int tolua_UI_Control_GetColorByIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetColorByIndex'",NULL);
#endif
  {
   unsigned long tolua_ret = ( unsigned long)  self->GetColorByIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetColorByIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsFadeIn of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_IsFadeIn00
static int tolua_UI_Control_IsFadeIn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsFadeIn'",NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->IsFadeIn();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsFadeIn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFadeIn of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_SetFadeIn00
static int tolua_UI_Control_SetFadeIn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControl* self = (CControl*)  tolua_tousertype(tolua_S,1,0);
  unsigned char byStep = (( unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFadeIn'",NULL);
#endif
  {
   self->SetFadeIn(byStep);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFadeIn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMainWnd of class  CControl */
#ifndef TOLUA_DISABLE_tolua_UI_Control_GetMainWnd00
static int tolua_UI_Control_GetMainWnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CControl* self = (const CControl*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMainWnd'",NULL);
#endif
  {
   CCtrlMainWin* tolua_ret = (CCtrlMainWin*)  self->GetMainWnd();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCtrlMainWin");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMainWnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetControls of class  CControlContainer */
#ifndef TOLUA_DISABLE_tolua_UI_ControlContainer_GetControls00
static int tolua_UI_ControlContainer_GetControls00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControlContainer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControlContainer* self = (CControlContainer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetControls'",NULL);
#endif
  {
   CControl* tolua_ret = (CControl*)  self->GetControls();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CControl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetControls'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetControls of class  CControlContainer */
#ifndef TOLUA_DISABLE_tolua_UI_ControlContainer_SetControls00
static int tolua_UI_ControlContainer_SetControls00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControlContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControlContainer* self = (CControlContainer*)  tolua_tousertype(tolua_S,1,0);
  CControl* pCtrl = ((CControl*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetControls'",NULL);
#endif
  {
   self->SetControls(pCtrl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetControls'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSonCtrl of class  CControlContainer */
#ifndef TOLUA_DISABLE_tolua_UI_ControlContainer_IsSonCtrl00
static int tolua_UI_ControlContainer_IsSonCtrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControlContainer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControlContainer* self = (CControlContainer*)  tolua_tousertype(tolua_S,1,0);
  CControl* pControl = ((CControl*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSonCtrl'",NULL);
#endif
  {
   BOOL tolua_ret = (BOOL)  self->IsSonCtrl(pControl);
   {
#ifdef __cplusplus
    void* tolua_obj = new BOOL(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"BOOL");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(BOOL));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"BOOL");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSonCtrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindControl of class  CControlContainer */
#ifndef TOLUA_DISABLE_tolua_UI_ControlContainer_FindControl00
static int tolua_UI_ControlContainer_FindControl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControlContainer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControlContainer* self = (CControlContainer*)  tolua_tousertype(tolua_S,1,0);
  int iPos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindControl'",NULL);
#endif
  {
   CControl* tolua_ret = (CControl*)  self->FindControl(iPos);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CControl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindControl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindControlByName of class  CControlContainer */
#ifndef TOLUA_DISABLE_tolua_UI_ControlContainer_FindControlByName00
static int tolua_UI_ControlContainer_FindControlByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CControlContainer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"eCtrlMode",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CControlContainer* self = (CControlContainer*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
  eCtrlMode filter = *((eCtrlMode*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindControlByName'",NULL);
#endif
  {
   CControl* tolua_ret = (CControl*)  self->FindControlByName(name,filter);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CControl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindControlByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetLifeTime of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_SetLifeTime00
static int tolua_UI_Window_SetLifeTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
  int dwTime = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetLifeTime'",NULL);
#endif
  {
   self->SetLifeTime(dwTime);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetLifeTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLifeTime of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_GetLifeTime00
static int tolua_UI_Window_GetLifeTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLifeTime'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLifeTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLifeTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCloseButton of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_SetCloseButton00
static int tolua_UI_Window_SetCloseButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
  int iX = ((int)  tolua_tonumber(tolua_S,2,0));
  int iY = ((int)  tolua_tonumber(tolua_S,3,0));
  int iTexIndex = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCloseButton'",NULL);
#endif
  {
   self->SetCloseButton(iX,iY,iTexIndex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCloseButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnClickCloseButton of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_OnClickCloseButton00
static int tolua_UI_Window_OnClickCloseButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnClickCloseButton'",NULL);
#endif
  {
   self->OnClickCloseButton();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnClickCloseButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnMouseMove of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_OnMouseMove00
static int tolua_UI_Window_OnMouseMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
  int iX = ((int)  tolua_tonumber(tolua_S,2,0));
  int iY = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnMouseMove'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnMouseMove(iX,iY);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnMouseMove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Draw of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_Draw00
static int tolua_UI_Window_Draw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Draw'",NULL);
#endif
  {
   self->Draw();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Draw'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Msg of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_Msg00
static int tolua_UI_Window_Msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"CControl",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwMsg = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
  unsigned long dwData = (( unsigned long)  tolua_tonumber(tolua_S,3,0));
  CControl* pControl = ((CControl*)  tolua_tousertype(tolua_S,4,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Msg'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Msg(dwMsg,dwData,pControl);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Msg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnKeyDown of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_OnKeyDown00
static int tolua_UI_Window_OnKeyDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"WORD",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"UCHAR",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
  WORD wState = *((WORD*)  tolua_tousertype(tolua_S,2,0));
  UCHAR cKey = *((UCHAR*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnKeyDown'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnKeyDown(wState,cKey);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnKeyDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnEscape of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_OnEscape00
static int tolua_UI_Window_OnEscape00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnEscape'",NULL);
#endif
  {
   self->OnEscape();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnEscape'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetLifeTime of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_SetLifeTime01
static int tolua_UI_Window_SetLifeTime01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwTime = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetLifeTime'",NULL);
#endif
  {
   self->SetLifeTime(dwTime);
  }
 }
 return 0;
tolua_lerror:
 return tolua_UI_Window_SetLifeTime00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLifeTime of class  CCtrlWindow */
#ifndef TOLUA_DISABLE_tolua_UI_Window_GetLifeTime01
static int tolua_UI_Window_GetLifeTime01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCtrlWindow* self = (CCtrlWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLifeTime'",NULL);
#endif
  {
   unsigned long tolua_ret = ( unsigned long)  self->GetLifeTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_UI_Window_GetLifeTime00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: UpdateLayOut of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_UpdateLayOut00
static int tolua_UI_MainWnd_UpdateLayOut00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CMainWnd::UpdateLayOut();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UpdateLayOut'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DisableIME of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_DisableIME00
static int tolua_UI_MainWnd_DisableIME00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CMainWnd::DisableIME();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DisableIME'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseX of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetMouseX00
static int tolua_UI_MainWnd_GetMouseX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseX'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetMouseX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseY of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetMouseY00
static int tolua_UI_MainWnd_GetMouseY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseY'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetMouseY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseXY of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetMouseXY00
static int tolua_UI_MainWnd_GetMouseXY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
  int iX = ((int)  tolua_tonumber(tolua_S,2,0));
  int iY = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseXY'",NULL);
#endif
  {
   self->GetMouseXY(iX,iY);
   tolua_pushnumber(tolua_S,(lua_Number)iX);
   tolua_pushnumber(tolua_S,(lua_Number)iY);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseXY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMouseOnID of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_SetMouseOnID00
static int tolua_UI_MainWnd_SetMouseOnID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"UINT",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"CControl",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
  UINT uID = *((UINT*)  tolua_tousertype(tolua_S,2,0));
  CControl* pCtrl = ((CControl*)  tolua_tousertype(tolua_S,3,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMouseOnID'",NULL);
#endif
  {
   self->SetMouseOnID(uID,pCtrl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMouseOnID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseOnID of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetMouseOnID00
static int tolua_UI_MainWnd_GetMouseOnID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseOnID'",NULL);
#endif
  {
   UINT tolua_ret = (UINT)  self->GetMouseOnID();
   {
#ifdef __cplusplus
    void* tolua_obj = new UINT(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"UINT");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(UINT));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"UINT");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseOnID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseOnCtrl of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetMouseOnCtrl00
static int tolua_UI_MainWnd_GetMouseOnCtrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseOnCtrl'",NULL);
#endif
  {
   CControl* tolua_ret = (CControl*)  self->GetMouseOnCtrl();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CControl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseOnCtrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTipOwnerWnd of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_SetTipOwnerWnd00
static int tolua_UI_MainWnd_SetTipOwnerWnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
  CControl* p = ((CControl*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTipOwnerWnd'",NULL);
#endif
  {
   self->SetTipOwnerWnd(p);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTipOwnerWnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTipOwnerWnd of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetTipOwnerWnd00
static int tolua_UI_MainWnd_GetTipOwnerWnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTipOwnerWnd'",NULL);
#endif
  {
   CControl* tolua_ret = (CControl*)  self->GetTipOwnerWnd();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CControl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTipOwnerWnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTipWnd of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetTipWnd00
static int tolua_UI_MainWnd_GetTipWnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTipWnd'",NULL);
#endif
  {
   CCtrlTip* tolua_ret = (CCtrlTip*)  self->GetTipWnd();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCtrlTip");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTipWnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHideCursor of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_SetHideCursor00
static int tolua_UI_MainWnd_SetHideCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"BOOL",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
  BOOL b = *((BOOL*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHideCursor'",NULL);
#endif
  {
   self->SetHideCursor(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHideCursor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsHideCursor of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_IsHideCursor00
static int tolua_UI_MainWnd_IsHideCursor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsHideCursor'",NULL);
#endif
  {
   BOOL tolua_ret = (BOOL)  self->IsHideCursor();
   {
#ifdef __cplusplus
    void* tolua_obj = new BOOL(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"BOOL");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(BOOL));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"BOOL");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsHideCursor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetGameState of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetGameState00
static int tolua_UI_MainWnd_GetGameState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CMainWnd* self = (const CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGameState'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetGameState();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGameState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFocusCtrl of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_GetFocusCtrl00
static int tolua_UI_MainWnd_GetFocusCtrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFocusCtrl'",NULL);
#endif
  {
   CControl* tolua_ret = (CControl*)  self->GetFocusCtrl();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CControl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFocusCtrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFocusCtrl of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_SetFocusCtrl00
static int tolua_UI_MainWnd_SetFocusCtrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
  CControl* pCtrl = ((CControl*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFocusCtrl'",NULL);
#endif
  {
   self->SetFocusCtrl(pCtrl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFocusCtrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ADDCONTROL of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_ADDCONTROL00
static int tolua_UI_MainWnd_ADDCONTROL00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
  CControl* pCtrl = ((CControl*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ADDCONTROL'",NULL);
#endif
  {
   self->ADDCONTROL(pCtrl);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ADDCONTROL'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DELETECONTROL of class  CMainWnd */
#ifndef TOLUA_DISABLE_tolua_UI_MainWnd_DELETECONTROL00
static int tolua_UI_MainWnd_DELETECONTROL00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CMainWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CMainWnd* self = (CMainWnd*)  tolua_tousertype(tolua_S,1,0);
  CControl* pCtrl = ((CControl*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DELETECONTROL'",NULL);
#endif
  {
   self->DELETECONTROL(&pCtrl);
   tolua_pushusertype(tolua_S,(void*)pCtrl,"CControl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DELETECONTROL'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnClick of class  CCtrlButton */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlButton_OnClick00
static int tolua_UI_CCtrlButton_OnClick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlButton* self = (CCtrlButton*)  tolua_tousertype(tolua_S,1,0);
  int iButton = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnClick'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnClick(iButton);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnClick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAutoSwitch of class  CCtrlButton */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlButton_SetAutoSwitch00
static int tolua_UI_CCtrlButton_SetAutoSwitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlButton",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlButton* self = (CCtrlButton*)  tolua_tousertype(tolua_S,1,0);
  bool bAuto = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAutoSwitch'",NULL);
#endif
  {
   self->SetAutoSwitch(bAuto);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAutoSwitch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSmall of class  CCtrlButton */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlButton_IsSmall00
static int tolua_UI_CCtrlButton_IsSmall00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlButton* self = (CCtrlButton*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSmall'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSmall();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSmall'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReloadTex of class  CCtrlButton */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlButton_ReloadTex00
static int tolua_UI_CCtrlButton_ReloadTex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlButton* self = (CCtrlButton*)  tolua_tousertype(tolua_S,1,0);
  int iTexIndex = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReloadTex'",NULL);
#endif
  {
   self->ReloadTex(iTexIndex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReloadTex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReloadTex of class  CCtrlButton */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlButton_ReloadTex01
static int tolua_UI_CCtrlButton_ReloadTex01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CCtrlButton* self = (CCtrlButton*)  tolua_tousertype(tolua_S,1,0);
  int wTex = ((int)  tolua_tonumber(tolua_S,2,0));
  int wMTex = ((int)  tolua_tonumber(tolua_S,3,0));
  int wCTex = ((int)  tolua_tonumber(tolua_S,4,0));
  int wDTex = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReloadTex'",NULL);
#endif
  {
   self->ReloadTex(wTex,wMTex,wCTex,wDTex);
  }
 }
 return 0;
tolua_lerror:
 return tolua_UI_CCtrlButton_ReloadTex00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSwitchTex of class  CCtrlButton */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlButton_SetSwitchTex00
static int tolua_UI_CCtrlButton_SetSwitchTex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlButton* self = (CCtrlButton*)  tolua_tousertype(tolua_S,1,0);
  int wTex = ((int)  tolua_tonumber(tolua_S,2,0));
  int wMTex = ((int)  tolua_tonumber(tolua_S,3,0));
  int wCTex = ((int)  tolua_tonumber(tolua_S,4,0));
  int wDTex = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSwitchTex'",NULL);
#endif
  {
   self->SetSwitchTex(wTex,wMTex,wCTex,wDTex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSwitchTex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetState of class  CCtrlButton */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlButton_SetState00
static int tolua_UI_CCtrlButton_SetState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlButton",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlButton* self = (CCtrlButton*)  tolua_tousertype(tolua_S,1,0);
  bool _bSwitchState = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetState'",NULL);
#endif
  {
   self->SetState(_bSwitchState);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetEffectTex of class  CCtrlButton */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlButton_SetEffectTex00
static int tolua_UI_CCtrlButton_SetEffectTex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlButton",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlButton* self = (CCtrlButton*)  tolua_tousertype(tolua_S,1,0);
  int wEff = ((int)  tolua_tonumber(tolua_S,2,0));
  int iOffX = ((int)  tolua_tonumber(tolua_S,3,0));
  int iOffY = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetEffectTex'",NULL);
#endif
  {
   self->SetEffectTex(wEff,iOffX,iOffY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetEffectTex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnClick of class  CCtrlRadio */
#ifndef TOLUA_DISABLE_tolua_UI_Radio_OnClick00
static int tolua_UI_Radio_OnClick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlRadio",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlRadio* self = (CCtrlRadio*)  tolua_tousertype(tolua_S,1,0);
  int iButton = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnClick'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnClick(iButton);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnClick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsChecked of class  CCtrlRadio */
#ifndef TOLUA_DISABLE_tolua_UI_Radio_IsChecked00
static int tolua_UI_Radio_IsChecked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlRadio",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlRadio* self = (CCtrlRadio*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsChecked'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsChecked();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsChecked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EnableChecked of class  CCtrlRadio */
#ifndef TOLUA_DISABLE_tolua_UI_Radio_EnableChecked00
static int tolua_UI_Radio_EnableChecked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlRadio",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlRadio* self = (CCtrlRadio*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EnableChecked'",NULL);
#endif
  {
   self->EnableChecked();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EnableChecked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DisableChecked of class  CCtrlRadio */
#ifndef TOLUA_DISABLE_tolua_UI_Radio_DisableChecked00
static int tolua_UI_Radio_DisableChecked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlRadio",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlRadio* self = (CCtrlRadio*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DisableChecked'",NULL);
#endif
  {
   self->DisableChecked();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DisableChecked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRadio of class  CCtrlRadio */
#ifndef TOLUA_DISABLE_tolua_UI_Radio_GetRadio00
static int tolua_UI_Radio_GetRadio00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlRadio",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlRadio* self = (CCtrlRadio*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRadio'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRadio();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRadio'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRadio of class  CCtrlRadio */
#ifndef TOLUA_DISABLE_tolua_UI_Radio_SetRadio00
static int tolua_UI_Radio_SetRadio00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlRadio",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlRadio* self = (CCtrlRadio*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRadio'",NULL);
#endif
  {
   self->SetRadio(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRadio'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddBuddy of class  CCtrlRadio */
#ifndef TOLUA_DISABLE_tolua_UI_Radio_AddBuddy00
static int tolua_UI_Radio_AddBuddy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlRadio",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCtrlRadio",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlRadio* self = (CCtrlRadio*)  tolua_tousertype(tolua_S,1,0);
  CCtrlRadio* buddy = ((CCtrlRadio*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddBuddy'",NULL);
#endif
  {
   self->AddBuddy(buddy);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddBuddy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPwd of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_SetPwd00
static int tolua_UI_Edit_SetPwd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPwd'",NULL);
#endif
  {
   self->SetPwd(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPwd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsPwd of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_IsPwd00
static int tolua_UI_Edit_IsPwd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsPwd'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsPwd();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsPwd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_Clear00
static int tolua_UI_Edit_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'",NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetText of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_GetText00
static int tolua_UI_Edit_GetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetText'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetText();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_SetText00
static int tolua_UI_Edit_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
  const char* sText = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'",NULL);
#endif
  {
   self->SetText(sText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNum of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_GetNum00
static int tolua_UI_Edit_GetNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNum'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMaxLength of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_SetMaxLength00
static int tolua_UI_Edit_SetMaxLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
  int iLength = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMaxLength'",NULL);
#endif
  {
   self->SetMaxLength(iLength);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMaxLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: InsertText of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_InsertText00
static int tolua_UI_Edit_InsertText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
  char* sText = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InsertText'",NULL);
#endif
  {
   self->InsertText(sText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'InsertText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDigital of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_SetDigital00
static int tolua_UI_Edit_SetDigital00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
  bool bDigital = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDigital'",NULL);
#endif
  {
   self->SetDigital(bDigital);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDigital'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetVisualWords of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_SetVisualWords00
static int tolua_UI_Edit_SetVisualWords00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
  int iWords = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetVisualWords'",NULL);
#endif
  {
   self->SetVisualWords(iWords);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetVisualWords'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCursorColor of class  CCtrlEdit */
#ifndef TOLUA_DISABLE_tolua_UI_Edit_SetCursorColor00
static int tolua_UI_Edit_SetCursorColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlEdit",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlEdit* self = (CCtrlEdit*)  tolua_tousertype(tolua_S,1,0);
  unsigned long dwColor = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCursorColor'",NULL);
#endif
  {
   self->SetCursorColor(dwColor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCursorColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetText of class  CCtrlTip */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlTip_SetText00
static int tolua_UI_CCtrlTip_SetText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlTip",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlTip* self = (CCtrlTip*)  tolua_tousertype(tolua_S,1,0);
  const char* sText = ((const char*)  tolua_tostring(tolua_S,2,0));
  bool bCenter = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetText'",NULL);
#endif
  {
   self->SetText(sText,bCenter);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddText of class  CCtrlTip */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlTip_AddText00
static int tolua_UI_CCtrlTip_AddText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlTip",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlTip* self = (CCtrlTip*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned long color = (( unsigned long)  tolua_tonumber(tolua_S,3,0));
  bool bCenter = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddText'",NULL);
#endif
  {
   self->AddText(str,color,bCenter);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  CCtrlTip */
#ifndef TOLUA_DISABLE_tolua_UI_CCtrlTip_Clear00
static int tolua_UI_CCtrlTip_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlTip",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlTip* self = (CCtrlTip*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'",NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRange of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_SetRange00
static int tolua_UI_Scroll_SetRange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
  int iRange = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRange'",NULL);
#endif
  {
   self->SetRange(iRange);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRange of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_GetRange00
static int tolua_UI_Scroll_GetRange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRange'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRange();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetStep of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_SetStep00
static int tolua_UI_Scroll_SetStep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
  int iStep = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetStep'",NULL);
#endif
  {
   self->SetStep(iStep);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetStep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStep of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_GetStep00
static int tolua_UI_Scroll_GetStep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStep'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetStep();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPos of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_SetPos00
static int tolua_UI_Scroll_SetPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
  int iPos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPos'",NULL);
#endif
  {
   self->SetPos(iPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPos of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_GetPos00
static int tolua_UI_Scroll_GetPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPos'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetPos();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetVertical of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_SetVertical00
static int tolua_UI_Scroll_SetVertical00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetVertical'",NULL);
#endif
  {
   self->SetVertical(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetVertical'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


/* method: ReSize of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_ReSize00
static int tolua_UI_Scroll_ReSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
  int iW = ((int)  tolua_tonumber(tolua_S,2,0));
  int iH = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReSize'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ReSize(iW,iH);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GoUp of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_GoUp00
static int tolua_UI_Scroll_GoUp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GoUp'",NULL);
#endif
  {
   self->GoUp();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GoUp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GoDown of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_GoDown00
static int tolua_UI_Scroll_GoDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GoDown'",NULL);
#endif
  {
   self->GoDown();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GoDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetButtonHeight of class  CCtrlScroll */
#ifndef TOLUA_DISABLE_tolua_UI_Scroll_SetButtonHeight00
static int tolua_UI_Scroll_SetButtonHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlScroll",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlScroll* self = (CCtrlScroll*)  tolua_tousertype(tolua_S,1,0);
  int iHeight = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetButtonHeight'",NULL);
#endif
  {
   self->SetButtonHeight(iHeight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetButtonHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: PointInPage of class  CCtrlWindowX */
#ifndef TOLUA_DISABLE_tolua_UI_WindowX_PointInPage00
static int tolua_UI_WindowX_PointInPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindowX",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindowX* self = (CCtrlWindowX*)  tolua_tousertype(tolua_S,1,0);
  int iX = ((int)  tolua_tonumber(tolua_S,2,0));
  int iY = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PointInPage'",NULL);
#endif
  {
   int tolua_ret = (int)  self->PointInPage(iX,iY);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PointInPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SwitchToPage of class  CCtrlWindowX */
#ifndef TOLUA_DISABLE_tolua_UI_WindowX_SwitchToPage00
static int tolua_UI_WindowX_SwitchToPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindowX",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindowX* self = (CCtrlWindowX*)  tolua_tousertype(tolua_S,1,0);
  int iPage = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SwitchToPage'",NULL);
#endif
  {
   self->SwitchToPage(iPage);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SwitchToPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPageRect of class  CCtrlWindowX */
#ifndef TOLUA_DISABLE_tolua_UI_WindowX_SetPageRect00
static int tolua_UI_WindowX_SetPageRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindowX",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindowX* self = (CCtrlWindowX*)  tolua_tousertype(tolua_S,1,0);
  int iPage = ((int)  tolua_tonumber(tolua_S,2,0));
  int left = ((int)  tolua_tonumber(tolua_S,3,0));
  int top = ((int)  tolua_tonumber(tolua_S,4,0));
  int right = ((int)  tolua_tonumber(tolua_S,5,0));
  int bottom = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPageRect'",NULL);
#endif
  {
   self->SetPageRect(iPage,left,top,right,bottom);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPageRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindScrollByName of class  CCtrlWindowX */
#ifndef TOLUA_DISABLE_tolua_UI_WindowX_FindScrollByName00
static int tolua_UI_WindowX_FindScrollByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindowX",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindowX* self = (CCtrlWindowX*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindScrollByName'",NULL);
#endif
  {
   CCtrlScroll* tolua_ret = (CCtrlScroll*)  self->FindScrollByName(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCtrlScroll");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindScrollByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindRadioByName of class  CCtrlWindowX */
#ifndef TOLUA_DISABLE_tolua_UI_WindowX_FindRadioByName00
static int tolua_UI_WindowX_FindRadioByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindowX",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindowX* self = (CCtrlWindowX*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindRadioByName'",NULL);
#endif
  {
   CCtrlRadio* tolua_ret = (CCtrlRadio*)  self->FindRadioByName(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCtrlRadio");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindRadioByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindButtonByName of class  CCtrlWindowX */
#ifndef TOLUA_DISABLE_tolua_UI_WindowX_FindButtonByName00
static int tolua_UI_WindowX_FindButtonByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindowX",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindowX* self = (CCtrlWindowX*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindButtonByName'",NULL);
#endif
  {
   CCtrlButton* tolua_ret = (CCtrlButton*)  self->FindButtonByName(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCtrlButton");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindButtonByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindEditByName of class  CCtrlWindowX */
#ifndef TOLUA_DISABLE_tolua_UI_WindowX_FindEditByName00
static int tolua_UI_WindowX_FindEditByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCtrlWindowX",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCtrlWindowX* self = (CCtrlWindowX*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindEditByName'",NULL);
#endif
  {
   CCtrlEdit* tolua_ret = (CCtrlEdit*)  self->FindEditByName(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCtrlEdit");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindEditByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLevel of class  CCharacter */
#ifndef TOLUA_DISABLE_tolua_UI_CCharacter_GetLevel00
static int tolua_UI_CCharacter_GetLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCharacter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCharacter* self = (CCharacter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLevel'",NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCareer of class  CCharacter */
#ifndef TOLUA_DISABLE_tolua_UI_CCharacter_GetCareer00
static int tolua_UI_CCharacter_GetCareer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CCharacter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CCharacter* self = (CCharacter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCareer'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetCareer();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCareer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetID of class  CGood */
#ifndef TOLUA_DISABLE_tolua_UI_Good_GetID00
static int tolua_UI_Good_GetID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CGood",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CGood* self = (CGood*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetID'",NULL);
#endif
  {
   unsigned long tolua_ret = ( unsigned long)  self->GetID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStdMode of class  CGood */
#ifndef TOLUA_DISABLE_tolua_UI_Good_GetStdMode00
static int tolua_UI_Good_GetStdMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CGood",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CGood* self = (CGood*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStdMode'",NULL);
#endif
  {
   int tolua_ret = (int)  self->GetStdMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStdMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNeed of class  CGood */
#ifndef TOLUA_DISABLE_tolua_UI_Good_GetNeed00
static int tolua_UI_Good_GetNeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CGood",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CGood* self = (CGood*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNeed'",NULL);
#endif
  {
   unsigned long tolua_ret = ( unsigned long)  self->GetNeed();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNeedLevel of class  CGood */
#ifndef TOLUA_DISABLE_tolua_UI_Good_GetNeedLevel00
static int tolua_UI_Good_GetNeedLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CGood",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CGood* self = (CGood*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNeedLevel'",NULL);
#endif
  {
   unsigned long tolua_ret = ( unsigned long)  self->GetNeedLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNeedLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_pThis */
#ifndef TOLUA_DISABLE_tolua_get_this_ptr
static int tolua_get_this_ptr(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)g_pThis,"CControl");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_pControl */
#ifndef TOLUA_DISABLE_tolua_get_ui_ptr
static int tolua_get_ui_ptr(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)g_pControl,"CMainWnd");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_pInput */
#ifndef TOLUA_DISABLE_tolua_get_input_ptr
static int tolua_get_input_ptr(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)g_pInput,"CInput");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_pChar */
#ifndef TOLUA_DISABLE_tolua_get_SELF_ptr
static int tolua_get_SELF_ptr(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)g_pChar,"CCharacter");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* function: PopupWindow */
#ifndef TOLUA_DISABLE_tolua_UI_PopupWindow00
static int tolua_UI_PopupWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* name = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   PopupWindow(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PopupWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: UpdateWindow */
#ifndef TOLUA_DISABLE_tolua_UI_UpdateWindow00
static int tolua_UI_UpdateWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* name = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   UpdateWindow(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UpdateWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: CloseWindow */
#ifndef TOLUA_DISABLE_tolua_UI_CloseWindow00
static int tolua_UI_CloseWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* name = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   CloseWindow(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CloseWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: PopSimpleAlert */
#ifndef TOLUA_DISABLE_tolua_UI_PopSimpleAlert00
static int tolua_UI_PopSimpleAlert00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* str = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   PopSimpleAlert(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PopSimpleAlert'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: PopSimpleComfirm */
#ifndef TOLUA_DISABLE_tolua_UI_PopSimpleComfirm00
static int tolua_UI_PopSimpleComfirm00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* str = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   PopSimpleComfirm(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PopSimpleComfirm'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: PopEditBox */
#ifndef TOLUA_DISABLE_tolua_UI_PopEditBox00
static int tolua_UI_PopEditBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* str = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   PopEditBox(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PopEditBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetDropGood */
#ifndef TOLUA_DISABLE_tolua_UI_GetDropGood00
static int tolua_UI_GetDropGood00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CGood* tolua_ret = (CGood*)  GetDropGood();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"CGood");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDropGood'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetDropGoodPos */
#ifndef TOLUA_DISABLE_tolua_UI_GetDropGoodPos00
static int tolua_UI_GetDropGoodPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GetDropGoodPos();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDropGoodPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetDropFromWnd */
#ifndef TOLUA_DISABLE_tolua_UI_GetDropFromWnd00
static int tolua_UI_GetDropFromWnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GetDropFromWnd();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDropFromWnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetCharacterMagicID */
#ifndef TOLUA_DISABLE_tolua_UI_GetCharacterMagicID00
static int tolua_UI_GetCharacterMagicID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int i = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   int tolua_ret = (int)  GetCharacterMagicID(i);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCharacterMagicID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetLearnedMagicNum */
#ifndef TOLUA_DISABLE_tolua_UI_GetLearnedMagicNum00
static int tolua_UI_GetLearnedMagicNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  GetLearnedMagicNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLearnedMagicNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: TestShowMessageBox */
#ifndef TOLUA_DISABLE_tolua_UI_TestShowMessageBox00
static int tolua_UI_TestShowMessageBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* strMsg = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   TestShowMessageBox(strMsg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TestShowMessageBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_UI_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"Control","CControl","",NULL);
  tolua_beginmodule(tolua_S,"Control");
   tolua_function(tolua_S,"GetName",tolua_UI_Control_GetName00);
   tolua_function(tolua_S,"SetName",tolua_UI_Control_SetName00);
   tolua_function(tolua_S,"GetX",tolua_UI_Control_GetX00);
   tolua_function(tolua_S,"GetY",tolua_UI_Control_GetY00);
   tolua_function(tolua_S,"GetWidth",tolua_UI_Control_GetWidth00);
   tolua_function(tolua_S,"GetHeight",tolua_UI_Control_GetHeight00);
   tolua_function(tolua_S,"GetScreenX",tolua_UI_Control_GetScreenX00);
   tolua_function(tolua_S,"GetScreenY",tolua_UI_Control_GetScreenY00);
   tolua_function(tolua_S,"GetMX",tolua_UI_Control_GetMX00);
   tolua_function(tolua_S,"GetMY",tolua_UI_Control_GetMY00);
   tolua_function(tolua_S,"GetParent",tolua_UI_Control_GetParent00);
   tolua_function(tolua_S,"CanFocus",tolua_UI_Control_CanFocus00);
   tolua_function(tolua_S,"SetNoFocus",tolua_UI_Control_SetNoFocus00);
   tolua_function(tolua_S,"SetFrame",tolua_UI_Control_SetFrame00);
   tolua_function(tolua_S,"SetMarge",tolua_UI_Control_SetMarge00);
   tolua_function(tolua_S,"GetMarge",tolua_UI_Control_GetMarge00);
   tolua_function(tolua_S,"SetText",tolua_UI_Control_SetText00);
   tolua_function(tolua_S,"GetText",tolua_UI_Control_GetText00);
   //tolua_function(tolua_S,"SetBack",tolua_UI_Control_SetBack00);
   tolua_function(tolua_S,"SetColor",tolua_UI_Control_SetColor00);
   tolua_function(tolua_S,"GetColor",tolua_UI_Control_GetColor00);
   tolua_function(tolua_S,"Show",tolua_UI_Control_Show00);
   tolua_function(tolua_S,"Hide",tolua_UI_Control_Hide00);
   tolua_function(tolua_S,"IsShow",tolua_UI_Control_IsShow00);
   tolua_function(tolua_S,"Enable",tolua_UI_Control_Enable00);
   tolua_function(tolua_S,"Disable",tolua_UI_Control_Disable00);
   tolua_function(tolua_S,"IsEnable",tolua_UI_Control_IsEnable00);
   tolua_function(tolua_S,"SetMode",tolua_UI_Control_SetMode00);
   tolua_function(tolua_S,"GetMode",tolua_UI_Control_GetMode00);
   tolua_function(tolua_S,"IsMouseOn",tolua_UI_Control_IsMouseOn00);
   tolua_function(tolua_S,"IsClick",tolua_UI_Control_IsClick00);
   tolua_function(tolua_S,"IsRBClick",tolua_UI_Control_IsRBClick00);
   tolua_function(tolua_S,"IsParent",tolua_UI_Control_IsParent00);
   tolua_function(tolua_S,"IsNoChangeLevel",tolua_UI_Control_IsNoChangeLevel00);
   tolua_function(tolua_S,"GetControlMode",tolua_UI_Control_GetControlMode00);
   tolua_function(tolua_S,"SetTips",tolua_UI_Control_SetTips00);
   tolua_function(tolua_S,"GetTips",tolua_UI_Control_GetTips00);
   tolua_function(tolua_S,"SetTipTime",tolua_UI_Control_SetTipTime00);
   tolua_function(tolua_S,"GetTipTime",tolua_UI_Control_GetTipTime00);
   tolua_function(tolua_S,"SetMask",tolua_UI_Control_SetMask00);
   tolua_function(tolua_S,"Move",tolua_UI_Control_Move00);
   tolua_function(tolua_S,"ReSize",tolua_UI_Control_ReSize00);
   tolua_function(tolua_S,"SetFocus",tolua_UI_Control_SetFocus00);
   tolua_function(tolua_S,"KillFocus",tolua_UI_Control_KillFocus00);
   tolua_function(tolua_S,"SetColorIndex",tolua_UI_Control_SetColorIndex00);
   tolua_function(tolua_S,"GetColorByIndex",tolua_UI_Control_GetColorByIndex00);
   tolua_function(tolua_S,"IsFadeIn",tolua_UI_Control_IsFadeIn00);
   tolua_function(tolua_S,"SetFadeIn",tolua_UI_Control_SetFadeIn00);
   tolua_function(tolua_S,"GetMainWnd",tolua_UI_Control_GetMainWnd00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ControlContainer","CControlContainer","CControl",NULL);
  tolua_beginmodule(tolua_S,"ControlContainer");
   tolua_function(tolua_S,"GetControls",tolua_UI_ControlContainer_GetControls00);
   tolua_function(tolua_S,"SetControls",tolua_UI_ControlContainer_SetControls00);
   tolua_function(tolua_S,"IsSonCtrl",tolua_UI_ControlContainer_IsSonCtrl00);
   tolua_function(tolua_S,"FindControl",tolua_UI_ControlContainer_FindControl00);
   tolua_function(tolua_S,"FindControlByName",tolua_UI_ControlContainer_FindControlByName00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Window","CCtrlWindow","CControlContainer",NULL);
  tolua_beginmodule(tolua_S,"Window");
   tolua_function(tolua_S,"SetLifeTime",tolua_UI_Window_SetLifeTime00);
   tolua_function(tolua_S,"GetLifeTime",tolua_UI_Window_GetLifeTime00);
   tolua_function(tolua_S,"SetCloseButton",tolua_UI_Window_SetCloseButton00);
   tolua_function(tolua_S,"OnClickCloseButton",tolua_UI_Window_OnClickCloseButton00);
   tolua_function(tolua_S,"OnMouseMove",tolua_UI_Window_OnMouseMove00);
   tolua_function(tolua_S,"Draw",tolua_UI_Window_Draw00);
   tolua_function(tolua_S,"Msg",tolua_UI_Window_Msg00);
   tolua_function(tolua_S,"OnKeyDown",tolua_UI_Window_OnKeyDown00);
   tolua_function(tolua_S,"OnEscape",tolua_UI_Window_OnEscape00);
   tolua_function(tolua_S,"SetLifeTime",tolua_UI_Window_SetLifeTime01);
   tolua_function(tolua_S,"GetLifeTime",tolua_UI_Window_GetLifeTime01);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MainWnd","CMainWnd","CCtrlWindow",NULL);
  tolua_beginmodule(tolua_S,"MainWnd");
   tolua_function(tolua_S,"UpdateLayOut",tolua_UI_MainWnd_UpdateLayOut00);
   tolua_function(tolua_S,"DisableIME",tolua_UI_MainWnd_DisableIME00);
   tolua_function(tolua_S,"GetMouseX",tolua_UI_MainWnd_GetMouseX00);
   tolua_function(tolua_S,"GetMouseY",tolua_UI_MainWnd_GetMouseY00);
   tolua_function(tolua_S,"GetMouseXY",tolua_UI_MainWnd_GetMouseXY00);
   tolua_function(tolua_S,"SetMouseOnID",tolua_UI_MainWnd_SetMouseOnID00);
   tolua_function(tolua_S,"GetMouseOnID",tolua_UI_MainWnd_GetMouseOnID00);
   tolua_function(tolua_S,"GetMouseOnCtrl",tolua_UI_MainWnd_GetMouseOnCtrl00);
   tolua_function(tolua_S,"SetTipOwnerWnd",tolua_UI_MainWnd_SetTipOwnerWnd00);
   tolua_function(tolua_S,"GetTipOwnerWnd",tolua_UI_MainWnd_GetTipOwnerWnd00);
   tolua_function(tolua_S,"GetTipWnd",tolua_UI_MainWnd_GetTipWnd00);
   tolua_function(tolua_S,"SetHideCursor",tolua_UI_MainWnd_SetHideCursor00);
   tolua_function(tolua_S,"IsHideCursor",tolua_UI_MainWnd_IsHideCursor00);
   tolua_function(tolua_S,"GetGameState",tolua_UI_MainWnd_GetGameState00);
   tolua_function(tolua_S,"GetFocusCtrl",tolua_UI_MainWnd_GetFocusCtrl00);
   tolua_function(tolua_S,"SetFocusCtrl",tolua_UI_MainWnd_SetFocusCtrl00);
   tolua_function(tolua_S,"ADDCONTROL",tolua_UI_MainWnd_ADDCONTROL00);
   tolua_function(tolua_S,"DELETECONTROL",tolua_UI_MainWnd_DELETECONTROL00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CCtrlButton","CCtrlButton","CControl",NULL);
  tolua_beginmodule(tolua_S,"CCtrlButton");
   tolua_function(tolua_S,"OnClick",tolua_UI_CCtrlButton_OnClick00);
   tolua_function(tolua_S,"SetAutoSwitch",tolua_UI_CCtrlButton_SetAutoSwitch00);
   tolua_function(tolua_S,"IsSmall",tolua_UI_CCtrlButton_IsSmall00);
   tolua_function(tolua_S,"ReloadTex",tolua_UI_CCtrlButton_ReloadTex00);
   tolua_function(tolua_S,"ReloadTex",tolua_UI_CCtrlButton_ReloadTex01);
   tolua_function(tolua_S,"SetSwitchTex",tolua_UI_CCtrlButton_SetSwitchTex00);
   tolua_function(tolua_S,"SetState",tolua_UI_CCtrlButton_SetState00);
   tolua_function(tolua_S,"SetEffectTex",tolua_UI_CCtrlButton_SetEffectTex00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Radio","CCtrlRadio","CControl",NULL);
  tolua_beginmodule(tolua_S,"Radio");
   tolua_function(tolua_S,"OnClick",tolua_UI_Radio_OnClick00);
   tolua_function(tolua_S,"IsChecked",tolua_UI_Radio_IsChecked00);
   tolua_function(tolua_S,"EnableChecked",tolua_UI_Radio_EnableChecked00);
   tolua_function(tolua_S,"DisableChecked",tolua_UI_Radio_DisableChecked00);
   tolua_function(tolua_S,"GetRadio",tolua_UI_Radio_GetRadio00);
   tolua_function(tolua_S,"SetRadio",tolua_UI_Radio_SetRadio00);
   tolua_function(tolua_S,"AddBuddy",tolua_UI_Radio_AddBuddy00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Edit","CCtrlEdit","CControl",NULL);
  tolua_beginmodule(tolua_S,"Edit");
   tolua_function(tolua_S,"SetPwd",tolua_UI_Edit_SetPwd00);
   tolua_function(tolua_S,"IsPwd",tolua_UI_Edit_IsPwd00);
   tolua_function(tolua_S,"Clear",tolua_UI_Edit_Clear00);
   tolua_function(tolua_S,"GetText",tolua_UI_Edit_GetText00);
   tolua_function(tolua_S,"SetText",tolua_UI_Edit_SetText00);
   tolua_function(tolua_S,"GetNum",tolua_UI_Edit_GetNum00);
   tolua_function(tolua_S,"SetMaxLength",tolua_UI_Edit_SetMaxLength00);
   tolua_function(tolua_S,"InsertText",tolua_UI_Edit_InsertText00);
   tolua_function(tolua_S,"SetDigital",tolua_UI_Edit_SetDigital00);
   tolua_function(tolua_S,"SetVisualWords",tolua_UI_Edit_SetVisualWords00);
   tolua_function(tolua_S,"SetCursorColor",tolua_UI_Edit_SetCursorColor00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CCtrlTip","CCtrlTip","CControl",NULL);
  tolua_beginmodule(tolua_S,"CCtrlTip");
   tolua_function(tolua_S,"SetText",tolua_UI_CCtrlTip_SetText00);
   tolua_function(tolua_S,"AddText",tolua_UI_CCtrlTip_AddText00);
   tolua_function(tolua_S,"Clear",tolua_UI_CCtrlTip_Clear00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Scroll","CCtrlScroll","CControl",NULL);
  tolua_beginmodule(tolua_S,"Scroll");
   tolua_function(tolua_S,"SetRange",tolua_UI_Scroll_SetRange00);
   tolua_function(tolua_S,"GetRange",tolua_UI_Scroll_GetRange00);
   tolua_function(tolua_S,"SetStep",tolua_UI_Scroll_SetStep00);
   tolua_function(tolua_S,"GetStep",tolua_UI_Scroll_GetStep00);
   tolua_function(tolua_S,"SetPos",tolua_UI_Scroll_SetPos00);
   tolua_function(tolua_S,"GetPos",tolua_UI_Scroll_GetPos00);
   tolua_function(tolua_S,"SetVertical",tolua_UI_Scroll_SetVertical00);
   tolua_function(tolua_S,"ReSize",tolua_UI_Scroll_ReSize00);
   tolua_function(tolua_S,"GoUp",tolua_UI_Scroll_GoUp00);
   tolua_function(tolua_S,"GoDown",tolua_UI_Scroll_GoDown00);
   tolua_function(tolua_S,"SetButtonHeight",tolua_UI_Scroll_SetButtonHeight00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"WindowX","CCtrlWindowX","CCtrlWindow",NULL);
  tolua_beginmodule(tolua_S,"WindowX");
   tolua_function(tolua_S,"PointInPage",tolua_UI_WindowX_PointInPage00);
   tolua_function(tolua_S,"SwitchToPage",tolua_UI_WindowX_SwitchToPage00);
   tolua_function(tolua_S,"SetPageRect",tolua_UI_WindowX_SetPageRect00);
   tolua_function(tolua_S,"FindScrollByName",tolua_UI_WindowX_FindScrollByName00);
   tolua_function(tolua_S,"FindRadioByName",tolua_UI_WindowX_FindRadioByName00);
   tolua_function(tolua_S,"FindButtonByName",tolua_UI_WindowX_FindButtonByName00);
   tolua_function(tolua_S,"FindEditByName",tolua_UI_WindowX_FindEditByName00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CCharacterAttr","CCharacterAttr","",NULL);
  tolua_beginmodule(tolua_S,"CCharacterAttr");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CCharacter","CCharacter","CCharacterAttr",NULL);
  tolua_beginmodule(tolua_S,"CCharacter");
   tolua_function(tolua_S,"GetLevel",tolua_UI_CCharacter_GetLevel00);
   tolua_function(tolua_S,"GetCareer",tolua_UI_CCharacter_GetCareer00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Good","CGood","",NULL);
  tolua_beginmodule(tolua_S,"Good");
   tolua_function(tolua_S,"GetID",tolua_UI_Good_GetID00);
   tolua_function(tolua_S,"GetStdMode",tolua_UI_Good_GetStdMode00);
   tolua_function(tolua_S,"GetNeed",tolua_UI_Good_GetNeed00);
   tolua_function(tolua_S,"GetNeedLevel",tolua_UI_Good_GetNeedLevel00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GoodGrid","CGoodGrid","CControl",NULL);
  tolua_beginmodule(tolua_S,"GoodGrid");
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"this",tolua_get_this_ptr,NULL);
  tolua_variable(tolua_S,"ui",tolua_get_ui_ptr,NULL);
  tolua_variable(tolua_S,"input",tolua_get_input_ptr,NULL);
  tolua_variable(tolua_S,"SELF",tolua_get_SELF_ptr,NULL);
  tolua_function(tolua_S,"PopupWindow",tolua_UI_PopupWindow00);
  tolua_function(tolua_S,"UpdateWindow",tolua_UI_UpdateWindow00);
  tolua_function(tolua_S,"CloseWindow",tolua_UI_CloseWindow00);
  tolua_function(tolua_S,"PopSimpleAlert",tolua_UI_PopSimpleAlert00);
  tolua_function(tolua_S,"PopSimpleComfirm",tolua_UI_PopSimpleComfirm00);
  tolua_function(tolua_S,"PopEditBox",tolua_UI_PopEditBox00);
  tolua_function(tolua_S,"GetDropGood",tolua_UI_GetDropGood00);
  tolua_function(tolua_S,"GetDropGoodPos",tolua_UI_GetDropGoodPos00);
  tolua_function(tolua_S,"GetDropFromWnd",tolua_UI_GetDropFromWnd00);
  tolua_function(tolua_S,"GetCharacterMagicID",tolua_UI_GetCharacterMagicID00);
  tolua_function(tolua_S,"GetLearnedMagicNum",tolua_UI_GetLearnedMagicNum00);
  tolua_function(tolua_S,"TestShowMessageBox",tolua_UI_TestShowMessageBox00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_UI (lua_State* tolua_S) {
 return tolua_UI_open(tolua_S);
};
#endif

