#pragma once

#include "Global/Global.h"

class CFindGood
{
public:

	CFindGood(void);
	~CFindGood(void);
	
	struct _FindData
	{
		string str;
		DWORD dwCount;

		_FindData(const char * name,int type =1)
		{
			str = name;
			if(type == 1)
				str  =str + " 被发现";
			else if(type ==2)
				str = str+ "金币 增加";
			dwCount = GetTickCount();
		}
	};

	typedef vector<_FindData> VFindData;

	void AddMessage(const char* str,int type);
	VFindData& GetDisplayString() {return m_VDisplay;}
private:
	VFindData  m_VDisplay;
};

extern CFindGood g_FindGood;