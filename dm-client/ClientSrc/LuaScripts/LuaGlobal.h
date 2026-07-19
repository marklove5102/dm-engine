#pragma once

#include "BaseClass/Control/Control.h"
#include "GameData/GameData.h"

//UI
void Log(const char* str);
void PopupWindow(const char* name);
void UpdateWindow(const char* name);
void CloseWindow(const char* name);
void PopSimpleAlert(const char* str);
void PopSimpleComfirm(const char* str);
void PopEditBox(const char* str);

//AI
CGood* GetDropGood();                   //获得拖动物品
int  GetDropGoodPos();                  //获得拖动物品位置
int  GetDropFromWnd();                  //获得拖动物品所属窗口
int  GetCharacterMagicID(int i);        //获得人物的魔法ID
int  GetLearnedMagicNum();              //获得人物掌握的魔法数目

//for test
void TestShowMessageBox(const char *strMsg);
