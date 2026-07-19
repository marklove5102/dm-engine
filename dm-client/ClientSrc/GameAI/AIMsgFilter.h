#pragma once

#include "Global/Global.h"

class CAIMsgFilter
{
public:
	bool FilterSend(const char* str);
	bool FilterRecv(const char* str);

	void GetNameOfMessage(const char* str, string& name);
	bool FilterBlack(const char* buf); //过滤黑名单
private:
	bool FindHeader(const string& header); //查找消息前缀
	string m_strMsg;
};

extern CAIMsgFilter g_MsgFilter;
