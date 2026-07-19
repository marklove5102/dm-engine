#include "stdafx.h"
#include "Global/DebugTry.h"
#include "Global/Interface/Callbackinterface.h"


void SaveException(char *file,char *fun,int line,const CSeException* pException,const char *msg)
{
	if (g_pCallBack)
	{
		g_pCallBack->CallBackSaveException(file,fun,line,pException,msg);
	}
}

