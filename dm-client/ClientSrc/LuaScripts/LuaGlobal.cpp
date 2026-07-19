#include "LuaGlobal.h"
#include "BaseClass/Control/Control.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIGoodMgr.h"

void Log(const char* str)
{
	output_debug("Lua LOG:%s\n",str);
}

void PopupWindow(const char* name)
{
	g_pControl->PopupWindow(name,OPER_CREATE);
}

void UpdateWindow(const char* name)
{
	g_pControl->PopupWindow(name,OPER_UPDATE);
}

void CloseWindow(const char* name)
{
	g_pControl->PopupWindow(name,OPER_CLOSE);
}

void PopSimpleAlert(const char* str)
{
	g_MsgBoxMgr.PopSimpleAlert(str);
}

void PopSimpleComfirm(const char* str)
{
	g_MsgBoxMgr.PopSimpleComfirm(str,0,0);
}
void PopEditBox(const char* str)
{
	g_MsgBoxMgr.PopEditBox(str,0,0);
}

void SwitchTalkViewer()
{
	g_pControl->Msg(MSG_CTRL_SWITCH_TALKVIEWER,0,0);
}

CGood* GetDropGood()
{
    return &(CGoodGrid::GetDropGoodFrom().DropGood);
}

int GetDropGoodPos()
{
    return CGoodGrid::GetDropGoodFrom().iWndPos;
}

int GetDropFromWnd()
{
    return CGoodGrid::GetDropGoodFrom().eFromWnd;
}

int GetCharacterMagicID(int i)
{
    return SELF.GetMagic(i).GetMagicID();
}

int GetLearnedMagicNum()
{
    int num = 0;
    for(int i = 0;i < MAX_MAGIC_SKILL;i++)
    {
        if(GetCharacterMagicID(i) != 0)
        {
            num++;
        }
    }
    return num;
}

void TestShowMessageBox(const char *strMsg)
{
	MessageBox(::GetActiveWindow(),strMsg,"test",MB_OK);
}