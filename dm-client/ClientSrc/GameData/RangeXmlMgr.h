#pragma once

#include "Global/Global.h"
#include <vector>
#include "Global/StringUtil.h"

class CRangeXmlMgr
{
public:
	struct tagRangeHelp
	{
		string strName;
		VString strDesc; //√Ë ˆ
	};
	CRangeXmlMgr(void);
	~CRangeXmlMgr(void);

	vector<string>* GetRangeDesc(const char* szName);
private:
	vector<tagRangeHelp> m_vecRange;
	bool LoadRangeHelp();
};

extern CRangeXmlMgr g_RangeXmlMgr; 
