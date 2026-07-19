// dumpgood.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "goodinfo.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

    
	return TRUE;
}
//获得物品的tip信息
bool DumpGoodInfo(const char* strMsg , char * strResult , int &len)
{
	return g_GoodInfo.DumpGood( strMsg , strResult,len);
}

//设置附魔残留属性的计算公式
void SetDemonRemainAttr(const char* strMsg2)
{
}

//设置有附魔残留属性物品的使用等级规则
void SetDemonRemainArmRule(const char* strMsg2)
{
}
