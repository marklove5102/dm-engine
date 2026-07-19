#pragma once

#include "Global/StdHeaders.h"
#include "Global/StringUtil.h"

enum GlobalStrings
{
	FuShiWnd_Put_Equipment,
	FuShiWnd_Put_Driller,
	FuShiWnd_Show_Expense,
	FuShiWnd_PutFuShi_To_Combine,
	FuShiWnd_PutHeChengFu
};

class CGlobalStrMgr
{
public:
	CGlobalStrMgr(void);
	~CGlobalStrMgr(void);

	const char * GetGlobalStr(GlobalStrings idx)
	{
		if(idx < m_GlobalStr.size()) 
			return m_GlobalStr[idx].c_str();
		else
			return "";
	}

private:
	VString m_GlobalStr;
};

extern CGlobalStrMgr g_StrMgr;